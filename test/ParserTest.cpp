#include "ParserTest.h"

void ParserTest::initTestCase() {
    repository.initialize();
    parser.setRepository(&repository);
}

void ParserTest::testValidSingleLanguage() {
    TranslationQuery *query = parser.parse(QStringLiteral("de Hallo"));
    QVERIFY(query != nullptr);
    QCOMPARE(query->getText(), QStringLiteral("Hallo"));
    QCOMPARE(query->getTargetAbbreviation(), QStringLiteral("de"));
    QVERIFY(!query->isValid());
    delete query;
}

void ParserTest::testValidSourceTargetLanguage() {
    TranslationQuery *query = parser.parse(QStringLiteral("en-de Hello"));
    QVERIFY(query != nullptr);
    QCOMPARE(query->getText(), QStringLiteral("Hello"));
    QCOMPARE(query->getSourceAbbreviation(), QStringLiteral("en"));
    QCOMPARE(query->getTargetAbbreviation(), QStringLiteral("de"));
    QVERIFY(query->isValid());
    delete query;
}

void ParserTest::testInvalidLanguageAbbreviation() {
    TranslationQuery *query = parser.parse(QStringLiteral("xyz hello"));
    QVERIFY(query != nullptr);
    QCOMPARE(query->getText(), QStringLiteral("hello"));
    QVERIFY(!query->isValid());
    delete query;
}

void ParserTest::testInvalidSourceTargetLanguage() {
    TranslationQuery *query = parser.parse(QStringLiteral("abc-xyz hello"));
    QVERIFY(query != nullptr);
    QCOMPARE(query->getText(), QStringLiteral("hello"));
    QVERIFY(!query->isValid());
    delete query;
}

void ParserTest::testIssue34InvalidSingleLanguageReproducer() {
    TranslationQuery *query = parser.parse(QStringLiteral("deeeee e"));
    QVERIFY(query != nullptr);
    query->updateSourceLanguage(SupportedLanguage::English, SupportedLanguage::Spanish);
    QCOMPARE(query->getSourceAbbreviation(), QStringLiteral("en"));
    QVERIFY(query->getTargetAbbreviation().isEmpty());
    QVERIFY(!query->isValid());
    delete query;
}

void ParserTest::testIssue34InvalidSourceTargetReproducer() {
    TranslationQuery *query = parser.parse(QStringLiteral("abc-abc a"));
    QVERIFY(query != nullptr);
    query->updateSourceLanguage(SupportedLanguage::English, SupportedLanguage::Spanish);
    QCOMPARE(query->getSourceAbbreviation(), QStringLiteral("en"));
    QVERIFY(query->getTargetAbbreviation().isEmpty());
    QVERIFY(!query->isValid());
    delete query;
}

void ParserTest::testEmptyQuery() {
    TranslationQuery *query = parser.parse(QStringLiteral(""));
    QVERIFY(query != nullptr);
    QVERIFY(!query->isValid());
    delete query;
}

void ParserTest::testNoSpaceInQuery() {
    TranslationQuery *query = parser.parse(QStringLiteral("de"));
    QVERIFY(query != nullptr);
    QVERIFY(!query->isValid());
    delete query;
}

QTEST_MAIN(ParserTest)

#include "ParserTest.moc"
