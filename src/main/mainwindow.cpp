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


#include "main/mainwindow.h"

#include <iostream>
#include <QDebug>
#include <QSplitter>
#include <QFileDialog>

#include <atomsciflow/base/crystal.h>

#include "modeling/qt3dwindow_custom.h"
#include "modeling/tools.h"
#include "calc/calccontrol.h"

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent) {

    m_root_menubar = new QMenuBar(this);
    setMenuBar(m_root_menubar);
    m_root_menubar->setObjectName(QObject::tr("m_root_menubar"));

    auto file_menu = new QMenu(m_root_menubar);
    file_menu->setTitle("File");
    file_menu->setObjectName(QObject::tr("File"));
    this->m_root_menubar->addAction(file_menu->menuAction());
    auto action_submenu_of_file = new QAction(this->m_root_menubar);
    file_menu->addAction(action_submenu_of_file);
    action_submenu_of_file->setObjectName(QObject::tr("Export Image"));
    action_submenu_of_file->setText("Export");
    QObject::connect(action_submenu_of_file, &QAction::triggered, this, &MainWindow::export_to_image);

    auto edit_menu = new QMenu(m_root_menubar);
    edit_menu->setTitle("Edit");
    edit_menu->setObjectName(QObject::tr("Edit"));
    this->m_root_menubar->addAction(edit_menu->menuAction());
    auto action_submenu_of_edit = new QAction(this->m_root_menubar);
    action_submenu_of_edit->setObjectName(QObject::tr("Sub of Edit"));
    edit_menu->addAction(action_submenu_of_edit);
    action_submenu_of_edit->setText("submenu");

    auto view_menu = new QMenu(m_root_menubar);
    view_menu->setTitle("View");
    view_menu->setObjectName(QObject::tr("View"));
    this->m_root_menubar->addAction(view_menu->menuAction());
    auto action_submenu_of_view = new QAction(this->m_root_menubar);
    action_submenu_of_view->setObjectName(QObject::tr("Sub of View"));
    view_menu->addAction(action_submenu_of_view);
    action_submenu_of_view->setText("submenu");

    auto help_menu = new QMenu(m_root_menubar);
    help_menu->setTitle("Help");
    help_menu->setObjectName(QObject::tr("Help"));
    this->m_root_menubar->addAction(help_menu->menuAction());
    auto action_submenu_of_help = new QAction(this->m_root_menubar);
    action_submenu_of_help->setObjectName(QObject::tr("Sub of Help"));
    help_menu->addAction(action_submenu_of_help);
    action_submenu_of_help->setText("submenu");


    this->m_central_widget = new QWidget(this);
    this->setCentralWidget(this->m_central_widget);
    this->m_central_widget->setWindowTitle(QStringLiteral("Atom Science Studio"));

    this->m_root_hlayout = new QHBoxLayout();
    this->m_central_widget->setLayout(this->m_root_hlayout);
    this->m_root_vlayout = new QVBoxLayout();
    m_root_hlayout->addLayout(this->m_root_vlayout);
    m_root_vlayout->setAlignment(Qt::AlignTop);

    this->m_root_tabwidget = new QTabWidget(this->m_central_widget);
    this->m_root_vlayout->addWidget(this->m_root_tabwidget);
    this->m_root_tabwidget->setTabPosition(QTabWidget::West);


    auto tab1 = new QWidget(this->m_central_widget);
    this->m_root_tabwidget->addTab(tab1, QObject::tr("Modeling"));
    auto tab1_hsplitter = new QSplitter(this->m_central_widget);
    auto tab1_hlayout= new QHBoxLayout(tab1);
    tab1_hlayout->addWidget(tab1_hsplitter);
    tab1_hsplitter->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    tab1_hsplitter->setVisible(true);
    tab1_hsplitter->setHandleWidth(10);
    tab1_hsplitter->setFrameShape(QFrame::StyledPanel);
    tab1_hsplitter->setFrameShadow(QFrame::Plain);
    tab1_hsplitter->setStyleSheet("QSplitter::handle {background-color: gray}");
    auto tab1_vlayout = new QVBoxLayout(tab1);
    tab1->setLayout(tab1_hlayout);
    
    auto qt3dwin = new Qt3DWindowCustom(tab1, tab1_vlayout, tab1_hlayout);
    QWidget* win_container = QWidget::createWindowContainer(qt3dwin);
    auto tools = new Tools(this->m_central_widget, qt3dwin);
    tab1_hsplitter->addWidget(tools);
    tab1_hsplitter->addWidget(win_container);
    QSize screenSize = this->screen()->size();
    win_container->setMinimumSize(QSize(200, 100));
    win_container->setMaximumSize(screenSize);

    auto tab2 = new CalcControl(this->m_central_widget);
    this->m_root_tabwidget->addTab(tab2, QObject::tr("Calculation"));

}

void MainWindow::export_to_image() {
    auto fd = new QFileDialog(this->m_central_widget);
    fd->setWindowTitle(QObject::tr("Output image path"));
    fd->setViewMode(QFileDialog::Detail);
    QString file_path;
    file_path = fd->getSaveFileName(this, tr("Save File"), "export.png");
    std::cout << file_path.toStdString() << std::endl;
    delete fd;
}
