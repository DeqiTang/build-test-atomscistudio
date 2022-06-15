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

#include "modeling_occ/occview.h"

#include <QMenu>
#include <QApplication>

#if defined(__linux__)
#include <Xw_Window.hxx>
#elif defined(__APPLE__)
#include <Cocoa_Window.hxx>
#elif defined(_WIN32)
#include <WNT_Window.hxx>
#endif

OccView::OccView(QWidget* parent) : QWidget{parent} {

    m_display_connection = new Aspect_DisplayConnection{};
    m_graphic_driver = new OpenGl_GraphicDriver{m_display_connection};
    m_v3d_viewer = new V3d_Viewer{m_graphic_driver};
    m_v3d_view = m_v3d_viewer->CreateView();

#if defined(__linux__)
    m_aspect_window = new Xw_Window{m_display_connection, (Window)winId()};
#elif defined(__APPLE__)
    m_aspect_window = new Cocoa_Window{(NSView *)winId()};
#elif defined(_WIN32)
    m_aspect_window = new WNT_Window{(Aspect_Handle)winId()};
#endif
    m_v3d_view->SetWindow(m_aspect_window);

    if (false == m_aspect_window->IsMapped()) {
        m_aspect_window->Map();
    }

    m_v3d_viewer->SetLightOn();
    m_v3d_viewer->SetDefaultLights();
    m_v3d_viewer->DefaultShadingModel();
    m_v3d_viewer->DefaultComputedMode();
    m_v3d_viewer->DefaultRenderingParams();
    m_v3d_view->SetBackgroundColor(Quantity_Color(
        0., 0., 0.,
        Quantity_TOC_sRGB
    ));
    m_v3d_view->Camera()->SetProjectionType(Graphic3d_Camera::Projection_Orthographic);
    m_v3d_view->ChangeRenderingParams().RenderResolutionScale = 1.0f;
    m_v3d_view->TriedronDisplay(Aspect_TOTP_LEFT_LOWER, Quantity_NOC_GOLD, 0.24, V3d_ZBUFFER);
    m_v3d_view->SetProj(V3d_XposYposZpos);
    m_v3d_view->SetViewOrientationDefault();
    m_v3d_view->MustBeResized();

    m_ais_context = new AIS_InteractiveContext{m_v3d_viewer};
    m_ais_context->SetDisplayMode(AIS_Shaded, Standard_True);
    m_ais_context->SelectionStyle()->SetColor(Quantity_NOC_RED);
    m_ais_context->SelectionStyle()->SetDisplayMode(AIS_Shaded);
    m_ais_context->SelectionStyle()->PlaneAspect();
    m_ais_context->SelectionStyle()->SetTransparency(0.5);

    m_draw_style = DisplayStyle::BallAndStick;

    setAttribute(Qt::WA_PaintOnScreen);
    setAttribute(Qt::WA_NoSystemBackground);
    setAttribute(Qt::WA_StyledBackground);
    setBackgroundRole(QPalette::NoRole);
    setFocusPolicy(Qt::StrongFocus);
    setMouseTracking(true);
    this->update();
}

OccView::~OccView() {
}

void OccView::paintEvent(QPaintEvent* event) {
    event->accept();
    m_v3d_view->InvalidateImmediate();
    FlushViewEvents(m_ais_context, m_v3d_view, true);
}

void OccView::resizeEvent(QResizeEvent* event) {
    event->accept();
    if(false == m_v3d_view.IsNull()) {
        m_v3d_view->MustBeResized();
    }
}

