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

#ifndef IVCALCULATOR_HPP
#define IVCALCULATOR_HPP

#include <Core/Util/Global.hpp>
#include <QLabel>

namespace Ui
{
    class IVCalculator;
}

class IVCalculator : public QWidget
{
    Q_OBJECT
signals:
    void sendIVs(int, int, int, QVector<u8>);

public:
    explicit IVCalculator(QWidget *parent = nullptr);
    ~IVCalculator() override;

    void setConnected(bool connected);

private:
    Ui::IVCalculator *ui;

    void setupModels();
    void displayIVs(QLabel *label, const QVector<u8> &ivs);
    bool connected;

private slots:
    void findIVs();
    void pokemonIndexChanged(int index);
    void altformIndexChanged(int index);
    void checkIVs();
};

#endif // IVCALCULATOR_HPP
