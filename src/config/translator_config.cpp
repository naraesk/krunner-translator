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

#include "translator_config.h"
#include "src/language/Language.h"
#include "src/llama_server.h"
#include <KSharedConfig>
#include <KConfigGroup>
#include <KPluginFactory>
#include <KRunner/AbstractRunner>
#include <KLocalizedString>
#include <QtDebug>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QFileDialog>
#include <QFutureWatcher>
#include <QtConcurrent>
#include <QLabel>
#include <QDir>
#include <QFileInfo>

K_PLUGIN_CLASS_WITH_JSON(TranslatorConfig, "kcm_krunner_translator.json")

TranslatorConfigForm::TranslatorConfigForm(QWidget *parent) : QWidget(parent) {
    setupUi(this);
}

TranslatorConfig::TranslatorConfig(QObject *parent, const KPluginMetaData &metaData) :
        KCModule(parent, metaData) {
    m_ui = new TranslatorConfigForm(widget());
    QGridLayout *layout = new QGridLayout(widget());
    layout->addWidget(m_ui, 0, 0);

    warningHandler();
    languages.initialize();

    const QList<Language> supportedLanguages = languages.getSupportedLanguages();
    for (auto language: supportedLanguages) {
        QVariant variant = QVariant::fromValue<Language>(language);
        m_ui->primaryLanguage->addItem(language.getCombinedName(), variant);
        m_ui->secondaryLanguage->addItem(language.getCombinedName(), variant);
    }

    connect(m_ui->primaryLanguage, &QComboBox::currentTextChanged, this, [this]() { setNeedsSave(true); });
    connect(m_ui->secondaryLanguage, &QComboBox::currentTextChanged, this, [this]() { setNeedsSave(true); });
    connect(m_ui->baiduAPPID, &QLineEdit::textChanged, this, [this]() { setNeedsSave(true); });
    connect(m_ui->baiduApiKey, &QLineEdit::textChanged, this, [this]() { setNeedsSave(true); });
    connect(m_ui->youdaoAPPID, &QLineEdit::textChanged, this, [this]() { setNeedsSave(true); });
    connect(m_ui->youdaoAppSec, &QLineEdit::textChanged, this, [this]() { setNeedsSave(true); });
    connect(m_ui->deeplApiKey, &QLineEdit::textChanged, this, [this]() { setNeedsSave(true); });
    connect(m_ui->baiduEnable, &QCheckBox::checkStateChanged, this, [this]() { setNeedsSave(true); });
    connect(m_ui->youdaoEnable, &QCheckBox::checkStateChanged, this, [this]() { setNeedsSave(true); });
    connect(m_ui->deeplEnable, &QCheckBox::checkStateChanged, this, [this]() { setNeedsSave(true); });
    connect(m_ui->googleEnable, &QCheckBox::checkStateChanged, this, [this]() { setNeedsSave(true); });
    connect(m_ui->bingEnable, &QCheckBox::checkStateChanged, this, [this]() { setNeedsSave(true); });
    connect(m_ui->llamacppEnable, &QCheckBox::checkStateChanged, this, [this]() { setNeedsSave(true); });
    connect(m_ui->llamacppModel, &QLineEdit::textChanged, this, [this]() { setNeedsSave(true); });
    connect(m_ui->llamacppDir, &QLineEdit::textChanged, this, [this]() { setNeedsSave(true); });
    connect(m_ui->llamacppPort, &QSpinBox::valueChanged, this, [this]() { setNeedsSave(true); });
    connect(m_ui->llamacppNgl, &QSpinBox::valueChanged, this, [this]() { setNeedsSave(true); });
    connect(m_ui->llamacppModelBrowse, &QPushButton::clicked, this, &TranslatorConfig::browseLlamaModel);
    connect(m_ui->llamacppDirBrowse, &QPushButton::clicked, this, &TranslatorConfig::browseLlamaDir);
    connect(m_ui->llamacppTestButton, &QPushButton::clicked, this, &TranslatorConfig::testLlamaServer);

    connect(m_ui->bingEnable, &QCheckBox::checkStateChanged, this, &TranslatorConfig::warningHandler);
    connect(m_ui->googleEnable, &QCheckBox::checkStateChanged, this, &TranslatorConfig::warningHandler);
    connect(m_ui->baiduEnable, &QCheckBox::checkStateChanged, this, &TranslatorConfig::warningHandler);
    connect(m_ui->youdaoEnable, &QCheckBox::checkStateChanged, this, &TranslatorConfig::warningHandler);
    connect(m_ui->deeplEnable, &QCheckBox::checkStateChanged, this, &TranslatorConfig::warningHandler);
    connect(m_ui->llamacppEnable, &QCheckBox::checkStateChanged, this, &TranslatorConfig::warningHandler);
}

