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

#include "translateShellProcess.h"

TranslateShellProcess::TranslateShellProcess(QObject *parent) : QProcess(parent) {
}

TranslateShellProcess::TranslateShellProcess(const QString &engine_, QObject *parent) : QProcess(parent),
                                                                                        engine(engine_) {
}

TranslateShellProcess::~TranslateShellProcess() = default;

QString TranslateShellProcess::translate(const QPair<QString, QString> &language, const QString &text) {
    QStringList arguments;
    arguments << language.first + QStringLiteral(":") + language.second
              << text
              << QStringLiteral("--brief")
              << QStringLiteral("-e")
              << engine;
    start(QStringLiteral("trans"), arguments);
    waitForFinished();
    return QString::fromUtf8(readLine());
}

void TranslateShellProcess::play(const QString &text) {
    QStringList arguments;
    arguments << text
              << QStringLiteral("-speak")
              << QStringLiteral("-no-translate");
    start(QStringLiteral("trans"), arguments);
    waitForFinished();
}
