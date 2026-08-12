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

#include "GoogleTranslate.h"
#include <QEventLoop>
#include <QIcon>
#include <QJsonArray>
#include <QJsonDocument>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QUrl>
#include <QUrlQuery>

GoogleTranslate::GoogleTranslate(KRunner::AbstractRunner *runner)
        : match(runner) {
}

KRunner::QueryMatch GoogleTranslate::translate(const TranslationQuery* query) {
    // Use the HTTPS gtx endpoint directly instead of the legacy translate-shell
    // plain-HTTP path.
    QUrl url(QStringLiteral("https://translate.googleapis.com/translate_a/single"));
    QUrlQuery q;
    q.addQueryItem(QStringLiteral("client"), QStringLiteral("gtx"));
    // Use the parser's explicit source language when the user declared one
    // (e.g. "de-en: house"); otherwise auto-detect, because the configured
    // default source makes Google return arbitrary text unchanged.
    q.addQueryItem(QStringLiteral("sl"),
                   query->isSourceExplicit() ? query->getSourceAbbreviation()
                                             : QStringLiteral("auto"));
    q.addQueryItem(QStringLiteral("tl"), query->getTargetAbbreviation());
    q.addQueryItem(QStringLiteral("dt"), QStringLiteral("t"));
    // QUrlQuery does not encode '+', which Google would decode as a space;
    // percent-encode the payload explicitly.
    q.addQueryItem(QStringLiteral("q"),
                   QString::fromUtf8(QUrl::toPercentEncoding(query->getText())));
    url.setQuery(q);

    const QString result = parseResponse(httpGet(url));
    // remember the target language so the play action can read it back
    match.setData(query->getTargetAbbreviation());
    match.setCategoryRelevance(KRunner::QueryMatch::CategoryRelevance::Highest);
    match.setIcon(QIcon::fromTheme(QStringLiteral("applications-education-language")));
    match.setText(result);
    match.setMultiLine(true);
    match.setSubtext(QStringLiteral("Google Translate"));
    match.setRelevance(1);
    return match;
}

QByteArray GoogleTranslate::httpGet(const QUrl &url) const {
    QNetworkAccessManager manager;
    QNetworkRequest request(url);
    request.setTransferTimeout(15000);
    QEventLoop loop;
    QNetworkReply *reply = manager.get(request);
    QObject::connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();
    const QByteArray data = reply->readAll();
    const bool ok = reply->error() == QNetworkReply::NoError;
    const QString errorString = reply->errorString();
    reply->deleteLater();
    if (!ok) {
        qWarning() << "krunner-translator: translate request failed:" << errorString;
        return QByteArray();
    }
    return data;
}

QString GoogleTranslate::parseResponse(const QByteArray &data) const {
    const QJsonDocument doc = QJsonDocument::fromJson(data);
    if (!doc.isArray()) {
        return QString();
    }
    const QJsonArray root = doc.array();
    if (root.isEmpty() || !root.at(0).isArray()) {
        return QString();
    }
    // long text is returned in multiple segments; concatenate them all
    QString out;
    const QJsonArray sentences = root.at(0).toArray();
    for (const auto &s : sentences) {
        if (!s.isArray()) {
            continue;
        }
        const QJsonArray part = s.toArray();
        if (!part.isEmpty() && part.at(0).isString()) {
            out += part.at(0).toString();
        }
    }
    return out;
}

GoogleTranslate::~GoogleTranslate() = default;
