/*
 * This file is part of RaidFinder
 * Copyright (C) 2019 by Admiral_Fish
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

#include "MainWindow.hpp"
#include "ui_MainWindow.h"
#include <Core/Nature.hpp>
#include <Core/Power.hpp>
#include <Core/RaidGenerator.hpp>
#include <Forms/ProfileManager.hpp>
#include <QSettings>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle(QString("RaidFinder %1").arg(VERSION));

    updateProfiles();
    setupModels();
}

MainWindow::~MainWindow()
{
    QSettings setting;
    setting.setValue("mainWindow/profile", ui->comboBoxProfiles->currentIndex());
    setting.setValue("mainWindow/geometry", this->saveGeometry());
    setting.endGroup();

    delete ui;
}

void MainWindow::setupModels()
{
    model = new FrameModel();
    ui->tableView->setModel(model);

    ui->comboBoxNature->setup(Nature::getNatures());
    ui->comboBoxHiddenPower->setup(Power::getPowers());

    QSettings setting;

    languageGroup = new QActionGroup(ui->menuLanguage);
    languageGroup->setExclusive(true);
    connect(languageGroup, &QActionGroup::triggered, this, &MainWindow::slotLanguageChanged);
    currentLanguage = setting.value("settings/locale", "en").toString();
    QStringList locales = { "de", "en", "es", "fr", "it", "ja", "ko", "zh_Hans_CN" };
    for (u8 i = 0; i < locales.size(); i++)
    {
        const QString &lang = locales.at(i);

        auto *action = ui->menuLanguage->actions()[i];
        action->setData(lang);

        if (currentLanguage == lang)
        {
            action->setChecked(true);
        }

        languageGroup->addAction(action);
    }

    styleGroup = new QActionGroup(ui->menuStyle);
    styleGroup->setExclusive(true);
    connect(styleGroup, &QActionGroup::triggered, this, &MainWindow::slotStyleChanged);
    currentStyle = setting.value("settings/style", "dark").toString();
    QStringList styles = { "dark", "light" };
    for (u8 i = 0; i < styles.size(); i++)
    {
        const QString &style = styles.at(i);

        auto *action = ui->menuStyle->actions()[i];
        action->setData(style);

        if (currentStyle == style)
        {
            action->setChecked(true);
        }

        styleGroup->addAction(action);
    }

    connect(ui->pushButtonProfileManager, &QPushButton::clicked, this, &MainWindow::openProfileManager);
    connect(ui->comboBoxProfiles, QOverload<int>::of(&QComboBox::currentIndexChanged), this,
        &MainWindow::profilesIndexChanged);
    connect(ui->pushButtonGenerate, &QPushButton::clicked, this, &MainWindow::generate);

    if (setting.contains("mainWindow/geometry"))
    {
        this->restoreGeometry(setting.value("mainWindow/geometry").toByteArray());
    }
}

void MainWindow::slotLanguageChanged(QAction *action) {}

void MainWindow::slotStyleChanged(QAction *action) {}

void MainWindow::updateProfiles()
{
    connect(ui->comboBoxProfiles, QOverload<int>::of(&QComboBox::currentIndexChanged), this,
        &MainWindow::profilesIndexChanged);

    profiles = Profile::loadProfileList();
    profiles.insert(profiles.begin(), Profile());

    ui->comboBoxProfiles->clear();

    for (const auto &profile : profiles)
    {
        ui->comboBoxProfiles->addItem(profile.getName());
    }

    QSettings setting;
    int val = setting.value("mainWindow/profile", 0).toInt();
    if (val < ui->comboBoxProfiles->count())
    {
        ui->comboBoxProfiles->setCurrentIndex(val);
    }
}

void MainWindow::profilesIndexChanged(int index) {}

void MainWindow::openProfileManager()
{
    auto *manager = new ProfileManager();
    connect(manager, &ProfileManager::updateProfiles, this, &MainWindow::updateProfiles);
    manager->show();
}

void MainWindow::generate()
{
    model->clearModel();

    auto &profile = profiles.at(ui->comboBoxProfiles->currentIndex());

    RaidGenerator generator(1, 10, 4, profile.getTSV(), 0, 121, 4);
    QVector<Frame> frames = generator.generate(0);

    model->addItems(frames);
}
