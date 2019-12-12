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

#include "ProfileEditor.hpp"
#include "ui_ProfileEditor.h"
#include <QMessageBox>
#include <QSettings>

ProfileEditor::ProfileEditor(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::ProfileEditor)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_QuitOnClose, false);

    setupModels();
}

ProfileEditor::ProfileEditor(const Profile &profile, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::ProfileEditor)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_QuitOnClose, false);

    setupModels();

    ui->lineEditProfileName->setText(profile.getName());
    ui->comboBoxProfileVersion->setCurrentIndex(ui->comboBoxProfileVersion->findData(profile.getVersion()));
    ui->textBoxProfileTID->setText(QString::number(profile.getTID()));
    ui->textBoxProfileSID->setText(QString::number(profile.getSID()));

    original = profile;
}

ProfileEditor::~ProfileEditor()
{
    QSettings setting;
    setting.setValue("profileEditor/geometry", this->saveGeometry());

    delete ui;
}

Profile ProfileEditor::getNewProfile()
{
    return fresh;
}

Profile ProfileEditor::getOriginal()
{
    return original;
}

void ProfileEditor::setupModels()
{
    ui->textBoxProfileTID->setValues(InputType::ID);
    ui->textBoxProfileSID->setValues(InputType::ID);

    ui->comboBoxProfileVersion->setItemData(0, Game::Sword);
    ui->comboBoxProfileVersion->setItemData(1, Game::Shield);

    QSettings setting;
    if (setting.contains("profileEditor/geometry"))
    {
        this->restoreGeometry(setting.value("profileEditor/geometry").toByteArray());
    }
}

void ProfileEditor::on_buttonBox_accepted()
{
    QString input = ui->lineEditProfileName->text().trimmed();
    if (input.isEmpty())
    {
        QMessageBox error;
        error.setText(tr("Enter a Profile Name."));
        error.exec();
        return;
    }

    fresh = Profile(input, ui->textBoxProfileTID->getUShort(), ui->textBoxProfileSID->getUShort(),
        static_cast<Game>(ui->comboBoxProfileVersion->currentData().toInt()));

    done(QDialog::Accepted);
}

void ProfileEditor::on_buttonBox_rejected()
{
    done(QDialog::Rejected);
}
