#ifndef RUNNERTRANSLATOR_LANGUAGEREPOSITORYTEST_H
#define RUNNERTRANSLATOR_LANGUAGEREPOSITORYTEST_H

#include <QtTest/QtTest>
#include <QtCore/QObject>
#include "../src/language/LanguageRepository.h"

class LanguageRepositoryTest : public QObject {

Q_OBJECT;

    LanguageRepository repository;

private Q_SLOTS:

    void initTestCase();

    void testGetSupportedLanguageValid();
    void testGetSupportedLanguageInvalid();
    void testGetAbbreviationValid();
    void testGetAbbreviationInvalid();

};

#endif
