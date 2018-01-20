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

#ifndef GLOSBE_H
#define GLOSBE_H

#include <KRunner/AbstractRunner>
#include <QtNetwork/QNetworkReply>
#include <helper.h>

/**
 * API Implementation for Glosbe (https://glosbe.com/a-api)
 */

class Glosbe : public QObject
{

    Q_OBJECT

public:
    Glosbe(Plasma::AbstractRunner*, Plasma::RunnerContext&, const QString &, const QPair<QString, QString> &, const bool);
    Glosbe(Plasma::AbstractRunner*, Plasma::RunnerContext&, const QString &, const QPair<QString, QString> &);
    ~Glosbe();
    
private Q_SLOTS:
   void parseExamples(QNetworkReply*);
   void parseResult(QNetworkReply*);

Q_SIGNALS:
   void finished();
   
private:
   Plasma::AbstractRunner * m_runner;
   QNetworkAccessManager * m_manager;
   Plasma::RunnerContext m_context;
};

#endif
