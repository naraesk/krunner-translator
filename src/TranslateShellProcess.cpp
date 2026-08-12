/******************************************************************************
 *  Copyright (C) 2013 – 2020 by David Baum <david.baum@naraesk.eu>           *
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

#include "TranslateShellProcess.h"
#include "src/language/Language.h"
#include "TranslationQuery.h"
#include <QDebug>
#include <QEventLoop>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QStandardPaths>
#include <QString>
#include <QUrl>
#include <QUrlQuery>

TranslateShellProcess::TranslateShellProcess(QObject *parent) : QProcess(parent) {
}

TranslateShellProcess::TranslateShellProcess(const QString &engine_, QObject *parent) : QProcess(parent),
                                                                                        engine(engine_) {
}

TranslateShellProcess::~TranslateShellProcess() = default;

QString TranslateShellProcess::translate(const TranslationQuery* query) {
    QStringList arguments;
    qDebug() << query->getSourceAbbreviation();
    arguments << query->getSourceAbbreviation() + QStringLiteral(":") + query->getTargetAbbreviation()
              << query->getText()
              << QStringLiteral("--brief")
              << QStringLiteral("-e")
              << engine;
    start(QStringLiteral("trans"), arguments);
    waitForFinished();
    QString composeOutput(QString::fromUtf8(readLine().trimmed()));
    return composeOutput;
}

void TranslateShellProcess::play(const QString &text, const QString &lang) {
    // Download TTS audio over HTTPS (no translate-shell, no plain HTTP),
    // then play it with ffplay which is bundled with ffmpeg.
    const QString ffplayPath = QStandardPaths::findExecutable(QStringLiteral("ffplay"));
    if (ffplayPath.isEmpty()) {
        qWarning() << "krunner-translator: ffplay not found, cannot play TTS audio";
        return;
    }

    const QString ttsLang = lang.isEmpty() ? QStringLiteral("en") : lang;
    // translate_tts caps q at 200 characters; fetch and play in chunks
    for (int i = 0; i < text.size(); i += 200) {
        const QString chunk = text.mid(i, 200);
        QUrl url(QStringLiteral("https://translate.google.com/translate_tts"));
        QUrlQuery q;
        q.addQueryItem(QStringLiteral("ie"), QStringLiteral("UTF-8"));
        q.addQueryItem(QStringLiteral("client"), QStringLiteral("tw-ob"));
        // QUrlQuery does not encode '+', which Google would decode as a space
        q.addQueryItem(QStringLiteral("q"),
                       QString::fromUtf8(QUrl::toPercentEncoding(chunk)));
        q.addQueryItem(QStringLiteral("tl"), ttsLang);
        url.setQuery(q);

        QNetworkAccessManager manager;
        QNetworkRequest request(url);
        request.setTransferTimeout(10000);
        QEventLoop loop;
        QNetworkReply *reply = manager.get(request);
        QObject::connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
        loop.exec();
        const QByteArray audio = reply->readAll();
        const bool ok = reply->error() == QNetworkReply::NoError;
        const QString errorString = reply->errorString();
        reply->deleteLater();
        if (!ok || audio.isEmpty()) {
            qWarning() << "krunner-translator: TTS download failed:" << errorString;
            return;
        }

        QProcess ffplay;
        ffplay.start(ffplayPath,
                     {QStringLiteral("-nodisp"), QStringLiteral("-autoexit"),
                      QStringLiteral("-loglevel"), QStringLiteral("quiet"),
                      QStringLiteral("-i"), QStringLiteral("pipe:")});
        ffplay.write(audio);
        ffplay.closeWriteChannel();
        // generous cap so long clips finish; kill only guards a hung ffplay
        if (!ffplay.waitForFinished(60000)) {
            ffplay.kill();
            ffplay.waitForFinished();
        }
    }
}
