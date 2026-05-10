#ifndef RUNNERTRANSLATOR_PARSERTEST_H
#define RUNNERTRANSLATOR_PARSERTEST_H

#include <QtTest/QtTest>
#include <QtCore/QObject>
#include "../src/Parser.h"
#include "src/language/LanguageRepository.h"

class ParserTest : public QObject {

Q_OBJECT;

    Parser parser;
    LanguageRepository repository;

private Q_SLOTS:

    void initTestCase();

    void testValidSingleLanguage();
    void testValidSourceTargetLanguage();
    void testInvalidLanguageAbbreviation();
    void testInvalidSourceTargetLanguage();
    void testIssue34InvalidSingleLanguageReproducer();
    void testIssue34InvalidSourceTargetReproducer();
    void testEmptyQuery();
    void testNoSpaceInQuery();

};

#endif
