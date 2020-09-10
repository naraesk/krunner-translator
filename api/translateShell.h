
//
// Created by David Baum on 10.09.20.
//

#ifndef RUNNERTRANSLATOR_TRANSLATESHELL_H
#define RUNNERTRANSLATOR_TRANSLATESHELL_H


#include <QtCore/QObject>
#include <KRunner/AbstractRunner>

class TranslateShell : public QObject {

    Q_OBJECT

public:
    TranslateShell(Plasma::AbstractRunner*, Plasma::RunnerContext&, const QString &, const QPair<QString, QString> &);

private:
    Plasma::AbstractRunner * m_runner;
    Plasma::RunnerContext m_context;
};


#endif //RUNNERTRANSLATOR_TRANSLATESHELL_H
