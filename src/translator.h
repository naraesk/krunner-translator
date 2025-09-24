/******************************************************************************
 *  Copyright (C) 2013 – 2018 by David Baum <david.baum@naraesk.eu>           *
 *                                                                            *
 *  This library is free software; you can redistribute it and/or modify      *
 *  it under the terms of the GNU Lesser General Public License as published  *
 *  by the Free Software Foundation; either version 2 of the License or (at   *
 *  your option) any later version.                                           *
 *                                                                            *
 *  This library is distributed in the hope that it will be useful,           *
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of            *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU         *
 *  Library General Public License for more details.                          *
 *                                                                            *
 *  You should have received a copy of the GNU Lesser General Public License  *
 *  along with this library; see the file COPYING.LIB.                        *
 *  If not, see <http://www.gnu.org/licenses/>.                               *
 *****************************************************************************/

#ifndef TRANSLATOR_H
#define TRANSLATOR_H

#include "provider/GoogleTranslate.h"
#include "LanguageRepository.h"

#include <KRunner/AbstractRunner>
#include <KRunner/Action>

class Translator : public KRunner::AbstractRunner
{
    Q_OBJECT

public:
    Translator(QObject *parent, const KPluginMetaData &args);
    void match(KRunner::RunnerContext &) override;
    void run(const KRunner::RunnerContext &, const KRunner::QueryMatch &) override;
    void reloadConfiguration() override;

private:
    bool parseTerm(const QString &, QString &, QPair<QString, QString> &);
    QList<KRunner::Action> actions;
    QString m_primary;
    QString m_secondary;
    QString m_baiduAPPID;
    QString m_baiduAPIKey;
    QString m_youdaoAPPID;
    QString m_youdaoAppSec;
    bool m_baiduEnable;
    bool m_youdaoEnable;
    QList<CommandLineEngine *> engines;
    LanguageRepository languages;
};

#endif
