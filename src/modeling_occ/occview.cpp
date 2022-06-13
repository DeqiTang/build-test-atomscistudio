#include "occview.h"

#include <QApplication>

#if defined(__linux__)
#include <Xw_Window.hxx>
#elif defined(__APPLE__)
#include <Cocoa_Window.hxx>
#elif defined(_WIN32)
#include <WNT_Window.hxx>
#endif

OccView::OccView(QWidget* parent) : QWidget(parent) {

    m_display_connection = new Aspect_DisplayConnection();
    m_graphic_driver = new OpenGl_GraphicDriver{m_display_connection};

    m_v3d_viewer = new V3d_Viewer{m_graphic_driver};
    m_v3d_view = m_v3d_viewer->CreateView();

    #if defined(__linux__)
    m_occwindow = new Xw_Window{m_display_connection, (Window)winId()};
    #elif defined(__APPLE__)
    m_occwindow = new Cocoa_Window{(NSView *)winId()};
    #elif defined(_WIN32)
    m_occwindow = new WNT_Window{(Aspect_Handle)winId()};
    #endif
    m_v3d_view->SetWindow(m_occwindow);

    if (!m_occwindow->IsMapped()) {
        m_occwindow->Map();
    }

    m_ais_context = new AIS_InteractiveContext(m_v3d_viewer);
    m_ais_context->SetDisplayMode(AIS_Shaded, Standard_True);

    m_draw_style = DisplayStyle::BallAndStick;
    m_v3d_viewer->SetDefaultLights();
    m_v3d_viewer->SetLightOn();
    m_v3d_view->SetBackgroundColor(Quantity_Color(
        0.5, 0.5, 0.5,
        Quantity_TOC_sRGB
    ));
    m_v3d_view->Camera()->SetProjectionType(Graphic3d_Camera::Projection_Orthographic);
    m_v3d_view->TriedronDisplay(Aspect_TOTP_LEFT_LOWER, Quantity_NOC_GOLD, this->devicePixelRatio() * 0.1, V3d_ZBUFFER);
    m_v3d_view->ChangeRenderingParams().RenderResolutionScale = 1.0f;

    m_ais_context->SelectionStyle()->SetColor(Quantity_NOC_RED);
    m_ais_context->SelectionStyle()->SetDisplayMode(AIS_Shaded);
    m_ais_context->SetDisplayMode(AIS_Shaded, Standard_True);

    m_v3d_view->TriedronDisplay(Aspect_TOTP_LEFT_LOWER, Quantity_NOC_GOLD, this->devicePixelRatio() * 0.1, V3d_ZBUFFER);
    m_v3d_view->MustBeResized();
    this->update();

     setAttribute(Qt::WA_PaintOnScreen);
     setAttribute(Qt::WA_NoSystemBackground);
     setBackgroundRole(QPalette::NoRole);
     setFocusPolicy(Qt::StrongFocus);
     setMouseTracking(true);
}

OccView::~OccView() {

}

void OccView::paintEvent(QPaintEvent* event ) {
    m_v3d_view->InvalidateImmediate();
    FlushViewEvents(m_ais_context, m_v3d_view, true);
}

void OccView::resizeEvent(QResizeEvent* event) {
    if(!m_v3d_view.IsNull()) {
        m_v3d_view->MustBeResized();
    }
}

