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

#include "Bing.h"
#include "src/translateShellProcess.h"

Bing::Bing(Plasma::AbstractRunner *runner)
        : match(runner) {
}

Plasma::QueryMatch Bing::translate(const QString &text, const QPair<QString, QString> &language) {
    TranslateShellProcess process("bing");
    QString result = process.translate(language, text);
    if (result == "\n") { // empty result
        match.setType(Plasma::QueryMatch::NoMatch);
    } else {
        match.setData(QStringLiteral("audio"));
        match.setType(Plasma::QueryMatch::ExactMatch);
        match.setIcon(QIcon::fromTheme(QStringLiteral("applications-education-language")));
        match.setText(result);
        match.setMultiLine(true);
        match.setSubtext(QStringLiteral("Bing"));
        match.setRelevance(1);
    }
    return match;
}

Bing::~Bing() = default;
