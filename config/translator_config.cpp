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
#include "languages.h"
#include "LanguageRepository.h"
#include <KSharedConfig>
#include <KPluginFactory>
#include <krunner/abstractrunner.h>
#include <QtDebug>
#include <QtWidgets/QGridLayout>

K_PLUGIN_FACTORY(TranslatorConfigFactory, registerPlugin<TranslatorConfig>("kcm_krunner_translator");)

TranslatorConfigForm::TranslatorConfigForm(QWidget* parent) : QWidget(parent)
{
    setupUi(this);
}

TranslatorConfig::TranslatorConfig(QWidget* parent, const QVariantList& args) :
        KCModule(parent, args)
{
    m_ui = new TranslatorConfigForm(this);
    QGridLayout* layout = new QGridLayout(this);
    layout->addWidget(m_ui, 0, 0);

    warningHandler();
    LanguageRepository::initialize();

    QList<Language*> * supportedLanguages = LanguageRepository::getSupportedLanguages();
    for(auto language: *supportedLanguages) {
        m_ui->primaryLanguage->addItem(language->getCombinedName());
        m_ui->secondaryLanguage->addItem(language->getCombinedName());
    }
    connect(m_ui->primaryLanguage,SIGNAL(currentTextChanged(QString)),this, SLOT(changed()));
    connect(m_ui->secondaryLanguage,SIGNAL(currentTextChanged(QString)),this, SLOT(changed()));
    connect(m_ui->baiduAPPID,SIGNAL(textChanged(QString)),this,SLOT(changed()));
    connect(m_ui->baiduApiKey,SIGNAL(textChanged(QString)),this,SLOT(changed()));
    connect(m_ui->youdaoAPPID,SIGNAL(textChanged(QString)),this,SLOT(changed()));
    connect(m_ui->youdaoAppSec,SIGNAL(textChanged(QString)),this,SLOT(changed()));
    connect(m_ui->baiduEnable, SIGNAL(stateChanged(int)),this,SLOT(changed()));
    connect(m_ui->youdaoEnable, SIGNAL(stateChanged(int)),this,SLOT(changed()));
    connect(m_ui->googleEnable, SIGNAL(stateChanged(int)), this, SLOT(changed()));
    connect(m_ui->bingEnable, SIGNAL(stateChanged(int)), this, SLOT(changed()));
    
    connect(m_ui->bingEnable, SIGNAL(stateChanged(int)), this, SLOT(warningHandler()));
    connect(m_ui->googleEnable, SIGNAL(stateChanged(int)), this, SLOT(warningHandler()));
    connect(m_ui->baiduEnable, SIGNAL(stateChanged(int)), this, SLOT(warningHandler()));
    connect(m_ui->youdaoEnable, SIGNAL(stateChanged(int)), this, SLOT(warningHandler()));
}

void TranslatorConfig::load()
{
    KCModule::load();

    KSharedConfig::Ptr cfg = KSharedConfig::openConfig(QStringLiteral("krunnerrc"));
    KConfigGroup grp = cfg->group("Runners");
    grp = KConfigGroup(&grp, "Translator");
    
    QString abbrPrimaryLanguage = grp.readEntry(CONFIG_PRIMARY, "en");
    QString abbrSecondaryLanguage = grp.readEntry(CONFIG_SECONDARY, "es");
    QString textPrimaryLanguage = LanguageRepository::getCombinedName(abbrPrimaryLanguage);
    QString textSecondaryLanguage = LanguageRepository::getCombinedName(abbrSecondaryLanguage);
    m_ui->primaryLanguage->setCurrentText(textPrimaryLanguage);
    m_ui->secondaryLanguage->setCurrentText(textSecondaryLanguage);
    m_ui->baiduAPPID->setText(grp.readEntry(CONFIG_BAIDU_APPID, ""));
    m_ui->baiduApiKey->setText(grp.readEntry(CONFIG_BAIDU_APIKEY, ""));
    m_ui->youdaoAPPID->setText(grp.readEntry(CONFIG_YOUDAO_APPID, ""));
    m_ui->youdaoAppSec->setText(grp.readEntry(CONFIG_YOUDAO_APPSEC, ""));
    m_ui->baiduEnable->setChecked(grp.readEntry(CONFIG_BAIDU_ENABLE, false));
    m_ui->youdaoEnable->setChecked(grp.readEntry(CONFIG_YOUDAO_ENABLE, false));
    m_ui->googleEnable->setChecked(grp.readEntry(CONFIG_GOOGLE_ENABLE, true));
    m_ui->bingEnable->setChecked(grp.readEntry(CONFIG_BING_ENABLE, false));
}

void TranslatorConfig::save()
{
    KCModule::save();

    KSharedConfig::Ptr cfg = KSharedConfig::openConfig(QStringLiteral("krunnerrc"));
    KConfigGroup grp = cfg->group("Runners");
    grp = KConfigGroup(&grp, "Translator");

    int indexPrimary = m_ui->primaryLanguage->currentIndex();
    int indexSecondary = m_ui-> secondaryLanguage->currentIndex();

    QList<Language*> * languages = LanguageRepository::getSupportedLanguages();
    grp.writeEntry(CONFIG_PRIMARY, languages->at(indexPrimary)->getAbbreviation());
    grp.writeEntry(CONFIG_SECONDARY, languages->at(indexSecondary)->getAbbreviation());
    grp.writeEntry(CONFIG_BAIDU_APPID,m_ui->baiduAPPID->text());
    grp.writeEntry(CONFIG_BAIDU_APIKEY,m_ui->baiduApiKey->text());
    grp.writeEntry(CONFIG_YOUDAO_APPID,m_ui->youdaoAPPID->text());
    grp.writeEntry(CONFIG_YOUDAO_APPSEC,m_ui->youdaoAppSec->text());
    grp.writeEntry(CONFIG_BAIDU_ENABLE, m_ui->baiduEnable->isChecked());
    grp.writeEntry(CONFIG_YOUDAO_ENABLE, m_ui->youdaoEnable->isChecked());
    grp.writeEntry(CONFIG_GOOGLE_ENABLE, m_ui->googleEnable->isChecked());
    grp.writeEntry(CONFIG_BING_ENABLE, m_ui->bingEnable->isChecked());
    emit changed(true);
}

void TranslatorConfig::warningHandler() {
    
    // show warning if only bing is enabled
    
    if(m_ui->bingEnable->isChecked() &&
        !m_ui->googleEnable->isChecked() &&
        !m_ui->baiduEnable->isChecked() &&
        !m_ui->youdaoEnable->isChecked()) {
        m_ui->bingWarningOnlyEngine->show();
    } else {
        m_ui->bingWarningOnlyEngine->hide();
    }
    
    // show warning if bing is enabled
    
    if(m_ui->bingEnable->isChecked()){
        m_ui->bingWarningReliability->show();
    } else {
        m_ui->bingWarningReliability->hide();
    }
    
    // show error message if all engines are disabled
    
    if(!m_ui->bingEnable->isChecked() &&
        !m_ui->googleEnable->isChecked() &&
        !m_ui->baiduEnable->isChecked() &&
        !m_ui->youdaoEnable->isChecked()) {
        m_ui->noEngineWarning->show();
    } else {
        m_ui->noEngineWarning->hide();
    }
}

#include "translator_config.moc"
