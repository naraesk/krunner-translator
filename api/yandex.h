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

#ifndef YANDEX_H
#define YANDEX_H

#include <KRunner/AbstractRunner>
#include <QtNetwork/QNetworkReply>

/**
 * API Implementation for Yandex https://tech.yandex.com/translate/doc/dg/concepts/api-overview-docpage/)
 */

class Yandex : public QObject
{

    Q_OBJECT

public:
    Yandex(Plasma::AbstractRunner*, Plasma::RunnerContext&, const QString &, const QPair<QString, QString> &, const QString &);
    ~Yandex();
    
private Q_SLOTS:
   void parseResult(QNetworkReply*);

Q_SIGNALS:
	void finished();
   
private:
   Plasma::AbstractRunner * m_runner;
   QNetworkAccessManager * m_manager;
   Plasma::RunnerContext m_context;
};

#endif
