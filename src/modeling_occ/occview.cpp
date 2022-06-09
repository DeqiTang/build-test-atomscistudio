#include "occview.h"

#include <QApplication>
#include <QColorDialog>
#include <QFileDialog>
#include <QObject>

Aspect_VKeyMouse map_qt_mouse_buttons_2_vkeys(Qt::MouseButtons buttons) {
    Aspect_VKeyMouse button = Aspect_VKeyMouse_NONE;
    if ((buttons & Qt::LeftButton) != 0) {
        button |= Aspect_VKeyMouse_LeftButton;
    }
    if ((buttons & Qt::MiddleButton) != 0) {
        button |= Aspect_VKeyMouse_MiddleButton;
    }
    if ((buttons & Qt::RightButton) != 0) {
        button |= Aspect_VKeyMouse_RightButton;
    }
    return button;
}

Aspect_VKeyFlags map_qt_mouse_modifiers_2_vkeys(Qt::KeyboardModifiers modifiers) {
    Aspect_VKeyFlags vkeys = Aspect_VKeyFlags_NONE;
    if ((modifiers & Qt::ShiftModifier) != 0) {
        vkeys |= Aspect_VKeyFlags_SHIFT;
    }
    if ((modifiers & Qt::ControlModifier) != 0) {
        vkeys |= Aspect_VKeyFlags_CTRL;
    }
    if ((modifiers & Qt::AltModifier) != 0) {
        vkeys |= Aspect_VKeyFlags_ALT;
    }
    return vkeys;
}

OccView::OccView(QWidget *parent) : QWidget(parent), m_device_px(devicePixelRatio()) {

    m_cur_mode = MouseGesture::Nothing;

    setFocusPolicy(Qt::StrongFocus);
    setAttribute(Qt::WA_PaintOnScreen);
    setMouseTracking(true);

    m_display_connection = new Aspect_DisplayConnection();
    m_graphic_driver = new OpenGl_GraphicDriver(m_display_connection);
    m_v3d_viewer = new V3d_Viewer{m_graphic_driver};

    m_context = new AIS_InteractiveContext(m_v3d_viewer);
    m_context->SetDisplayMode(AIS_Shaded, Standard_True);

    m_draw_style =  DisplayMode::Shaded;

    if (m_v3d_view.IsNull()) {
        m_v3d_view = m_context->CurrentViewer()->CreateView();
    }

    m_occwindow = new OccWindow{this};
    m_v3d_view->SetWindow(m_occwindow);

    if (!m_occwindow->IsMapped()) {
        m_occwindow->Map();
    }

    m_v3d_view->MustBeResized();
    m_v3d_viewer->SetDefaultLights();
    m_v3d_viewer->SetLightOn();
    m_v3d_view->SetBackgroundColor(Quantity_Color(
        0.5, 0.5, 0.5,
        Quantity_TOC_sRGB
    ));
    m_context->SelectionStyle()->SetColor(Quantity_NOC_RED);
    m_context->SelectionStyle()->SetDisplayMode(AIS_Shaded);

    m_v3d_view->TriedronDisplay(Aspect_TOTP_LEFT_LOWER, Quantity_NOC_GOLD, this->devicePixelRatio() * 0.1, V3d_ZBUFFER);

    if (m_is_raytracing) {
        m_v3d_view->ChangeRenderingParams().Method = Graphic3d_RM_RAYTRACING;
    }

    this->update();
}

OccView::~OccView() {

}

QPaintEngine* OccView::paintEngine() const {
    return nullptr;
}

void OccView::paintEvent(QPaintEvent* event ) {
    m_v3d_view->InvalidateImmediate();
    FlushViewEvents(m_context, m_v3d_view, true);
}

void OccView::resizeEvent(QResizeEvent* event) {
    if(!m_v3d_view.IsNull()) {
        m_v3d_view->MustBeResized();
    }
}

