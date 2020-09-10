
//
// Created by David Baum on 10.09.20.
//

#ifndef RUNNERTRANSLATOR_TRANSLATESHELLPROCESS_H
#define RUNNERTRANSLATOR_TRANSLATESHELLPROCESS_H

#include <QProcess>
#include <QString>

class TranslateShellProcess : public QProcess
{
Q_OBJECT
public:
    explicit TranslateShellProcess( QObject *parent = 0);
    ~TranslateShellProcess() override;

public Q_SLOTS:
    QString translate(const QPair<QString, QString> &language, const QString &file);
};

#endif //RUNNERTRANSLATOR_TRANSLATESHELLPROCESS_H
