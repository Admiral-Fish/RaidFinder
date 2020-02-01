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

#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <Core/Results/Den.hpp>
#include <Core/Results/Profile.hpp>
#include <QActionGroup>
#include <QMainWindow>
#include <QMenu>

class FrameModel;

namespace Ui
{
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(bool debug = false, QWidget *parent = nullptr);
    ~MainWindow() override;

private:
    Ui::MainWindow *ui;
    QActionGroup *languageGroup;
    QActionGroup *styleGroup;
    QActionGroup *threadGroup;
    QString currentLanguage;
    QString currentStyle;
    QVector<Profile> profiles;
    Profile currentProfile;
    Den den;
    FrameModel *model;
    QMenu *menu;
    bool debug;

    void setupModels();
    QByteArray downloadFile(const QString &url);

private slots:
    void slotLanguageChanged(QAction *action);
    void slotStyleChanged(QAction *action);
    void slotThreadChanged(QAction *action);
    void updateProfiles();
    void profilesIndexChanged(int index);
    void openProfileManager();
    void openDenMap();
    void openEncounterLookup();
    void openIVCalculator();
    void openSeedSearcher();
    void downloadEventData();
    void denIndexChanged(int index);
    void rarityIndexChange(int index);
    void speciesIndexChanged(int index);
    void tableViewContextMenu(QPoint pos);
    void generate();
};
#endif // MAINWINDOW_HPP
