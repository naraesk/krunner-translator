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

#include <QtGlobal>
#include <QJsonArray>
#include <QJsonDocument>
#include <QUrlQuery>
#include <QDebug>
#include <QCryptographicHash>
#include <QDateTime>
#include <QUuid>

Youdao::Youdao(Plasma::AbstractRunner *runner, Plasma::RunnerContext &context, const QString &text,
               const QPair<QString, QString> &language, const QString &appid, const QString &appSec)
        : m_runner(runner), m_context(context) {
    m_manager = new QNetworkAccessManager(this);

    QString salt = QUuid::createUuid().toString().mid(1, 36);
    qint64 timestamp = QDateTime::currentDateTime().toSecsSinceEpoch();

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
    postData.addQueryItem(QStringLiteral("sign"), hash.toHex());
    postData.addQueryItem(QStringLiteral("signType"), "v3");
    postData.addQueryItem(QStringLiteral("curtime"), QString::number(timestamp));

    QNetworkRequest request;
    request.setUrl(QUrl("http://openapi.youdao.com/provider"));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    //request.setSslConfiguration(QSslConfiguration::defaultConfiguration());

    m_manager->post(request, postData.toString(QUrl::FullyEncoded).toUtf8());
    connect(m_manager, &QNetworkAccessManager::finished, this, &Youdao::parseResult);
}

void Youdao::parseResult(QNetworkReply *reply) {
    if (reply->attribute(QNetworkRequest::HttpStatusCodeAttribute) != 200) {
        emit finished();
        return;
    }

    const QString s = QString::fromUtf8(reply->readAll());
    const QJsonObject jsonObject = QJsonDocument::fromJson(s.toUtf8()).object();
    const int errorCode = jsonObject.find("errorCode").value().toInt();
    if (errorCode == 0) {

        QList<Plasma::QueryMatch> matches;

        const QJsonArray results = jsonObject.find("translation").value().toArray();
        float relevance = 1;
        for (const QJsonValue result: results) {
            Plasma::QueryMatch match(m_runner);
            match.setType(Plasma::QueryMatch::InformationalMatch);
            match.setIcon(QIcon::fromTheme("applications-education-language"));
            match.setText(result.toString());
            match.setRelevance(relevance);
            matches.append(match);
            relevance -= 0.01;
        }
        // for youdao basic dict
        if (jsonObject.contains("basic")) {
            const QJsonArray baseExplains = jsonObject.find("basic").value().toObject().find(
                    "explains").value().toArray();
            for (const QJsonValue explain: baseExplains) {
                Plasma::QueryMatch match(m_runner);
                match.setType(Plasma::QueryMatch::InformationalMatch);
                match.setIcon(QIcon::fromTheme("applications-education-language"));
                match.setText(explain.toString());
                match.setRelevance(relevance);
                matches.append(match);
                relevance -= 0.01;
            }
        }
        m_context.addMatches(matches);
    } else {
        Plasma::QueryMatch match(m_runner);
        match.setType(Plasma::QueryMatch::HelperMatch);
        match.setIcon(QIcon::fromTheme(QStringLiteral("dialog-error")));
        match.setText(QString::fromUtf8("(Youdao) Error code: %1").arg(QString::number(errorCode)));
        match.setRelevance(1);
        m_context.addMatch(match);
    }
    emit finished();
}

QString Youdao::langMapper(QString lang) {
    QString lang2 = lang;
    if (lang == "zh") return "zh-CHS";
    else { return lang2; }
}

#include "moc_youdao.cpp"
