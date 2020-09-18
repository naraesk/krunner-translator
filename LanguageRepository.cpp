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

#include "LanguageRepository.h"

void LanguageRepository::initialize() {
    addSupportedLanguage(Afrikaans, "Afrikaans", "af");
    addSupportedLanguage(Albanian, "Albanian", "sq");
    addSupportedLanguage(Arabic, "Arabic", "ar");
    addSupportedLanguage(Armenian, "Armenian", "hy");
    addSupportedLanguage(Azerbaijan, "Azerbaijan", "az");
    addSupportedLanguage(Basque, "Basque", "eu");
    addSupportedLanguage(Belarusian, "Belarusian", "be");
    addSupportedLanguage(Bosnian, "Bosnian", "bs");
    addSupportedLanguage(Bulgarian, "Bulgarian", "bg");
    addSupportedLanguage(Catalan, "Catalan", "ca");
    addSupportedLanguage(Chinese, "Chinese", "zh");
    addSupportedLanguage(Croatian, "Croatian", "hr");
    addSupportedLanguage(Czech, "Czech", "cs");
    addSupportedLanguage(Danish, "Danish", "da");
    addSupportedLanguage(Dutch, "Dutch", "nl");
    addSupportedLanguage(English, "English", "en");
    addSupportedLanguage(Estonian, "Estonian", "et");
    addSupportedLanguage(Finish, "Finish", "fi");
    addSupportedLanguage(French, "French", "fr");
    addSupportedLanguage(Galician, "Galician", "gl");
    addSupportedLanguage(Georgian, "Georgian", "ka");
    addSupportedLanguage(German, "German", "de");
    addSupportedLanguage(Greek, "Greek", "el");
    addSupportedLanguage(Haitian, "Haitian (Creole)", "ht");
    addSupportedLanguage(Hungarian, "Hungarian", "hu");
    addSupportedLanguage(Icelandic, "Icelandic", "is");
    addSupportedLanguage(Indonesian, "Indonesian", "id");
    addSupportedLanguage(Irish, "Irish", "ga");
    addSupportedLanguage(Italian, "Italian", "it");
    addSupportedLanguage(Japanese, "Japanese", "ja");
    addSupportedLanguage(Kazakh, "Kazakh", "kk");
    addSupportedLanguage(Korean, "Korean", "ko");
    addSupportedLanguage(Kyrgyz, "Kyrgyz", "ky");
    addSupportedLanguage(Latin, "Latin", "la");
    addSupportedLanguage(Latvian, "Latvian", "lv");
    addSupportedLanguage(Lithuanian, "Lithuanian", "lt");
    addSupportedLanguage(Macedonian, "Macedonian", "mk");
    addSupportedLanguage(Malagasy, "Malagasy", "mg");
    addSupportedLanguage(Malay, "Malay", "ms");
    addSupportedLanguage(Maltese, "Maltese", "mt");
    addSupportedLanguage(Mongolian, "Mongolian", "mn");
    addSupportedLanguage(Norwegian, "Norwegian", "no");
    addSupportedLanguage(Persian, "Persian", "fa");
    addSupportedLanguage(Polish, "Polish", "pl");
    addSupportedLanguage(Portuguese, "Portuguese", "pt");
    addSupportedLanguage(Romanian, "Romanian", "ro");
    addSupportedLanguage(Russian, "Russian", "ru");
    addSupportedLanguage(Serbian, "Serbian", "sr");
    addSupportedLanguage(Slovakian, "Slovakian", "sk");
    addSupportedLanguage(Slovenian, "Slovenian", "sl");
    addSupportedLanguage(Spanish, "Spanish", "es");
    addSupportedLanguage(Swahili, "Swahili", "sw");
    addSupportedLanguage(Swedish, "Swedish", "sv");
    addSupportedLanguage(Tagalog, "Tagalog", "tl");
    addSupportedLanguage(Tajik, "Tajik", "tg");
    addSupportedLanguage(Tatar, "Tatar", "tt");
    addSupportedLanguage(Thai, "Thai", "th");
    addSupportedLanguage(Turkish, "Turkish", "tr");
    addSupportedLanguage(Ukrainian, "Ukrainian", "uk");
    addSupportedLanguage(Uzbek, "Uzbek", "uz");
    addSupportedLanguage(Vietnamese, "Vietnamese", "vi");
    addSupportedLanguage(Welsh, "Welsh", "cy");
    addSupportedLanguage(Yiddish, "Yiddish", "he");
}

void LanguageRepository::addSupportedLanguage(SupportedLanguage supportedLanguage, QString name, QString abbreviation) {
    Language language(supportedLanguage, name, abbreviation);
    supportedLanguages->insert(supportedLanguage, language);
}

QList<Language> LanguageRepository::getSupportedLanguages() {
    return supportedLanguages->values();
}

QString LanguageRepository::getCombinedName(QString abbreviation) {
    for(auto language : *supportedLanguages) {
        if(language.getAbbreviation() == abbreviation) {
            return language.getCombinedName();
        }
    }
    return "";
}
