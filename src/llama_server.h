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

#ifndef RUNNERTRANSLATOR_LLAMA_SERVER_H
#define RUNNERTRANSLATOR_LLAMA_SERVER_H

#include <QString>

/**
 * Shared management for the local llama.cpp server (managed by the
 * repository's `llama-server.sh`). Both the KRunner engine and the KCM
 * configuration module run in separate processes and both need to probe and
 * start the same shared server, so the logic lives here in one place.
 *
 * The script keeps its plain command-line interface (`start`/`stop`/`status`/
 * `test`); per-user settings are handed to it through the environment
 * (MODEL / LLAMA_DIR / PORT / NGL), which it already honours.
 */
class LlamaServer {
public:
    // One-shot HTTP probe of 127.0.0.1:<port>/health. A refused connection on
    // localhost fails fast, so this returns in a few milliseconds either way.
    static bool probeHealth(int port);
    // Start the shared server if it is not already answering, passing the
    // given settings through the environment. Returns true once the endpoint
    // is healthy (or was already up).
    static bool ensureServer(int port, const QString &model, const QString &llamaDir, int ngl);
};

#endif //RUNNERTRANSLATOR_LLAMA_SERVER_H
