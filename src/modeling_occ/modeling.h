#ifndef MODELING_OCC_MODELING_H
#define MODELING_OCC_MODELING_H

#include <QWidget>
#include <QVBoxLayout>

#include <AIS_ColoredShape.hxx>

#include <atomsciflow/base/crystal.h>
#include <atomsciflow/base/atomic_radius.h>

#include "modeling/atomic_color.h"
#include "occview.h"

class ModelingControl : public QWidget {
    Q_OBJECT
public:
    ModelingControl(QWidget* parent = nullptr);
    ~ModelingControl() = default;

    void draw_atoms();

    std::shared_ptr<atomsciflow::Crystal> m_crystal;

private:

    QVBoxLayout* m_layout;

    std::shared_ptr<atomsciflow::AtomicRadius> m_atomic_radius;
    std::shared_ptr<AtomicColor> m_atomic_color;
    OccView* m_occview;
};
#endif // MODELING_OCC_MODELING_H
