
//
// Created by David Baum on 23.09.20.
//

#include "TranslationQuery.h"
#include <QtDebug>

void TranslationQuery::setSourceLanguage(SupportedLanguage sourceLanguage) {
    TranslationQuery::sourceLanguage = sourceLanguage;
}

void TranslationQuery::setTargetLanguage(SupportedLanguage targetLanguage) {
    TranslationQuery::targetLanguage = targetLanguage;
}

const QString &TranslationQuery::getText() const {
    return text;
}

void TranslationQuery::setText(const QString &text) {
    TranslationQuery::text = text;
}

TranslationQuery::TranslationQuery(LanguageRepository * repository, SupportedLanguage sourceLanguage, SupportedLanguage targetLanguage, QString& text)
: repository(repository), sourceLanguage(sourceLanguage), targetLanguage(targetLanguage), text(text){

}

QString TranslationQuery::getSourceAbbreviation() const{
    return repository->getAbbreviation(sourceLanguage);
}

QString TranslationQuery::getTargetAbbreviation() const {
    return repository->getAbbreviation(targetLanguage);
}

TranslationQuery::TranslationQuery(LanguageRepository *repository)
: repository(repository), sourceLanguage(INVALID), targetLanguage(INVALID) {

}

bool TranslationQuery::isValid() const {
    if (text.isNull()) return false;
    if (sourceLanguage == INVALID) return false;
    if (targetLanguage == INVALID) return false;
    return true;
}

void TranslationQuery::updateSourceLanguage(SupportedLanguage defaultLanguage, SupportedLanguage alternativeDefaultLanguage) {
    if(sourceLanguage != SupportedLanguage::INVALID ) return; // source language already defined, nothing to do here
    if(targetLanguage == defaultLanguage) {
        sourceLanguage = alternativeDefaultLanguage;
    } else {
        sourceLanguage = defaultLanguage;
    }
}
