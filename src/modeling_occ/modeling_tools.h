#ifndef MODELING_OCC_MODELING_TOOLS_H
#define MODELING_OCC_MODELING_TOOLS_H

#include <QWidget>
#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QSlider>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

#include "modeling_occ/modeling.h"

class ModelingTools : public QWidget {
    Q_OBJECT
public:
    explicit ModelingTools(QWidget* parent = nullptr, ModelingControl* modeling_widget = nullptr);

    void set_modeling_widget(ModelingControl* modeling_widget) {
        this->m_modeling_widget = modeling_widget;
    }

    ModelingControl* m_modeling_widget; // never destory this resource in this class

signals:

private slots:

    void on_checkbox_state_changed(int arg1);
};

#endif // MODELING_OCC_MODELING_TOOLS_H
