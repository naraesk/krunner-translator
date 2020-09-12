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

#include "yandex.h"

#include <QJsonArray>
#include <QJsonDocument>
#include <QUrlQuery>
#include <QDebug>

Yandex::Yandex(Plasma::AbstractRunner * runner, Plasma::RunnerContext& context, const QString &text, const QPair<QString, QString> &language, const QString &key)
: m_runner (runner), m_context (context)
{
    m_manager = new QNetworkAccessManager(this);

    QUrlQuery query;
    query.addQueryItem("key", key);
    query.addQueryItem("text", text);
    query.addQueryItem("lang", language.first + "-" + language.second);
        
    QNetworkRequest request(QUrl("https://translate.yandex.net/provider/v1.5/tr.json/translate?" + QUrl(query.query(QUrl::FullyEncoded).toUtf8()).toEncoded()));
    request.setSslConfiguration(QSslConfiguration::defaultConfiguration());
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

    m_manager -> get(request);
    connect(m_manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(parseResult(QNetworkReply*)));
}

void Yandex::parseResult(QNetworkReply* reply)
{
    if (reply -> attribute(QNetworkRequest::HttpStatusCodeAttribute) != 200) {
        emit finished();
        return;
    }

    const QString s = QString::fromUtf8(reply->readAll());
    const QJsonObject jsonObject = QJsonDocument::fromJson(s.toUtf8()).object();
    
    QList<Plasma::QueryMatch> matches;
    const QJsonArray texts = jsonObject.find("text").value().toArray();
    float relevance = 1;
    for(const auto& text: texts) {
        Plasma::QueryMatch match(m_runner);
            match.setType(Plasma::QueryMatch::InformationalMatch);
            match.setIcon(QIcon::fromTheme("applications-education-language"));
            match.setText(text.toString());
            match.setRelevance(relevance);
            matches.append(match);
            relevance -= 0.01;
    }
    m_context.addMatches(matches);
    emit finished();
}

#include "moc_yandex.cpp"
