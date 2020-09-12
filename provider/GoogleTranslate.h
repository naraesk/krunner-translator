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

#ifndef RUNNERTRANSLATOR_GOOGLETRANSLATE_H
#define RUNNERTRANSLATOR_GOOGLETRANSLATE_H


#include <QtCore/QObject>
#include <KRunner/AbstractRunner>

class GoogleTranslate : public QObject {

    Q_OBJECT

public:
    GoogleTranslate(Plasma::AbstractRunner*, Plasma::RunnerContext&, QAction * action);
    void translate(const QString &text, const QPair<QString, QString> &language);

private Q_SLOTS:
    void run(QString &result);

private:
    Plasma::AbstractRunner * m_runner;
    Plasma::RunnerContext m_context;
    QAction * m_action;
};

#endif //RUNNERTRANSLATOR_GOOGLETRANSLATE_H
