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

#include "ProfileManager.hpp"
#include "ui_ProfileManager.h"
#include <Core/Loader/ProfileLoader.hpp>
#include <Forms/Profile/ProfileEditor.hpp>
#include <Models/ProfileModel.hpp>
#include <QMessageBox>
#include <QSettings>

ProfileManager::ProfileManager(QWidget *parent) : QWidget(parent), ui(new Ui::ProfileManager)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_QuitOnClose, false);
    setAttribute(Qt::WA_DeleteOnClose);

    setupModels();
}

ProfileManager::~ProfileManager()
{
    QSettings setting;
    setting.setValue("profileManager/geometry", this->saveGeometry());

    delete ui;
}

void ProfileManager::setupModels()
{
    model = new ProfileModel(ui->tableView);
    model->addItems(ProfileLoader::getProfiles());
    ui->tableView->setModel(model);
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    connect(ui->pushButtonNew, &QPushButton::clicked, this, &ProfileManager::create);
    connect(ui->pushButtonEdit, &QPushButton::clicked, this, &ProfileManager::edit);
    connect(ui->pushButtonDelete, &QPushButton::clicked, this, &ProfileManager::remove);

    QSettings setting;
    if (setting.contains("profileManager/geometry"))
    {
        this->restoreGeometry(setting.value("profileManager/geometry").toByteArray());
    }
}

void ProfileManager::create()
{
    QScopedPointer<ProfileEditor> dialog(new ProfileEditor);
    if (dialog->exec() == QDialog::Accepted)
    {
        Profile profile = dialog->getNewProfile();
        ProfileLoader::addProfile(profile);
        model->addItem(profile);
        emit updateProfiles();
    }
}

void ProfileManager::edit()
{
    int r = ui->tableView->currentIndex().row();

    if (r < 0)
    {
        QMessageBox error;
        error.setText(tr("Please select a profile."));
        error.exec();
        return;
    }

    QScopedPointer<ProfileEditor> dialog(new ProfileEditor(model->getItem(r)));
    if (dialog->exec() == QDialog::Accepted)
    {
        Profile profile = dialog->getNewProfile();
        ProfileLoader::updateProfile(profile, dialog->getOriginal());
        model->updateItem(profile, r);
        emit updateProfiles();
    }
}

void ProfileManager::remove()
{
    int r = ui->tableView->currentIndex().row();

    if (r < 0)
    {
        QMessageBox error;
        error.setText(tr("Please select a profile."));
        error.exec();
        return;
    }

    QMessageBox message(QMessageBox::Question, tr("Delete profile"), tr("Are you sure you wish to delete this profile?"),
                        QMessageBox::Yes | QMessageBox::No);
    if (message.exec() == QMessageBox::Yes)
    {
        Profile profile = model->getItem(r);
        ProfileLoader::removeProfile(profile);
        model->removeItem(r);
        emit updateProfiles();
    }
}
