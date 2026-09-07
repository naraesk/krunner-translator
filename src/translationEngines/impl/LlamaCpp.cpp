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

#include "LlamaCpp.h"
#include <QIcon>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QHash>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QTimer>
#include <QUrl>
#include "src/llama_server.h"

namespace {
// Maps the repository's ISO abbreviations to the English language names the
// model is prompted with. The table mirrors src/language/LanguageRepository
// and deliberately covers every language the runner knows: the prompt is
// built dynamically ("Translate ... into <name>"), and which pairs actually
// produce good results is up to the selected GGUF model (Hy-MT2 is
// multilingual; see README). Any abbreviation not in the table (i.e. not a
// known language) returns an empty name, which gates this engine off.
QString languageName(const QString &abbr) {
    static const QHash<QString, QString> names = {
        {QStringLiteral("af"), QStringLiteral("Afrikaans")},
        {QStringLiteral("sq"), QStringLiteral("Albanian")},
        {QStringLiteral("am"), QStringLiteral("Amharic")},
        {QStringLiteral("ar"), QStringLiteral("Arabic")},
        {QStringLiteral("hy"), QStringLiteral("Armenian")},
        {QStringLiteral("az"), QStringLiteral("Azerbaijani")},
        {QStringLiteral("eu"), QStringLiteral("Basque")},
        {QStringLiteral("be"), QStringLiteral("Belarusian")},
        {QStringLiteral("bn"), QStringLiteral("Bengali")},
        {QStringLiteral("bs"), QStringLiteral("Bosnian")},
        {QStringLiteral("bg"), QStringLiteral("Bulgarian")},
        {QStringLiteral("my"), QStringLiteral("Burmese")},
        {QStringLiteral("ca"), QStringLiteral("Catalan")},
        {QStringLiteral("ceb"), QStringLiteral("Cebuano")},
        {QStringLiteral("ny"), QStringLiteral("Chewa")},
        {QStringLiteral("zh"), QStringLiteral("Chinese")},
        {QStringLiteral("co"), QStringLiteral("Corsican")},
        {QStringLiteral("hr"), QStringLiteral("Croatian")},
        {QStringLiteral("cs"), QStringLiteral("Czech")},
        {QStringLiteral("da"), QStringLiteral("Danish")},
        {QStringLiteral("nl"), QStringLiteral("Dutch")},
        {QStringLiteral("en"), QStringLiteral("English")},
        {QStringLiteral("eo"), QStringLiteral("Esperanto")},
        {QStringLiteral("et"), QStringLiteral("Estonian")},
        {QStringLiteral("fil"), QStringLiteral("Filipino")},
        {QStringLiteral("fi"), QStringLiteral("Finnish")},
        {QStringLiteral("fr"), QStringLiteral("French")},
        {QStringLiteral("gl"), QStringLiteral("Galician")},
        {QStringLiteral("ka"), QStringLiteral("Georgian")},
        {QStringLiteral("de"), QStringLiteral("German")},
        {QStringLiteral("el"), QStringLiteral("Greek")},
        {QStringLiteral("gu"), QStringLiteral("Gujarati")},
        {QStringLiteral("ht"), QStringLiteral("Haitian Creole")},
        {QStringLiteral("ha"), QStringLiteral("Hausa")},
        {QStringLiteral("haw"), QStringLiteral("Hawaiian")},
        {QStringLiteral("he"), QStringLiteral("Hebrew")},
        {QStringLiteral("hi"), QStringLiteral("Hindi")},
        {QStringLiteral("hmn"), QStringLiteral("Hmong")},
        {QStringLiteral("hu"), QStringLiteral("Hungarian")},
        {QStringLiteral("is"), QStringLiteral("Icelandic")},
        {QStringLiteral("ig"), QStringLiteral("Igbo")},
        {QStringLiteral("id"), QStringLiteral("Indonesian")},
        {QStringLiteral("ga"), QStringLiteral("Irish")},
        {QStringLiteral("it"), QStringLiteral("Italian")},
        {QStringLiteral("ja"), QStringLiteral("Japanese")},
        {QStringLiteral("jv"), QStringLiteral("Javanese")},
        {QStringLiteral("kn"), QStringLiteral("Kannada")},
        {QStringLiteral("kk"), QStringLiteral("Kazakh")},
        {QStringLiteral("km"), QStringLiteral("Khmer")},
        {QStringLiteral("rw"), QStringLiteral("Kinyarwanda")},
        {QStringLiteral("ko"), QStringLiteral("Korean")},
        {QStringLiteral("ku"), QStringLiteral("Kurdish")},
        {QStringLiteral("ky"), QStringLiteral("Kyrgyz")},
        {QStringLiteral("lo"), QStringLiteral("Lao")},
        {QStringLiteral("la"), QStringLiteral("Latin")},
        {QStringLiteral("lv"), QStringLiteral("Latvian")},
        {QStringLiteral("lt"), QStringLiteral("Lithuanian")},
        {QStringLiteral("lb"), QStringLiteral("Luxembourgish")},
        {QStringLiteral("mk"), QStringLiteral("Macedonian")},
        {QStringLiteral("mg"), QStringLiteral("Malagasy")},
        {QStringLiteral("ms"), QStringLiteral("Malay")},
        {QStringLiteral("ml"), QStringLiteral("Malayalam")},
        {QStringLiteral("mt"), QStringLiteral("Maltese")},
        {QStringLiteral("mi"), QStringLiteral("Maori")},
        {QStringLiteral("mr"), QStringLiteral("Marathi")},
        {QStringLiteral("mn"), QStringLiteral("Mongolian")},
        {QStringLiteral("ne"), QStringLiteral("Nepali")},
        {QStringLiteral("no"), QStringLiteral("Norwegian")},
        {QStringLiteral("or"), QStringLiteral("Odia")},
        {QStringLiteral("ps"), QStringLiteral("Pashto")},
        {QStringLiteral("fa"), QStringLiteral("Persian")},
        {QStringLiteral("pl"), QStringLiteral("Polish")},
        {QStringLiteral("pt"), QStringLiteral("Portuguese")},
        {QStringLiteral("pa"), QStringLiteral("Punjabi")},
        {QStringLiteral("ro"), QStringLiteral("Romanian")},
        {QStringLiteral("ru"), QStringLiteral("Russian")},
        {QStringLiteral("sm"), QStringLiteral("Samoan")},
        {QStringLiteral("gd"), QStringLiteral("Scots Gaelic")},
        {QStringLiteral("sr"), QStringLiteral("Serbian")},
        {QStringLiteral("sn"), QStringLiteral("Shona")},
        {QStringLiteral("sd"), QStringLiteral("Sindhi")},
        {QStringLiteral("si"), QStringLiteral("Sinhala")},
        {QStringLiteral("sk"), QStringLiteral("Slovak")},
        {QStringLiteral("sl"), QStringLiteral("Slovenian")},
        {QStringLiteral("so"), QStringLiteral("Somali")},
        {QStringLiteral("st"), QStringLiteral("Sotho")},
        {QStringLiteral("es"), QStringLiteral("Spanish")},
        {QStringLiteral("su"), QStringLiteral("Sundanese")},
        {QStringLiteral("sw"), QStringLiteral("Swahili")},
        {QStringLiteral("sv"), QStringLiteral("Swedish")},
        {QStringLiteral("tl"), QStringLiteral("Tagalog")},
        {QStringLiteral("tg"), QStringLiteral("Tajik")},
        {QStringLiteral("ta"), QStringLiteral("Tamil")},
        {QStringLiteral("tt"), QStringLiteral("Tatar")},
        {QStringLiteral("te"), QStringLiteral("Telugu")},
        {QStringLiteral("th"), QStringLiteral("Thai")},
        {QStringLiteral("tr"), QStringLiteral("Turkish")},
        {QStringLiteral("tk"), QStringLiteral("Turkmen")},
        {QStringLiteral("uk"), QStringLiteral("Ukrainian")},
        {QStringLiteral("ur"), QStringLiteral("Urdu")},
        {QStringLiteral("ug"), QStringLiteral("Uyghur")},
        {QStringLiteral("uz"), QStringLiteral("Uzbek")},
        {QStringLiteral("vi"), QStringLiteral("Vietnamese")},
        {QStringLiteral("cy"), QStringLiteral("Welsh")},
        {QStringLiteral("fy"), QStringLiteral("West Frisian")},
        {QStringLiteral("xh"), QStringLiteral("Xhosa")},
        {QStringLiteral("yi"), QStringLiteral("Yiddish")},
        {QStringLiteral("yo"), QStringLiteral("Yoruba")},
        {QStringLiteral("zu"), QStringLiteral("Zulu")},
    };
    return names.value(abbr);
}
} // namespace

