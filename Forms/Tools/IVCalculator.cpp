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

#include "IVCalculator.hpp"
#include "ui_IVCalculator.h"
#include <Core/Loader/PersonalLoader.hpp>
#include <Core/Util/IVChecker.hpp>
#include <Core/Util/Translator.hpp>
#include <QCompleter>
#include <QInputDialog>
#include <QMessageBox>
#include <QSettings>

IVCalculator::IVCalculator(QWidget *parent) : QWidget(parent), ui(new Ui::IVCalculator), connected(false)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_QuitOnClose, false);

    setupModels();
}

IVCalculator::~IVCalculator()
{
    QSettings setting;
    setting.setValue("ivCalculator/geometry", this->saveGeometry());

    delete ui;
}

void IVCalculator::setConnected(bool connected)
{
    this->connected = connected;
}

void IVCalculator::setupModels()
{
    for (u16 i = 1; i <= 890; i++)
    {
        if (PersonalLoader::getInfo(i).getIncluded())
        {
            ui->comboBoxPokemon->addItem(Translator::getSpecie(i), i);
        }
    }
    pokemonIndexChanged(0);
    altformIndexChanged(0);

    ui->comboBoxNature->addItems(Translator::getNatures());

    ui->comboBoxPokemon->setEditable(true);
    ui->comboBoxPokemon->setInsertPolicy(QComboBox::NoInsert);
    ui->comboBoxPokemon->completer()->setCompletionMode(QCompleter::PopupCompletion);

    connect(ui->pushButtonFindIVs, &QPushButton::clicked, this, &IVCalculator::findIVs);
    connect(ui->comboBoxPokemon, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &IVCalculator::pokemonIndexChanged);
    connect(ui->comboBoxAltForm, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &IVCalculator::altformIndexChanged);
    connect(ui->pushButtonSend, &QPushButton::clicked, this, &IVCalculator::checkIVs);

    QSettings setting;
    if (setting.contains("ivCalculator/geometry"))
    {
        this->restoreGeometry(setting.value("ivCalculator/geometry").toByteArray());
    }
}

void IVCalculator::displayIVs(QLabel *label, const QVector<u8> &ivs)
{
    QString result;

    if (ivs.isEmpty())
    {
        result = tr("Invalid");
    }
    else
    {
        bool flag = false;
        for (int i = 0; i < ivs.size(); i++)
        {
            if (i == 0)
            {
                result += QString::number(ivs.at(i));
            }
            else
            {
                if (ivs.at(i) == ivs.at(i - 1) + 1)
                {
                    flag = true;

                    //  Check to see if we need to cap here.
                    if (i == ivs.size() - 1)
                    {
                        result += QString("-%1").arg(ivs.at(i));
                    }
                }
                else
                {
                    if (flag)
                    {
                        flag = false;
                        result += QString("-%1").arg(ivs.at(i - 1));
                        result += QString(", %1").arg(ivs.at(i));
                    }
                    else
                    {
                        result += QString(", %1").arg(ivs.at(i));
                    }
                }
            }
        }
    }

    label->setText(result);
}

void IVCalculator::findIVs()
{
    QVector<QVector<u16>> stats;
    QVector<u8> levels;

    QStringList entries = ui->textEdit->toPlainText().split("\n");
    entries.removeAll(QString());

    bool flag = !entries.isEmpty();

    for (const QString &entry : entries)
    {
        QStringList values = entry.split(" ");
        values.removeAll(QString());

        if (values.size() != 7)
        {
            flag = false;
            break;
        }

        levels.append(static_cast<u8>(values.at(0).toUInt(&flag)));
        if (!flag)
        {
            break;
        }

        QVector<u16> stat;
        for (u8 i = 1; i < 7; i++)
        {
            stat.append(static_cast<u16>(values.at(i).toUInt(&flag)));
            if (!flag)
            {
                break;
            }
        }
        stats.append(stat);

        if (!flag)
        {
            break;
        }
    }

    if (!flag)
    {
        QMessageBox error;
        error.setText(tr("Invalid input"));
        error.exec();
        return;
    }

    u8 nature = static_cast<u8>(ui->comboBoxNature->currentIndex());

    u16 species = static_cast<u16>(ui->comboBoxPokemon->currentData().toUInt());
    u8 form = static_cast<u8>(ui->comboBoxAltForm->currentIndex());
    auto info = PersonalLoader::getInfo(species, form);

    auto possible = IVChecker::calculateIVRange(info.getBaseStats(), stats, levels, nature);

    displayIVs(ui->labelHPIVValue, possible.at(0));
    displayIVs(ui->labelAtkIVValue, possible.at(1));
    displayIVs(ui->labelDefIVValue, possible.at(2));
    displayIVs(ui->labelSpAIVValue, possible.at(3));
    displayIVs(ui->labelSpDIVValue, possible.at(4));
    displayIVs(ui->labelSpeIVValue, possible.at(5));
}

