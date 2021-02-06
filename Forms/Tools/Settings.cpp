/*
 * This file is part of RaidFinder
 * Copyright (C) 2019-2020 by Admiral_Fish
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 3
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#include "Settings.hpp"
#include "ui_Settings.h"
#include <Core/Loader/ProfileLoader.hpp>
#include <QApplication>
#include <QFileDialog>
#include <QMessageBox>
#include <QProcess>
#include <QSettings>

Settings::Settings(QWidget *parent) : QWidget(parent), ui(new Ui::Settings)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_QuitOnClose, false);
    setAttribute(Qt::WA_DeleteOnClose);

    setupModels();
}

Settings::~Settings()
{
    QSettings setting;
    setting.setValue("settingsForm/geometry", this->saveGeometry());

    delete ui;
}

void Settings::setupModels()
{
    QSettings setting;
    setting.beginGroup("settings");

    // Language
    QString language = setting.value("locale").toString();
    QStringList languages = { "zh", "tw", "en", "fr", "de", "it", "ja", "ko", "pt", "es" };
    for (int i = 0; i < languages.size(); i++)
    {
        const QString &lang = languages[i];
        ui->comboBoxLanguage->setItemData(i, lang);
        if (language == lang)
        {
            ui->comboBoxLanguage->setCurrentIndex(i);
        }
    }

    // Style
    QString style = setting.value("style").toString();
    QStringList styles = { "dark", "light" };
    for (int i = 0; i < styles.size(); i++)
    {
        const QString &sty = styles[i];
        ui->comboBoxStyle->setItemData(i, sty);
        if (style == sty)
        {
            ui->comboBoxStyle->setCurrentIndex(i);
        }
    }

    // Profiles
    QString profile = setting.value("profiles").toString();
    ui->lineEditProfiles->setText(profile);

    setting.endGroup();

    connect(ui->comboBoxLanguage, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &Settings::onLanguageChanged);
    connect(ui->comboBoxStyle, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &Settings::onStyleChanged);
    connect(ui->pushButtonProfile, &QPushButton::clicked, this, &Settings::changeProfiles);

    if (setting.contains("settingsForm/geometry"))
    {
        this->restoreGeometry(setting.value("settingsForm/geometry").toByteArray());
    }
}

void Settings::onLanguageChanged(int index)
{
    (void)index;

    QSettings setting;
    QString currentLanguage = setting.value("settings/locale").toString();
    QString language = ui->comboBoxLanguage->currentData().toString();

    if (currentLanguage != language)
    {
        setting.setValue("settings/locale", language);

        QMessageBox message(QMessageBox::Question, tr("Language update"), tr("Restart for changes to take effect. Restart now?"),
                            QMessageBox::Yes | QMessageBox::No);
        if (message.exec() == QMessageBox::Yes)
        {
            QProcess::startDetached(QApplication::applicationFilePath());
            QApplication::quit();
        }
    }
}

void Settings::onStyleChanged(int index)
{
    (void)index;

    QSettings setting;
    QString currentStyle = setting.value("settings/style").toString();
    QString style = ui->comboBoxStyle->currentData().toString();
    if (currentStyle != style)
    {
        setting.setValue("settings/style", style);

        QMessageBox message(QMessageBox::Question, tr("Style change"), tr("Restart for changes to take effect. Restart now?"),
                            QMessageBox::Yes | QMessageBox::No);
        if (message.exec() == QMessageBox::Yes)
        {
            QProcess::startDetached(QApplication::applicationFilePath());
            QApplication::quit();
        }
    }
}

void Settings::changeProfiles()
{
    QString fileName = QFileDialog::getSaveFileName(this, "Select Profile json", QDir::currentPath(), "json (*.json)");
    if (!fileName.isEmpty())
    {
        if (!QFile::exists(fileName))
        {
            QFile f(fileName);
            if (!f.open(QIODevice::WriteOnly))
            {
                QMessageBox message(QMessageBox::Information, tr("Profile File"), tr("There was a problem creating the file"),
                                    QMessageBox::Ok);
                message.exec();
                return;
            }
        }

        QSettings setting;
        setting.setValue("settings/profiles", fileName);

        ProfileLoader::init(fileName.toStdString());

        ui->lineEditProfiles->setText(fileName);
    }
}