bool LlamaCpp::isServerUp() const {
    return LlamaServer::probeHealth(m_port);
}

KRunner::QueryMatch LlamaCpp::placeholder(const QString &text) const {
    KRunner::QueryMatch p(match.runner());
    p.setId(matchId());
    p.setCategoryRelevance(KRunner::QueryMatch::CategoryRelevance::Moderate);
    p.setIcon(QIcon::fromTheme(QStringLiteral("applications-education-language")));
    p.setText(text);
    p.setMultiLine(true);
    p.setSubtext(QStringLiteral("llama.cpp (local)"));
    p.setRelevance(0.5);
    return p;
}

bool LlamaCpp::canTranslate(const TranslationQuery *query) const {
    return !languageName(query->getTargetAbbreviation()).isEmpty()
        && !languageName(query->getSourceAbbreviation()).isEmpty();
}

void LlamaCpp::setContext(KRunner::RunnerContext *ctx) {
    m_context = ctx;
}

LlamaCpp::LlamaCpp(KRunner::AbstractRunner *runner, int port, int ngl,
                   const QString &model, const QString &llamaDir)
        : match(runner), m_port(port), m_ngl(ngl), m_model(model), m_llamaDir(llamaDir) {
}
bool LlamaCpp::ensureServer() {
    return LlamaServer::ensureServer(m_port, m_model, m_llamaDir, m_ngl);
}

