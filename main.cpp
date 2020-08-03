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

#include <Core/Util/Translator.hpp>
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

    QString locale = setting.value("settings/locale", "en").toString();
    if (!QStringList({ "de", "en", "es", "fr", "it", "ja", "ko", "pt", "tw", "zh" }).contains(locale))
    {
        locale = "en";
        setting.setValue("settings/locale", "en");
    }

    Translator::init(locale);

    QTranslator translator;
    if (translator.load(QString(":/i18n/RaidFinder_%1.qm").arg(locale)))
    {
        app.installTranslator(&translator);
    }

    MainWindow w;
    w.show();
    w.raise();

    return app.exec();
}
