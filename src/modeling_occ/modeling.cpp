#include "modeling.h"

#include <QAction>

#include <BRepPrimAPI_MakeSphere.hxx>

ModelingControl::ModelingControl(QWidget* parent)
    : QWidget{parent} {

    this->m_crystal = std::make_shared<atomsciflow::Crystal>();
    this->m_atomic_radius = std::make_shared<atomsciflow::AtomicRadius>();
    this->m_atomic_color = std::make_shared<AtomicColor>();

    m_layout = new QVBoxLayout(this);
    m_layout->setSpacing(0);
    m_layout->setContentsMargins(3, 3, 3, 3);

    m_occview = new OccView(this);
    m_layout->addWidget(m_occview);

    this->setLayout(m_layout);

    this->show();
    this->setMinimumSize(QSize(1300,800));

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
        m_occview->fit_all();
    }
}