void OccView::OnSelectionChanged(
    const Handle(AIS_InteractiveContext)& context,
    const Handle(V3d_View)& view) {

    emit selection_changed();
}

void OccView::mousePressEvent(QMouseEvent* event) {
    Graphic3d_Vec2i point;
    point.SetValues(
        m_device_px * event->pos().x(),
        m_device_px * event->pos().y()
    );
    const Aspect_VKeyFlags flags = map_qt_mouse_modifiers_2_vkeys(event->modifiers());
    if (!m_v3d_view.IsNull() && UpdateMouseButtons(point, map_qt_mouse_buttons_2_vkeys(event->buttons()), flags, false)) {
        this->update();
    }
    m_click_pos = point;
}

void OccView::mouseReleaseEvent(QMouseEvent* event) {
    Graphic3d_Vec2i point;
    point.SetValues(
        m_device_px * event->pos().x(),
        m_device_px * event->pos().y()
    );
    const Aspect_VKeyFlags flags = map_qt_mouse_modifiers_2_vkeys(event->modifiers());
    if (!m_v3d_view.IsNull() && UpdateMouseButtons(point, map_qt_mouse_buttons_2_vkeys(event->buttons()), flags, false)) {
        this->update();
    }
    if (m_cur_mode == MouseGesture::GlobalPanning) {
        m_v3d_view->Place(point.x(), point.y(), m_cur_zoom);
    }
    if (m_cur_mode != MouseGesture::Nothing) {
        bind_mouse_gestures(MouseGesture::Nothing);
    }
    if (event->button() == Qt::RightButton && (flags & Aspect_VKeyFlags_CTRL) == 0 && (m_click_pos - point).cwiseAbs().maxComp() <= 4) {
        if (m_context->NbSelected()) { // if any object is selected
            QMenu* tool_menu = new QMenu{nullptr};
            tool_menu->exec(QCursor::pos());
            delete tool_menu;
        } else {
            auto context_menu = new QMenu{this};
            auto action = new QAction("Fit View", context_menu);
            action->setToolTip("Fit view to all objects");
            QObject::connect(action, &QAction::triggered, this, [&](){
                m_v3d_view->FitAll();
                m_v3d_view->ZFitAll();
                m_v3d_view->Redraw();
            });
            context_menu->addAction(action);
            context_menu->addSeparator();

            auto view_menu = context_menu->addMenu("Views");
            action = new QAction("Front", this);
            action->setToolTip("Front view");
            connect(action, &QAction::triggered, this, [&](){
                m_v3d_view->SetProj(V3d_Yneg);
                m_v3d_view->FitAll();
            });
            view_menu->addAction(action);
            action = new QAction("Back", this);
            action->setToolTip("View from back");
            connect(action, &QAction::triggered, this, [&]() {
                m_v3d_view->SetProj(V3d_Ypos);
                m_v3d_view->FitAll();
            });
            view_menu->addAction(action);

            action = new QAction("Left", this);
            action->setToolTip("View from left");
            connect(action, &QAction::triggered, this, [&]() {
                m_v3d_view->SetProj(V3d_Xneg);
                m_v3d_view->FitAll();
            });
            view_menu->addAction(action);
            action = new QAction("Right", this);
            action->setToolTip("Right view");
            connect(action, &QAction::triggered, this, [&]() {
                m_v3d_view->SetProj(V3d_Xpos);
                m_v3d_view->FitAll();
            });
            view_menu->addAction(action);

            action = new QAction("Top", this);
            action->setToolTip("Top view");
            connect(action, &QAction::triggered, this, [&]() {
                m_v3d_view->SetProj(V3d_Zpos);
                m_v3d_view->FitAll();
            });
            view_menu->addAction(action);

            action = new QAction("Bottom", this);
            action->setToolTip("Bottom view");
            connect(action, &QAction::triggered, this, [&]() {
                m_v3d_view->SetProj(V3d_Zneg);
                m_v3d_view->FitAll();
            });
            view_menu->addAction(action);

            auto style_menu = context_menu->addMenu("Style");
            auto wireframe = new QAction("Wireframe", this );
            wireframe->setToolTip("Wireframe style");
            connect(wireframe, &QAction::triggered, this, [&]() {
                QApplication::setOverrideCursor(Qt::WaitCursor);
                m_v3d_view->SetComputedMode(false);
                m_context->SetDisplayMode(AIS_Shaded, Standard_False);
                m_context->SetDisplayMode(AIS_WireFrame, Standard_True);
                m_draw_style =  DisplayMode::WireFrame;
                m_v3d_view->Redraw();
                QApplication::restoreOverrideCursor();
            });
            wireframe->setCheckable(true);

            auto shaded = new QAction("Shaded", this );
            shaded->setToolTip("Shaded style");
            connect(shaded, &QAction::triggered, this, [&]() {
                QApplication::setOverrideCursor(Qt::WaitCursor);
                m_context->SetDisplayMode(AIS_Shaded, Standard_True);
                m_v3d_view->SetComputedMode(false);
                m_v3d_view->Redraw();
                m_draw_style =  DisplayMode::Shaded;
                QApplication::restoreOverrideCursor();
            });
            shaded->setCheckable(true);

            switch(m_draw_style) {
                case  DisplayMode::WireFrame:
                    wireframe->setChecked(true);
                    break;
                case  DisplayMode::Shaded:
                    shaded->setChecked(true);
                    break;
            }

            style_menu->addAction(wireframe);
            style_menu->addAction(shaded);

            context_menu->exec(QCursor::pos());
        }
    }
}

