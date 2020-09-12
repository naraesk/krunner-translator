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
#include "helper.h"
#include <KSharedConfig>
#include <KPluginFactory>
#include <krunner/abstractrunner.h>
#include <QtDebug>

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
    
    const QStringList languages = {"Albanian", "Afrikaans", "Arabic", "Armenian", "Azerbaijan", "Basque", "Belarusian", "Bosnian", "Bulgarian", "Catalan", "Chinese", "Croatian", "Czech", "Danish", "Dutch", "English", "Estonian", "Finish", "French", "Galician", "Georgian", "German", "Greek", "Haitian (Creole)", "Hungarian", "Icelandic", "Indonesian", "Irish", "Italian", "Japanese", "Kazakh", "Korean", "Kyrgyz", "Latin", "Latvian", "Lithuanian", "Macedonian", "Malagasy", "Malay", "Maltese", "Mongolian", "Norwegian", "Persian", "Polish", "Portuguese", "Romanian", "Russian", "Serbian", "Slovakian", "Slovenian", "Spanish", "Swahili", "Swedish", "Tagalog", "Tajik", 
    "Tatar", "Thai", "Turkish", "Ukrainian", "Uzbek", "Vietnamese", "Welsh", "Yiddish"};
    
    m_ui->primaryLanguage -> addItems(languages);
    m_ui->secondaryLanguage -> addItems(languages);

    connect(m_ui->primaryLanguage,SIGNAL(currentTextChanged(QString)),this, SLOT(changed()));
    connect(m_ui->secondaryLanguage,SIGNAL(currentTextChanged(QString)),this, SLOT(changed()));
    connect(m_ui->yandexKey,SIGNAL(textChanged(QString)),this,SLOT(changed()));
    connect(m_ui->baiduAPPID,SIGNAL(textChanged(QString)),this,SLOT(changed()));
    connect(m_ui->baiduApiKey,SIGNAL(textChanged(QString)),this,SLOT(changed()));
    connect(m_ui->youdaoAPPID,SIGNAL(textChanged(QString)),this,SLOT(changed()));
    connect(m_ui->youdaoAppSec,SIGNAL(textChanged(QString)),this,SLOT(changed()));
    connect(m_ui->yandex_word, SIGNAL(stateChanged(int)),this,SLOT(changed()));
    connect(m_ui->yandex_phrase, SIGNAL(stateChanged(int)),this,SLOT(changed()));
    connect(m_ui->glosbe_word, SIGNAL(stateChanged(int)),this,SLOT(changed()));
    connect(m_ui->glosbe_phrase, SIGNAL(stateChanged(int)),this,SLOT(changed()));
    connect(m_ui->glosbe_examples, SIGNAL(stateChanged(int)),this,SLOT(changed()));
    connect(m_ui->baiduEnable, SIGNAL(stateChanged(int)),this,SLOT(changed()));
    connect(m_ui->youdaoEnable, SIGNAL(stateChanged(int)),this,SLOT(changed()));
    connect(m_ui->googleEnable, SIGNAL(stateChanged(int)), this, SLOT(changed()));
    connect(m_ui->bingEnable, SIGNAL(stateChanged(int)), this, SLOT(changed()));
}