void OccView::mousePressEvent(QMouseEvent* event) {
    event->accept();
    if (true == m_v3d_view.IsNull()) {
        return;
    }

    Graphic3d_Vec2i position;
    position.SetValues(event->pos().x(), event->pos().y());

    auto event_modifiers = event->modifiers();
    Aspect_VKeyFlags vkey_flags = Aspect_VKeyFlags_NONE;
    switch (event_modifiers) {
        case Qt::ShiftModifier:
            vkey_flags = Aspect_VKeyFlags_SHIFT;
            break;
        case Qt::ControlModifier:
            vkey_flags = Aspect_VKeyFlags_CTRL;
            break;
        case Qt::AltModifier:
            vkey_flags = Aspect_VKeyFlags_ALT;
            break;
        case Qt::MetaModifier:
            vkey_flags = Aspect_VKeyFlags_META;
            break;
        default:
            break;
    }

    auto mouse_button = event->buttons();
    Aspect_VKeyMouse vkey_mouse = Aspect_VKeyMouse_NONE;
    if (Qt::LeftButton == mouse_button) {
        vkey_mouse = Aspect_VKeyMouse_LeftButton;
    } else if (Qt::MiddleButton == mouse_button) {
        vkey_mouse = Aspect_VKeyMouse_MiddleButton;
    } else if (Qt::RightButton == mouse_button) {
        vkey_mouse = Aspect_VKeyMouse_RightButton;
    }

    m_mouse_click_pos = position;

    if (UpdateMouseButtons(position, vkey_mouse, vkey_flags, false)) {
        this->update();
    }
}

void OccView::mouseReleaseEvent(QMouseEvent* event) {
    event->accept();
    if (true == m_v3d_view.IsNull()) {
        return;
    }

    Graphic3d_Vec2i position;
    position.SetValues(event->pos().x(), event->pos().y());

    auto event_modifiers = event->modifiers();
    Aspect_VKeyFlags vkey_flags = Aspect_VKeyFlags_NONE;
    switch (event_modifiers) {
        case Qt::ShiftModifier:
            vkey_flags = Aspect_VKeyFlags_SHIFT;
            break;
        case Qt::ControlModifier:
            vkey_flags = Aspect_VKeyFlags_CTRL;
            break;
        case Qt::AltModifier:
            vkey_flags = Aspect_VKeyFlags_ALT;
            break;
        case Qt::MetaModifier:
            vkey_flags = Aspect_VKeyFlags_META;
            break;
        default:
            break;
    }

    auto mouse_button = event->buttons();
    Aspect_VKeyMouse vkey_mouse = Aspect_VKeyMouse_NONE;
    if (Qt::LeftButton == mouse_button) {
        vkey_mouse = Aspect_VKeyMouse_LeftButton;
    } else if (Qt::MiddleButton == mouse_button) {
        vkey_mouse = Aspect_VKeyMouse_MiddleButton;
    } else if (Qt::RightButton == mouse_button) {
        vkey_mouse = Aspect_VKeyMouse_RightButton;
    }

    if (UpdateMouseButtons(position, vkey_mouse, vkey_flags, false)) {
        this->update();
    }

    if (Qt::RightButton == event->button()) {
        auto context_menu = new QMenu{this};
        auto action = new QAction("Fit View", context_menu);
        action->setToolTip("Fit vie to all objects");
        QObject::connect(action, &QAction::triggered, this, [&](){
            FitAllAuto(m_ais_context, m_v3d_view);
        });
        context_menu->addAction(action);
        context_menu->addSeparator();

        auto view_menu = context_menu->addMenu("Views");

        action = new QAction("X", this);
        view_menu->addAction(action);
        action->setToolTip("X");
        QObject::connect(action, &QAction::triggered, this, [&](){
            m_v3d_view->SetProj(V3d_Xpos);
            FitAllAuto(m_ais_context, m_v3d_view);
        });

        action = new QAction("Y", this);
        view_menu->addAction(action);
        action->setToolTip("Y");
        QObject::connect(action, &QAction::triggered, this, [&]() {
            m_v3d_view->SetProj(V3d_Ypos);
            FitAllAuto(m_ais_context, m_v3d_view);
        });

        action = new QAction("Z", this);
        view_menu->addAction(action);
        action->setToolTip("Z");
        QObject::connect(action, &QAction::triggered, this, [&]() {
            m_v3d_view->SetProj(V3d_Zpos);
            FitAllAuto(m_ais_context, m_v3d_view);
        });

        auto style_menu = context_menu->addMenu("Style");

        auto ball_and_stick_style = new QAction("Ball and Stick");
        style_menu->addAction(ball_and_stick_style);
        ball_and_stick_style->setToolTip("Ball and Stick");
        QObject::connect(ball_and_stick_style, &QAction::triggered, this, &OccView::set_ball_and_stick_style);
        ball_and_stick_style->setCheckable(true);

        auto van_der_waals_style = new QAction("Van der Waals", this);
        style_menu->addAction(van_der_waals_style);
        van_der_waals_style->setToolTip("Van der Waals");
        QObject::connect(van_der_waals_style, &QAction::triggered, this, &OccView::set_van_der_waals_style);
        van_der_waals_style->setCheckable(true);

        auto stick_style = new QAction("Stick", this);
        style_menu->addAction(stick_style);
        stick_style->setToolTip("Stick");
        QObject::connect(stick_style, &QAction::triggered, this, &OccView::set_stick_style);
        stick_style->setCheckable(true);

        switch(m_draw_style) {
            case DisplayStyle::BallAndStick:
                ball_and_stick_style->setChecked(true);
                break;
            case DisplayStyle::VanDerWaals:
                van_der_waals_style->setChecked(true);
                break;
            case DisplayStyle::Stick:
                stick_style->setChecked(true);
                break;
            default:
                break;
        }

        context_menu->exec(QCursor::pos());
        delete context_menu;
    }
}

