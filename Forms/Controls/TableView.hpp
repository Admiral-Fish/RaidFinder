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

#ifndef TABLEVIEW_HPP
#define TABLEVIEW_HPP

#include <QTableView>

class TableView final : public QTableView
{
    Q_OBJECT
public:
    explicit TableView(QWidget *parent = nullptr);
    void resizeEvent(QResizeEvent *event) override final;
    void mouseDoubleClickEvent(QMouseEvent *event) override final;
    void keyPressEvent(QKeyEvent *event) override final;
    void outputModel(bool csv) const;

private:
    void setSelectionToClipBoard();
};

#endif // TABLEVIEW_HPP
