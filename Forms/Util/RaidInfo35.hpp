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

#ifndef RAIDINFO35_HPP
#define RAIDINFO35_HPP

#include <Core/Results/Den.hpp>
#include <Core/Results/Pokemon.hpp>
#include <QWidget>

namespace Ui
{
    class RaidInfo35;
}

class RaidInfo35 : public QWidget
{
    Q_OBJECT
public:
    explicit RaidInfo35(QWidget *parent = nullptr);
    ~RaidInfo35() override;
    void setDen(const Den &den, Game game);
    bool isValid() const;
    QVector<u8> getIVs(int index) const;
    QVector<u8> getConditionIVs() const;
    QVector<int> getIVCounts() const;
    Pokemon getPokemonDay4_1() const;
    Pokemon getPokemonDay4_2() const;
    Pokemon getPokemonDay5() const;
    Pokemon getPokemonDay6() const;

public slots:
    void displayDay6(int flag);

private:
    Ui::RaidInfo35 *ui;
    Den den;
    Game game;

    void setupModels();
    void checkDay4();

private slots:
    void raidDay4_1IndexChanged(int index);
    void raidDay4_2IndexChanged(int index);
    void raidDay5IndexChanged(int index);
    void raidDay6IndexChanged(int index);
};

#endif // RAIDINFO35_HPP
