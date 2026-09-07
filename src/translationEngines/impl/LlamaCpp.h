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

#ifndef RUNNERTRANSLATOR_LLAMACPP_H
#define RUNNERTRANSLATOR_LLAMACPP_H

#include <KRunner/AbstractRunner>
#include <src/translationEngines/api/CommandLineEngine.h>

/**
 * Local translation engine backed by a llama.cpp OpenAI-compatible server
 * (default model tencent/Hy-MT2-1.8B-GGUF, managed by `llama-server.sh`).
 * The server is shared on the configured port (default 127.0.0.1:8080)
 * with other tools that use the same local backend.
 *
 * Server settings (model file, llama.cpp directory, port, GPU layers) are
 * passed to the constructor by the runner, which reads them from the KRunner
 * configuration. When the engine has to start the server itself, those
 * values are handed to the shared script through the environment
 * (MODEL/LLAMA_DIR/PORT/NGL), so the script keeps its interface unchanged.
 */
class LlamaCpp : public CommandLineEngine {

public:
    explicit LlamaCpp(KRunner::AbstractRunner*,
                      int port = 8080,
                      int ngl = 99,
                      const QString &model = QString(),
                      const QString &llamaDir = QString());
    ~LlamaCpp() override;
    KRunner::QueryMatch translate(const TranslationQuery* query) override;

    // True if the local llama.cpp server is already answering on the port.
    bool isServerUp() const;
    // Placeholder match ("唤起中..." / "翻译中...") shown while work happens;
    // the real result replaces it (same match id, unique+weak results).
    KRunner::QueryMatch placeholder(const QString &text) const;
    // Whether this engine can serve the query (source and target must both be
    // languages the local model handles; see languageName() in the .cpp).
    bool canTranslate(const TranslationQuery *query) const;
    // Lets translate() push an intermediate "翻译中..." match while the
    // translation request is in flight.
    void setContext(KRunner::RunnerContext *ctx);
    // Stable id shared by placeholders and the real result so KRunner
    // replaces the former with the latter.
    static QString matchId() { return QStringLiteral("llamacpp"); }

private:
    KRunner::QueryMatch match;
    KRunner::RunnerContext *m_context = nullptr;
    int m_port = 8080;
    int m_ngl = 99;
    QString m_model;    // empty: the script's own default model
    QString m_llamaDir; // empty: the script's own default llama.cpp checkout
    static QString buildPrompt(const TranslationQuery* query);
    bool ensureServer();
};

#endif //RUNNERTRANSLATOR_LLAMACPP_H
