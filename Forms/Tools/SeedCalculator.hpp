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

#ifndef SEEDCALCULATOR_HPP
#define SEEDCALCULATOR_HPP

#include <Core/Util/Game.hpp>
#include <Core/Util/Global.hpp>
#include <QWidget>

namespace Ui
{
    class SeedCalculator;
}

class SeedCalculator : public QWidget
{
    Q_OBJECT
public:
    explicit SeedCalculator(Game version, QWidget *parent = nullptr);
    ~SeedCalculator() override;

public slots:
    void setIVs(int star, int index, int nature, const QVector<u8> &ivs);

private:
    Ui::SeedCalculator *ui;

    void setupModels();
    void toggleControls(bool flag);
    void search35();
    void search12();

private slots:
    void denIndexChanged(int index);
    void rarityIndexChanged(int index);
    void gameIndexChanged(int index);
    void search();
    void clear();
};

#endif // SEEDCALCULATOR_HPP
