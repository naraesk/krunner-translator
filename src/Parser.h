
//
// Created by David Baum on 23.09.20.
//

#ifndef RUNNERTRANSLATOR_PARSER_H
#define RUNNERTRANSLATOR_PARSER_H


#include <QtCore/QString>
#include <src/language/Language.h>
#include <src/language/LanguageRepository.h>
#include "TranslationQuery.h"

class Parser {
public:
    void setRepository(LanguageRepository * repository);
    TranslationQuery* parse(const QString &term);

private:
    LanguageRepository * repository;
    const QString separator = QStringLiteral("-");
};

#endif //RUNNERTRANSLATOR_PARSER_H
