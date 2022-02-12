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


#ifndef MODELING_ATOMS3D_H
#define MODELING_ATOMS3D_H

#include <memory>
#include <vector>
#include <algorithm>

#include <QtCore/QObject>
#include <Qt3DCore/qentity.h>
#include <Qt3DCore/qtransform.h>
#include <Qt3DExtras/QSphereMesh>
#include <Qt3DExtras/QPhongMaterial>
#include <QObjectPicker>
#include <QPickEvent>
#include <QMenu>
#include <QWidgetAction>

#include <atomsciflow/base/crystal.h>
#include <atomsciflow/base/atomic_radius.h>
#include "modeling/atomic_color.h"

class Atoms3D : public QWidget {
    Q_OBJECT
public:
    enum class AtomStatus {
        Normal,
        Drawn,
        Removed,
        Hidden,
        Selected,
    };

    explicit Atoms3D(QWidget* parent, Qt3DCore::QEntity* root_entity);
    ~Atoms3D() {
    };

    void clean_draw();
    void draw_atoms();

    void disable_removed_atoms();

    void set_atom_status_by_id(int id, AtomStatus);

    Qt3DCore::QEntity* m_root_entity;

    std::shared_ptr<atomsciflow::Crystal> m_crystal;
    std::vector<Qt3DCore::QEntity*> m_atoms_entity;
    std::vector<AtomStatus> m_atoms_status;
    std::vector<int> m_atoms_entity_id;

    std::map<qint64, AtomStatus> m_map_atoms_entity_id_status;
    QMenu* m_rightpop_menu;

signals:

public slots:

    void enable_atoms_entity(bool enabled);

    void handle_picker_press(const Qt3DRender::QPickEvent* pick);
    void handle_picker_click(const Qt3DRender::QPickEvent* pick);

    void handle_delete_atom();

private:

    std::shared_ptr<atomsciflow::AtomicRadius> m_atomic_radius;
    std::shared_ptr<AtomicColor> m_atomic_color;
};

class AtomStatusComponent : public Qt3DCore::QComponent {
public:
    Atoms3D::AtomStatus status;
};


#endif // MODELING_ATOMS3D_H
