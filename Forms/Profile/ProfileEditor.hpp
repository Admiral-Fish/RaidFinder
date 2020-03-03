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

#ifndef PROFILEEDITOR_HPP
#define PROFILEEDITOR_HPP

#include <Core/Results/Profile.hpp>
#include <QDialog>

namespace Ui
{
    class ProfileEditor;
}

class ProfileEditor final : public QDialog
{
    Q_OBJECT
signals:
    void newProfile(Profile);
    void editProfile(Profile, Profile);

public:
    explicit ProfileEditor(QWidget *parent = nullptr);
    explicit ProfileEditor(const Profile &profile, QWidget *parent = nullptr);
    explicit ProfileEditor(u32 tick, u32 offset, QWidget *parent = nullptr);
    ~ProfileEditor() override final;
    Profile getNewProfile();
    Profile getOriginal();

private:
    Ui::ProfileEditor *ui;
    Profile original;
    Profile fresh;

    void setupModels();

private slots:
    void okay();
};

#endif // PROFILEEDITOR_HPP
