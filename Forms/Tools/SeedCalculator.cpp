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

#include "SeedCalculator.hpp"
#include "ui_SeedCalculator.h"
#include <Core/Loader/DenLoader.hpp>
#include <Core/Loader/PersonalLoader.hpp>
#include <Core/Results/Pokemon.hpp>
#include <Core/Searcher/SeedSearcher12.hpp>
#include <Core/Searcher/SeedSearcher35.hpp>
#include <Core/Util/Game.hpp>
#include <Core/Util/Translator.hpp>
#include <QApplication>
#include <QFile>
#include <QMessageBox>
#include <QSettings>
#include <QTimer>
#include <QtConcurrent>
#include <ctime>

SeedCalculator::SeedCalculator(QWidget *parent) : QWidget(parent), ui(new Ui::SeedCalculator)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_QuitOnClose, false);
    setAttribute(Qt::WA_DeleteOnClose);

    setupModels();
}

SeedCalculator::~SeedCalculator()
{
    delete ui;
}

void SeedCalculator::setupModels()
{
    for (u8 i = 0; i < 100; i++)
    {
        if (i == 16)
        {
            continue;
        }

        QString location = Translator::getLocation(DenLoader::getLocation(i));
        ui->comboBoxDen->addItem(QString("%1: %2").arg(i + 1).arg(location), i);
    }
    if (QFile::exists(QApplication::applicationDirPath() + "/nests_event.json"))
    {
        ui->comboBoxDen->addItem(tr("Event"), 100);
    }

    denIndexChanged(0);

    ui->comboBoxGame->setItemData(0, Game::Sword);
    ui->comboBoxGame->setItemData(1, Game::Shield);

    connect(ui->comboBoxDen, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &SeedCalculator::denIndexChanged);
    connect(ui->comboBoxRarity, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &SeedCalculator::rarityIndexChanged);
    connect(ui->comboBoxGame, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &SeedCalculator::gameIndexChanged);
    connect(ui->pushButtonSearch, &QPushButton::clicked, this, &SeedCalculator::search);
    connect(ui->pushButtonClear, &QPushButton::clicked, this, &SeedCalculator::clear);
    connect(ui->checkBoxDay6, &QCheckBox::stateChanged, ui->raidInfo35, &RaidInfo35::displayDay6);
    connect(ui->tabWidgetStars, &QTabWidget::currentChanged, this, [=](int index) { ui->checkBoxDay6->setVisible(index == 0); });
}

void SeedCalculator::toggleControls(bool flag)
{
    ui->comboBoxDen->setEnabled(flag);
    ui->comboBoxRarity->setEnabled(flag);
    ui->comboBoxGame->setEnabled(flag);
    ui->spinBoxIVDeviationMin->setEnabled(flag);
    ui->spinBoxIVDeviationMax->setEnabled(flag);
    ui->checkBoxStop->setEnabled(flag);
    ui->checkBoxDay6->setEnabled(flag);
    ui->pushButtonSearch->setEnabled(flag);
    ui->pushButtonCancel->setEnabled(!flag);

    ui->tabWidgetStars->setEnabled(flag);

    ui->progressBar->setEnabled(!flag);
    ui->progressLabel->setEnabled(!flag);
    if (!flag)
    {
        ui->progressLabel->setText("");
    }
}

