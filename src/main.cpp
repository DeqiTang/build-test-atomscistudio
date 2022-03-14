/************************************************************************
 *
 * Atom Science Studio
 * Copyright (C) 2022  Deqi Tang
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 ***********************************************************************/

#include <QApplication>
#include <QShortcut>
#include <QKeySequence>

#include "main/mainwindow.h"

int main(int argc, char* argv[]) {

    QApplication app(argc, argv);

    MainWindow main_win;

    auto shortcut_close = new QShortcut(QKeySequence(QObject::tr("Ctrl+W")), main_win.m_central_widget);
    QObject::connect(shortcut_close, &QShortcut::activated, &app, [&]() {
        main_win.close();
        app.quit();
    });

    main_win.show();
    main_win.resize(1920, 1080);

    return app.exec();
}
