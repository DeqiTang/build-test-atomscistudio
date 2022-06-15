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

#ifndef MODELING_OCC_OCCVIEW_H
#define MODELING_OCC_OCCVIEW_H

#include <QWidget>
#include <QFileDialog>
#include <QMouseEvent>

#include <Aspect_DisplayConnection.hxx>
#include <OpenGl_GraphicDriver.hxx>
#include <V3d_View.hxx>
#include <AIS_InteractiveContext.hxx>
#include <AIS_ViewController.hxx>

class OccView : public QWidget, protected AIS_ViewController {
    Q_OBJECT
public:

    OccView(QWidget* parent = nullptr);
    ~OccView();

    const Handle(V3d_View)& get_view() const {
        return m_v3d_view;
    }
    const Handle(AIS_InteractiveContext)& get_context() const {
        return m_ais_context;
    }
    void fit_all_auto() {
        FitAllAuto(m_ais_context, m_v3d_view);
    }

    void set_ball_and_stick_style();
    void set_van_der_waals_style();
    void set_stick_style();

public:
    enum DisplayStyle {
        BallAndStick,
        VanDerWaals,
        Stick,
    };

signals:

public slots:

protected:
    virtual void paintEvent(QPaintEvent* event) override;
    virtual void resizeEvent(QResizeEvent* event) override;
    virtual void mousePressEvent(QMouseEvent* event) override;
    virtual void mouseReleaseEvent(QMouseEvent* event) override;
    virtual void mouseMoveEvent(QMouseEvent* event) override;
    virtual void wheelEvent(QWheelEvent* event) override;

private:
    DisplayStyle m_draw_style;

    Graphic3d_Vec2i m_mouse_click_pos;
    Handle(Aspect_DisplayConnection) m_display_connection;
    Handle(Graphic3d_GraphicDriver) m_graphic_driver;
    Handle(V3d_Viewer) m_v3d_viewer;
    Handle(V3d_View) m_v3d_view;
    Handle(AIS_InteractiveContext) m_ais_context;
    Handle(Aspect_Window) m_aspect_window;
};

#endif // MODELING_OCC_OCCVIEW_H