void SeedCalculator::search35()
{
    if (!ui->raidInfo35->isValid())
    {
        QMessageBox message(QMessageBox::Warning, tr("Invalid settings"), tr("Search settings are invalid"));
        message.show();
        message.exec();
        return;
    }
    toggleControls(false);

    QVector<int> ivCount = ui->raidInfo35->getIVCounts();
    QVector<Pokemon> pokemon = { ui->raidInfo35->getPokemonDay4_1(), ui->raidInfo35->getPokemonDay4_2(), ui->raidInfo35->getPokemonDay5() };
    if (ui->checkBoxDay6->isChecked())
    {
        pokemon.append(ui->raidInfo35->getPokemonDay6());
    }

    auto *searcher = new SeedSearcher35(pokemon, ivCount, ui->checkBoxStop->isChecked());
    searcher->setIVs(ui->raidInfo35->getConditionIVs());
    connect(ui->pushButtonCancel, &QPushButton::clicked, [searcher] { searcher->cancelSearch(); });

    ui->progressBar->setRange(0, searcher->getMaxProgress());

    auto *timer = new QTimer();
    auto startTime = time(nullptr);
    connect(timer, &QTimer::timeout, [=] {
        auto progress = searcher->getProgress();
        ui->progressBar->setValue(progress);
        auto elapsedTime = time(nullptr) - startTime;
        auto estimatedTime = elapsedTime * (searcher->getMaxProgress() - progress) / progress;
        ui->progressLabel->setText(tr("Elapsed time: %1:%2:%3 - Estimated time: %4:%5:%6")
                                   .arg((elapsedTime / 60) / 60, 2, 10, QLatin1Char('0'))
                                   .arg((elapsedTime / 60) % 60, 2, 10, QLatin1Char('0'))
                                   .arg(elapsedTime % 60, 2, 10, QLatin1Char('0'))
                                   .arg((estimatedTime / 60) / 60, 2, 10, QLatin1Char('0'))
                                   .arg((estimatedTime / 60) % 60, 2, 10, QLatin1Char('0'))
                                   .arg(estimatedTime % 60, 2, 10, QLatin1Char('0')));
    });

    auto *watcher = new QFutureWatcher<void>();
    connect(watcher, &QFutureWatcher<void>::finished, watcher, &QFutureWatcher<void>::deleteLater);
    connect(watcher, &QFutureWatcher<void>::destroyed, this, [=] {
        toggleControls(true);

        timer->stop();
        delete timer;

        QVector<u64> seeds = searcher->getResults();
        ui->progressBar->setValue(searcher->getProgress());
        auto elapsedTime = time(nullptr) - startTime;
        ui->progressLabel->setText(tr("Elapsed time: %1:%2:%3 - Estimated time: %4:%5:%6")
                                   .arg((elapsedTime / 60) / 60, 2, 10, QLatin1Char('0'))
                                   .arg((elapsedTime / 60) % 60, 2, 10, QLatin1Char('0'))
                                   .arg(elapsedTime % 60, 2, 10, QLatin1Char('0'))
                                   .arg("00").arg("00").arg("00"));

        delete searcher;

        for (u64 seed : seeds)
        {
            ui->textEdit->append(QString("0x%1").arg(QString::number(seed, 16).toUpper().rightJustified(16, '0')));
        }
    });

    QSettings setting;
    int threads = setting.value("settings/thread", QThread::idealThreadCount()).toInt();

    int minRolls = ui->spinBoxIVDeviationMin->value();
    int maxRolls = ui->spinBoxIVDeviationMax->value();
    auto future = QtConcurrent::run([=] { searcher->startSearch(minRolls, maxRolls, threads); });

    watcher->setFuture(future);
    timer->start(1000);
}