void TranslatorConfig::load()
{
    KCModule::load();
    
    KSharedConfig::Ptr cfg = KSharedConfig::openConfig(QStringLiteral("krunnerrc"));
    KConfigGroup grp = cfg->group("Runners");
    grp = KConfigGroup(&grp, "Translator");
    
    int indexPrimary = m_abbr.indexOf(grp.readEntry(CONFIG_PRIMARY, "en"));
    int indexSecondary = m_abbr.indexOf(grp.readEntry(CONFIG_SECONDARY, "es"));
    m_ui->primaryLanguage->setCurrentIndex(indexPrimary);
    m_ui->secondaryLanguage->setCurrentIndex(indexSecondary);
    m_ui->yandexKey->setText(grp.readEntry(CONFIG_YANDEX_KEY));
    m_ui->baiduAPPID->setText(grp.readEntry(CONFIG_BAIDU_APPID));
    m_ui->baiduApiKey->setText(grp.readEntry(CONFIG_BAIDU_APIKEY));
    m_ui->youdaoAPPID->setText(grp.readEntry(CONFIG_YOUDAO_APPID));
    m_ui->youdaoAppSec->setText(grp.readEntry(CONFIG_YOUDAO_APPSEC));
    m_ui->glosbe_word->setChecked(stringToBool(grp.readEntry(CONFIG_GLOSBE_WORD)));
    m_ui->glosbe_phrase->setChecked(stringToBool(grp.readEntry(CONFIG_GLOSBE_PHRASE)));
    m_ui->glosbe_examples->setChecked(stringToBool(grp.readEntry(CONFIG_GLOSBE_EXAMPLES)));
    m_ui->yandex_word->setChecked(stringToBool(grp.readEntry(CONFIG_YANDEX_WORD)));
    m_ui->yandex_phrase->setChecked(stringToBool(grp.readEntry(CONFIG_YANDEX_PHRASE)));
    m_ui->baiduEnable->setChecked(stringToBool(grp.readEntry(CONFIG_BAIDU_ENABLE)));
    m_ui->youdaoEnable->setChecked(stringToBool(grp.readEntry(CONFIG_YOUDAO_ENABLE)));
    m_ui->googleEnable->setChecked(stringToBool(grp.readEntry(CONFIG_GOOGLE_ENABLE)));
    m_ui->bingEnable->setChecked(stringToBool(grp.readEntry(CONFIG_BING_ENABLE)));
}

void TranslatorConfig::save()
{
    KCModule::save();

    KSharedConfig::Ptr cfg = KSharedConfig::openConfig(QStringLiteral("krunnerrc"));
    KConfigGroup grp = cfg->group("Runners");
    grp = KConfigGroup(&grp, "Translator");

    int indexPrimary = m_ui->primaryLanguage->currentIndex();
    int indexSecondary = m_ui-> secondaryLanguage->currentIndex();

    grp.writeEntry(CONFIG_PRIMARY, m_abbr.at(indexPrimary));
    grp.writeEntry(CONFIG_SECONDARY, m_abbr.at(indexSecondary));

    grp.writeEntry(CONFIG_YANDEX_KEY,m_ui->yandexKey->text());
    grp.writeEntry(CONFIG_BAIDU_APPID,m_ui->baiduAPPID->text());
    grp.writeEntry(CONFIG_BAIDU_APIKEY,m_ui->baiduApiKey->text());
    grp.writeEntry(CONFIG_YOUDAO_APPID,m_ui->youdaoAPPID->text());
    grp.writeEntry(CONFIG_YOUDAO_APPSEC,m_ui->youdaoAppSec->text());
    
    grp.writeEntry(CONFIG_GLOSBE_WORD, boolToString(m_ui->glosbe_word->isChecked()));
    grp.writeEntry(CONFIG_GLOSBE_PHRASE, boolToString(m_ui->glosbe_phrase->isChecked()));
    grp.writeEntry(CONFIG_GLOSBE_EXAMPLES, boolToString(m_ui->glosbe_examples->isChecked()));
    grp.writeEntry(CONFIG_YANDEX_WORD, boolToString(m_ui->yandex_word->isChecked()));
    grp.writeEntry(CONFIG_YANDEX_PHRASE, boolToString(m_ui->yandex_phrase->isChecked()));
    grp.writeEntry(CONFIG_BAIDU_ENABLE, boolToString(m_ui->baiduEnable->isChecked()));
    grp.writeEntry(CONFIG_YOUDAO_ENABLE, boolToString(m_ui->youdaoEnable->isChecked()));
    grp.writeEntry(CONFIG_GOOGLE_ENABLE, boolToString(m_ui->googleEnable->isChecked()));
    grp.writeEntry(CONFIG_BING_ENABLE, boolToString(m_ui->bingEnable->isChecked()));
    emit changed(true);
}

const QVector<QString>TranslatorConfig::m_abbr = {"sq", "af", "ar", "hy", "az", "eu", "be", "bs", "bg", "ca", "zh", "hr", "cs", "da", "nl", "en", "et", "fi", "fr", "gl", "ka", "de", "el", "ht", "hu", "is", "id", "ga", "it", "ja",                   
                                "kk", "ko", "ky", "la", "lv", "lt", "mk", "mg", "ms", "mt", "mn", "no", "fa", "pl", "pt", "ro", "ru", "sr", "sk", "sl", "es", "sw", "sv", "tl", "tg", "tt", "th", "tr", "uk", "uz", "vi", "cy", "he"};

#include "translator_config.moc"
