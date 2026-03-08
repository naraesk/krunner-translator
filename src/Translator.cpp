/******************************************************************************
 *  Copyright (C) 2013 – 2018 by David Baum <david.baum@naraesk.eu>           *
 *                                                                            *
 *  This library is free software; you can redistribute it and/or modify      *
 *  it under the terms of the GNU Lesser General Public License as published  *
 *  by the Free Software Foundation; either version 2 of the License or (at   *
 *  your option) any later version.                                           *
 *                                                                            *
 *  This library is distributed in the hope that it will be useful,           *
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of            *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU         *
 *  Library General Public License for more details.                          *
 *                                                                            *
 *  You should have received a copy of the GNU Lesser General Public License  *
 *  along with this library; see the file COPYING.LIB.                        *
 *  If not, see <http://www.gnu.org/licenses/>.                               *
 *****************************************************************************/

#include "Translator.h"
#include "config/config_keys.h"
#include "translationEngines/impl/GoogleTranslate.h"
#include "translationEngines/impl/baidu.h"
#include "translationEngines/impl/youdao.h"
#include "translationEngines/impl/Bing.h"
#include "Parser.h"
#include <src/TranslateShellProcess.h>
#include <KLocalizedString>
#include <QGuiApplication>
#include <QClipboard>
#include <KConfig>
#include <KConfigGroup>

K_PLUGIN_CLASS_WITH_JSON(Translator, "plasma-runner-translator.json")

Translator::Translator(QObject *parent, const KPluginMetaData &metaData)
        : KRunner::AbstractRunner(parent, metaData) {
    KRunner::Action copy(QStringLiteral("copy"),
                         QStringLiteral("edit-copy"),
                         QStringLiteral("Copy to Clipboard"));
    KRunner::Action play(QStringLiteral("play"),
                         QStringLiteral("media-playback-start"),
                         QStringLiteral("Play audio"));
    actions = {copy, play};
    addSyntax(KRunner::RunnerSyntax(QString::fromLatin1("%1:q:").arg(i18n("<language code>")),
                                     i18n("Translates the word(s) :q: into target language")));
    addSyntax(KRunner::RunnerSyntax(QString::fromLatin1("%1:q:").arg(i18n("<source language>-<target language>")),
                                     i18n("Translates the word(s) :q: from the source into target language")));
    languages.initialize();
    parser.setRepository(&languages);
}

void Translator::match(KRunner::RunnerContext &context) {
    const QString term = context.query();
    TranslationQuery * query = parser.parse(term);
    query->updateSourceLanguage(defaultLanguage, alternativeDefaultLanguage);
    if(!query->isValid()) return;
    if (!context.isValid()) return;

    if (m_baiduEnable) {
        QEventLoop baiduLoop;
        Baidu baidu(this, context, query, m_baiduAPPID, m_baiduAPIKey);
        connect(&baidu, SIGNAL(finished()), &baiduLoop, SLOT(quit()));
        baiduLoop.exec();
    }
    if (m_youdaoEnable) {
        QEventLoop youdaoLoop;
        Youdao youdao(this, context, query, m_youdaoAPPID, m_youdaoAppSec);
        connect(&youdao, SIGNAL(finished()), &youdaoLoop, SLOT(quit()));
        youdaoLoop.exec();
    }
    for (auto engine : engines) {
        KRunner::QueryMatch match = engine->translate(query);
        match.setActions(actions);
        context.addMatch(match);
    }
    delete query;
}

void Translator::run(const KRunner::RunnerContext &context, const KRunner::QueryMatch &match) {
    Q_UNUSED(context);
    QGuiApplication::clipboard()->setText(match.text());
    if (match.selectedAction() && match.selectedAction().id() == QLatin1String("play")) {
        TranslateShellProcess process;
        process.play(match.text());
    }
}

void Translator::reloadConfiguration() {
    auto grp = config();
    QString defaultLanguageAbbreviation = grp.readEntry(CONFIG_PRIMARY, QStringLiteral("en"));
    QString alternativeDefaultLanguageAbbreviation = grp.readEntry(CONFIG_SECONDARY, QStringLiteral("es"));
    defaultLanguage = languages.getSupportedLanguage(defaultLanguageAbbreviation);
    alternativeDefaultLanguage = languages.getSupportedLanguage(alternativeDefaultLanguageAbbreviation);
    m_baiduAPPID = grp.readEntry(CONFIG_BAIDU_APPID, QStringLiteral(""));
    m_baiduAPIKey = grp.readEntry(CONFIG_BAIDU_APIKEY, QStringLiteral(""));
    m_youdaoAPPID = grp.readEntry(CONFIG_YOUDAO_APPID, QStringLiteral(""));
    m_youdaoAppSec = grp.readEntry(CONFIG_YOUDAO_APPSEC, QStringLiteral(""));
    m_baiduEnable = grp.readEntry(CONFIG_BAIDU_ENABLE, false);
    m_youdaoEnable = grp.readEntry(CONFIG_YOUDAO_ENABLE, false);

    const bool googleEnable = grp.readEntry(CONFIG_GOOGLE_ENABLE, true);
    if (googleEnable) {
        CommandLineEngine *googleTranslate = new GoogleTranslate(this);
        engines.push_front(googleTranslate);
    }

    const bool bingEnable = grp.readEntry(CONFIG_BING_ENABLE, false);
    if (bingEnable) {
        CommandLineEngine *bingTranslate = new Bing(this);
        engines.push_front(bingTranslate);
    }
}

Translator::~Translator() {
    for(auto engine : engines) {
        delete engine;
    }
}

#include "Translator.moc"
