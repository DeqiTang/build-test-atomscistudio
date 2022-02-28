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


#include "calccontrol.h"

#include <iostream>
#include <QTabWidget>
#include <QCheckBox>
#include <QSplitter>

#include "calc/leftzone.h"
#include "calc/rightzone.h"

#include <atomsciflow/remote/ssh.h>
#include <yaml-cpp/yaml.h>

CalcControl::CalcControl(QWidget *parent) : QWidget{parent} {

    this->m_hlayout = new QHBoxLayout(this);
    this->setLayout(this->m_hlayout);

    auto h_splitter = new QSplitter(this);
    this->m_hlayout->addWidget(h_splitter);

    auto left_zone = new LeftZone(this);
    auto right_zone = new RightZone(this);
    h_splitter->addWidget(left_zone);
    h_splitter->addWidget(right_zone);

    h_splitter->setVisible(true);
    h_splitter->setHandleWidth(10);
    h_splitter->setFrameShape(QFrame::StyledPanel);
    h_splitter->setFrameShadow(QFrame::Plain);
    h_splitter->setStyleSheet("QSplitter::handle {background-color: gray}");

    atomsciflow::Ssh ssh;

    YAML::Node yaml_node = YAML::Load("[1, 2, 3, 4, 5, 6]");
    //for (int i = 0; i < yaml_node.size(); i++) {
    //    std::cout << yaml_node[i].as<int>() << std::endl;
    //}

}
