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

#ifndef RUNNERTRANSLATOR_BING_H
#define RUNNERTRANSLATOR_BING_H

#include <KRunner/AbstractRunner>
#include <api/CommandLineEngine.h>

class Bing : public CommandLineEngine {
public:
    Bing(Plasma::AbstractRunner*, QAction *action);
    ~Bing() override;
    Plasma::QueryMatch translate(const QString &text, const QPair<QString, QString> &language) override;

private:
    Plasma::AbstractRunner * m_runner;
    QAction * m_action;
};

#endif //RUNNERTRANSLATOR_BING_H