void IVCalculator::pokemonIndexChanged(int index)
{
    if (index >= 0)
    {
        u16 specie = static_cast<u16>(ui->comboBoxPokemon->currentData().toUInt());

        PersonalInfo base = PersonalLoader::getInfo(specie);
        u8 formCount = base.getFormCount();

        ui->labelAltForm->setVisible(formCount > 1);
        ui->comboBoxAltForm->setVisible(formCount > 1);

        ui->comboBoxAltForm->clear();
        for (u8 i = 0; i < formCount; i++)
        {
            ui->comboBoxAltForm->addItem(QString::number(i));
        }
    }
}

void IVCalculator::altformIndexChanged(int index)
{
    if (index >= 0)
    {
        u16 specie = static_cast<u16>(ui->comboBoxPokemon->currentData().toUInt());

        PersonalInfo info = PersonalLoader::getInfo(specie, static_cast<u8>(index));

        QVector<u8> stats = info.getBaseStats();
        ui->labelBaseHPValue->setText(QString::number(stats[0]));
        ui->labelBaseAtkValue->setText(QString::number(stats[1]));
        ui->labelBaseDefValue->setText(QString::number(stats[2]));
        ui->labelBaseSpAValue->setText(QString::number(stats[3]));
        ui->labelBaseSpDValue->setText(QString::number(stats[4]));
        ui->labelBaseSpeValue->setText(QString::number(stats[5]));
    }
}

void IVCalculator::checkIVs()
{
    if (!connected)
    {
        QMessageBox error(QMessageBox::Critical, tr("Cannot send IVs"), tr("The seed calculator must be open to send IVs"),
                          QMessageBox::Ok);
        error.exec();
        return;
    }

    auto labels
        = { ui->labelHPIVValue, ui->labelAtkIVValue, ui->labelDefIVValue, ui->labelSpAIVValue, ui->labelSpDIVValue, ui->labelSpeIVValue };

    QVector<u8> ivs;
    for (const auto &label : labels)
    {
        QString text = label->text();

        if (text.contains(",") || text.contains("-"))
        {
            QMessageBox error(QMessageBox::Critical, tr("Multiple IVs Present"), tr("There must be only one IV present per stat"),
                              QMessageBox::Ok);
            error.exec();
            return;
        }
        if (text == tr("Invalid"))
        {
            QMessageBox error(QMessageBox::Critical, tr("Invalid IVs"), tr("All stats must have valid IVs"), QMessageBox::Ok);
            error.exec();
            return;
        }

        ivs.append(static_cast<u8>(text.toUInt()));
    }

    bool flag;
    QStringList starType = { "1-2★", "3-5★" };
    QString starItem = QInputDialog::getItem(this, tr("Choose star"), tr("Star"), starType, 0, false, &flag,
                                             Qt::WindowTitleHint | Qt::WindowCloseButtonHint);
    if (!flag)
    {
        return;
    }

    int starIndex = starType.indexOf(starItem);

    QStringList dayType;
    if (starIndex == 0)
    {
        dayType = QStringList({ tr("Day 1"), tr("Day 2") });
    }
    else
    {
        dayType = QStringList({ tr("Day 4 (1st)"), tr("Day 4 (2nd)"), tr("Day 5"), tr("Day 6") });
    }
    QString dayItem = QInputDialog::getItem(this, tr("Choose day"), tr("Day"), dayType, 0, false, &flag,
                                            Qt::WindowTitleHint | Qt::WindowCloseButtonHint);

    if (!flag)
    {
        return;
    }

    int dayIndex = dayType.indexOf(dayItem);

    emit sendIVs(starIndex, dayIndex, ui->comboBoxNature->currentIndex(), ivs);
}
