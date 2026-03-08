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
#include <QIcon>
#include <src/TranslationQuery.h>

Baidu::Baidu(KRunner::AbstractRunner *runner, KRunner::RunnerContext &context, const TranslationQuery* query, const QString &appid, const QString &key)
        : m_runner(runner), m_context(context) {
    m_manager = new QNetworkAccessManager(this);

    QRandomGenerator randomGenerator = QRandomGenerator(QTime(0, 0, 0).secsTo(QTime::currentTime()));

    quint32 salt = randomGenerator.generate();

    QByteArray sign;
    sign.append(appid.toUtf8());
    sign.append(query->getText().toUtf8());
    sign.append(QString::number(salt).toUtf8());
    sign.append(key.toUtf8());
    QByteArray hash = QCryptographicHash::hash(sign, QCryptographicHash::Md5);
    QString signMD5 = QString::fromLatin1(hash.toHex());

    QUrlQuery urlQuery;
    urlQuery.addQueryItem(QStringLiteral("appid"), appid);
    urlQuery.addQueryItem(QStringLiteral("q"), query->getText());
    // TODO fix
    //urlQuery.addQueryItem(QStringLiteral("from"), langMapper(language.first.getAbbreviation()));
    //urlQuery.addQueryItem(QStringLiteral("to"), langMapper(language.second.getAbbreviation()));
    urlQuery.addQueryItem(QStringLiteral("salt"), QString::number(salt));
    urlQuery.addQueryItem(QStringLiteral("sign"), signMD5);


    QNetworkRequest request(QUrl(QStringLiteral("https://fanyi-api.baidu.com/api/trans/vip/translate?") +
                                 urlQuery.query(QUrl::FullyEncoded)));
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
        match.setCategoryRelevance(KRunner::QueryMatch::CategoryRelevance::Low);
        match.setIcon(QIcon::fromTheme(QStringLiteral("dialog-error")));
        match.setText(
                QStringLiteral("(Baidu) Error code: %1").arg(jsonObject.find(QStringLiteral("error_code")).value().toString()));
        match.setRelevance(1);
        m_context.addMatch(match);
    } else {
        QList<KRunner::QueryMatch> matches;
        const QJsonArray results = jsonObject.find(QStringLiteral("trans_result")).value().toArray();
        float relevance = 1;
        for (const QJsonValue result: results) {
            KRunner::QueryMatch match(m_runner);
            match.setCategoryRelevance(KRunner::QueryMatch::CategoryRelevance::Moderate);
            match.setIcon(QIcon::fromTheme(QStringLiteral("applications-education-language")));
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
    QString lang2 = lang;
    if (lang == QLatin1String("ko")) return QStringLiteral("kor");
    else if (lang == QLatin1String("bg")) return QStringLiteral("bul");
    else if (lang == QLatin1String("fi")) return QStringLiteral("fin");
    else if (lang == QLatin1String("sk")) return QStringLiteral("slo");
    else if (lang == QLatin1String("fr")) return QStringLiteral("fra");
    else if (lang == QLatin1String("ar")) return QStringLiteral("ara");
    else if (lang == QLatin1String("et")) return QStringLiteral("est");
    else if (lang == QLatin1String("sv")) return QStringLiteral("swe");
    else if (lang == QLatin1String("ja")) return QStringLiteral("jp");
    else if (lang == QLatin1String("es")) return QStringLiteral("spa");
    else if (lang == QLatin1String("da")) return QStringLiteral("dan");
    else if (lang == QLatin1String("ro")) return QStringLiteral("rom");
    else { return lang2; }
}

#include "moc_baidu.cpp"
