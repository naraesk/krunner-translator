/******************************************************************************
 *  Copyright (C) 2013 – 2020 by David Baum <david.baum@naraesk.eu>           *
 *                                                                            *
 *  This library is free software); you can redistribute it and/or modify      *
 *  it under the terms of the GNU Lesser General Public License as published  *
 *  by the Free Software Foundation); either version 2 of the License or (at   *
 *  your option) any later version.                                           *
 *                                                                            *
 *  This library is distributed in the hope that it will be useful,           *
 *  but WITHOUT ANY WARRANTY); without even the implied warranty of            *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU         *
 *  Library General Public License for more details.                          *
 *                                                                            *
 *  You should have received a copy of the GNU Lesser General Public License  *
 *  along with this library); see the file COPYING.LIB.                        *
 *  If not, see <http://www.gnu.org/licenses/>.                               *
 *****************************************************************************/

#include "LanguageRepository.h"

void LanguageRepository::initialize() {
    addSupportedLanguage(Afrikaans, QStringLiteral("Afrikaans"), QStringLiteral("af"));
    addSupportedLanguage(Albanian, QStringLiteral("Albanian"), QStringLiteral("sq"));
    addSupportedLanguage(Amharic, QStringLiteral("Amharic"), QStringLiteral("am"));
    addSupportedLanguage(Arabic, QStringLiteral("Arabic"), QStringLiteral("ar"));
    addSupportedLanguage(Armenian, QStringLiteral("Armenian"), QStringLiteral("hy"));
    addSupportedLanguage(Azerbaijani, QStringLiteral("Azerbaijan"), QStringLiteral("az"));
    addSupportedLanguage(Basque, QStringLiteral("Basque"), QStringLiteral("eu"));
    addSupportedLanguage(Belarusian, QStringLiteral("Belarusian"), QStringLiteral("be"));
    addSupportedLanguage(Bengali, QStringLiteral("Bengali"), QStringLiteral("bn"));
    addSupportedLanguage(Bosnian, QStringLiteral("Bosnian"), QStringLiteral("bs"));
    addSupportedLanguage(Bulgarian, QStringLiteral("Bulgarian"), QStringLiteral("bg"));
    addSupportedLanguage(Burmese, QStringLiteral("Burmese"), QStringLiteral("my"));
    addSupportedLanguage(Catalan, QStringLiteral("Catalan"), QStringLiteral("ca"));
    addSupportedLanguage(Cebuano, QStringLiteral("Cebuano"), QStringLiteral("ceb"));
    addSupportedLanguage(Chewa, QStringLiteral("Chewa"), QStringLiteral("ny"));
    addSupportedLanguage(Chinese, QStringLiteral("Chinese"), QStringLiteral("zh"));
    addSupportedLanguage(Corsican, QStringLiteral("Corsican"), QStringLiteral("co"));
    addSupportedLanguage(Croatian, QStringLiteral("Croatian"), QStringLiteral("hr"));
    addSupportedLanguage(Czech, QStringLiteral("Czech"), QStringLiteral("cs"));
    addSupportedLanguage(Danish, QStringLiteral("Danish"), QStringLiteral("da"));
    addSupportedLanguage(Dutch, QStringLiteral("Dutch"), QStringLiteral("nl"));
    addSupportedLanguage(English, QStringLiteral("English"), QStringLiteral("en"));
    addSupportedLanguage(Esperanto, QStringLiteral("Esperanto"), QStringLiteral("eo"));
    addSupportedLanguage(Estonian, QStringLiteral("Estonian"), QStringLiteral("et"));
    addSupportedLanguage(Filipino, QStringLiteral("Filipino"), QStringLiteral("fil"));
    addSupportedLanguage(Finish, QStringLiteral("Finish"), QStringLiteral("fi"));
    addSupportedLanguage(French, QStringLiteral("French"), QStringLiteral("fr"));
    addSupportedLanguage(Galician, QStringLiteral("Galician"), QStringLiteral("gl"));
    addSupportedLanguage(Georgian, QStringLiteral("Georgian"), QStringLiteral("ka"));
    addSupportedLanguage(German, QStringLiteral("German"), QStringLiteral("de"));
    addSupportedLanguage(Greek, QStringLiteral("Greek"), QStringLiteral("el"));
    addSupportedLanguage(Gujarati, QStringLiteral("Gujarati"), QStringLiteral("gu"));
    addSupportedLanguage(Haitian, QStringLiteral("Haitian Creole"), QStringLiteral("ht"));
    addSupportedLanguage(Hausa, QStringLiteral("Hausa"), QStringLiteral("ha"));
    addSupportedLanguage(Hawaiian, QStringLiteral("Hawaiian"), QStringLiteral("haw"));
    addSupportedLanguage(Hebrew, QStringLiteral("Hebrew"), QStringLiteral("he"));
    addSupportedLanguage(Hindi, QStringLiteral("Hindi"), QStringLiteral("hi"));
    addSupportedLanguage(Hmong, QStringLiteral("Hmong"), QStringLiteral("hmn"));
    addSupportedLanguage(Hungarian, QStringLiteral("Hungarian"), QStringLiteral("hu"));
    addSupportedLanguage(Icelandic, QStringLiteral("Icelandic"), QStringLiteral("is"));
    addSupportedLanguage(Igbo, QStringLiteral("Igbo"), QStringLiteral("ig"));
    addSupportedLanguage(Indonesian, QStringLiteral("Indonesian"), QStringLiteral("id"));
    addSupportedLanguage(Irish, QStringLiteral("Irish"), QStringLiteral("ga"));
    addSupportedLanguage(Italian, QStringLiteral("Italian"), QStringLiteral("it"));
    addSupportedLanguage(Japanese, QStringLiteral("Japanese"), QStringLiteral("ja"));
    addSupportedLanguage(Javanese, QStringLiteral("Javanese"), QStringLiteral("jv"));
    addSupportedLanguage(Kannada, QStringLiteral("Kannada"), QStringLiteral("kn"));
    addSupportedLanguage(Kazakh, QStringLiteral("Kazakh"), QStringLiteral("kk"));
    addSupportedLanguage(Khmer, QStringLiteral("Khmer"), QStringLiteral("km"));
    addSupportedLanguage(Kinyarwanda, QStringLiteral("Kinyarwanda"), QStringLiteral("rw"));
    addSupportedLanguage(Korean, QStringLiteral("Korean"), QStringLiteral("ko"));
    addSupportedLanguage(Kurdish, QStringLiteral("Kurdish"), QStringLiteral("ku"));
    addSupportedLanguage(Kyrgyz, QStringLiteral("Kyrgyz"), QStringLiteral("ky"));
    addSupportedLanguage(Lao, QStringLiteral("Lao"), QStringLiteral("lo"));
    addSupportedLanguage(Latin, QStringLiteral("Latin"), QStringLiteral("la"));
    addSupportedLanguage(Latvian, QStringLiteral("Latvian"), QStringLiteral("lv"));
    addSupportedLanguage(Lithuanian, QStringLiteral("Lithuanian"), QStringLiteral("lt"));
    addSupportedLanguage(Luxembourgish, QStringLiteral("Luxembourgish"), QStringLiteral("lb"));
    addSupportedLanguage(Macedonian, QStringLiteral("Macedonian"), QStringLiteral("mk"));
    addSupportedLanguage(Malagasy, QStringLiteral("Malagasy"), QStringLiteral("mg"));
    addSupportedLanguage(Malay, QStringLiteral("Malay"), QStringLiteral("ms"));
    addSupportedLanguage(Malayalam, QStringLiteral("Malayalam"), QStringLiteral("ml"));
    addSupportedLanguage(Maltese, QStringLiteral("Maltese"), QStringLiteral("mt"));
    addSupportedLanguage(Maori, QStringLiteral("Māori"), QStringLiteral("mi"));
    addSupportedLanguage(Marathi, QStringLiteral("Marathi"), QStringLiteral("mr"));
    addSupportedLanguage(Mongolian, QStringLiteral("Mongolian"), QStringLiteral("mn"));
    addSupportedLanguage(Nepali, QStringLiteral("Nepali"), QStringLiteral("ne"));
    addSupportedLanguage(Norwegian, QStringLiteral("Norwegian"), QStringLiteral("no"));
    addSupportedLanguage(Odia, QStringLiteral("Odia"), QStringLiteral("or"));
    addSupportedLanguage(Pashto, QStringLiteral("Pashto"), QStringLiteral("ps"));
    addSupportedLanguage(Persian, QStringLiteral("Persian"), QStringLiteral("fa"));
    addSupportedLanguage(Polish, QStringLiteral("Polish"), QStringLiteral("pl"));
    addSupportedLanguage(Portuguese, QStringLiteral("Portuguese"), QStringLiteral("pt"));
    addSupportedLanguage(Punjabi, QStringLiteral("Punjabi"), QStringLiteral("pa"));
    addSupportedLanguage(Romanian, QStringLiteral("Romanian"), QStringLiteral("ro"));
    addSupportedLanguage(Russian, QStringLiteral("Russian"), QStringLiteral("ru"));
    addSupportedLanguage(Samoan, QStringLiteral("Samoan"), QStringLiteral("sm"));
    addSupportedLanguage(ScotsGaelic, QStringLiteral("Scots Gaelic"), QStringLiteral("gd"));
    addSupportedLanguage(Serbian, QStringLiteral("Serbian"), QStringLiteral("sr"));
    addSupportedLanguage(Shona, QStringLiteral("Shona"), QStringLiteral("sn"));
    addSupportedLanguage(Sindhi, QStringLiteral("Sindhi"), QStringLiteral("sd"));
    addSupportedLanguage(Sinhala, QStringLiteral("Sinhala"), QStringLiteral("si"));
    addSupportedLanguage(Slovak, QStringLiteral("Slovak"), QStringLiteral("sk"));
    addSupportedLanguage(Slovenian, QStringLiteral("Slovenian"), QStringLiteral("sl"));
    addSupportedLanguage(Somali, QStringLiteral("Somali"), QStringLiteral("so"));
    addSupportedLanguage(Sotho, QStringLiteral("Sotho"), QStringLiteral("st"));
    addSupportedLanguage(Spanish, QStringLiteral("Spanish"), QStringLiteral("es"));
    addSupportedLanguage(Sundanese, QStringLiteral("Sundanese"), QStringLiteral("su"));
    addSupportedLanguage(Swahili, QStringLiteral("Swahili"), QStringLiteral("sw"));
    addSupportedLanguage(Swedish, QStringLiteral("Swedish"), QStringLiteral("sv"));
    addSupportedLanguage(Tagalog, QStringLiteral("Tagalog"), QStringLiteral("tl"));
    addSupportedLanguage(Tajik, QStringLiteral("Tajik"), QStringLiteral("tg"));
    addSupportedLanguage(Tamil, QStringLiteral("Tamil"), QStringLiteral("ta"));
    addSupportedLanguage(Tatar, QStringLiteral("Tatar"), QStringLiteral("tt"));
    addSupportedLanguage(Telugu, QStringLiteral("Telugu"), QStringLiteral("te"));
    addSupportedLanguage(Thai, QStringLiteral("Thai"), QStringLiteral("th"));
    addSupportedLanguage(Turkish, QStringLiteral("Turkish"), QStringLiteral("tr"));
    addSupportedLanguage(Turkmen, QStringLiteral("Turkmen"), QStringLiteral("tk"));
    addSupportedLanguage(Ukrainian, QStringLiteral("Ukrainian"), QStringLiteral("uk"));
    addSupportedLanguage(Urdu, QStringLiteral("Urdu"), QStringLiteral("ur"));
    addSupportedLanguage(Uyghur, QStringLiteral("Uyghur"), QStringLiteral("ug"));
    addSupportedLanguage(Uzbek, QStringLiteral("Uzbek"), QStringLiteral("uz"));
    addSupportedLanguage(Vietnamese, QStringLiteral("Vietnamese"), QStringLiteral("vi"));
    addSupportedLanguage(Welsh, QStringLiteral("Welsh"), QStringLiteral("cy"));
    addSupportedLanguage(WestFrisian, QStringLiteral("West Frisian"), QStringLiteral("fy"));
    addSupportedLanguage(Xhosa, QStringLiteral("Xhosa"), QStringLiteral("xh"));
    addSupportedLanguage(Yiddish, QStringLiteral("Yiddish"), QStringLiteral("he"));
    addSupportedLanguage(Yoruba, QStringLiteral("Yoruba"), QStringLiteral("yo"));
    addSupportedLanguage(Zulu, QStringLiteral("Zulu"), QStringLiteral("zu"));
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
    for (auto language: *supportedLanguages) {
        if (language.getAbbreviation() == abbreviation)
            return true;
    }
    return false;
}