void OccView::mousePressEvent(QMouseEvent* event) {
    Graphic3d_Vec2i position;
    position.SetValues(
        event->pos().x(),
        event->pos().y()
    );
    Aspect_VKeyFlags vkey_flags = Aspect_VKeyFlags_NONE;
    if (event->modifiers() == Qt::ShiftModifier) {
        vkey_flags = Aspect_VKeyFlags_SHIFT;
    } else if (event->modifiers() == Qt::ControlModifier) {
        vkey_flags = Aspect_VKeyFlags_CTRL;
    } else if (event->modifiers() == Qt::AltModifier) {
        vkey_flags = Aspect_VKeyFlags_ALT;
    } else if (event->modifiers() & Qt::MetaModifier) {
        vkey_flags = Aspect_VKeyFlags_META;
    }

    Aspect_VKeyMouse vkey_mouse = Aspect_VKeyMouse_NONE;
    auto mouse_button = event->buttons();
    if (mouse_button == Qt::LeftButton) {
        vkey_mouse = Aspect_VKeyMouse_LeftButton;
    } else if (mouse_button == Qt::MiddleButton) {
        vkey_mouse = Aspect_VKeyMouse_MiddleButton;
    } else if (mouse_button == Qt::RightButton) {
        vkey_mouse = Aspect_VKeyMouse_RightButton;
    }

    if (!m_v3d_view.IsNull() && UpdateMouseButtons(position, vkey_mouse, vkey_flags, false)) {
        this->update();
    }
    m_click_pos = position;
}

void OccView::mouseReleaseEvent(QMouseEvent* event) {
    Graphic3d_Vec2i position;
    position.SetValues(
        event->pos().x(),
        event->pos().y()
    );
    Aspect_VKeyFlags vkey_flags = Aspect_VKeyFlags_NONE;
    if (event->modifiers() == Qt::ShiftModifier) {
        vkey_flags = Aspect_VKeyFlags_SHIFT;
    } else if (event->modifiers() == Qt::ControlModifier) {
        vkey_flags = Aspect_VKeyFlags_CTRL;
    } else if (event->modifiers() == Qt::AltModifier) {
        vkey_flags = Aspect_VKeyFlags_ALT;
    } else if (event->modifiers() & Qt::MetaModifier) {
        vkey_flags = Aspect_VKeyFlags_META;
    }

    Aspect_VKeyMouse vkey_mouse = Aspect_VKeyMouse_NONE;
    auto mouse_button = event->buttons();
    if (mouse_button == Qt::LeftButton) {
        vkey_mouse = Aspect_VKeyMouse_LeftButton;
    } else if (mouse_button == Qt::MiddleButton) {
        vkey_mouse = Aspect_VKeyMouse_MiddleButton;
    } else if (mouse_button == Qt::RightButton) {
        vkey_mouse = Aspect_VKeyMouse_RightButton;
    }

    if (!m_v3d_view.IsNull() && UpdateMouseButtons(position, vkey_mouse, vkey_flags, false)) {
        this->update();
    }

    if (event->button() == Qt::RightButton && (vkey_flags & Aspect_VKeyFlags_CTRL) == 0 && (m_click_pos - position).cwiseAbs().maxComp() <= 4) {
        if (m_ais_context->NbSelected() > 0) {
            QMenu* tool_menu = new QMenu{nullptr};
            tool_menu->exec(QCursor::pos());
            delete tool_menu;
        } else {
            auto context_menu = new QMenu{this};
            auto action = new QAction("Fit View", context_menu);
            action->setToolTip("Fit view to all objects");
            QObject::connect(action, &QAction::triggered, this, [&](){
                FitAllAuto(m_ais_context, m_v3d_view);
            });
            context_menu->addAction(action);
            context_menu->addSeparator();

            auto view_menu = context_menu->addMenu("Views");

            action = new QAction("X", this);
            view_menu->addAction(action);
            action->setToolTip("X");
            connect(action, &QAction::triggered, this, [&](){
                m_v3d_view->SetProj(V3d_Xpos);
                FitAllAuto(m_ais_context, m_v3d_view);
            });

            action = new QAction("Y", this);
            view_menu->addAction(action);
            action->setToolTip("Y");
            connect(action, &QAction::triggered, this, [&]() {
                m_v3d_view->SetProj(V3d_Ypos);
                FitAllAuto(m_ais_context, m_v3d_view);
            });

            action = new QAction("Z", this);
            view_menu->addAction(action);
            action->setToolTip("Z");
            connect(action, &QAction::triggered, this, [&]() {
                m_v3d_view->SetProj(V3d_Zpos);
                FitAllAuto(m_ais_context, m_v3d_view);
            });

            auto style_menu = context_menu->addMenu("Style");

            auto ball_and_stick = new QAction("Ball & Stick");
            style_menu->addAction(ball_and_stick);
            ball_and_stick->setToolTip("Ball & Stick");
            connect(ball_and_stick, &QAction::triggered, this, &OccView::set_ball_and_stick_style);
            ball_and_stick->setCheckable(true);

            auto van_der_waals = new QAction("Van der Waals", this);
            style_menu->addAction(van_der_waals);
            van_der_waals->setToolTip("Van der Waals");
            connect(van_der_waals, &QAction::triggered, this, &OccView::set_van_der_waals_style);
            van_der_waals->setCheckable(true);

            auto stick = new QAction("Stick", this);
            style_menu->addAction(stick);
            stick->setToolTip("Stick");
            connect(stick, &QAction::triggered, this, &OccView::set_stick_style);
            stick->setCheckable(true);

            switch(m_draw_style) {
                case DisplayStyle::BallAndStick:
                    ball_and_stick->setChecked(true);
                    break;
                case DisplayStyle::VanDerWaals:
                    van_der_waals->setChecked(true);
                    break;
                case DisplayStyle::Stick:
                    stick->setChecked(true);
                    break;
                default:
                    break;
            }
            context_menu->exec(QCursor::pos());
            delete context_menu;
        }
    }
}

