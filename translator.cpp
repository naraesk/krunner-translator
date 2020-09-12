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
#include "config/translator_config.h"
#include "api/glosbe.h"
#include "api/yandex.h"
#include "api/baidu.h"
#include "api/youdao.h"
#include "helper.h"

#include <KLocalizedString>
#include <QApplication>
#include <QClipboard>
#include <QAction>
#include <api/translateShell.h>
#include <api/translateShellProcess.h>
#include <KConfigCore/KConfig>

Translator::Translator(QObject *parent, const QVariantList &args)
    : Plasma::AbstractRunner(parent, args)
{
    Q_UNUSED(args);
    
    setObjectName(QLatin1String("Translator"));
    KConfigGroup grp = config();

    m_primary = grp.readEntry(CONFIG_PRIMARY);
    m_secondary = grp.readEntry(CONFIG_SECONDARY);
    m_yandexKey = grp.readEntry(CONFIG_YANDEX_KEY);
    m_baiduAPPID = grp.readEntry(CONFIG_BAIDU_APPID);
    m_baiduAPIKey = grp.readEntry(CONFIG_BAIDU_APIKEY);
    m_youdaoAPPID = grp.readEntry(CONFIG_YOUDAO_APPID);
    m_youdaoAppSec = grp.readEntry(CONFIG_YOUDAO_APPSEC);
    m_glosbeWord = stringToBool(grp.readEntry(CONFIG_GLOSBE_WORD));
    m_glosbePhrase = stringToBool(grp.readEntry(CONFIG_GLOSBE_PHRASE));
    m_glosbeExamples = stringToBool(grp.readEntry(CONFIG_GLOSBE_EXAMPLES));
    m_yandexWord = stringToBool(grp.readEntry(CONFIG_YANDEX_WORD));
    m_yandexPhrase = stringToBool(grp.readEntry(CONFIG_YANDEX_PHRASE));
    m_baiduEnable = stringToBool(grp.readEntry(CONFIG_BAIDU_ENABLE));
    m_youdaoEnable = stringToBool(grp.readEntry(CONFIG_YOUDAO_ENABLE));
    m_translateShellEnable = stringToBool(grp.readEntry(CONFIG_GOOGLE_ENABLE));
    setIgnoredTypes(Plasma::RunnerContext::Directory |
                    Plasma::RunnerContext::File |
                    Plasma::RunnerContext::NetworkLocation);
    setSpeed(AbstractRunner::SlowSpeed);
    setPriority(HighestPriority);
    QAction * copy =  new QAction();
    copy->setIcon(QIcon::fromTheme(QStringLiteral("editcopy")));
    copy->setText("Copy to Clipboard");
    copy->setData("copy");
    addAction("copy", copy);
    auto *play = addAction(QStringLiteral("play"),
                           QIcon::fromTheme(QStringLiteral("cs-sound")),
                           QStringLiteral("Play audio"));
    play->setData(QStringLiteral("play"));
    actions = {action, play};
    setDefaultSyntax(Plasma::RunnerSyntax(QString::fromLatin1("%1:q:").arg(i18n("<language code>")),i18n("Translates the word(s) :q: into target language")));
    setDefaultSyntax(Plasma::RunnerSyntax(QString::fromLatin1("%1:q:").arg(i18n("<source languagce>-<target languagce>")), i18n("Translates the word(s) :q: from the source into target language")));
}

