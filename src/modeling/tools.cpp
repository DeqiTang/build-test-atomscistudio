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

#include <QSplitter>
#include <QGroupBox>
#include <QButtonGroup>

Tools::Tools(QWidget* parent, Qt3DWindowCustom* qt3dwindow_custom)
    : QWidget(parent) {

    this->set_qt3dwindow_custom(qt3dwindow_custom);

    if (this->objectName().isEmpty()) {
        this->setObjectName(QString::fromUtf8("Tools"));
    }
    QSizePolicy size_policy_expanding(QSizePolicy::Expanding, QSizePolicy::Expanding);
    QSizePolicy size_policy_preferred(QSizePolicy::Preferred, QSizePolicy::Preferred);
    this->setSizePolicy(size_policy_expanding);
    this->setMinimumSize(QSize(500, 800));
    this->setWindowTitle(QCoreApplication::translate("Atoms3DTools", "Form", nullptr));

    auto vertical_layout = new QVBoxLayout(this);
    this->setLayout(vertical_layout);
    vertical_layout->setObjectName(QString::fromUtf8("vertical_layout"));

    auto v_splitter = new QSplitter(this);
    vertical_layout->addWidget(v_splitter);
    v_splitter->setOrientation(Qt::Orientation::Vertical);
    v_splitter->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    v_splitter->setVisible(true);
    v_splitter->setHandleWidth(5);
    v_splitter->setFrameShape(QFrame::StyledPanel);
    v_splitter->setFrameShadow(QFrame::Plain);
    v_splitter->setStyleSheet("QSplitter::handle {background-color: gray}");

    auto tab_widget = new QTabWidget(this);
    v_splitter->addWidget(tab_widget);
    tab_widget->setObjectName(QString::fromUtf8("tab_widget"));
    tab_widget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    tab_widget->setSizePolicy(size_policy_expanding);
    tab_widget->setCurrentIndex(0);

    auto tab_1 = new QWidget(this);
    tab_widget->addTab(tab_1, tr("Atom"));
    tab_widget->setTabText(tab_widget->indexOf(tab_1), QCoreApplication::translate("Tools", "Atom", nullptr));
    tab_1->setObjectName(QString::fromUtf8("Atom"));
    tab_1->setSizePolicy(size_policy_expanding);

    auto grid_layout = new QGridLayout(tab_1);
    tab_1->setLayout(grid_layout);
    grid_layout->setObjectName(QString::fromUtf8("grid_layout"));

    auto checkbox_show_atoms = new QCheckBox(tab_1);
    grid_layout->addWidget(checkbox_show_atoms, 0, 0, 1, 1);
    checkbox_show_atoms->setObjectName(QString::fromUtf8("show_atoms"));
    checkbox_show_atoms->setSizePolicy(size_policy_preferred);
    checkbox_show_atoms->setChecked(true);
    checkbox_show_atoms->setText(QCoreApplication::translate("Atoms3DTools", "Show Atoms", nullptr));
    QObject::connect(checkbox_show_atoms, &QCheckBox::stateChanged, this, &Tools::on_checkbox_state_changed);

    auto horizontal_slider = new QSlider(tab_1);
    grid_layout->addWidget(horizontal_slider, 1, 0, 1, 1);
    horizontal_slider->setObjectName(QString::fromUtf8("horizontal_slider"));
    horizontal_slider->setOrientation(Qt::Horizontal);

    auto tab_2 = new QWidget(this);
    tab_widget->addTab(tab_2, QObject::tr("Crystal"));
    tab_widget->setTabText(tab_widget->indexOf(tab_2), QCoreApplication::translate("Tools", "Crystal", nullptr));
    tab_2->setObjectName(QString::fromUtf8("Crystal"));

    auto grid_layout_2 = new QGridLayout(tab_2);
    tab_2->setLayout(grid_layout_2);
    grid_layout_2->setObjectName(tr("grid_layout_2"));

    auto group_box = new QGroupBox(tr("Display"));
    grid_layout_2->addWidget(group_box);
    group_box->setCheckable(false);
    group_box->setChecked(false);
    auto v_layout_tab_2_group_box = new QVBoxLayout(tab_2);
    group_box->setLayout(v_layout_tab_2_group_box);
    auto button_group = new QButtonGroup(tab_2);
    button_group->setExclusive(true);

    auto checkbox_ball_and_stick = new QCheckBox(tab_2);
    v_layout_tab_2_group_box->addWidget(checkbox_ball_and_stick);
    button_group->addButton(checkbox_ball_and_stick);
    checkbox_ball_and_stick->setObjectName((tr("ball and stick")));
    checkbox_ball_and_stick->setSizePolicy(size_policy_preferred);
    checkbox_ball_and_stick->setText(QCoreApplication::translate("Atoms3DTools", "Ball and Stick", nullptr));
    checkbox_ball_and_stick->setChecked(true);
    auto checkbox_van_der_waals = new QCheckBox(tab_2);
    v_layout_tab_2_group_box->addWidget(checkbox_van_der_waals);
    button_group->addButton(checkbox_van_der_waals);
    checkbox_van_der_waals->setObjectName(tr("Van der Waals"));
    checkbox_van_der_waals->setSizePolicy(size_policy_preferred);
    checkbox_van_der_waals->setText(QCoreApplication::translate("Atoms3DTools", "Van der Waals", nullptr));
    checkbox_van_der_waals->setChecked(false);
    auto checkbox_wireframe = new QCheckBox(tab_2);
    v_layout_tab_2_group_box->addWidget(checkbox_wireframe);
    button_group->addButton(checkbox_wireframe);
    checkbox_wireframe->setObjectName(tr("WIreframe"));
    checkbox_wireframe->setSizePolicy(size_policy_preferred);
    checkbox_wireframe->setText(QCoreApplication::translate("Atoms3DTools", "Wireframe", nullptr));
    checkbox_wireframe->setChecked(false);

    auto text_browser = new QTextBrowser(this);
    v_splitter->addWidget(text_browser);
    text_browser->setObjectName(QString::fromUtf8("m_text_browser"));
    text_browser->setText(QObject::tr(
"Atom Science Studio will be a GUI application to provide modeling and workflow automation "
"for simulations involving atoms."
    ));
}

void Tools::on_checkbox_state_changed(int arg1) {

    this->m_qt3dwindow_custom->m_atoms3d->enable_atoms_entity(arg1);
}
