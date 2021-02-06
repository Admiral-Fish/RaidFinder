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

#include <Core/Loader/ProfileLoader.hpp>
#include <Core/Util/Translator.hpp>
#include <Forms/MainWindow.hpp>
#include <QApplication>
#include <QFile>
#include <QJsonDocument>
#include <QSettings>
#include <QTextStream>
#include <QTranslator>

void validateSettings(QSettings &setting)
{
    if (!setting.contains("settings/profiles"))
    {
        setting.setValue("settings/profiles", QString("%1/profiles.json").arg(QApplication::applicationDirPath()));
    }

    if (!setting.contains("settings/style"))
    {
        setting.setValue("settings/style", "dark");
    }

    if (!setting.contains("settings/locale"))
    {
        setting.setValue("settings/locale", "en");
    }
}

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setApplicationName("RaidFinder");
    app.setOrganizationName("Admiral-Fish");

    QSettings setting;
    validateSettings(setting);

    QString profilePath = setting.value("settings/profiles").toString();
    ProfileLoader::init(profilePath.toStdString());

    // Transfer profiles to new setup
    // TODO: remove in a future version
    if (!setting.contains("settings/migrated"))
    {
        QByteArray data = setting.value("profiles").toByteArray();
        QJsonDocument profiles(QJsonDocument::fromJson(data));
        if (!profiles.isNull())
        {
            QFile f(profilePath);
            if (f.open(QIODevice::WriteOnly))
            {
                f.write(QJsonDocument(profiles).toJson());
            }
        }
        setting.setValue("settings/migrated", true);
    }

    // Buttons currently aren't easy to press with style sheet
    // Disable it for now on MacOS
#ifndef Q_OS_MAC
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
#endif

    QString locale = setting.value("settings/locale", "en").toString();
    Translator::init(locale.toStdString());

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
