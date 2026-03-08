/******************************************************************************
 *  Copyright (C) 2024 by Sheng Huang <shenghuang147@gmail.com>               *
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

#ifndef DEEPL_H
#define DEEPL_H

#include <KRunner/AbstractRunner>
#include <QtNetwork/QNetworkReply>
#include <src/language/Language.h>
#include <src/TranslationQuery.h>

/**
 * API Implementation for DeepL https://developers.deepl.com/docs/api-reference/translate
 */

class Deepl : public QObject
{

    Q_OBJECT

public:
    Deepl(KRunner::AbstractRunner*, KRunner::RunnerContext&, const TranslationQuery*, const QString &);

private Q_SLOTS:
   void parseResult(QNetworkReply*);

Q_SIGNALS:
	void finished();

private:
   KRunner::AbstractRunner * m_runner;
   QNetworkAccessManager * m_manager;
   KRunner::RunnerContext m_context;
   QString langMapper(QString);
};

#endif
