/******************************************************************************
 *  Copyright (C) 2020 by P3psi Boo <boo@p3psi.xyz>                           *
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

#include "baidu.h"

#include <QtGlobal>
#include <QJsonArray>
#include <QJsonDocument>
#include <QUrlQuery>
#include <QDebug>
#include <QCryptographicHash>
#include <QRandomGenerator>

Baidu::Baidu(Plasma::AbstractRunner *runner, Plasma::RunnerContext &context, const QString &text,
             const QPair<QString, QString> &language, const QString &appid, const QString &key)
        : m_runner(runner), m_context(context) {
    m_manager = new QNetworkAccessManager(this);

    QRandomGenerator randomGenerator = QRandomGenerator(QTime(0, 0, 0).secsTo(QTime::currentTime()));

    quint32 salt = randomGenerator.generate();

    QString sign;
    sign.append(appid);
    sign.append(text);
    sign.append(QString::number(salt));
    sign.append(key);
    QByteArray hash = QCryptographicHash::hash(sign.toUtf8(), QCryptographicHash::Md5);
    QString signMD5 = hash.toHex();

    QUrlQuery query;
    query.addQueryItem("appid", appid);
    query.addQueryItem("q", text);
    query.addQueryItem("from", langMapper(language.first));
    query.addQueryItem("to", langMapper(language.second));
    query.addQueryItem("salt", QString::number(salt));
    query.addQueryItem("sign", signMD5);


    QNetworkRequest request(QUrl("https://fanyi-api.baidu.com/api/trans/vip/translate?" +
                                 QUrl(query.query(QUrl::FullyEncoded).toUtf8()).toEncoded()));
    //request.setSslConfiguration(QSslConfiguration::defaultConfiguration());
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

    m_manager->get(request);
    connect(m_manager, &QNetworkAccessManager::finished, this, &Baidu::parseResult);
}

void Baidu::parseResult(QNetworkReply *reply) {
    if (reply->attribute(QNetworkRequest::HttpStatusCodeAttribute) != 200) {
        emit finished();
        return;
    }

    const QString s = QString::fromUtf8(reply->readAll());
    const QJsonObject jsonObject = QJsonDocument::fromJson(s.toUtf8()).object();
    if (jsonObject.contains(QStringLiteral("error_code"))) {
        Plasma::QueryMatch match(m_runner);
        match.setType(Plasma::QueryMatch::HelperMatch);
        match.setIcon(QIcon::fromTheme(QStringLiteral("dialog-error")));
        match.setText(
                QString::fromUtf8("(Baidu) Error code: %1").arg(jsonObject.find("error_code").value().toString()));
        match.setRelevance(1);
        m_context.addMatch(match);
    } else {
        QList<Plasma::QueryMatch> matches;
        const QJsonArray results = jsonObject.find("trans_result").value().toArray();
        float relevance = 1;
        for (const QJsonValue result: results) {
            Plasma::QueryMatch match(m_runner);
            match.setType(Plasma::QueryMatch::InformationalMatch);
            match.setIcon(QIcon::fromTheme("applications-education-language"));
            match.setText(result.toObject().find("dst").value().toString());
            match.setMultiLine(true);
            match.setRelevance(relevance);
            matches.append(match);
            relevance -= 0.01;
        }
        m_context.addMatches(matches);
    }
    emit finished();
}

QString Baidu::langMapper(QString lang) {
    QString lang2 = lang;
    if (lang == "ko") return "kor";
    else if (lang == "bg") return "bul";
    else if (lang == "fi") return "fin";
    else if (lang == "sk") return "slo";
    else if (lang == "fr") return "fra";
    else if (lang == "ar") return "ara";
    else if (lang == "et") return "est";
    else if (lang == "sv") return "swe";
    else if (lang == "ja") return "jp";
    else if (lang == "es") return "spa";
    else if (lang == "da") return "dan";
    else if (lang == "ro") return "rom";
    else { return lang2; }
}

#include "moc_baidu.cpp"
