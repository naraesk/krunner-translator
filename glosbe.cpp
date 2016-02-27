/******************************************************************************
 *  Copyright (C) 2013 – 2016 by David Baum <david.baum@naraesk.eu>           *
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

#include "glosbe.h"

#include <QJsonArray>
#include <QJsonDocument>
#include <QUrlQuery>

Glosbe::Glosbe(Plasma::AbstractRunner * runner, Plasma::RunnerContext& context, const QString &text, const QPair<QString, QString> &language)
: m_runner (runner), m_context (context)
{
    m_manager = new QNetworkAccessManager(this);

    QUrlQuery query;
    query.addQueryItem("from", language.first);
    query.addQueryItem("dest",language.second);
    query.addQueryItem("format","json");
    query.addQueryItem("phrase", text);
    query.addQueryItem("tm", "false");
    query.addQueryItem("pretty", "true");
        
    QNetworkRequest request(QUrl("https://glosbe.com/gapi/translate?" + QUrl(query.query(QUrl::FullyEncoded).toUtf8()).toEncoded()));
    request.setSslConfiguration(QSslConfiguration::defaultConfiguration());
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

    m_manager -> get(request);
    connect(m_manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(parseResult(QNetworkReply*)));
}

Glosbe::~Glosbe()
{
}

void Glosbe::parseResult(QNetworkReply* reply)
{
    QJsonObject jsonObject = QJsonDocument::fromJson(QString::fromUtf8(reply->readAll()).toUtf8()).object();

    if (jsonObject.value("result").toString() != "ok") return;
    
    QList<Plasma::QueryMatch> matches;
    QJsonArray tuc = jsonObject.find("tuc").value().toArray();
    QVariantList v = tuc.toVariantList();
    float relevance = 1;
    foreach(QJsonValue a, tuc) {
        QString s = a.toObject().value("phrase").toObject().value("text").toString();
        if (s.size() > 0) {
            relevance -= 0.01;
            Plasma::QueryMatch match(m_runner);
            match.setType(Plasma::QueryMatch::InformationalMatch);
            match.setIcon(QIcon::fromTheme("applications-education-language"));
            match.setText(s);
            match.setRelevance(relevance);
            matches.append(match);
        }
    }   
    m_context.addMatches(matches);
    emit(finished());
}

#include "moc_glosbe.cpp"
