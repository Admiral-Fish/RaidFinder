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

#include "DenMap.hpp"
#include "ui_DenMap.h"
#include <Core/Loader/DenLoader.hpp>
#include <Core/Util/Translator.hpp>
#include <QPainter>

DenMap::DenMap(QWidget *parent) : QWidget(parent), ui(new Ui::DenMap)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_QuitOnClose, false);
    setAttribute(Qt::WA_DeleteOnClose);

    setupModels();
}

DenMap::~DenMap()
{
    delete ui;
}

void DenMap::setupModels()
{
    for (u8 i = 0; i < 190; i++)
    {
        QString location = Translator::getLocation(DenLoader::getLocation(i));
        ui->comboBoxDen->addItem(QString("%1: %2").arg(i + 1).arg(location));
    }

    connect(ui->comboBoxDen, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &DenMap::denIndexChanged);

    denIndexChanged(0);
}

void DenMap::denIndexChanged(int index)
{
    if (index >= 0)
    {
        QVector<u16> coordinates = DenLoader::getCoordinates(index);

        QPixmap image;
        if (index < 100)
        {
            image.load(":/images/map.png");
            this->resize(245, 578);
        }
        else
        {
            image.load(":/images/map_ioa.png");
            this->resize(609, 637);
        }

        QPainter paint(&image);
        paint.setPen(QPen(QBrush(Qt::red), 20));
        paint.drawEllipse(QPoint(coordinates[0], coordinates[1]), 5, 5);

        ui->labelMap->setPixmap(image);
    }
}
