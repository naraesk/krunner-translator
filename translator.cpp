/******************************************************************************
 *  Copyright (C) 2013 – 2015 by David Baum <david.baum@naraesk.eu>           *
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
#include "translatorjob.h"

#include <QDebug>
#include <QFileDialog>
#include <QStandardPaths>
#include <KLocalizedString>
#include <QApplication>
#include <QIcon>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include <QtCore/QRegExp>
#include <QtCore/QPair>
#include <QtCore/QObject>
#include <QtCore/QList>
#include <QtCore/QListIterator>
#include <QJsonArray>
#include <QJsonDocument>
#include <QtCore/QEventLoop>
#include <QClipboard>

Translator::Translator(QObject *parent, const QVariantList &args)
    : Plasma::AbstractRunner(parent, args)
{
    Q_UNUSED(args);
    
    setObjectName(QLatin1String("Translator"));
    setHasRunOptions(false);
    setIgnoredTypes(Plasma::RunnerContext::Directory |
                    Plasma::RunnerContext::File |
                    Plasma::RunnerContext::NetworkLocation);
    setSpeed(AbstractRunner::SlowSpeed);
    setPriority(HighestPriority);
    setDefaultSyntax(Plasma::RunnerSyntax(QString::fromLatin1("%1:q:").arg(i18n("<language code>")),i18n("Translates the word(s) :q: into target language")));
    addSyntax(Plasma::RunnerSyntax(QString::fromLatin1("%1:q:").arg(i18n("<source languagce>-<target languagce>")), i18n("Translates the word(s) :q: from the source into target language")));
}

Translator::~Translator()
{
}

void Translator::match(Plasma::RunnerContext &context)
{
    const QString term = context.query();
    QString text;
    QPair<QString, QString> language;
    if (!parseTerm(term, text, language)) return;
    if (!context.isValid()) return;
    QEventLoop loop;
    TranslatorJob job(text, language);
    connect(&job, SIGNAL(finished()), &loop, SLOT(quit()));
    loop.exec();
    parseResult(job.result(), context, text);
}

void Translator::run(const Plasma::RunnerContext &context, const Plasma::QueryMatch &match)
{
    Q_UNUSED(context);
    QApplication::clipboard()->setText(match.text());
}

bool Translator::parseTerm(const QString& term, QString& text, QPair<QString, QString> &language)
{
    const int index = term.indexOf(" ");
    if (index == -1) return false;
    text = term.mid(index + 1);
    QString languageTerm = term.left(index);

    if (languageTerm.contains("-")) {
        int index = languageTerm.indexOf("-");
        language.first = languageTerm.left(index);
        language.second = languageTerm.mid(index + 1);

        return supportedLanguages.contains(language.first) && supportedLanguages.contains(language.second);
    } else {
        language.first.clear();
        language.second = languageTerm;
        return supportedLanguages.contains(language.second);
    }
}

void Translator::parseResult(const QString &result, Plasma::RunnerContext& context, const QString &text)
{
    QString jsonData = result;
//  jsonData contains arrays like this: ["foo",,"bar"]
//  but this is not valid JSON for QJSON, it expects empty strings: ["foo","","bar"]
    jsonData = jsonData.replace(QRegExp(",{3,3}"), ",\"\",\"\",");
    jsonData = jsonData.replace(QRegExp(",{2,2}"), ",\"\",");
    
    QJsonArray jsonArray = QJsonDocument::fromJson(jsonData.toUtf8()).array();
    QVariantList json = jsonArray.toVariantList();
    bool oldVersion = true;
    QMultiMap<int, QPair<QString, double> > sentences;

    QList<Plasma::QueryMatch> matches;
    // we are going recursively through the nested json-arry
    // level0 contains the data of the outer array, level1 of the next one and so on
    foreach (const QVariant& level0, json) {
        const QVariantList listLevel0 = level0.toList();
        if (listLevel0.isEmpty()) {
            continue;
        }
        foreach (const QVariant& level1, listLevel0) {
            if (level1.toList().size() <= 2 || level1.toList().at(2).toList().isEmpty()) {
                continue;
            }
            const int indexLevel1 = listLevel0.indexOf(level1);
            const QVariantList listLevel1 = level1.toList().at(2).toList();
            foreach (const QVariant& level2, listLevel1) {
                const QVariantList listLevel2 = level2.toList();

                // The JSON we get from Google has not always the same structure.
                // There is a version with addiotanal information like synonyms and frequency,
                // this is called newVersion oldVersion doesn't cointain something like this.

                const bool foundWordNew = (listLevel2.size() > 1) && (!listLevel2.at(1).toList().isEmpty());
                const bool foundWordOld = (listLevel2.size() == 4) && (oldVersion == true) && (listLevel2.at(1).toDouble() > 0);

                if (foundWordNew || foundWordOld) {
                    if (level1.toList().at(0).toString() != text && foundWordOld) {
                        // sentences are translated phrase by phrase
                        // first we have to add all phrases to sentences and then rebuild them
                        sentences.insert(indexLevel1, qMakePair(listLevel2.at(0).toString(), listLevel2.at(1).toDouble() / 1000));
                    } else {
                        Plasma::QueryMatch match(this);
                        match.setType(Plasma::QueryMatch::InformationalMatch);
                        match.setIcon(QIcon::fromTheme("applications-education-language"));
                        match.setText(listLevel2.at(0).toString());

                        if (foundWordNew) {
                            oldVersion = false;
                            match.setSubtext(level1.toList().at(0).toString());
                            if (listLevel2.size() > 3) {
                                match.setRelevance(listLevel2.at(3).toDouble());
                            } else {
                                match.setRelevance(0.0001);
                            }
                        } else {
                            match.setRelevance(listLevel2.at(1).toDouble() / 1000);
                        }

                        matches.append(match);
                    }
                }
            }
        }
    }

    if (!sentences.isEmpty()) {
        QPair<QString, double> pair;
        QMapIterator<int, QPair<QString, double> > it(sentences);
        int currentKey = -1;
        double currentRel = 1;
        QString currentString;

        while (it.hasNext()) {
            pair = it.next().value();

            // we're on to another key, process previous results, if any
            if (currentKey != it.key()) {

                currentKey = it.key();
                currentRel = 1;
                currentString.append(' ').append(pair.first);
                currentRel *= pair.second;
            }
        }
        if (!currentString.isEmpty()) {
            Plasma::QueryMatch match(this);
            match.setType(Plasma::QueryMatch::InformationalMatch);
            match.setIcon(QIcon::fromTheme("applications-education-language"));
            match.setText(currentString);
            match.setRelevance(currentRel);
            matches.append(match);

        }
    }

    context.addMatches(matches);
}

// https://developers.google.com/translate/v2/using_rest?hl=de#language-params

const QStringList Translator::supportedLanguages = QStringList() << "af" << "sq" << "ar" << "az"
					<< "eu" << "bn" << "be" << "bg" << "ca" << "zh-CN" << "zh-TW" << "hr" << "cs"
					<< "da" << "nl" << "en" << "eo" << "et" << "tl" << "fi" << "fr" << "gl"
					<< "ka" << "de" << "el" << "gu" << "ht" << "iw" << "hi" << "hu" << "is"
					<< "id" << "ga" << "it" << "ja" << "kn" << "ko" << "la" << "lv" << "lt"
					<< "mk" << "ms" << "mt" << "no" << "fa" << "pl" << "pt" << "ro" << "ru"
					<< "sr" << "sk" << "sl" << "es" << "sw" << "sv" << "ta" << "te" << "th"
					<< "tr" << "uk" << "ur" << "vi" << "cy" << "yi";
		
#include "moc_translator.cpp"