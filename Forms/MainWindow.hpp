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

class StateModel;

namespace Ui
{
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

private:
    Ui::MainWindow *ui;
    QActionGroup *languageGroup;
    QActionGroup *styleGroup;
    std::vector<Profile> profiles;
    Profile currentProfile;
    Den den;
    StateModel *model;
    QMenu *menu;

    void setupModels();
    QByteArray downloadFile(const QString &url);

signals:
    void generated(bool results);
    void denInfo(int denID, int denType, int species, int starsMin, int starsMax, bool gmax, int shinyLock);

private slots:
    void updateProfiles();
    void profilesIndexChanged(int index);
    void openProfileManager();
    void openDenMap();
    void openEncounterLookup();
    void openIVCalculator();
    void openSettings();
    void openBots();
    void downloadEventData();
    void checkUpdates();
    void locationIndexChanged(int index);
    void denIndexChanged(int index);
    void rarityIndexChange(int index);
    void speciesIndexChanged(int index);
    void showStatsToggled(bool flag);
    void levelValueChanged(int value);
    void tableViewContextMenu(QPoint pos);
    void sendDenInfo();
    void generate2(QString seed);
    void generate();
    void lockBoxes(bool location, bool den, bool rarity, bool species, bool seed);
    void unlockBoxes();
};
#endif // MAINWINDOW_HPP