bool Translator::parseTerm(const QString& term, QString& text, QPair<QString, QString> &language)
{
    const int index = term.indexOf(" ");
    if (index == -1) return false;
    text = term.mid(index + 1);
    const QString languageTerm = term.left(index);

    if (languageTerm.contains("-")) {
        int languageIndex = languageTerm.indexOf("-");
        language.first = languageTerm.left(languageIndex);
        language.second = languageTerm.mid(languageIndex + 1);

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

void Translator::match(Plasma::RunnerContext &context)
{

    KConfigGroup grp = config();
    m_translateShellEnable = stringToBool(grp.readEntry(CONFIG_GOOGLE_ENABLE));
    const QString term = context.query();
    QString text;
    QPair<QString, QString> language;

    if (!parseTerm(term, text, language)) return;
    if (!context.isValid()) return;

    if (m_baiduEnable)
    {
        QEventLoop baiduLoop;
        Baidu baidu(this, context, text, language, m_baiduAPPID, m_baiduAPIKey);
        connect(&baidu, SIGNAL(finished()), &baiduLoop, SLOT(quit()));
        baiduLoop.exec();
    }
    if (m_youdaoEnable)
    {
        QEventLoop youdaoLoop;
        Youdao youdao(this, context, text, language, m_youdaoAPPID, m_youdaoAppSec);
        connect(&youdao, SIGNAL(finished()), &youdaoLoop, SLOT(quit()));
        youdaoLoop.exec();
    }

    if (m_translateShellEnable){
        QEventLoop translateShellLoop;
        TranslateShell translateShell(this, context, text, language, actions.first());
        connect(&translateShell, SIGNAL(finished()), &translateShellLoop, SLOT(quit()));
        translateShellLoop.exec();
    }
    if (text.contains(" ")) {
        if(m_yandexPhrase) {
            QEventLoop loop;
            Yandex yandex(this, context, text, language, m_yandexKey);
            connect(&yandex, SIGNAL(finished()), &loop, SLOT(quit()));
            loop.exec();
        }
        if(m_glosbePhrase) {
            if(m_glosbeExamples) {
                QEventLoop loop;
                Glosbe glosbe(this, context, text, language, m_glosbeExamples);
                connect(&glosbe, SIGNAL(finished()), &loop, SLOT(quit()));
                loop.exec();
            }
            QEventLoop loop;
            Glosbe glosbe(this, context, text, language);
            connect(&glosbe, SIGNAL(finished()), &loop, SLOT(quit()));
            loop.exec();
        }
    } else {
        if(m_yandexWord) {
            QEventLoop loop;
            Yandex yandex(this, context, text, language, m_yandexKey);
            connect(&yandex, SIGNAL(finished()), &loop, SLOT(quit()));
            loop.exec();
        }
        if(m_glosbeWord) {
            if(m_glosbeExamples) {
                QEventLoop loop;
                Glosbe glosbe(this, context, text, language, m_glosbeExamples);
                connect(&glosbe, SIGNAL(finished()), &loop, SLOT(quit()));
                loop.exec();
            }
            QEventLoop loop;
            Glosbe glosbe(this, context, text, language);
            connect(&glosbe, SIGNAL(finished()), &loop, SLOT(quit()));
            loop.exec();
        }
    }
}

void Translator::run(const Plasma::RunnerContext &context, const Plasma::QueryMatch &match)
{
    Q_UNUSED(context);
    qDebug() << "XXXXX";
    QApplication::clipboard()->setText(match.text());
    qDebug() << "action: " << match.selectedAction()->text();
    if(match.selectedAction()->data().toString() == QLatin1String("play")) {
        TranslateShellProcess process(this);
        process.play(match.text());
    } else if (!match.selectedAction()) {
        QApplication::clipboard()->setText(match.text());
    }
    QApplication::clipboard()->setText(match.text());
}

void Translator::reloadConfiguration()
{
    KConfigGroup grp = config();
    m_primary = grp.readEntry(CONFIG_PRIMARY);
    m_secondary = grp.readEntry(CONFIG_SECONDARY);
    m_yandexKey = grp.readEntry(CONFIG_YANDEX_KEY);
    m_baiduAPPID = grp.readEntry(CONFIG_BAIDU_APPID);
    m_baiduAPIKey = grp.readEntry(CONFIG_BAIDU_APIKEY);
    m_youdaoAPPID = grp.readEntry(CONFIG_YOUDAO_APPID);
    m_youdaoAppSec = grp.readEntry(CONFIG_YOUDAO_APPSEC);
    m_glosbeWord = stringToBool(grp.readEntry(CONFIG_GLOSBE_WORD));
    m_glosbePhrase = stringToBool(grp.readEntry(CONFIG_GLOSBE_PHRASE));
    m_glosbeExamples = stringToBool(grp.readEntry(CONFIG_GLOSBE_EXAMPLES));
    m_yandexWord = stringToBool(grp.readEntry(CONFIG_YANDEX_WORD));
    m_yandexPhrase = stringToBool(grp.readEntry(CONFIG_YANDEX_PHRASE));
    m_baiduEnable = stringToBool(grp.readEntry(CONFIG_BAIDU_ENABLE));
    m_youdaoEnable = stringToBool(grp.readEntry(CONFIG_YOUDAO_ENABLE));
    m_translateShellEnable = stringToBool(grp.readEntry(CONFIG_GOOGLE_ENABLE));
}

QList<QAction *> Translator::actionsForMatch(const Plasma::QueryMatch &match) {
    if(match.data().toString() == "audio") {
        return actions;
    }
    return {actions.first()};
}

void Translator::reload() {
    KConfigGroup grp = config();
    m_primary = grp.readEntry(CONFIG_PRIMARY);
    m_secondary = grp.readEntry(CONFIG_SECONDARY);
    m_yandexKey = grp.readEntry(CONFIG_YANDEX_KEY);
    m_baiduAPPID = grp.readEntry(CONFIG_BAIDU_APPID);
    m_baiduAPIKey = grp.readEntry(CONFIG_BAIDU_APIKEY);
    m_youdaoAPPID = grp.readEntry(CONFIG_YOUDAO_APPID);
    m_youdaoAppSec = grp.readEntry(CONFIG_YOUDAO_APPSEC);
    m_glosbeWord = stringToBool(grp.readEntry(CONFIG_GLOSBE_WORD));
    m_glosbePhrase = stringToBool(grp.readEntry(CONFIG_GLOSBE_PHRASE));
    m_glosbeExamples = stringToBool(grp.readEntry(CONFIG_GLOSBE_EXAMPLES));
    m_yandexWord = stringToBool(grp.readEntry(CONFIG_YANDEX_WORD));
    m_yandexPhrase = stringToBool(grp.readEntry(CONFIG_YANDEX_PHRASE));
    m_baiduEnable = stringToBool(grp.readEntry(CONFIG_BAIDU_ENABLE));
    m_youdaoEnable = stringToBool(grp.readEntry(CONFIG_YOUDAO_ENABLE));
    m_translateShellEnable = stringToBool(grp.readEntry(CONFIG_GOOGLE_ENABLE));
}

void Translator::init() {
    reloadConfiguration();
}

#include "moc_translator.cpp"
