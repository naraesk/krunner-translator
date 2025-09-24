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

#include "translator.h"
#include "translateShellProcess.h"
#include "config/translator_config.h"
#include "provider/GoogleTranslate.h"
#include "provider/baidu.h"
#include "provider/youdao.h"
#include "provider/Bing.h"

#include <KConfigGroup>
#include <klocalizedstring.h>

#include <QClipboard>

Translator::Translator(QObject *parent, const KPluginMetaData &pluginMetaData)
        : KRunner::AbstractRunner(parent, pluginMetaData) {
    actions.append(KRunner::Action(QStringLiteral("copy"), QStringLiteral("edit-copy-symbolic"), i18n("Copy to Clipboard")));
    actions.append(KRunner::Action(QStringLiteral("play"), QStringLiteral("audio-symbolic"), i18n("Play audio")));

    addSyntax(i18n("<language code> :q:"), i18n("Translates the word(s) :q: into target language"));
    addSyntax(i18n("<source language>-<target language> :q:"),
              i18n("Translates the word(s) :q: from the source into target language"));
    languages.initialize();
}

bool Translator::parseTerm(const QString &term, QString &text, QPair<QString, QString> &language) {
    const int index = term.indexOf(QStringLiteral(" "));
    if (index == -1) return false;
    text = term.mid(index + 1);
    const QString languageTerm = term.left(index);

    if (languageTerm.contains(QStringLiteral("-"))) {
        int languageIndex = languageTerm.indexOf(QStringLiteral("-"));
        language.first = languageTerm.left(languageIndex);
        language.second = languageTerm.mid(languageIndex + 1);
        if(languages.containsAbbreviation(language.first) && languages.containsAbbreviation(language.second) ) {
            return true;
        } else {
            return false;
        }
    } else {
        if (m_primary == languageTerm) {
            language.first = m_secondary;
        } else {
            language.first = m_primary;
        }
        language.second = languageTerm;
    }
    return true;
}

void Translator::match(KRunner::RunnerContext &context) {
    const QString term = context.query();
    QString text;
    QPair<QString, QString> language;

    if (!parseTerm(term, text, language)) return;
    if (!context.isValid()) return;

    if (m_baiduEnable) {
        QEventLoop baiduLoop;
        Baidu baidu(this, context, text, language, m_baiduAPPID, m_baiduAPIKey);
        connect(&baidu, &Baidu::finished, &baiduLoop, &QEventLoop::quit);
        baiduLoop.exec();
    }
    if (m_youdaoEnable) {
        QEventLoop youdaoLoop;
        Youdao youdao(this, context, text, language, m_youdaoAPPID, m_youdaoAppSec);
        connect(&youdao, &Youdao::finished, &youdaoLoop, &QEventLoop::quit);
        youdaoLoop.exec();
    }
    for (auto engine : engines) {
        auto match = engine->translate(text, language);
        if (match.data().toString() == QStringLiteral("audio")) {
            match.setActions(actions);
        } else {
            match.setActions({ actions.at(0) });
        }
        context.addMatch(match);
    }
}

void Translator::run(const KRunner::RunnerContext &context, const KRunner::QueryMatch &match) {
    Q_UNUSED(context);
    QApplication::clipboard()->setText(match.text());
    KRunner::Action action = match.selectedAction();
    if ((bool)action && action.id() == QStringLiteral("play")) {
        TranslateShellProcess process;
        process.play(match.text());
    }
}

void Translator::reloadConfiguration() {
    auto grp = config();
    m_primary = grp.readEntry(CONFIG_PRIMARY, QStringLiteral("en"));
    m_secondary = grp.readEntry(CONFIG_SECONDARY, QStringLiteral("es"));
    m_baiduAPPID = grp.readEntry(CONFIG_BAIDU_APPID, QString());
    m_baiduAPIKey = grp.readEntry(CONFIG_BAIDU_APIKEY, QString());
    m_youdaoAPPID = grp.readEntry(CONFIG_YOUDAO_APPID, QString());
    m_youdaoAppSec = grp.readEntry(CONFIG_YOUDAO_APPSEC, QString());
    m_baiduEnable = grp.readEntry(CONFIG_BAIDU_ENABLE, false);
    m_youdaoEnable = grp.readEntry(CONFIG_YOUDAO_ENABLE, false);

    const bool googleEnable = grp.readEntry(CONFIG_GOOGLE_ENABLE, true);
    const bool bingEnable = grp.readEntry(CONFIG_BING_ENABLE, false);

    engines.clear();
    if (googleEnable) {
        CommandLineEngine *googleTranslate = new GoogleTranslate(this);
        engines.push_front(googleTranslate);
    }
    if (bingEnable) {
        CommandLineEngine *bingTranslate = new Bing(this);
        engines.push_front(bingTranslate);
    }
}

K_PLUGIN_CLASS_WITH_JSON(Translator, "translator.json")

#include "translator.moc"
