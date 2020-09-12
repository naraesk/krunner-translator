/******************************************************************************
 *  Copyright (C) 2013 – 2020 by David Baum <david.baum@naraesk.eu>           *
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

#ifndef RUNNERTRANSLATOR_TRANSLATESHELL_H
#define RUNNERTRANSLATOR_TRANSLATESHELL_H


#include <QtCore/QObject>
#include <KRunner/AbstractRunner>

class TranslateShell : public QObject {

    Q_OBJECT

public:
    TranslateShell(Plasma::AbstractRunner*, Plasma::RunnerContext&, const QString &, const QPair<QString, QString> &, QAction* action);

Q_SIGNALS:
    void finished();

private:
    Plasma::AbstractRunner * m_runner;
    Plasma::RunnerContext m_context;
};


#endif //RUNNERTRANSLATOR_TRANSLATESHELL_H
