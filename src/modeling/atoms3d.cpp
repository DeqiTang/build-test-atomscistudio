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



#include "atoms3d.h"

#include <iostream>


Atoms3D::Atoms3D(QWidget* parent, Qt3DCore::QEntity* root_entity)
    : QWidget(parent), m_root_entity(root_entity) {

    this->m_crystal = std::make_shared<atomsciflow::Crystal>();
    this->m_atomic_radius = std::make_shared<atomsciflow::AtomicRadius>();
    this->m_atomic_color = std::make_shared<AtomicColor>();

    this->m_rightpop_menu = new QMenu(this);
    auto action_delete_atom = new QWidgetAction(this);
    action_delete_atom->setText(QObject::tr("Delete Atom"));

    auto action_change_atom = new QWidgetAction(this);
    action_change_atom->setText(tr("Change Atom"));
    this->m_rightpop_menu->addAction(action_delete_atom);
    this->m_rightpop_menu->addAction(action_change_atom);

    QObject::connect(action_delete_atom, &QAction::triggered, this, &Atoms3D::handle_delete_atom);

    this->m_crystal->read_xyz_str(
"3\n"
"cell: 15.000000 0.000000 0.000000 | 0.000000 15.000000 0.000000 | 0.000000 0.000000 15.000000\n"
"H	6.759403	6.670494	6.820388\n"
"H	5.762761	7.476846	6.820388\n"
"O	5.815481	6.650009	6.468440\n"
    );
    std::cout << this->m_crystal->natom() << std::endl;
    int natom = this->m_crystal->natom();
    for (int i = 0; i < natom; i++) {
        auto sphere_entity = new Qt3DCore::QEntity(this->m_root_entity);
        this->m_atoms_entity_id.push_back(sphere_entity->id().id());
        sphere_entity->setObjectName(QString::fromStdString(this->m_crystal->atoms[i].name));
        this->m_atoms_entity.push_back(sphere_entity);
        this->m_atoms_status.push_back(AtomStatus::Normal);
    }

    this->draw_atoms();

}


void Atoms3D::draw_atoms() {
    this->clean_draw();

    int i = -1;
    for (auto& atom : this->m_crystal->atoms) {
        i++;

        if (this->m_atoms_status[i] != AtomStatus::Normal || this->m_atoms_status[i] == AtomStatus::Drawn) {
            continue;
        }

        Qt3DExtras::QSphereMesh *sphere_mesh = new Qt3DExtras::QSphereMesh();
        sphere_mesh->setRings(20);
        sphere_mesh->setSlices(20);
        sphere_mesh->setRadius(this->m_atomic_radius->calculated[atom.name]);

        Qt3DCore::QTransform *sphere_transform = new Qt3DCore::QTransform();
        sphere_transform->setScale(1.0);
        sphere_transform->setTranslation(QVector3D(atom.x, atom.y, atom.z));

        Qt3DExtras::QPhongMaterial *sphere_material = new Qt3DExtras::QPhongMaterial();
        auto rgba = this->m_atomic_color->jmol[atom.name];
        sphere_material->setDiffuse(QColor(rgba[0], rgba[1], rgba[2]));

        Qt3DRender::QObjectPicker* picker = new Qt3DRender::QObjectPicker(this->m_atoms_entity[i]);
        picker->setHoverEnabled(true);
        picker->setEnabled(true);

        QObject::connect(picker, &Qt3DRender::QObjectPicker::pressed, this, &Atoms3D::handle_picker_press);
        QObject::connect(picker, &Qt3DRender::QObjectPicker::clicked, this, &Atoms3D::handle_picker_click);

        if (this->m_atoms_entity[i] == nullptr) {
            this->m_atoms_entity[i] = new Qt3DCore::QEntity(m_root_entity);
            this->m_atoms_entity_id[i] = this->m_atoms_entity[i]->id().id();
        }

        this->m_atoms_entity[i]->addComponent(sphere_mesh);
        this->m_atoms_entity[i]->addComponent(sphere_material);
        this->m_atoms_entity[i]->addComponent(sphere_transform);
        this->m_atoms_entity[i]->addComponent(picker);
        auto atom_status_component = new AtomStatusComponent;
        atom_status_component->status = AtomStatus::Normal;
        atom_status_component->setObjectName(QObject::tr("Status"));
        this->m_atoms_entity[i]->addComponent(atom_status_component);
        this->m_atoms_entity[i]->setEnabled(true);
        this->m_atoms_status[i] = AtomStatus::Drawn;

    }
}


void Atoms3D::enable_atoms_entity(bool enabled)
{
    for (auto& entity : this->m_atoms_entity) {
        if (entity == nullptr) {
            continue;
        }
        entity->setEnabled(enabled);
    }
}


void Atoms3D::clean_draw() {
    int natom = this->m_crystal->natom();
    for (int i = 0; i < natom; i++) {
        if (this->m_atoms_status[i] == AtomStatus::Removed) {
            delete this->m_atoms_entity[i];
            this->m_atoms_entity[i] = nullptr;
        }
    }
}


void Atoms3D::handle_picker_press(const Qt3DRender::QPickEvent* pick) {
    std::cout << "Pressed " << "object name: "
              << pick->entity()->objectName().toStdString()
              << std::endl;
    std::cout << "Position"
              << " ->x " << pick->position().x()
              << " ->y " << pick->position().y()
              << std::endl;
    std::cout << pick->entity()->components().length()
              << std::endl;
    std::cout << pick->entity()->components()[4]->objectName().toStdString()
              << std::endl;

    this->set_atom_status_by_id(pick->entity()->id().id(), AtomStatus::Removed);
    if (pick->button() == Qt3DRender::QPickEvent::Buttons::RightButton) {
        this->m_rightpop_menu->popup(QCursor::pos());
    }
}


void Atoms3D::handle_picker_click(const Qt3DRender::QPickEvent* pick) {
    std::cout << "Clicked " << "object name: "
              << pick->entity()->objectName().toStdString()
              << std::endl;
    std::cout << "Pick Position"
              << " ->x " << pick->position().x()
              << " ->y " << pick->position().y()
              << std::endl;
}


void Atoms3D::handle_delete_atom() {
    std::cout << "Delete atom" << std::endl;
    this->draw_atoms();
}


void Atoms3D::set_atom_status_by_id(int id, AtomStatus status) {
    int natom = this->m_crystal->natom();
    for (int index = 0; index < natom; index++) {
        if (this->m_atoms_entity_id[index] == id) {
            this->m_atoms_status[index] = status;
            break;
        }
    }
}



