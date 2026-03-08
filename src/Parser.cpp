
//
// Created by David Baum on 23.09.20.
//

#include "Parser.h"
#include <QDebug>

TranslationQuery* Parser::parse(const QString &term) {
    TranslationQuery *query = new TranslationQuery(repository);
    const int index = term.indexOf(QStringLiteral(" "));
    if (index == -1) return query;
    QString text = term.mid(index + 1);
    query->setText(text);
    const QString languageTerm = term.left(index);
    int languageIndex = languageTerm.indexOf(separator);
    if (languageIndex != -1) {
        QString sourceLanguageAbbreviation = languageTerm.left(languageIndex);
        QString targetLanguageAbbreviation = languageTerm.mid(languageIndex + 1);
        SupportedLanguage sourceLanguage = repository->getSupportedLanguage(sourceLanguageAbbreviation);
        SupportedLanguage targetLanguage = repository->getSupportedLanguage(targetLanguageAbbreviation);
        query->setSourceLanguage(sourceLanguage);
        query->setTargetLanguage(targetLanguage);
    } else {
        // TODO: Set source Language;
        SupportedLanguage targetLanguage = repository->getSupportedLanguage(languageTerm);
        query->setTargetLanguage(targetLanguage);
    }
    return query;
}

void Parser::setRepository(LanguageRepository *newRepository) {
    repository = newRepository;
}
