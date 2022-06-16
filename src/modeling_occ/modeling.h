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

#ifndef MODELING_OCC_MODELING_H
#define MODELING_OCC_MODELING_H

#include <QWidget>
#include <QVBoxLayout>

#include <AIS_ColoredShape.hxx>

#include <atomsciflow/base/crystal.h>
#include <atomsciflow/base/atomic_radius.h>

#include "modeling/atomic_color.h"
#include "modeling_occ/occview.h"

class ModelingControl : public QWidget {
    Q_OBJECT
public:
    ModelingControl(QWidget* parent = nullptr);
    ~ModelingControl() = default;

    void draw_atoms();
    void hide_atoms();

    std::shared_ptr<atomsciflow::Crystal> m_crystal;

private:

    QVBoxLayout* m_layout;

    std::shared_ptr<atomsciflow::AtomicRadius> m_atomic_radius;
    std::shared_ptr<AtomicColor> m_atomic_color;
    OccView* m_occview;
};
#endif // MODELING_OCC_MODELING_H
