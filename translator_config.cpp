/******************************************************************************
 *  Copyright (C) 2013 – 2016 by David Baum <david.baum@naraesk.eu>           *
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
#include <KSharedConfig>
#include <KPluginFactory>
#include <krunner/abstractrunner.h>

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

    connect(m_ui->primaryLanguage,SIGNAL(textChanged(QString)),this,SLOT(changed()));
    connect(m_ui->secondaryLanguage,SIGNAL(textChanged(QString)),this,SLOT(changed()));
    connect(m_ui->yandexKey,SIGNAL(textChanged(QString)),this,SLOT(changed()));

    load();
}

void TranslatorConfig::load()
{
    KCModule::load();

    KSharedConfig::Ptr cfg = KSharedConfig::openConfig(QStringLiteral("krunnerrc"));
    KConfigGroup grp = cfg->group("Runners");
    grp = KConfigGroup(&grp, "Translator");

    m_ui->primaryLanguage->setText(grp.readEntry(CONFIG_PRIMARY));
    m_ui->secondaryLanguage->setText(grp.readEntry(CONFIG_SECONDARY));
    m_ui->yandexKey->setText(grp.readEntry(CONFIG_YANDEX_KEY));

    emit changed(false);
}

void TranslatorConfig::save()
{
    KCModule::save();

    KSharedConfig::Ptr cfg = KSharedConfig::openConfig(QStringLiteral("krunnerrc"));
    KConfigGroup grp = cfg->group("Runners");
    grp = KConfigGroup(&grp, "Translator");

    grp.writeEntry(CONFIG_PRIMARY,m_ui->primaryLanguage->text());
    grp.writeEntry(CONFIG_SECONDARY,m_ui->secondaryLanguage->text());
    grp.writeEntry(CONFIG_YANDEX_KEY,m_ui->yandexKey->text());

    emit changed(false);
}

#include "translator_config.moc"
