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

#include <Forms/MainWindow.hpp>
#include <QApplication>
#include <QFile>
#include <QSettings>
#include <QTextStream>
#include <QTranslator>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setApplicationName("RaidFinder");
    app.setOrganizationName("Admiral-Fish");

    QSettings setting;

    QString style = setting.value("settings/style", "dark").toString();
    if (style == "dark")
    {
        QFile file(":/qdarkstyle/style.qss");
        if (file.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            QTextStream ts(&file);
            app.setStyleSheet(ts.readAll());
            file.close();
        }
    }

    QTranslator translator;
    if (translator.load(QString(":/i18n/RaidFinder_%1.qm").arg(setting.value("settings/locale", "en").toString())))
    {
        app.installTranslator(&translator);
    }

    MainWindow w;
    w.show();
    w.raise();

    return app.exec();
}
