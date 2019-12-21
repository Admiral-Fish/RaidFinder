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

#include "DenMap.hpp"
#include "ui_DenMap.h"
#include <Core/DenLoader.hpp>
#include <Core/Util/Translator.hpp>
#include <QPainter>

DenMap::DenMap(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::DenMap)
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
    for (u8 i = 0; i < 99; i++)
    {
        QString location = Translator::getLocation(DenLoader::getLocation(i));
        ui->comboBoxDen->addItem(QString("%1: %2").arg(i + 1).arg(location));
    }

    connect(ui->comboBoxDen, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &DenMap::denIndexChanged);
    denIndexChanged(0);
}

void DenMap::denIndexChanged(int index)
{
    QVector<u8> coordinates = DenLoader::getCoordinates(static_cast<u8>(index));

    QPixmap image(":/images/map.png");

    QPainter paint(&image);
    paint.setPen(QPen(QBrush(Qt::red), 20));
    paint.drawEllipse(coordinates[0], 1180 - coordinates[1], 10, 10);

    ui->labelMap->setPixmap(image);
}
