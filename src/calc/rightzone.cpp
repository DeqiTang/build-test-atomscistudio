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


#include "rightzone.h"

RightZone::RightZone(QWidget *parent) : QWidget{parent} {
    QSizePolicy size_policy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    size_policy.setHorizontalStretch(1);
    size_policy.setVerticalStretch(1);
    size_policy.setHeightForWidth(this->sizePolicy().hasHeightForWidth());
    this->setSizePolicy(size_policy);
    this->setMinimumSize(QSize(1200, 600));

    auto vertical_layout = new QVBoxLayout(this);
    vertical_layout->setObjectName(QString::fromUtf8("vertical_layout"));
    vertical_layout->setSizeConstraint(QLayout::SetDefaultConstraint);
    vertical_layout->setContentsMargins(0, 0, 0, 0);

    auto tab_widget = new QTabWidget(this);
    vertical_layout->addWidget(tab_widget);
    tab_widget->setObjectName(QString::fromUtf8("tab_widget"));

    auto tab_1 = new QWidget();
    tab_widget->addTab(tab_1, QString());
    tab_1->setObjectName(QString::fromUtf8("tab"));
    auto grid_layout = new QGridLayout(tab_1);
    grid_layout->setObjectName(QString::fromUtf8("grid_layout"));
    grid_layout->setContentsMargins(0, 0, 0, 0);

    auto check_box = new QCheckBox(tab_1);
    grid_layout->addWidget(check_box, 0, 0, 1, 1);
    check_box->setObjectName(QString::fromUtf8("check_box"));
    check_box->setText(QObject::tr("Show atoms"));
    check_box->setChecked(true);
    check_box->setText(QCoreApplication::translate("Checkbox", "check_box", nullptr));

    auto horizontal_slider = new QSlider(tab_1);
    grid_layout->addWidget(horizontal_slider, 1, 0, 1, 1);
    horizontal_slider->setObjectName(QString::fromUtf8("horizontal_slider"));
    horizontal_slider->setOrientation(Qt::Horizontal);

    auto tab_2 = new QWidget();
    tab_2->setObjectName(QString::fromUtf8("tab_2"));
    tab_widget->addTab(tab_2, QString());

    tab_widget->setTabText(tab_widget->indexOf(tab_1), QCoreApplication::translate("Atoms3DTools", "Tab 1", nullptr));
    tab_widget->setTabText(tab_widget->indexOf(tab_2), QCoreApplication::translate("Atoms3DTools", "Tab 2", nullptr));
    tab_widget->setCurrentIndex(0);

    auto text_browser = new QTextBrowser(this);
    vertical_layout->addWidget(text_browser);
    text_browser->setObjectName(QString::fromUtf8("text_browser"));
    text_browser->setMinimumSize(800, 600);
    text_browser->setText(QObject::tr(
"provide really powerful controlling of the calculation workflow"
    ));

}