void TranslatorConfig::load() {
    KCModule::load();

    KSharedConfig::Ptr cfg = KSharedConfig::openConfig(QStringLiteral("krunnerrc"));
    KConfigGroup grp = cfg->group(QStringLiteral("Runners"));
    grp = KConfigGroup(&grp, QStringLiteral("krunner_translator"));

    QString abbrPrimaryLanguage = grp.readEntry(CONFIG_PRIMARY, QStringLiteral("en"));
    QString abbrSecondaryLanguage = grp.readEntry(CONFIG_SECONDARY, QStringLiteral("es"));
    QString textPrimaryLanguage = languages.getCombinedName(abbrPrimaryLanguage);
    QString textSecondaryLanguage = languages.getCombinedName(abbrSecondaryLanguage);
    m_ui->primaryLanguage->setCurrentText(textPrimaryLanguage);
    m_ui->secondaryLanguage->setCurrentText(textSecondaryLanguage);
    m_ui->baiduAPPID->setText(grp.readEntry(CONFIG_BAIDU_APPID, QStringLiteral("")));
    m_ui->baiduApiKey->setText(grp.readEntry(CONFIG_BAIDU_APIKEY, QStringLiteral("")));
    m_ui->youdaoAPPID->setText(grp.readEntry(CONFIG_YOUDAO_APPID, QStringLiteral("")));
    m_ui->youdaoAppSec->setText(grp.readEntry(CONFIG_YOUDAO_APPSEC, QStringLiteral("")));
    m_ui->deeplApiKey->setText(grp.readEntry(CONFIG_DEEPL_APIKEY, QStringLiteral("")));
    m_ui->baiduEnable->setChecked(grp.readEntry(CONFIG_BAIDU_ENABLE, false));
    m_ui->youdaoEnable->setChecked(grp.readEntry(CONFIG_YOUDAO_ENABLE, false));
    m_ui->deeplEnable->setChecked(grp.readEntry(CONFIG_DEEPL_ENABLE, false));
    m_ui->googleEnable->setChecked(grp.readEntry(CONFIG_GOOGLE_ENABLE, false));
    m_ui->bingEnable->setChecked(grp.readEntry(CONFIG_BING_ENABLE, false));
    m_ui->llamacppEnable->setChecked(grp.readEntry(CONFIG_LLAMACPP_ENABLE, true));
    m_ui->llamacppModel->setText(grp.readEntry(CONFIG_LLAMACPP_MODEL, QStringLiteral("")));
    m_ui->llamacppDir->setText(grp.readEntry(CONFIG_LLAMACPP_DIR, QStringLiteral("")));
    m_ui->llamacppPort->setValue(grp.readEntry(CONFIG_LLAMACPP_PORT, 8080));
    m_ui->llamacppNgl->setValue(grp.readEntry(CONFIG_LLAMACPP_NGL, 99));
}

void TranslatorConfig::save() {
    KCModule::save();

    KSharedConfig::Ptr cfg = KSharedConfig::openConfig(QStringLiteral("krunnerrc"));
    KConfigGroup grp = cfg->group(QStringLiteral("Runners"));
    grp = KConfigGroup(&grp, QStringLiteral("krunner_translator"));

    Language primaryLanguage = m_ui->primaryLanguage->currentData().value<Language>();
    Language secondaryLanguage = m_ui->secondaryLanguage->currentData().value<Language>();

    grp.writeEntry(CONFIG_PRIMARY, primaryLanguage.getAbbreviation());
    grp.writeEntry(CONFIG_SECONDARY, secondaryLanguage.getAbbreviation());
    grp.writeEntry(CONFIG_BAIDU_APPID, m_ui->baiduAPPID->text());
    grp.writeEntry(CONFIG_BAIDU_APIKEY, m_ui->baiduApiKey->text());
    grp.writeEntry(CONFIG_YOUDAO_APPID, m_ui->youdaoAPPID->text());
    grp.writeEntry(CONFIG_YOUDAO_APPSEC, m_ui->youdaoAppSec->text());
    grp.writeEntry(CONFIG_DEEPL_APIKEY, m_ui->deeplApiKey->text());
    grp.writeEntry(CONFIG_BAIDU_ENABLE, m_ui->baiduEnable->isChecked());
    grp.writeEntry(CONFIG_YOUDAO_ENABLE, m_ui->youdaoEnable->isChecked());
    grp.writeEntry(CONFIG_DEEPL_ENABLE, m_ui->deeplEnable->isChecked());
    grp.writeEntry(CONFIG_GOOGLE_ENABLE, m_ui->googleEnable->isChecked());
    grp.writeEntry(CONFIG_BING_ENABLE, m_ui->bingEnable->isChecked());
    grp.writeEntry(CONFIG_LLAMACPP_ENABLE, m_ui->llamacppEnable->isChecked());
    grp.writeEntry(CONFIG_LLAMACPP_MODEL, m_ui->llamacppModel->text().trimmed());
    grp.writeEntry(CONFIG_LLAMACPP_DIR, m_ui->llamacppDir->text().trimmed());
    grp.writeEntry(CONFIG_LLAMACPP_PORT, m_ui->llamacppPort->value());
    grp.writeEntry(CONFIG_LLAMACPP_NGL, m_ui->llamacppNgl->value());
}

