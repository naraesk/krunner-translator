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
    addSupportedLanguage(Amharic, "Amharic", "am");
    addSupportedLanguage(Arabic, "Arabic", "ar");
    addSupportedLanguage(Armenian, "Armenian", "hy");
    addSupportedLanguage(Azerbaijani, "Azerbaijan", "az");
    addSupportedLanguage(Basque, "Basque", "eu");
    addSupportedLanguage(Belarusian, "Belarusian", "be");
    addSupportedLanguage(Bengali, "Bengali", "bn");
    addSupportedLanguage(Bosnian, "Bosnian", "bs");
    addSupportedLanguage(Bulgarian, "Bulgarian", "bg");
    addSupportedLanguage(Burmese, "Burmese", "my");
    addSupportedLanguage(Catalan, "Catalan", "ca");
    addSupportedLanguage(Cebuano, "Cebuano", "ceb");
    addSupportedLanguage(Chewa, "Chewa", "ny");
    addSupportedLanguage(ChineseSimplified, "Chinese (Simplified)", "zh");
    addSupportedLanguage(ChineseTraditional, "Chinese (Traditional)", "zhh");
    addSupportedLanguage(Corsican, "Corsican", "co");
    addSupportedLanguage(Croatian, "Croatian", "hr");
    addSupportedLanguage(Czech, "Czech", "cs");
    addSupportedLanguage(Danish, "Danish", "da");
    addSupportedLanguage(Dutch, "Dutch", "nl");
    addSupportedLanguage(English, "English", "en");
    addSupportedLanguage(Esperanto, "Esperanto", "eo");
    addSupportedLanguage(Estonian, "Estonian", "et");
    addSupportedLanguage(Filipino, "Filipino", "fil");
    addSupportedLanguage(Finish, "Finish", "fi");
    addSupportedLanguage(French, "French", "fr");
    addSupportedLanguage(Galician, "Galician", "gl");
    addSupportedLanguage(Georgian, "Georgian", "ka");
    addSupportedLanguage(German, "German", "de");
    addSupportedLanguage(Greek, "Greek", "el");
    addSupportedLanguage(Gujarati, "Gujarati", "gu");
    addSupportedLanguage(Haitian, "Haitian Creole", "ht");
    addSupportedLanguage(Hausa, "Hausa", "ha");
    addSupportedLanguage(Hawaiian, "Hawaiian", "haw");
    addSupportedLanguage(Hebrew, "Hebrew", "he");
    addSupportedLanguage(Hindi, "Hindi", "hi");
    addSupportedLanguage(Hmong, "Hmong", "hmn");
    addSupportedLanguage(Hungarian, "Hungarian", "hu");
    addSupportedLanguage(Icelandic, "Icelandic", "is");
    addSupportedLanguage(Igbo, "Igbo", "ig");
    addSupportedLanguage(Indonesian, "Indonesian", "id");
    addSupportedLanguage(Irish, "Irish", "ga");
    addSupportedLanguage(Italian, "Italian", "it");
    addSupportedLanguage(Japanese, "Japanese", "ja");
    addSupportedLanguage(Javanese, "Javanese", "jv");
    addSupportedLanguage(Kannada, "Kannada", "kn");
    addSupportedLanguage(Kazakh, "Kazakh", "kk");
    addSupportedLanguage(Khmer, "Khmer", "km");
    addSupportedLanguage(Kinyarwanda, "Kinyarwanda", "rw");
    addSupportedLanguage(Korean, "Korean", "ko");
    addSupportedLanguage(Kurdish, "Kurdish", "ku");
    addSupportedLanguage(Kyrgyz, "Kyrgyz", "ky");
    addSupportedLanguage(Lao, "Lao", "lo");
    addSupportedLanguage(Latin, "Latin", "la");
    addSupportedLanguage(Latvian, "Latvian", "lv");
    addSupportedLanguage(Lithuanian, "Lithuanian", "lt");
    addSupportedLanguage(Luxembourgish, "Luxembourgish", "lb");
    addSupportedLanguage(Macedonian, "Macedonian", "mk");
    addSupportedLanguage(Malagasy, "Malagasy", "mg");
    addSupportedLanguage(Malay, "Malay", "ms");
    addSupportedLanguage(Malayalam, "Malayalam", "ml");
    addSupportedLanguage(Maltese, "Maltese", "mt");
    addSupportedLanguage(Maori, "Māori", "mi");
    addSupportedLanguage(Marathi, "Marathi", "mr");
    addSupportedLanguage(Mongolian, "Mongolian", "mn");
    addSupportedLanguage(Nepali, "Nepali", "ne");
    addSupportedLanguage(Norwegian, "Norwegian", "no");
    addSupportedLanguage(Odia, "Odia", "or");
    addSupportedLanguage(Pashto, "Pashto", "ps");
    addSupportedLanguage(Persian, "Persian", "fa");
    addSupportedLanguage(Polish, "Polish", "pl");
    addSupportedLanguage(Portuguese, "Portuguese", "pt");
    addSupportedLanguage(Punjabi, "Punjabi", "pa");
    addSupportedLanguage(Romanian, "Romanian", "ro");
    addSupportedLanguage(Russian, "Russian", "ru");
    addSupportedLanguage(Samoan, "Samoan", "sm");
    addSupportedLanguage(ScotsGaelic, "Scots Gaelic ", "gd");
    addSupportedLanguage(Serbian, "Serbian", "sr");
    addSupportedLanguage(Shona, "Shona", "sn");
    addSupportedLanguage(Sindhi, "Sindhi", "sd");
    addSupportedLanguage(Sinhala, "Sinhala", "si");
    addSupportedLanguage(Slovak, "Slovak", "sk");
    addSupportedLanguage(Slovenian, "Slovenian", "sl");
    addSupportedLanguage(Somali, "Somali", "so");
    addSupportedLanguage(Sotho, "Sotho", "st");
    addSupportedLanguage(Spanish, "Spanish", "es");
    addSupportedLanguage(Sundanese, "Sundanese", "su");
    addSupportedLanguage(Swahili, "Swahili", "sw");
    addSupportedLanguage(Swedish, "Swedish", "sv");
    addSupportedLanguage(Tagalog, "Tagalog", "tl");
    addSupportedLanguage(Tajik, "Tajik", "tg");
    addSupportedLanguage(Tamil, "Tamil", "ta");
    addSupportedLanguage(Tatar, "Tatar", "tt");
    addSupportedLanguage(Telugu, "Telugu", "te");
    addSupportedLanguage(Thai, "Thai", "th");
    addSupportedLanguage(Turkish, "Turkish", "tr");
    addSupportedLanguage(Turkmen, "Turkmen", "tk");
    addSupportedLanguage(Ukrainian, "Ukrainian", "uk");
    addSupportedLanguage(Urdu, "Urdu", "ur");
    addSupportedLanguage(Uyghur, "Uyghur", "ug");
    addSupportedLanguage(Uzbek, "Uzbek", "uz");
    addSupportedLanguage(Vietnamese, "Vietnamese", "vi");
    addSupportedLanguage(Welsh, "Welsh", "cy");
    addSupportedLanguage(WestFrisian, "West Frisian", "fy");
    addSupportedLanguage(Xhosa, "Xhosa", "xh");
    addSupportedLanguage(Yiddish, "Yiddish", "he");
    addSupportedLanguage(Yoruba, "Yoruba", "yo");
    addSupportedLanguage(Zulu, "Zulu", "zu");
}

void LanguageRepository::addSupportedLanguage(SupportedLanguage supportedLanguage, QString name, QString abbreviation) {
    Language language(supportedLanguage, name, abbreviation);
    supportedLanguages->insert(supportedLanguage, language);
}

QList<Language> LanguageRepository::getSupportedLanguages() {
    return supportedLanguages->values();
}

QString LanguageRepository::getCombinedName(QString abbreviation) {
    for (auto language : *supportedLanguages) {
        if (language.getAbbreviation() == abbreviation) {
            return language.getCombinedName();
        }
    }
    return "";
}

bool LanguageRepository::containsAbbreviation(QString abbreviation) {
    for(auto language: supportedLanguages->values()) {
        if(language.getAbbreviation() == abbreviation)
            return true;
    }
    return false;
}
