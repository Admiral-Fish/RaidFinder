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

#include "TableView.hpp"
#include <QApplication>
#include <QClipboard>
#include <QHeaderView>
#include <QMouseEvent>

TableView::TableView(QWidget *parent)
    : QTableView(parent)
{
}

void TableView::resizeEvent(QResizeEvent *event)
{
    QTableView::resizeEvent(event);

    QHeaderView *header = this->horizontalHeader();
    header->setSectionResizeMode(QHeaderView::Stretch);

    for (int column = 0; column < header->count(); column++)
    {
        int width = header->sectionSize(column);
        header->setSectionResizeMode(column, QHeaderView::Interactive);
        header->resizeSection(column, width);
    }
}

void TableView::mouseDoubleClickEvent(QMouseEvent *event)
{
    if (event && event->type() == QMouseEvent::MouseButtonDblClick)
    {
        setSelectionToClipBoard();
    }
}

void TableView::keyPressEvent(QKeyEvent *event)
{
    QTableView::keyPressEvent(event);

    if (event && (event->key() == Qt::Key_C) && (event->modifiers() == Qt::ControlModifier))
    {
        setSelectionToClipBoard();
    }
}

void TableView::setSelectionToClipBoard()
{
    QModelIndexList indexes = this->selectionModel()->selectedIndexes();
    if (!indexes.isEmpty())
    {
        QString selectedText;

        for (auto i = 0; i < indexes.size(); i++)
        {
            QModelIndex current = indexes[i];
            QString text = current.data().toString();

            if (i + 1 < selectedIndexes().count())
            {
                QModelIndex next = indexes[i + 1];

                if (next.row() != current.row())
                {
                    text += "\n";
                }
                else
                {
                    text += "\t";
                }
            }
            selectedText += text;
        }

        QApplication::clipboard()->setText(selectedText);
    }
}
