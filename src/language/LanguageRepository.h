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

#ifndef RUNNERTRANSLATOR_LANGUAGEREPOSITORY_H
#define RUNNERTRANSLATOR_LANGUAGEREPOSITORY_H

#include "Language.h"

class LanguageRepository {
public:
    void addSupportedLanguage(SupportedLanguage language, QString name, QString abbreviation);

    void initialize();

    QList<class Language> getSupportedLanguages();

    QString getCombinedName(QString abbreviation);

    SupportedLanguage getSupportedLanguage(QString abbr);

    QString getAbbreviation(SupportedLanguage language) const;

    bool containsAbbreviation(QString abbreviation);

private:
    QMap<SupportedLanguage, Language> *supportedLanguages = new QMap<SupportedLanguage, Language>;
    QMap<QString, SupportedLanguage> *abbrMap = new QMap<QString, SupportedLanguage>;
};

#endif //RUNNERTRANSLATOR_LANGUAGEREPOSITORY_H
