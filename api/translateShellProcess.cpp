
//
// Created by David Baum on 10.09.20.
//

#include "translateShellProcess.h"

TranslateShellProcess::TranslateShellProcess(QObject *parent) : QProcess(parent) {
}

TranslateShellProcess::~TranslateShellProcess() = default;

QString TranslateShellProcess::translate(const QPair<QString, QString> &language, const QString &text) {
    QStringList arguments;
    arguments << language.first + ":" + language.second
              << text
              << "--brief";
    start("trans", arguments);
    waitForFinished();
    QString composeOutput(readAllStandardOutput());
    return composeOutput;
}
