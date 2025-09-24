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

#include <klocalizedstring.h>

#include <QJsonArray>
#include <QJsonDocument>
#include <QUrlQuery>
#include <QCryptographicHash>
#include <QRandomGenerator>

Baidu::Baidu(KRunner::AbstractRunner *runner, KRunner::RunnerContext &context, const QString &text,
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
    QString signMD5 = QString::fromLatin1(hash.toHex());

    QUrlQuery query;
    query.addQueryItem(QStringLiteral("appid"), appid);
    query.addQueryItem(QStringLiteral("q"), text);
    query.addQueryItem(QStringLiteral("from"), langMapper(language.first));
    query.addQueryItem(QStringLiteral("to"), langMapper(language.second));
    query.addQueryItem(QStringLiteral("salt"), QString::number(salt));
    query.addQueryItem(QStringLiteral("sign"), signMD5);

    QString urlString = QStringLiteral("https://fanyi-api.baidu.com/api/trans/vip/translate?")
                        .append(QString::fromUtf8(QUrl(query.query(QUrl::FullyEncoded)).toEncoded()));
    auto request = QNetworkRequest(QUrl(urlString));
    //request.setSslConfiguration(QSslConfiguration::defaultConfiguration());
    request.setHeader(QNetworkRequest::ContentTypeHeader, QStringLiteral("application/x-www-form-urlencoded"));

    m_manager->get(request);
    connect(m_manager, &QNetworkAccessManager::finished, this, &Baidu::parseResult);
}

void Baidu::parseResult(QNetworkReply *reply) {
    if (reply->attribute(QNetworkRequest::HttpStatusCodeAttribute) != 200) {
        Q_EMIT finished();
        return;
    }

    const QString s = QString::fromUtf8(reply->readAll());
    const QJsonObject jsonObject = QJsonDocument::fromJson(s.toUtf8()).object();
    if (jsonObject.contains(QStringLiteral("error_code"))) {
        KRunner::QueryMatch match(m_runner);
        match.setIconName(QStringLiteral("dialog-error"));
        match.setText(i18n("(Baidu) Error code: %1", jsonObject.find(QStringLiteral("error_code")).value().toString()));
        match.setRelevance(1);
        m_context.addMatch(match);
    } else {
        QList<KRunner::QueryMatch> matches;
        const QJsonArray results = jsonObject.find(QStringLiteral("trans_result")).value().toArray();
        float relevance = 1;
        for (const QJsonValue &result: results) {
            KRunner::QueryMatch match(m_runner);
            match.setIconName(QStringLiteral("applications-education-language"));
            match.setText(result.toObject().find(QStringLiteral("dst")).value().toString());
            match.setMultiLine(true);
            match.setRelevance(relevance);
            matches.append(match);
            relevance -= 0.01;
        }
        m_context.addMatches(matches);
    }
    Q_EMIT finished();
}

QString Baidu::langMapper(QString lang) {
    if (lang == QStringLiteral("ko")) return QStringLiteral("kor");
    if (lang == QStringLiteral("bg")) return QStringLiteral("bul");
    if (lang == QStringLiteral("fi")) return QStringLiteral("fin");
    if (lang == QStringLiteral("sk")) return QStringLiteral("slo");
    if (lang == QStringLiteral("fr")) return QStringLiteral("fra");
    if (lang == QStringLiteral("ar")) return QStringLiteral("ara");
    if (lang == QStringLiteral("et")) return QStringLiteral("est");
    if (lang == QStringLiteral("sv")) return QStringLiteral("swe");
    if (lang == QStringLiteral("ja")) return QStringLiteral("jp");
    if (lang == QStringLiteral("es")) return QStringLiteral("spa");
    if (lang == QStringLiteral("da")) return QStringLiteral("dan");
    if (lang == QStringLiteral("ro")) return QStringLiteral("rom");
    return lang;
}

#include "moc_baidu.cpp"
