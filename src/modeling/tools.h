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


#ifndef MODELING_TOOLS_H
#define MODELING_TOOLS_H

#include <QWidget>
#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QSlider>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>


#include "modeling/qt3dwindow_custom.h"

class Tools : public QWidget {
    Q_OBJECT
public:
    explicit Tools(QWidget* parent = nullptr, Qt3DWindowCustom* qt3dwindow_custom = nullptr);

    void set_qt3dwindow_custom(Qt3DWindowCustom* qt3dwindow_custom) {
        this->m_qt3dwindow_custom = qt3dwindow_custom;
    }

    Qt3DWindowCustom* m_qt3dwindow_custom; // never destory this resource in this class


signals:

private slots:

    void on_checkbox_state_changed(int arg1);
};


#endif // MODELING_TOOLS_H

