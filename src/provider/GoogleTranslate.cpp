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

#include "GoogleTranslate.h"
#include "src/translateShellProcess.h"

GoogleTranslate::GoogleTranslate(KRunner::AbstractRunner *runner)
        : match(runner) {
}

KRunner::QueryMatch GoogleTranslate::translate(const QString &text, const QPair<QString, QString> &language) {
    TranslateShellProcess process;
    QString result = process.translate(language, text);
    match.setData(QStringLiteral("audio"));
    match.setIconName(QStringLiteral("applications-education-language"));
    match.setText(result);
    match.setMultiLine(true);
    match.setSubtext(QStringLiteral("Google Translate"));
    match.setRelevance(1);
    return match;
}

GoogleTranslate::~GoogleTranslate() = default;
