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

#include "modeling_occ/modeling.h"

#include <QAction>

#include <BRepPrimAPI_MakeSphere.hxx>

ModelingControl::ModelingControl(QWidget* parent)
    : QWidget{parent} {

    this->m_crystal = std::make_shared<atomsciflow::Crystal>();
    this->m_atomic_radius = std::make_shared<atomsciflow::AtomicRadius>();
    this->m_atomic_color = std::make_shared<AtomicColor>();

    m_layout = new QVBoxLayout(this);
    m_layout->setSpacing(0);
    m_layout->setContentsMargins(2, 2, 2, 2);

    m_occview = new OccView(this);
    m_layout->addWidget(m_occview);

    this->setLayout(m_layout);

    this->show();

    this->m_crystal->read_xyz_str(
"3\n"
"cell: 15.000000 0.000000 0.000000 | 0.000000 15.000000 0.000000 | 0.000000 0.000000 15.000000\n"
"H	6.759403	6.670494	6.820388\n"
"H	5.762761	7.476846	6.820388\n"
"O	5.815481	6.650009	6.468440\n"
    );
    this->draw_atoms();
}

void ModelingControl::draw_atoms() {
    gp_Ax2 axis;
    for (const auto& atom : this->m_crystal->atoms) {
        axis.SetLocation(gp_Pnt(atom.x, atom.y, atom.z));
        TopoDS_Shape atom_topo = BRepPrimAPI_MakeSphere(
            axis,
            this->m_atomic_radius->calculated[atom.name]
        ).Shape();
        Handle(AIS_Shape) atom_sphere = new AIS_Shape(atom_topo);

        auto rgba = this->m_atomic_color->jmol[atom.name];
        atom_sphere->SetColor(Quantity_Color{rgba[0] / 255., rgba[1] / 255., rgba[2] / 255., Quantity_TOC_sRGB});
        atom_sphere->Attributes()->SetFaceBoundaryDraw(Standard_False);

        m_occview->get_context()->Display(atom_sphere, Standard_True);
        m_occview->fit_all_auto();
    }
}
