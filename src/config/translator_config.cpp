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
#include "src/languages.h"

#include <KRunner/AbstractRunner>
#include <KSharedConfig>

#include <QGridLayout>

K_PLUGIN_CLASS(TranslatorConfig)

TranslatorConfigForm::TranslatorConfigForm(QWidget *parent) : QWidget(parent) {
    setupUi(this);
}

TranslatorConfig::TranslatorConfig(QObject *parent, const KPluginMetaData &args) :
        KCModule(parent, args) {
    m_ui = new TranslatorConfigForm(this->widget());
    QGridLayout *layout = new QGridLayout(this->widget());
    layout->addWidget(m_ui, 0, 0);

    KSharedConfigPtr shared_config = KSharedConfig::openConfig(QStringLiteral("krunnerrc"));
    config = shared_config->group(QStringLiteral("Runners")).group(QStringLiteral("krunner_translator"));
    config.config()->reparseConfiguration();

    warningHandler();
    languages.initialize();

    const QList<Language> supportedLanguages = languages.getSupportedLanguages();
    for (auto language: supportedLanguages) {
        QVariant variant = QVariant::fromValue<Language>(language);
        m_ui->primaryLanguage->addItem(language.getCombinedName(), variant);
        m_ui->secondaryLanguage->addItem(language.getCombinedName(), variant);
    }

    // TODO: QCheckBox::stateChanged() is deprecated in newer versions of Qt, but Ubuntu is behind.
    connect(m_ui->primaryLanguage, &QComboBox::currentTextChanged, this, &TranslatorConfig::markAsChanged);
    connect(m_ui->secondaryLanguage, &QComboBox::currentTextChanged, this, &TranslatorConfig::markAsChanged);
    connect(m_ui->baiduAPPID, &QLineEdit::textChanged, this, &TranslatorConfig::markAsChanged);
    connect(m_ui->baiduApiKey, &QLineEdit::textChanged, this, &TranslatorConfig::markAsChanged);
    connect(m_ui->youdaoAPPID, &QLineEdit::textChanged, this, &TranslatorConfig::markAsChanged);
    connect(m_ui->youdaoAppSec, &QLineEdit::textChanged, this, &TranslatorConfig::markAsChanged);
    connect(m_ui->baiduEnable, &QCheckBox::stateChanged, this, &TranslatorConfig::markAsChanged);
    connect(m_ui->youdaoEnable, &QCheckBox::stateChanged, this, &TranslatorConfig::markAsChanged);
    connect(m_ui->googleEnable, &QCheckBox::stateChanged, this, &TranslatorConfig::markAsChanged);
    connect(m_ui->bingEnable, &QCheckBox::stateChanged, this, &TranslatorConfig::markAsChanged);

    connect(m_ui->bingEnable, &QCheckBox::stateChanged, this, &TranslatorConfig::warningHandler);
    connect(m_ui->googleEnable, &QCheckBox::stateChanged, this, &TranslatorConfig::warningHandler);
    connect(m_ui->baiduEnable, &QCheckBox::stateChanged, this, &TranslatorConfig::warningHandler);
    connect(m_ui->youdaoEnable, &QCheckBox::stateChanged, this, &TranslatorConfig::warningHandler);
}

void TranslatorConfig::load() {
    config.config()->reparseConfiguration();

    QString abbrPrimaryLanguage = config.readEntry(CONFIG_PRIMARY, "en");
    QString abbrSecondaryLanguage = config.readEntry(CONFIG_SECONDARY, "es");
    QString textPrimaryLanguage = languages.getCombinedName(abbrPrimaryLanguage);
    QString textSecondaryLanguage = languages.getCombinedName(abbrSecondaryLanguage);
    m_ui->primaryLanguage->setCurrentText(textPrimaryLanguage);
    m_ui->secondaryLanguage->setCurrentText(textSecondaryLanguage);
    m_ui->baiduAPPID->setText(config.readEntry(CONFIG_BAIDU_APPID, ""));
    m_ui->baiduApiKey->setText(config.readEntry(CONFIG_BAIDU_APIKEY, ""));
    m_ui->youdaoAPPID->setText(config.readEntry(CONFIG_YOUDAO_APPID, ""));
    m_ui->youdaoAppSec->setText(config.readEntry(CONFIG_YOUDAO_APPSEC, ""));
    m_ui->baiduEnable->setChecked(config.readEntry(CONFIG_BAIDU_ENABLE, false));
    m_ui->youdaoEnable->setChecked(config.readEntry(CONFIG_YOUDAO_ENABLE, false));
    m_ui->googleEnable->setChecked(config.readEntry(CONFIG_GOOGLE_ENABLE, true));
    m_ui->bingEnable->setChecked(config.readEntry(CONFIG_BING_ENABLE, false));

    markAsChanged();
}

void TranslatorConfig::save() {
    Language primaryLanguage = m_ui->primaryLanguage->currentData().value<Language>();
    Language secondaryLanguage = m_ui->secondaryLanguage->currentData().value<Language>();

    config.writeEntry(CONFIG_PRIMARY, primaryLanguage.getAbbreviation());
    config.writeEntry(CONFIG_SECONDARY, secondaryLanguage.getAbbreviation());
    config.writeEntry(CONFIG_BAIDU_APPID, m_ui->baiduAPPID->text());
    config.writeEntry(CONFIG_BAIDU_APIKEY, m_ui->baiduApiKey->text());
    config.writeEntry(CONFIG_YOUDAO_APPID, m_ui->youdaoAPPID->text());
    config.writeEntry(CONFIG_YOUDAO_APPSEC, m_ui->youdaoAppSec->text());
    config.writeEntry(CONFIG_BAIDU_ENABLE, m_ui->baiduEnable->isChecked());
    config.writeEntry(CONFIG_YOUDAO_ENABLE, m_ui->youdaoEnable->isChecked());
    config.writeEntry(CONFIG_GOOGLE_ENABLE, m_ui->googleEnable->isChecked());
    config.writeEntry(CONFIG_BING_ENABLE, m_ui->bingEnable->isChecked());

    config.config()->sync();
}

void TranslatorConfig::warningHandler() {

    // show warning if only bing is enabled

    if (m_ui->bingEnable->isChecked() &&
        !m_ui->googleEnable->isChecked() &&
        !m_ui->baiduEnable->isChecked() &&
        !m_ui->youdaoEnable->isChecked()) {
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
        !m_ui->youdaoEnable->isChecked()) {
        m_ui->noEngineWarning->show();
    } else {
        m_ui->noEngineWarning->hide();
    }
}

#include "translator_config.moc"