QString LlamaCpp::buildPrompt(const TranslationQuery *query) {
    const QString target = languageName(query->getTargetAbbreviation());
    const QString text = query->getText();
    // A single word (no whitespace) gets its common senses listed; anything
    // longer (phrase/sentence) is translated normally.
    const bool isSingleWord = !text.contains(QChar::Space) && !text.contains(QLatin1Char('\n'));
    if (isSingleWord) {
        return QStringLiteral("Translate the word \"%2\" into %1 and list its common meanings or senses (one word, multiple meanings). Output only the meanings as a numbered list, without any additional explanation.")
                .arg(target, text);
    }
    // Hy-MT2 prompt format (English), see tencent/Hy-MT2 README "Default Translation".
    return QStringLiteral("Translate the following text into %1. Note that you should only output the translated result without any additional explanation:\n\n%2")
            .arg(target, text);
}

KRunner::QueryMatch LlamaCpp::translate(const TranslationQuery *query) {
    if (!query->isValid()) {
        return match;
    }
    // Stable id so KRunner replaces the placeholder with this result.
    match.setId(matchId());

    // Any source/target pair both in the known-language table is served; an
    // unknown abbreviation (empty name) gates this engine off.
    if (languageName(query->getTargetAbbreviation()).isEmpty() ||
        languageName(query->getSourceAbbreviation()).isEmpty()) {
        match.setCategoryRelevance(KRunner::QueryMatch::CategoryRelevance::Lowest);
        match.setRelevance(0.1);
        return match;
    }

    // One probe decides the placeholder, and exactly one placeholder is added:
    // KRunner replaces each match id only once (its uniqueIds table is never
    // refreshed on replacement), so the result below is the single
    // replacement. Server up  -> "翻译中..." (cold start would have shown
    // "唤起中..." instead, but the framework can't chain two intermediate
    // states onto one id).
    const bool serverWasUp = isServerUp();
    if (m_context) {
        m_context->addMatch(placeholder(serverWasUp
            ? QStringLiteral("翻译中...")
            : QStringLiteral("唤起中...")));
    }
    // Make sure the local llama.cpp server is up (start it if needed).
    ensureServer();

    const QUrl url(QStringLiteral("http://127.0.0.1:%1/v1/chat/completions").arg(m_port));
    const QJsonObject message {
        {QStringLiteral("role"), QStringLiteral("user")},
        {QStringLiteral("content"), buildPrompt(query)}
    };
    // No "model" field on purpose: llama.cpp's single-model server ignores it
    // (it always serves the model given via `-m`), so the model the user wants
    // is chosen in llama-server.sh's MODEL variable, not here.
    const QJsonObject payload {
        {QStringLiteral("messages"), QJsonArray{message}},
        {QStringLiteral("temperature"), 0.7},
        {QStringLiteral("top_p"), 0.6},
        {QStringLiteral("top_k"), 20},
        {QStringLiteral("repeat_penalty"), 1.05},
        {QStringLiteral("max_tokens"), 4096}
    };

    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, QStringLiteral("application/json"));

    QNetworkAccessManager manager;
    QEventLoop loop;
    QNetworkReply *reply = manager.post(request, QJsonDocument(payload).toJson());
    QObject::connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    // Guard against a dead/hung server.
    QTimer::singleShot(60000, &loop, &QEventLoop::quit);
    loop.exec();

    QString result;
    if (reply->error() == QNetworkReply::NoError) {
        const QJsonObject root = QJsonDocument::fromJson(reply->readAll()).object();
        const QJsonArray choices = root.value(QStringLiteral("choices")).toArray();
        if (!choices.isEmpty()) {
            result = choices.at(0).toObject()
                    .value(QStringLiteral("message")).toObject()
                    .value(QStringLiteral("content")).toString().trimmed();
        }
    }
    reply->deleteLater();

    if (result.isEmpty()) {
        match.setCategoryRelevance(KRunner::QueryMatch::CategoryRelevance::Lowest);
        match.setSubtext(QStringLiteral("llama.cpp"));
        match.setRelevance(0.1);
        return match;
    }

    match.setCategoryRelevance(KRunner::QueryMatch::CategoryRelevance::Highest);
    match.setIcon(QIcon::fromTheme(QStringLiteral("applications-education-language")));
    match.setText(result);
    match.setMultiLine(true);
    match.setSubtext(QStringLiteral("llama.cpp (local)"));
    match.setRelevance(1);
    return match;
}

LlamaCpp::~LlamaCpp() = default;
