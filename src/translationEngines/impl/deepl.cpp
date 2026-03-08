/******************************************************************************
 *  Copyright (C) 2024 by Sheng Huang <shenghuang147@gmail.com>               *
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

#include "deepl.h"

#include <QJsonArray>
#include <QJsonDocument>
#include <QUrlQuery>
#include <QIcon>
#include <src/TranslationQuery.h>

Deepl::Deepl(KRunner::AbstractRunner *runner, KRunner::RunnerContext &context, const TranslationQuery* query, const QString &apiKey)
        : m_runner(runner), m_context(context) {
    m_manager = new QNetworkAccessManager(this);

    QUrlQuery urlQuery;
    urlQuery.addQueryItem(QStringLiteral("text"), query->getText());
    urlQuery.addQueryItem(QStringLiteral("source_lang"), langMapper(query->getSourceAbbreviation()).toUpper());
    urlQuery.addQueryItem(QStringLiteral("target_lang"), langMapper(query->getTargetAbbreviation()).toUpper());

    QNetworkRequest request(QUrl(QStringLiteral("https://api-free.deepl.com/v2/translate")));
    request.setHeader(QNetworkRequest::ContentTypeHeader, QStringLiteral("application/x-www-form-urlencoded"));
    request.setRawHeader("Authorization", QStringLiteral("DeepL-Auth-Key %1").arg(apiKey).toUtf8());

    m_manager->post(request, urlQuery.query(QUrl::FullyEncoded).toUtf8());
    connect(m_manager, &QNetworkAccessManager::finished, this, &Deepl::parseResult);
}

void Deepl::parseResult(QNetworkReply *reply) {
    if (reply->attribute(QNetworkRequest::HttpStatusCodeAttribute) != 200) {
        Q_EMIT finished();
        return;
    }

    const QString s = QString::fromUtf8(reply->readAll());
    const QJsonObject jsonObject = QJsonDocument::fromJson(s.toUtf8()).object();
    if (jsonObject.contains(QStringLiteral("message"))) {
        KRunner::QueryMatch match(m_runner);
        match.setCategoryRelevance(KRunner::QueryMatch::CategoryRelevance::Low);
        match.setIcon(QIcon::fromTheme(QStringLiteral("dialog-error")));
        match.setText(
                QStringLiteral("(DeepL) Error: %1").arg(jsonObject.find(QStringLiteral("message")).value().toString()));
        match.setRelevance(1);
        m_context.addMatch(match);
    } else {
        QList<KRunner::QueryMatch> matches;
        const QJsonArray results = jsonObject.find(QStringLiteral("translations")).value().toArray();
        float relevance = 1;
        for (const QJsonValue result: results) {
            KRunner::QueryMatch match(m_runner);
            match.setCategoryRelevance(KRunner::QueryMatch::CategoryRelevance::Moderate);
            match.setIcon(QIcon::fromTheme(QStringLiteral("applications-education-language")));
            match.setText(result.toObject().find(QStringLiteral("text")).value().toString());
            match.setSubtext(QStringLiteral("DeepL"));
            match.setMultiLine(true);
            match.setRelevance(relevance);
            matches.append(match);
            relevance -= 0.01;
        }
        m_context.addMatches(matches);
    }
    Q_EMIT finished();
}

QString Deepl::langMapper(QString lang) {
    if (lang == QLatin1String("zh")) return QStringLiteral("ZH");
    else if (lang == QLatin1String("pt")) return QStringLiteral("PT-PT");
    else { return lang; }
}

#include "moc_deepl.cpp"