void OccView::mouseMoveEvent(QMouseEvent* event) {
    Graphic3d_Vec2i point;
    point.SetValues(m_device_px * event->pos().x(), m_device_px * event->pos().y());
    if (
            !m_v3d_view.IsNull()
            && UpdateMousePosition(
                point,
                map_qt_mouse_buttons_2_vkeys(event->buttons()),
                map_qt_mouse_modifiers_2_vkeys(event->modifiers()),
                false
            )
        ) {
        this->update();
    }
}

void OccView::wheelEvent(QWheelEvent* event) {
    Graphic3d_Vec2i pos;
    pos.SetValues(event->position().x(), event->position().y());
    int num_pixels = event->pixelDelta().y();
    int num_degrees = event->angleDelta().y() / 8;
    Standard_Real delta{0.0};
    if (num_pixels != 0) {
        delta = num_pixels;
    } else if (num_degrees != 0) {
        delta = num_degrees / 15;
    }
    if (!m_v3d_view.IsNull() && UpdateZoom(Aspect_ScrollDelta(pos, delta))) {
        this->update();
    }
}

void OccView::bind_mouse_gestures(MouseGesture mode) {
    m_cur_mode = mode;
    myMouseGestureMap.Clear();

    switch(m_cur_mode) {
//        case MouseGesture::Nothing:
//            myMouseGestureMap = m_mouse_default_gestures;
//            break;
        case MouseGesture::Zoom:
            myMouseGestureMap.Bind(Aspect_VKeyMouse_LeftButton, AIS_MouseGesture_Zoom);
            break;
        case MouseGesture::GlobalPanning:
            break;
        case MouseGesture::ZoomWindow:
            myMouseGestureMap.Bind(Aspect_VKeyMouse_LeftButton, AIS_MouseGesture_ZoomWindow);
            break;
        case MouseGesture::Pan:
            myMouseGestureMap.Bind(Aspect_VKeyMouse_LeftButton, AIS_MouseGesture_Pan);
            break;
        case MouseGesture::RotateOrbit:
            myMouseGestureMap.Bind(Aspect_VKeyMouse_LeftButton, AIS_MouseGesture_RotateOrbit);
            break;
        case MouseGesture::SelectRectangle:
            myMouseGestureMap.Bind(Aspect_VKeyMouse_LeftButton, AIS_MouseGesture_SelectRectangle);
            break;
    }
}
