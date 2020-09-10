
//
// Created by David Baum on 10.09.20.
//

#include "translateShell.h"
#include "translateShellProcess.h"

TranslateShell::TranslateShell(Plasma::AbstractRunner *runner, Plasma::RunnerContext &context, const QString &text,
                               const QPair<QString, QString> &language)
        : m_runner(runner), m_context(context) {

    TranslateShellProcess process(this);
    QString result = process.translate(language, text);
    Plasma::QueryMatch match(m_runner);
    match.setType(Plasma::QueryMatch::InformationalMatch);
    match.setIcon(QIcon::fromTheme("applications-education-language"));
    match.setText(result);
    match.setRelevance(0.01);
    m_context.addMatch(match);
    emit finished();
}

#include "moc_translateShell.cpp"
