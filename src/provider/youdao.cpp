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

#include "youdao.h"

#include <klocalizedstring.h>

#include <QJsonArray>
#include <QJsonDocument>
#include <QUrlQuery>
#include <QCryptographicHash>
#include <QDateTime>
#include <QUuid>

Youdao::Youdao(KRunner::AbstractRunner *runner, KRunner::RunnerContext &context, const QString &text,
               const QPair<QString, QString> &language, const QString &appid, const QString &appSec)
        : m_runner(runner), m_context(context) {
    m_manager = new QNetworkAccessManager(this);

    QString salt = QUuid::createUuid().toString().mid(1, 36);
    qint64 timestamp = QDateTime::currentSecsSinceEpoch();

    QString input;
    if (text.length() > 20) {
        input = text.left(10) + QString::number(text.length()) + text.right(10);
    } else {
        input = text;
    }

    QString sign;
    // sha256(应用ID+input+salt+curtime+应用密钥)
    sign.append(appid);
    sign.append(input);
    sign.append(salt);
    sign.append(QString::number(timestamp));
    sign.append(appSec);
    QByteArray hash = QCryptographicHash::hash(sign.toUtf8(), QCryptographicHash::Sha256);
    //QString signSha256 = QString::fromUtf8(hash);

    QUrlQuery postData;
    postData.addQueryItem(QStringLiteral("q"), text);
    postData.addQueryItem(QStringLiteral("from"), langMapper(language.first));
    postData.addQueryItem(QStringLiteral("to"), langMapper(language.second));
    postData.addQueryItem(QStringLiteral("appKey"), appid);
    postData.addQueryItem(QStringLiteral("salt"), salt);
    postData.addQueryItem(QStringLiteral("sign"), QString::fromLatin1(hash.toHex()));
    postData.addQueryItem(QStringLiteral("signType"), QStringLiteral("v3"));
    postData.addQueryItem(QStringLiteral("curtime"), QString::number(timestamp));

    QNetworkRequest request;
    request.setUrl(QUrl(QStringLiteral("https://openapi.youdao.com/api")));
    request.setHeader(QNetworkRequest::ContentTypeHeader, QStringLiteral("application/x-www-form-urlencoded"));
    //request.setSslConfiguration(QSslConfiguration::defaultConfiguration());

    m_manager->post(request, postData.toString(QUrl::FullyEncoded).toUtf8());
    connect(m_manager, &QNetworkAccessManager::finished, this, &Youdao::parseResult);
}

void Youdao::parseResult(QNetworkReply *reply) {
    if (reply->attribute(QNetworkRequest::HttpStatusCodeAttribute) != 200) {
        Q_EMIT finished();
        return;
    }

    const QString s = QString::fromUtf8(reply->readAll());
    const QJsonObject jsonObject = QJsonDocument::fromJson(s.toUtf8()).object();
    const int errorCode = jsonObject.find(QStringLiteral("errorCode")).value().toInt();
    if (errorCode == 0) {

        QList<KRunner::QueryMatch> matches;

        const QJsonArray results = jsonObject.find(QStringLiteral("translation")).value().toArray();
        float relevance = 1;
        for (const QJsonValue &result: results) {
            KRunner::QueryMatch match(m_runner);
            match.setIconName(QStringLiteral("applications-education-language"));
            match.setText(result.toString());
            match.setMultiLine(true);
            match.setRelevance(relevance);
            matches.append(match);
            relevance -= 0.01;
        }
        // for youdao basic dict
        if (jsonObject.contains(QStringLiteral("basic"))) {
            const QJsonArray baseExplains = jsonObject.find(QStringLiteral("basic")).value().toObject().find(
                    QStringLiteral("explains")).value().toArray();
            for (const QJsonValue &explain: baseExplains) {
                KRunner::QueryMatch match(m_runner);
                match.setIconName(QStringLiteral("applications-education-language"));
                match.setText(explain.toString());
                match.setMultiLine(true);
                match.setRelevance(relevance);
                matches.append(match);
                relevance -= 0.01;
            }
        }
        m_context.addMatches(matches);
    } else {
        KRunner::QueryMatch match(m_runner);
        match.setIconName(QStringLiteral("dialog-error"));
        match.setText(i18n("(Youdao) Error code: %1", QString::number(errorCode)));
        match.setRelevance(1);
        m_context.addMatch(match);
    }
    Q_EMIT finished();
}

QString Youdao::langMapper(QString lang) {
    if (lang == QStringLiteral("zh")) return QStringLiteral("zs-CHS");
    return lang;
}

#include "moc_youdao.cpp"