void SeedCalculator::search12()
{
    if (!ui->raidInfo12->isValid())
    {
        QMessageBox message(QMessageBox::Warning, tr("Invalid settings"), tr("Search settings are invalid"));
        message.show();
        message.exec();
        return;
    }
    toggleControls(false);

    QVector<int> ivCount = ui->raidInfo12->getIVCounts();
    QVector<Pokemon> pokemon = { ui->raidInfo12->getPokemonDay1(), ui->raidInfo12->getPokemonDay2() };

    auto *searcher = new SeedSearcher12(pokemon, ivCount, ui->checkBoxStop->isChecked(), pokemon.at(0).getAbility() != 255);
    connect(ui->pushButtonCancel, &QPushButton::clicked, [searcher] { searcher->cancelSearch(); });

    ui->progressBar->setRange(0, searcher->getMaxProgress());

    auto *timer = new QTimer();
    auto startTime = time(nullptr);
    connect(timer, &QTimer::timeout, [=] {
        auto progress = searcher->getProgress();
        ui->progressBar->setValue(progress);
        auto elapsedTime = time(nullptr) - startTime;
        auto estimatedTime = elapsedTime * (searcher->getMaxProgress() - progress) / progress;
        ui->progressLabel->setText(tr("Elapsed time: %1:%2:%3 - Estimated time: %4:%5:%6")
                                   .arg((elapsedTime / 60) / 60, 2, 10, QLatin1Char('0'))
                                   .arg((elapsedTime / 60) % 60, 2, 10, QLatin1Char('0'))
                                   .arg(elapsedTime % 60, 2, 10, QLatin1Char('0'))
                                   .arg((estimatedTime / 60) / 60, 2, 10, QLatin1Char('0'))
                                   .arg((estimatedTime / 60) % 60, 2, 10, QLatin1Char('0'))
                                   .arg(estimatedTime % 60, 2, 10, QLatin1Char('0')));
    });

    auto *watcher = new QFutureWatcher<void>();
    connect(watcher, &QFutureWatcher<void>::finished, watcher, &QFutureWatcher<void>::deleteLater);
    connect(watcher, &QFutureWatcher<void>::destroyed, this, [=] {
        toggleControls(true);

        timer->stop();
        delete timer;

        QVector<u64> seeds = searcher->getResults();
        ui->progressBar->setValue(searcher->getProgress());
        auto elapsedTime = time(nullptr) - startTime;
        ui->progressLabel->setText(tr("Elapsed time: %1:%2:%3 - Estimated time: %4:%5:%6")
                                   .arg((elapsedTime / 60) / 60, 2, 10, QLatin1Char('0'))
                                   .arg((elapsedTime / 60) % 60, 2, 10, QLatin1Char('0'))
                                   .arg(elapsedTime % 60, 2, 10, QLatin1Char('0'))
                                   .arg("00").arg("00").arg("00"));

        delete searcher;

        for (u64 seed : seeds)
        {
            ui->textEdit->append(QString("0x%1").arg(QString::number(seed, 16).toUpper().rightJustified(16, '0')));
        }
    });

    QSettings setting;
    int threads = setting.value("settings/thread", QThread::idealThreadCount()).toInt();

    int minRolls = ui->spinBoxIVDeviationMin->value();
    int maxRolls = ui->spinBoxIVDeviationMax->value();
    auto future = QtConcurrent::run([=] { searcher->startSearch(minRolls, maxRolls, threads); });

    watcher->setFuture(future);
    timer->start(1000);
}

void SeedCalculator::denIndexChanged(int index)
{
    if (index >= 0)
    {
        u8 rarity = static_cast<u8>(ui->comboBoxRarity->currentIndex());
        auto den = DenLoader::getDen(static_cast<u8>(ui->comboBoxDen->currentData().toInt()), rarity);
        auto game = static_cast<Game>(ui->comboBoxGame->currentData().toInt());

        ui->raidInfo35->setDen(den, game);
        ui->raidInfo12->setDen(den, game);
    }
}

void SeedCalculator::rarityIndexChanged(int index)
{
    if (index >= 0)
    {
        denIndexChanged(ui->comboBoxDen->currentIndex());
    }
}

void SeedCalculator::gameIndexChanged(int index)
{
    if (index >= 0)
    {
        denIndexChanged(ui->comboBoxDen->currentIndex());
    }
}

void SeedCalculator::search()
{
    if (ui->tabWidgetStars->currentIndex() == 0)
    {
        search35();
    }
    else
    {
        search12();
    }
}

void SeedCalculator::clear()
{
    ui->textEdit->clear();
    ui->progressBar->setValue(0);
    ui->progressLabel->setText("");
}
