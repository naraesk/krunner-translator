
//
// Created by David Baum on 10.09.20.
//

#include "translateShell.h"

TranslateShell::TranslateShell(Plasma::AbstractRunner * runner, Plasma::RunnerContext& context, const QString &text, const QPair<QString, QString> &language)
        : m_runner (runner), m_context (context)
{
}
