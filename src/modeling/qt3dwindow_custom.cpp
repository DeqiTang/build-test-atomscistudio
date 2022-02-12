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



#include "qt3dwindow_custom.h"

#include <armadillo>
#include <QMouseDevice>
#include <Qt3DRender/QCameraLens>
#include <QToolBar>
#include <QSplitter>

#include "modeling/tools.h"

Qt3DWindowCustom::Qt3DWindowCustom(QWidget* parent, QLayout* vlayout, QHBoxLayout* hlayout) {

    this->m_root_widget = parent;

    this->defaultFrameGraph()->setClearColor(QColor(0, 0, 0));

    this->m_root_entity = new Qt3DCore::QEntity();
    this->setRootEntity(m_root_entity);

    this->m_right_pop_menu = new QMenu(this->m_root_widget);
    auto actionDeleteAtom = new QAction(QObject::tr("Delete Atom"), this->m_root_widget);
    auto actionChangeAtom = new QAction(QObject::tr("Change Atom"), this->m_root_widget);
    this->m_right_pop_menu->addAction(actionDeleteAtom);
    this->m_right_pop_menu->addAction(actionChangeAtom);


    this->m_atoms3d = new Atoms3D(this->m_root_widget, m_root_entity);

    this->m_camera_entity = this->camera();
    this->m_camera_entity->setProjectionType(Qt3DRender::QCameraLens::ProjectionType::PerspectiveProjection);
    this->m_camera_entity->setFieldOfView(10);
    this->m_camera_entity->setNearPlane(0.01);
    this->m_camera_entity->setFarPlane(1000);


    int n = 0;
    float mean_x = 0, mean_y = 0, mean_z = 0;
    for (auto& atom : this->m_atoms3d->m_crystal->atoms) {
        mean_x += atom.x;
        mean_y += atom.y;
        mean_z += atom.z;
        n += 1;
    }
    mean_x /= n;
    mean_y /= n;
    mean_z /= n;
    std::cout << "Mean->XYZ: " << mean_x << " " << mean_y << " " << mean_z << std::endl;

    this->m_camera_entity->setViewCenter(QVector3D(mean_x, mean_y, mean_z));
    std::cout << "View Center of the camera: "
              << this->m_camera_entity->viewCenter().x() << " "
              << this->m_camera_entity->viewCenter().y() << " "
              << this->m_camera_entity->viewCenter().z()
              << std::endl;
    this->m_camera_entity->setPosition(QVector3D(0, 0, mean_z * 5));
    this->m_camera_entity->setUpVector(QVector3D(0, 1, 0));


    auto light_entity_1 = new Qt3DCore::QEntity(m_root_entity);
    auto light_1 = new Qt3DRender::QPointLight(light_entity_1);
    light_1->setColor("white");
    light_1->setIntensity(1);
    light_entity_1->addComponent(light_1);
    auto light_transform_1 = new Qt3DCore::QTransform(light_entity_1);
    light_transform_1->setTranslation(m_camera_entity->position());
    light_entity_1->addComponent(light_transform_1);

    auto light_entity_2 = new Qt3DCore::QEntity(m_root_entity);
    auto light_2 = new Qt3DRender::QPointLight(light_entity_2);
    light_2->setColor("white");
    light_2->setIntensity(1);
    light_entity_2->addComponent(light_2);
    auto light_transform_2 = new Qt3DCore::QTransform(light_entity_2);
    light_transform_2->setTranslation(QVector3D(0, 0, 0));
    light_entity_2->addComponent(light_transform_2);

    
    auto orbit_cam_controller = new Qt3DExtras::QOrbitCameraController(m_root_entity);
    orbit_cam_controller->setCamera(m_camera_entity);

    Qt3DRender::QObjectPicker* picker = new Qt3DRender::QObjectPicker(this->m_root_entity);
    picker->setHoverEnabled(true);
    picker->setEnabled(true);

    QObject::connect(picker, &Qt3DRender::QObjectPicker::pressed, this, &Qt3DWindowCustom::handle_picker_press);
    QObject::connect(picker, &Qt3DRender::QObjectPicker::clicked, this, &Qt3DWindowCustom::handle_picker_click);

}


void Qt3DWindowCustom::handle_picker_press(const Qt3DRender::QPickEvent* pick) {

    std::cout << "Pressed " << "object name: " 
              << pick->objectName().toStdString() 
              << std::endl;
    std::cout << "Position"
              << " ->x " << pick->position().x()
              << " ->y " << pick->position().y() 
              << std::endl;
}


void Qt3DWindowCustom::handle_picker_click(const Qt3DRender::QPickEvent* pick) {

    std::cout << "Clicked " << "object name: " 
              << pick->entity()->objectName().toStdString() 
              << std::endl;
    std::cout << "Pick Position"
              << " ->x " << pick->position().x()
              << " ->y " << pick->position().y() 
              << std::endl;
}


