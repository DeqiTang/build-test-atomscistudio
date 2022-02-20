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
#include <QMessageBox>

#include <atomsciflow/base/crystal.h>

#include "modeling/qt3dwindow_custom.h"
#include "modeling/tools.h"
#include "calc/calccontrol.h"

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent) {

    m_root_menubar = new QMenuBar(this);
    setMenuBar(m_root_menubar);
    m_root_menubar->setObjectName(QObject::tr("m_root_menubar"));

    auto menu_file = new QMenu(m_root_menubar);
    this->m_root_menubar->addMenu(menu_file);
    menu_file->setTitle("&File");
    menu_file->setObjectName(QObject::tr("File"));
    auto action_new = new QAction(this->m_root_menubar);
    menu_file->addAction(action_new);
    action_new->setObjectName(tr("New"));
    action_new->setText(tr("New"));
    action_new->setToolTip(tr("New project"));
    action_new->setStatusTip(tr("New project"));
    action_new->setShortcuts(QKeySequence::New);
    auto action_open = new QAction(this->m_root_menubar);
    menu_file->addAction(action_open);
    action_open->setObjectName("Open");
    action_open->setText(tr("Open"));
    action_open->setStatusTip("Open a project");
    action_open->setShortcuts(QKeySequence::Open);
    auto action_close = new QAction(this->m_root_menubar);
    menu_file->addAction(action_close);
    action_close->setObjectName(tr("Close"));
    action_close->setText(tr("Close"));
    menu_file->addSeparator()->setText(tr("Project"));
    auto menu_file_export = new QMenu(this->m_root_menubar);
    menu_file->addMenu(menu_file_export);
    auto action_export_image = new QAction(this->m_root_menubar);
    menu_file_export->addAction(action_export_image);
    menu_file_export->setTitle(tr("Export"));
    action_export_image->setObjectName(QObject::tr("Export Image"));
    action_export_image->setText("Export Image");
    QObject::connect(action_export_image, &QAction::triggered, this, &MainWindow::export_to_image);

    auto menu_edit = new QMenu(m_root_menubar);
    this->m_root_menubar->addMenu(menu_edit);
    menu_edit->setTitle("&Edit");
    menu_edit->setObjectName(QObject::tr("Edit"));
    auto action_undo = new QAction(this->m_root_menubar);
    menu_edit->addAction(action_undo);
    action_undo->setObjectName(tr("Undo"));
    action_undo->setText(tr("Undo"));
    action_undo->setShortcut(tr("Ctrl+Z"));
    auto action_redo = new QAction(this->m_root_menubar);
    menu_edit->addAction(action_redo);
    action_redo->setObjectName(tr("Redo"));
    action_redo->setText(tr("Redo"));
    action_redo->setShortcut(tr("Ctrl+Shift+Z"));
    menu_edit->addSeparator();
    auto action_copy = new QAction(this->m_root_menubar);
    menu_edit->addAction(action_copy);
    action_copy->setObjectName(tr("Copy"));
    action_copy->setText(tr("Copy"));
    action_copy->setShortcuts(QKeySequence::Copy);
    menu_edit->addSeparator();
    auto action_edit_preference = new QAction(this->m_root_menubar);
    menu_edit->addAction(action_edit_preference);
    action_edit_preference->setObjectName(QObject::tr("Preference"));
    action_edit_preference->setText(tr("Preference"));

    auto menu_view = new QMenu(m_root_menubar);
    this->m_root_menubar->addMenu(menu_view);
    menu_view->setTitle("&View");
    menu_view->setObjectName(QObject::tr("View"));
    auto action_view_projection = new QAction(this->m_root_menubar);
    menu_view->addAction(action_view_projection);
    action_view_projection->setObjectName(QObject::tr("Projection"));
    action_view_projection->setText(tr("Projection"));
    menu_view->addSeparator();
    auto action_view_atoms = new QAction(this->m_root_menubar);
    menu_view->addAction(action_view_atoms);
    action_view_atoms->setObjectName(tr("Atoms"));
    action_view_atoms->setText(tr("Atoms"));
    auto menu_view_plots = new QMenu(m_root_menubar);
    menu_view->addMenu(menu_view_plots);
    menu_view_plots->setTitle(tr("Plots"));
    auto action_view_plots_polar = new QAction(m_root_menubar);
    menu_view_plots->addAction(action_view_plots_polar);
    action_view_plots_polar->setObjectName(tr("Polar"));
    action_view_plots_polar->setText(tr("Polar"));


    auto menu_modeling = new QMenu(m_root_menubar);
    this->m_root_menubar->addMenu(menu_modeling);
    menu_modeling->setTitle("&Modeling");
    menu_modeling->setObjectName(QObject::tr("Modeling"));
    auto action_insert = new QAction(this->m_root_menubar);
    menu_modeling->addAction(action_insert);
    action_insert->setObjectName(QObject::tr("Insert"));
    action_insert->setText(tr("Insert"));
    menu_modeling->addSeparator();
    menu_modeling->addSeparator()->setText(tr("Structure"));
    auto menu_modeling_structure = new QMenu(this->m_root_menubar);
    menu_modeling->addMenu(menu_modeling_structure);
    menu_modeling_structure->setTitle("&Structure");
    auto action_build_supercell = new QAction(this->m_root_menubar);
    menu_modeling_structure->addAction(action_build_supercell);
    action_build_supercell->setObjectName(QObject::tr("Build Supercell"));
    action_build_supercell->setText("Build Supercell");
    menu_modeling_structure->addSeparator()->setText(tr("Structure"));

    auto menu_analysis = new QMenu(m_root_menubar);
    this->m_root_menubar->addMenu(menu_analysis);
    menu_analysis->setTitle("&Analysis");
    menu_analysis->setObjectName(QObject::tr("Analysis"));
    auto action_analysis_dynamics = new QAction(this->m_root_menubar);
    action_analysis_dynamics->setObjectName(tr("Dynamics"));
    menu_analysis->addAction(action_analysis_dynamics);
    action_analysis_dynamics->setText("Dynamics");
    auto menu_analysis_properties = new QMenu(this->m_root_menubar);
    menu_analysis->addMenu(menu_analysis_properties);
    menu_analysis_properties->setTitle(tr("&Properties"));
    auto action_molecule = new QAction(this->m_root_menubar);
    menu_analysis_properties->addAction(action_molecule);
    action_molecule->setObjectName(tr("Molecule"));
    action_molecule->setText(tr("Molecule"));
    menu_analysis_properties->addSeparator();

    auto menu_help = new QMenu(m_root_menubar);
    this->m_root_menubar->addMenu(menu_help);
    menu_help->setTitle("Help");
    menu_help->setObjectName(QObject::tr("Help"));
    auto action_about = new QAction(this->m_root_menubar);
    menu_help->addAction(action_about);
    action_about->setObjectName(QObject::tr("Sub of Help"));
    action_about->setText("About");
    QObject::connect(action_about, &QAction::triggered, this, &MainWindow::popup_about);


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

void MainWindow::popup_about() {
    auto msg_box = new QMessageBox(this->m_central_widget);
    msg_box->setText("About Atomscistudio");
    msg_box->setInformativeText(QObject::tr(
"Atom Science Studio will be a GUI application to provide modeling and workflow automation "
"for simulations involving atoms."
    ));
    msg_box->setStandardButtons(QMessageBox::Ok | QMessageBox::Close | QMessageBox::Abort);
    msg_box->setDefaultButton(QMessageBox::Ok);
    msg_box->exec();
    delete msg_box;
}