void OccView::mouseMoveEvent(QMouseEvent* event) {
    event->accept();
    if (true == m_v3d_view.IsNull()) {
        return;
    }

    Graphic3d_Vec2i position;
    position.SetValues(event->pos().x(), event->pos().y());

    auto event_modifiers = event->modifiers();
    Aspect_VKeyFlags vkey_flags = Aspect_VKeyFlags_NONE;
    switch (event_modifiers) {
        case Qt::ShiftModifier:
            vkey_flags = Aspect_VKeyFlags_SHIFT;
            break;
        case Qt::ControlModifier:
            vkey_flags = Aspect_VKeyFlags_CTRL;
            break;
        case Qt::AltModifier:
            vkey_flags = Aspect_VKeyFlags_ALT;
            break;
        case Qt::MetaModifier:
            vkey_flags = Aspect_VKeyFlags_META;
            break;
        default:
            break;
    }

    auto mouse_button = event->buttons();
    Aspect_VKeyMouse vkey_mouse = Aspect_VKeyMouse_NONE;
    if (Qt::LeftButton == mouse_button) {
        vkey_mouse = Aspect_VKeyMouse_LeftButton;
    } else if (Qt::MiddleButton == mouse_button) {
        vkey_mouse = Aspect_VKeyMouse_MiddleButton;
    } else if (Qt::RightButton == mouse_button) {
        vkey_mouse = Aspect_VKeyMouse_RightButton;
    }

    if (UpdateMousePosition(position, vkey_mouse, vkey_flags, false)) {
        this->update();
    }
}

void OccView::wheelEvent(QWheelEvent* event) {
    event->accept();
    if (true == m_v3d_view.IsNull()) {
        return;
    }

    Graphic3d_Vec2i position;
    position.SetValues(event->position().x(), event->position().y());
    int delta_pixels = event->pixelDelta().y();
    int delta_degrees = event->angleDelta().y() / 8.0;
    Standard_Real delta = delta_pixels != 0 ? delta_pixels : (delta_degrees != 0 ? (delta_degrees / 15) : 0);
    if (UpdateZoom(Aspect_ScrollDelta(position, delta))) {
        this->update();
    }
}

void OccView::set_ball_and_stick_style() {
    //TODO: improve Ball & Stick style displaying of structure
    m_ais_context->SetDisplayMode(AIS_Shaded, Standard_True);
    m_v3d_view->SetComputedMode(false);
    m_v3d_view->Redraw();
    QApplication::setOverrideCursor(Qt::WaitCursor);
    QApplication::restoreOverrideCursor();
    m_draw_style = DisplayStyle::BallAndStick;
    return;
}

void OccView::set_van_der_waals_style() {
    //TODO: set Van der Waals style displaying of structure
    m_draw_style = DisplayStyle::VanDerWaals;
    return;
}

void OccView::set_stick_style() {
    //TODO: set Stick style displaying of structure
    m_draw_style = DisplayStyle::Stick;
    return;
}
