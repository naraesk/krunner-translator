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

#ifndef YOUDAO_H
#define YOUDAO_H

#include <KRunner/AbstractRunner>
#include <QtNetwork/QNetworkReply>

/**
 * API Implementation for Youdao https://ai.youdao.com/DOCSIRMA/html/%E8%87%AA%E7%84%B6%E8%AF%AD%E8%A8%80%E7%BF%BB%E8%AF%91/API%E6%96%87%E6%A1%A3/%E6%96%87%E6%9C%AC%E7%BF%BB%E8%AF%91%E6%9C%8D%E5%8A%A1/%E6%96%87%E6%9C%AC%E7%BF%BB%E8%AF%91%E6%9C%8D%E5%8A%A1-API%E6%96%87%E6%A1%A3.html)
 */

class Youdao : public QObject
{

    Q_OBJECT

public:
    Youdao(Plasma::AbstractRunner*, Plasma::RunnerContext&, const QString &, const QPair<QString, QString> &, const QString &, const QString &);

private Q_SLOTS:
   void parseResult(QNetworkReply*);

Q_SIGNALS:
	void finished();
   
private:
   Plasma::AbstractRunner * m_runner;
   QNetworkAccessManager * m_manager;
   Plasma::RunnerContext m_context;
   QString langMapper(QString);
};

#endif
