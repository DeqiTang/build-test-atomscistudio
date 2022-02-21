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


#include "modeling/tools.h"

Tools::Tools(QWidget* parent, Qt3DWindowCustom* qt3dwindow_custom)
    : QWidget(parent) {

    this->set_qt3dwindow_custom(qt3dwindow_custom);

    if (this->objectName().isEmpty()) {
        this->setObjectName(QString::fromUtf8("Tools"));
    }
    QSizePolicy size_policy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    this->setSizePolicy(size_policy);
    this->setMinimumSize(QSize(500, 800));
    this->setWindowTitle(QCoreApplication::translate("Atoms3DTools", "Form", nullptr));


    auto vertical_layout = new QVBoxLayout(this);
    this->setLayout(vertical_layout);
    vertical_layout->setObjectName(QString::fromUtf8("vertical_layout"));


    auto tab_widget = new QTabWidget(this);
    vertical_layout->addWidget(tab_widget);
    tab_widget->setObjectName(QString::fromUtf8("tab_widget"));
    tab_widget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    tab_widget->setSizePolicy(size_policy);
    tab_widget->setCurrentIndex(0);

    auto tab_1 = new QWidget(this);
    tab_widget->addTab(tab_1, QString());
    tab_1->setObjectName(QString::fromUtf8("tab_1"));
    tab_1->setSizePolicy(size_policy);

    auto grid_layout = new QGridLayout(tab_1);
    tab_1->setLayout(grid_layout);
    grid_layout->setObjectName(QString::fromUtf8("grid_layout"));

    auto checkbox_show_atoms = new QCheckBox(tab_1);
    grid_layout->addWidget(checkbox_show_atoms, 0, 0, 1, 1);
    checkbox_show_atoms->setObjectName(QString::fromUtf8("show_atoms"));
    checkbox_show_atoms->setSizePolicy(size_policy);
    checkbox_show_atoms->setText(QObject::tr("Show Atoms"));
    checkbox_show_atoms->setChecked(true);
    checkbox_show_atoms->setText(QCoreApplication::translate("Atoms3DTools", "Show Atoms", nullptr));
    QObject::connect(checkbox_show_atoms, &QCheckBox::stateChanged, this, &Tools::on_checkbox_state_changed);

    auto horizontal_slider = new QSlider(tab_1);
    grid_layout->addWidget(horizontal_slider, 1, 0, 1, 1);
    horizontal_slider->setObjectName(QString::fromUtf8("horizontal_slider"));
    horizontal_slider->setOrientation(Qt::Horizontal);

    auto tab_2 = new QWidget(this);
    tab_widget->addTab(tab_2, QString());
    tab_2->setObjectName(QString::fromUtf8("tab_2"));

    tab_widget->setTabText(tab_widget->indexOf(tab_1), QCoreApplication::translate("Tools", "Tab 1", nullptr));
    tab_widget->setTabText(tab_widget->indexOf(tab_2), QCoreApplication::translate("Tools", "Tab 2", nullptr));

    auto text_browser = new QTextBrowser(this);
    vertical_layout->addWidget(text_browser);
    text_browser->setObjectName(QString::fromUtf8("m_text_browser"));
    text_browser->setText(QObject::tr(
"Atom Science Studio will be a GUI application to provide modeling and workflow automation "
"for simulations involving atoms."
    ));

}



void Tools::on_checkbox_state_changed(int arg1) {

    this->m_qt3dwindow_custom->m_atoms3d->enable_atoms_entity(arg1);
}


