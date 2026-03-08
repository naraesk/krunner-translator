/******************************************************************************
 *  Copyright (C) 2013 – 2020 by David Baum <david.baum@naraesk.eu>           *
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

#ifndef RUNNERTRANSLATOR_TRANSLATESHELLPROCESSTEST_H
#define RUNNERTRANSLATOR_TRANSLATESHELLPROCESSTEST_H

#include <QtTest/QtTest>
#include <QtCore/QObject>
#include "../src/TranslateShellProcess.h"
#include "src/language/LanguageRepository.h"

class TranslateShellProcessTest : public QObject {

Q_OBJECT;

    TranslateShellProcess process;
    LanguageRepository repository;

private Q_SLOTS:

    void initTestCase();

    void germanToEnglishTest();

};


#endif //RUNNERTRANSLATOR_TRANSLATESHELLPROCESSTEST_H
