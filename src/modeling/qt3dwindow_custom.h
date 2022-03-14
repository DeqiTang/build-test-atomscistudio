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

#ifndef MODELING_QT3DWINDOWCUSTOM_H
#define MODELING_QT3DWINDOWCUSTOM_H

#include <QtWidgets/QWidget>
#include <Qt3DCore/qentity.h>
#include <Qt3DRender/qcamera.h>
#include <Qt3DRender/qcameralens.h>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QCommandLinkButton>
#include <QtGui/QScreen>
#include <Qt3DRender/qpointlight.h>
#include <Qt3DCore/qtransform.h>
#include <Qt3DCore/qaspectengine.h>
#include <Qt3DRender/qrenderaspect.h>
#include <Qt3DExtras/qforwardrenderer.h>
#include <Qt3DExtras/qt3dwindow.h>
#include <QOrbitCameraController>
#include <QMenu>

#include "modeling/atoms3d.h"

class Qt3DWindowCustom: public Qt3DExtras::Qt3DWindow {
public:
    Qt3DWindowCustom(QWidget* parent, QLayout* vlayout, QHBoxLayout* hlayout);

    QWidget* m_root_widget;
    Qt3DCore::QEntity* m_root_entity;
    Qt3DRender::QCamera* m_camera_entity;
    Qt3DExtras::QOrbitCameraController* m_orbit_cam_controller;
    Atoms3D* m_atoms3d;
    QMenu* m_right_pop_menu;

public slots:
    void handle_picker_press(const Qt3DRender::QPickEvent* pick);
    void handle_picker_click(const Qt3DRender::QPickEvent* pick);

};

#endif // MODELING_QT3DWINDOWCUSTOM_H
