
//
// Created by David Baum on 23.09.20.
//

#ifndef RUNNERTRANSLATOR_TRANSLATIONQUERY_H
#define RUNNERTRANSLATOR_TRANSLATIONQUERY_H


#include <src/language/SupportedLanguages.h>
#include <QtCore/QString>
#include <src/language/Language.h>
#include <src/language/LanguageRepository.h>

class TranslationQuery {
public:
    TranslationQuery(LanguageRepository * repository);
    TranslationQuery(LanguageRepository * repository, SupportedLanguage, SupportedLanguage, QString&);
    ~TranslationQuery() = default;

    void setSourceLanguage(SupportedLanguage sourceLanguage);

    void setTargetLanguage(SupportedLanguage targetLanguage);

    const QString &getText() const;

    void setText(const QString &text);

    QString getSourceAbbreviation() const;
    QString getTargetAbbreviation() const;
    bool isValid() const;

private:
    LanguageRepository * repository;
    SupportedLanguage sourceLanguage;
    SupportedLanguage targetLanguage;
    QString text;
public:
    void updateSourceLanguage(SupportedLanguage defaultLanguage, SupportedLanguage alternativeDefaultLanguage);
};

#endif //RUNNERTRANSLATOR_TRANSLATIONQUERY_H