void OccView::mouseMoveEvent(QMouseEvent* event) {
    Graphic3d_Vec2i position;
    position.SetValues(event->pos().x(), event->pos().y());
    Aspect_VKeyFlags vkey_flags = Aspect_VKeyFlags_NONE;
    if (event->modifiers() == Qt::ShiftModifier) {
        vkey_flags = Aspect_VKeyFlags_SHIFT;
    } else if (event->modifiers() == Qt::ControlModifier) {
        vkey_flags = Aspect_VKeyFlags_CTRL;
    } else if (event->modifiers() == Qt::AltModifier) {
        vkey_flags = Aspect_VKeyFlags_ALT;
    } else if (event->modifiers() & Qt::MetaModifier) {
        vkey_flags = Aspect_VKeyFlags_META;
    }

    Aspect_VKeyMouse vkey_mouse = Aspect_VKeyMouse_NONE;
    auto mouse_button = event->buttons();
    if (mouse_button == Qt::LeftButton) {
        vkey_mouse = Aspect_VKeyMouse_LeftButton;
    } else if (mouse_button == Qt::MiddleButton) {
        vkey_mouse = Aspect_VKeyMouse_MiddleButton;
    } else if (mouse_button == Qt::RightButton) {
        vkey_mouse = Aspect_VKeyMouse_RightButton;
    }

    if (!m_v3d_view.IsNull() &&
        UpdateMousePosition(
            position,
            vkey_mouse,
            vkey_flags,
            false
        )
    ) {
        this->update();
    }
}

void OccView::wheelEvent(QWheelEvent* event) {
    Graphic3d_Vec2i pos;
    pos.SetValues(event->position().x(), event->position().y());
    int delta_pixels = event->pixelDelta().y();
    int delta_degrees = event->angleDelta().y() / 8;
    Standard_Real delta{0.0};
    delta = delta_pixels != 0 ? delta_pixels : (delta_degrees != 0 ? (delta_degrees / 15) : 0);
    if (!m_v3d_view.IsNull() && UpdateZoom(Aspect_ScrollDelta(pos, delta))) {
        this->update();
    }
}

void OccView::set_ball_and_stick_style() {
    QApplication::setOverrideCursor(Qt::WaitCursor);
    m_ais_context->SetDisplayMode(AIS_Shaded, Standard_True);
    m_v3d_view->SetComputedMode(false);
    m_v3d_view->Redraw();
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
