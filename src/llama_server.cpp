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

#include "llama_server.h"

#include <QEventLoop>
#include <QFile>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QProcess>
#include <QStringList>
#include <QTimer>
#include <QUrl>

namespace {

// Locate llama-server.sh at runtime. The compile-time default bakes in the
// CMake source dir, which is wrong on machines that install the .so directly
// (e.g. via install.sh or a CI artifact), so probe the likely places first.
QString serverScriptPath() {
    const QStringList candidates = {
        QStringLiteral(HY_MT2_SERVER_SCRIPT),                             // repo checkout (dev setup)
        QStringLiteral("/usr/share/krunner-translator/llama-server.sh"),  // installed by install.sh
    };
    for (const QString &path : candidates) {
        if (QFile::exists(path)) {
            return path;
        }
    }
    return QStringLiteral(HY_MT2_SERVER_SCRIPT);
}

} // namespace

bool LlamaServer::probeHealth(int port) {
    // One-shot HTTP probe; returns after the reply arrives or a 2 s timeout.
    // A refused connection on localhost fails fast, so this returns in a few
    // milliseconds whether the server is up or down.
    const QUrl health(QStringLiteral("http://127.0.0.1:%1/health").arg(port));
    QNetworkAccessManager probe;
    QEventLoop probeLoop;
    QNetworkReply *probeReply = probe.get(QNetworkRequest(health));
    QObject::connect(probeReply, &QNetworkReply::finished, &probeLoop, &QEventLoop::quit);
    QTimer::singleShot(2000, &probeLoop, &QEventLoop::quit);
    probeLoop.exec();
    const bool up = probeReply->error() == QNetworkReply::NoError;
    probeReply->deleteLater();
    return up;
}

bool LlamaServer::ensureServer(int port, const QString &model, const QString &llamaDir, int ngl) {
    if (probeHealth(port)) {
        return true;
    }
    // Not running: launch via the shared script. Per-user settings are handed
    // over through the environment (the script reads MODEL/LLAMA_DIR/PORT/
    // NGL); only non-default values are set, so the script's own defaults
    // stay in effect for an empty field.
    const QByteArray savedModel = qgetenv("MODEL");
    const QByteArray savedLlamaDir = qgetenv("LLAMA_DIR");
    const QByteArray savedPort = qgetenv("PORT");
    const QByteArray savedNgl = qgetenv("NGL");
    if (!model.isEmpty()) {
        qputenv("MODEL", model.toLocal8Bit());
    }
    if (!llamaDir.isEmpty()) {
        qputenv("LLAMA_DIR", llamaDir.toLocal8Bit());
    }
    qputenv("PORT", QByteArray::number(port));
    qputenv("NGL", QByteArray::number(ngl));
    const QString script = serverScriptPath();
    const bool started = QProcess::startDetached(script, {QStringLiteral("start")}, QString(), nullptr);
    // The detached child keeps the environment captured at fork time; restore
    // the process values so the rest of krunner/kcms does not inherit our
    // overrides.
    if (savedModel.isEmpty()) {
        qunsetenv("MODEL");
    } else {
        qputenv("MODEL", savedModel);
    }
    if (savedLlamaDir.isEmpty()) {
        qunsetenv("LLAMA_DIR");
    } else {
        qputenv("LLAMA_DIR", savedLlamaDir);
    }
    if (savedPort.isEmpty()) {
        qunsetenv("PORT");
    } else {
        qputenv("PORT", savedPort);
    }
    if (savedNgl.isEmpty()) {
        qunsetenv("NGL");
    } else {
        qputenv("NGL", savedNgl);
    }
    if (started) {
        // Model load takes a few seconds; poll until the endpoint is healthy.
        for (int i = 0; i < 30; ++i) {
            QEventLoop loop;
            QTimer::singleShot(1000, &loop, &QEventLoop::quit);
            loop.exec();
            if (probeHealth(port)) {
                return true;
            }
        }
    }
    return false;
}
