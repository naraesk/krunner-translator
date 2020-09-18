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

#ifndef RUNNERTRANSLATOR_LANGUAGES_H
#define RUNNERTRANSLATOR_LANGUAGES_H


#include <QtCore/QString>
#include <QtCore/QList>
#include <QtCore/QVariant>

class Language {
public:
    static void initialize();

    static void addSupportedLanguage(QString name, QString abbreviation);

    QString getCombinedName();

    QString getAbbreviation();

    static QString getCombinedName(QString abbreviation);

private:
    Language(QString name, QString abbreviation);

    const QString name;
    const QString abbreviation;
    static QList<Language *> *supportedLanguages;

public:
    static QList<Language *> *getSupportedLanguages();
};

#endif //RUNNERTRANSLATOR_LANGUAGES_H
