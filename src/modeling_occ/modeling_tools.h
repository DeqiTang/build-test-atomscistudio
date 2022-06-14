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

#ifndef MODELING_OCC_MODELING_TOOLS_H
#define MODELING_OCC_MODELING_TOOLS_H

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

#include "modeling_occ/modeling.h"

class ModelingTools : public QWidget {
    Q_OBJECT
public:
    explicit ModelingTools(QWidget* parent = nullptr, ModelingControl* modeling_widget = nullptr);

    void set_modeling_widget(ModelingControl* modeling_widget) {
        this->m_modeling_widget = modeling_widget;
    }

    ModelingControl* m_modeling_widget; // never destory this resource in this class

signals:

private slots:

    void on_checkbox_state_changed(int arg1);
};

#endif // MODELING_OCC_MODELING_TOOLS_H
