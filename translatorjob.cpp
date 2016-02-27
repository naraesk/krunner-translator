/******************************************************************************
 *  Copyright (C) 2013 – 2015 by David Baum <david.baum@naraesk.eu>           *
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

#include "translatorjob.h"
#include <QUrlQuery>

TranslatorJob::TranslatorJob(const QString &text, const QPair<QString, QString> &language)
: m_manager(0)
{
	m_manager = new QNetworkAccessManager(this);
	
	QNetworkRequest request(QUrl("https://glosbe.com/gapi/translate"));
        request.setSslConfiguration(QSslConfiguration::defaultConfiguration());
	request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
	
        QUrl postData;
        QUrlQuery query;
        query.addQueryItem("client", "gtx");
        query.addQueryItem("sl", language.first);
        query.addQueryItem("tl",language.second);
        query.addQueryItem("dt","t");
        query.addQueryItem("q", text);
        postData = query.query(QUrl::FullyEncoded).toUtf8();

        qDebug() << postData.toEncoded() << endl;
	
        m_manager -> post(request, postData.toEncoded());
	connect(m_manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(jobCompleted(QNetworkReply*)));
	connect(m_manager, SIGNAL(sslErrors(QNetworkReply*, const QList<QSslError> &)), this, SLOT(jobCompleted2(QNetworkReply*, const QList<QSslError> &)));
}

void TranslatorJob::jobCompleted(QNetworkReply* reply)
{
	m_result = QString::fromUtf8(reply->readAll());
        qDebug() << "result" << endl;
        qDebug() << m_result << endl;
	emit finished();
}

void TranslatorJob::jobCompleted2(QNetworkReply* reply, const QList<QSslError> & errors)
{
	qDebug() << "sslErrors " << endl;
}

QString TranslatorJob::result() const
{
	return m_result;
}