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

#include "DateCalculator.hpp"
#include "ui_DateCalculator.h"
#include <Core/Util/Translator.hpp>
#include <QPainter>

DateCalculator::DateCalculator(QWidget *parent) : QWidget(parent), ui(new Ui::DateCalculator)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_QuitOnClose, false);
    setAttribute(Qt::WA_DeleteOnClose);

    setupModels();
}

DateCalculator::~DateCalculator()
{
    delete ui;
}

void DateCalculator::setupModels(){
    ui->dateEdit->setDate(QDate::currentDate());
	connect(ui->pushButtonCalculateDate, &QPushButton::clicked, this, &DateCalculator::calculateFutureDate);
}

void DateCalculator::calculateFutureDate() {
	QDate currDate = ui->dateEdit->date();
	QDate advancedDate = currDate.addDays(ui->dateSpinBox->value());
	ui->advancedDateLabel->setText("Result: "+advancedDate.toString("MMMM dd, yyyy"));
	ui->advancedDateLabel->setEnabled(true);
}