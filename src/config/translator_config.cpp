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
#include <KSharedConfig>
#include <KConfigGroup>
#include <KPluginFactory>
#include <KRunner/AbstractRunner>
#include <QtDebug>
#include <QtWidgets/QGridLayout>

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

    connect(m_ui->bingEnable, &QCheckBox::checkStateChanged, this, &TranslatorConfig::warningHandler);
    connect(m_ui->googleEnable, &QCheckBox::checkStateChanged, this, &TranslatorConfig::warningHandler);
    connect(m_ui->baiduEnable, &QCheckBox::checkStateChanged, this, &TranslatorConfig::warningHandler);
    connect(m_ui->youdaoEnable, &QCheckBox::checkStateChanged, this, &TranslatorConfig::warningHandler);
    connect(m_ui->deeplEnable, &QCheckBox::checkStateChanged, this, &TranslatorConfig::warningHandler);
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
    m_ui->googleEnable->setChecked(grp.readEntry(CONFIG_GOOGLE_ENABLE, true));
    m_ui->bingEnable->setChecked(grp.readEntry(CONFIG_BING_ENABLE, false));
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
}

void TranslatorConfig::warningHandler() {

    // show warning if only bing is enabled

    if (m_ui->bingEnable->isChecked() &&
        !m_ui->googleEnable->isChecked() &&
        !m_ui->baiduEnable->isChecked() &&
        !m_ui->youdaoEnable->isChecked() &&
        !m_ui->deeplEnable->isChecked()) {
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
        !m_ui->deeplEnable->isChecked()) {
        m_ui->noEngineWarning->show();
    } else {
        m_ui->noEngineWarning->hide();
    }
}

#include "translator_config.moc"
