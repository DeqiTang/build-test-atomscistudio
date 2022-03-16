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

/// TODO:
/// Currently we use QTabWidget to separate the different space
/// with different functionality, which might be replaced by
/// more flexible QStackWidget + QComboBox in the future.

#ifndef MAIN_MAINWINDOW_H
#define MAIN_MAINWINDOW_H

#include <QMainWindow>
#include <QtWidgets/QHBoxLayout>
#include <QMenuBar>

#include <boost/filesystem.hpp>

#include "config/config_manager.h"
#include "modeling/qt3dwindow_custom.h"

namespace fs = boost::filesystem;

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow() {
    };

    void export_to_image();
    void popup_about();
    void popup_config();

    QWidget* m_central_widget;
    QMenuBar* m_root_menubar;
    QHBoxLayout* m_root_hlayout;
    QVBoxLayout* m_root_vlayout;
    QTabWidget* m_root_tabwidget;
    ConfigManager m_config_manager;
private slots:

private:

};

#endif // MAIN_MAINWINDOW_H
