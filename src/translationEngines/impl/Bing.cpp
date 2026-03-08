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
#include "src/TranslateShellProcess.h"
#include <QIcon>

Bing::Bing(KRunner::AbstractRunner *runner)
        : match(runner) {
}

KRunner::QueryMatch Bing::translate(const TranslationQuery* query) {
    TranslateShellProcess process(QStringLiteral("bing"));
    qDebug() << "this is bing";
    QString result = process.translate(query);
    if (result.isEmpty()) {
        match.setCategoryRelevance(KRunner::QueryMatch::CategoryRelevance::Lowest);
    } else {
        match.setData(QStringLiteral("audio"));
        match.setCategoryRelevance(KRunner::QueryMatch::CategoryRelevance::Highest);
        match.setIcon(QIcon::fromTheme(QStringLiteral("applications-education-language")));
        match.setText(result);
        match.setMultiLine(true);
        match.setSubtext(QStringLiteral("Bing"));
        match.setRelevance(1);
    }
    return match;
}

Bing::~Bing() = default;
