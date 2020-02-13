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

#ifndef RAIDINFO12_HPP
#define RAIDINFO12_HPP

#include <Core/Results/Den.hpp>
#include <Core/Results/Pokemon.hpp>
#include <QWidget>

namespace Ui
{
    class RaidInfo12;
}

class RaidInfo12 : public QWidget
{
    Q_OBJECT
public:
    explicit RaidInfo12(QWidget *parent = nullptr);
    ~RaidInfo12() override;
    void setDen(const Den &den, Game game);
    QVector<u8> getIVs(int index) const;
    void setIVs(int index, const QVector<u8> &ivs);
    QVector<int> getIVCounts() const;
    Pokemon getPokemonDay1() const;
    Pokemon getPokemonDay2() const;
    bool isValid() const;

private:
    Ui::RaidInfo12 *ui;
    Den den;
    Game game;

    void setupModels();

private slots:
    void raidDay1IndexChanged(int index);
    void raidDay2IndexChanged(int index);
    void checkValid();
};

#endif // RAIDINFO12_HPP
