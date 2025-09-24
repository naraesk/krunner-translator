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

#ifndef RUNNERTRANSLATOR_TRANSLATESHELLPROCESS_H
#define RUNNERTRANSLATOR_TRANSLATESHELLPROCESS_H

#include <QProcess>
#include <QString>

class TranslateShellProcess : public QProcess
{
Q_OBJECT
public:
    explicit TranslateShellProcess(QObject *parent = nullptr);
    explicit TranslateShellProcess(const QString &engine, QObject *parent = nullptr);
    ~TranslateShellProcess() override;

public Q_SLOTS:
    QString translate(const QPair<QString, QString> &language, const QString &text);
    void play(const QString &text);
private:
    QString engine = QStringLiteral("google");
};

#endif //RUNNERTRANSLATOR_TRANSLATESHELLPROCESS_H
