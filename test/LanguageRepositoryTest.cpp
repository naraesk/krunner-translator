#include "LanguageRepositoryTest.h"

void LanguageRepositoryTest::initTestCase() {
    repository.initialize();
}

void LanguageRepositoryTest::testGetSupportedLanguageValid() {
    SupportedLanguage lang = repository.getSupportedLanguage(QStringLiteral("de"));
    QCOMPARE(lang, SupportedLanguage::German);
}

void LanguageRepositoryTest::testGetSupportedLanguageInvalid() {
    SupportedLanguage lang = repository.getSupportedLanguage(QStringLiteral("xyz"));
    QCOMPARE(lang, SupportedLanguage::INVALID);
}

void LanguageRepositoryTest::testGetAbbreviationValid() {
    QString abbr = repository.getAbbreviation(SupportedLanguage::German);
    QCOMPARE(abbr, QStringLiteral("de"));
}

void LanguageRepositoryTest::testGetAbbreviationInvalid() {
    QString abbr = repository.getAbbreviation(SupportedLanguage::INVALID);
    QVERIFY(abbr.isEmpty());
}

QTEST_MAIN(LanguageRepositoryTest)

#include "LanguageRepositoryTest.moc"