void TranslatorConfig::browseLlamaModel() {
    QString startDir;
    const QString model = m_ui->llamacppModel->text().trimmed();
    const QString dir = m_ui->llamacppDir->text().trimmed();
    if (!model.isEmpty()) {
        startDir = QFileInfo(model).absolutePath();
    } else if (!dir.isEmpty()) {
        startDir = dir + QStringLiteral("/models");
    } else {
        startDir = QDir::homePath();
    }
    const QString file = QFileDialog::getOpenFileName(widget(),
            i18n("Select model file"),
            startDir,
            i18n("GGUF models") + QStringLiteral(" (*.gguf);;") + i18n("All files") + QStringLiteral(" (*)"));
    if (!file.isEmpty()) {
        m_ui->llamacppModel->setText(file);
    }
}

void TranslatorConfig::browseLlamaDir() {
    const QString dir = QFileDialog::getExistingDirectory(widget(),
            i18n("Select llama.cpp directory"),
            m_ui->llamacppDir->text().isEmpty() ? QDir::homePath() : m_ui->llamacppDir->text());
    if (!dir.isEmpty()) {
        m_ui->llamacppDir->setText(dir);
    }
}

void TranslatorConfig::testLlamaServer() {
    // Run the probe/start off the GUI thread: a cold start loads the model
    // and polls up to 30 s, which must not freeze the settings dialog.
    m_ui->llamacppTestButton->setEnabled(false);
    m_ui->llamacppTestResult->setText(i18n("Checking… (first start loads the model, this can take a while)"));
    const int port = m_ui->llamacppPort->value();
    const QString model = m_ui->llamacppModel->text().trimmed();
    const QString dir = m_ui->llamacppDir->text().trimmed();
    const int ngl = m_ui->llamacppNgl->value();

    auto *watcher = new QFutureWatcher<bool>(this);
    connect(watcher, &QFutureWatcher<bool>::finished, this, [this, watcher, port]() {
        watcher->deleteLater();
        m_ui->llamacppTestButton->setEnabled(true);
        if (watcher->result()) {
            m_ui->llamacppTestResult->setText(i18n("Server is up on 127.0.0.1:%1.").arg(port));
        } else {
            m_ui->llamacppTestResult->setText(i18n(
                "Server did not start on 127.0.0.1:%1. Check the model path, the llama.cpp directory, and the log /tmp/krunner-translator-llama.log.")
                    .arg(port));
        }
    });
    watcher->setFuture(QtConcurrent::run([port, model, dir, ngl]() -> bool {
        return LlamaServer::ensureServer(port, model, dir, ngl);
    }));
}

void TranslatorConfig::warningHandler() {

    // show warning if only bing is enabled

    if (m_ui->bingEnable->isChecked() &&
        !m_ui->googleEnable->isChecked() &&
        !m_ui->baiduEnable->isChecked() &&
        !m_ui->youdaoEnable->isChecked() &&
        !m_ui->deeplEnable->isChecked() &&
        !m_ui->llamacppEnable->isChecked()) {
        m_ui->bingWarningOnlyEngine->show();
    } else {
        m_ui->bingWarningOnlyEngine->hide();
    }

    // show warning if bing is enabled

    if (m_ui->bingEnable->isChecked()) {
        m_ui->bingWarningReliability->show();
    } else {
        m_ui->bingWarningReliability->hide();
    }

    // show error message if all engines are disabled

    if (!m_ui->bingEnable->isChecked() &&
        !m_ui->googleEnable->isChecked() &&
        !m_ui->baiduEnable->isChecked() &&
        !m_ui->youdaoEnable->isChecked() &&
        !m_ui->deeplEnable->isChecked() &&
        !m_ui->llamacppEnable->isChecked()) {
        m_ui->noEngineWarning->show();
    } else {
        m_ui->noEngineWarning->hide();
    }
}

#include "translator_config.moc"
