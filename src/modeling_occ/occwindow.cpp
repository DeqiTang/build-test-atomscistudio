#include "occwindow.h"

OccWindow::OccWindow(QWidget* parent): m_widget{parent} {

    auto rect = m_widget->rect();
    m_x_left = rect.left();
    m_y_top = rect.top();
    m_x_right = rect.right();
    m_y_bottom = rect.bottom();

    this->DoResize();
}

Aspect_Drawable OccWindow::NativeParentHandle() const {
    auto parent = m_widget->parentWidget();
    if (parent != NULL) {
        return (Aspect_Drawable)parent->winId();
    } else {
        return 0;
    }
}

Aspect_Drawable OccWindow::NativeHandle() const {
    return (Aspect_Drawable)m_widget->winId();
}

Standard_Boolean OccWindow::IsMapped() const {
    if (m_widget->isMinimized() || m_widget->isHidden()) {
        return Standard_False;
    } else {
        return Standard_True;
    }
}

Standard_Boolean OccWindow::DoMapping() const {
    if (IsMapped()) {
        return Standard_True;
    } else {
        return Standard_False;
    }
}

void OccWindow::Map() const {
    if (IsVirtual()) {
        return;
    } else {
        m_widget->show();
        m_widget->update();
    }
}

void OccWindow::Unmap() const {
    if (IsVirtual()) {
        return;
    } else {
        m_widget->hide();
        m_widget->update();
    }
}

Aspect_TypeOfResize OccWindow::DoResize() {
    int bit_mask = 0;
    auto type_of_resize = Aspect_TOR_UNKNOWN;

    auto rect = m_widget->rect();

    if (!m_widget->isMinimized()) {
        if (Abs(rect.left() - m_x_left) > 2.) {
            bit_mask |= 1;
        }
        if (Abs(rect.right() - m_x_right) > 2.) {
            bit_mask |= 2;
        }
        if (Abs(rect.top() - m_y_top) > 2.) {
            bit_mask |= 4;
        }
        if (Abs(rect.bottom() - m_y_bottom) > 2.) {
            bit_mask |= 8;
        }

        switch (bit_mask) {
            case 0:
                type_of_resize = Aspect_TOR_NO_BORDER;
                break;
            case 1:
                type_of_resize = Aspect_TOR_LEFT_BORDER;
                break;
            case 2:
                type_of_resize = Aspect_TOR_RIGHT_BORDER;
                break;
            case 4:
                type_of_resize = Aspect_TOR_TOP_BORDER;
                break;
            case 5:
                type_of_resize = Aspect_TOR_LEFT_AND_TOP_BORDER;
                break;
            case 6:
                type_of_resize = Aspect_TOR_TOP_AND_RIGHT_BORDER;
                break;
            case 8:
                type_of_resize = Aspect_TOR_BOTTOM_BORDER;
                break;
            case 9:
                type_of_resize = Aspect_TOR_BOTTOM_AND_LEFT_BORDER;
                break;
            case 10:
                type_of_resize = Aspect_TOR_RIGHT_AND_BOTTOM_BORDER;
                break;
            default:
                break;
        }
        m_x_left = rect.left();
        m_x_right = rect.right();
        m_y_top = rect.top();
        m_y_bottom = rect.bottom();
    }
    return type_of_resize;
}

Standard_Real OccWindow::Ratio() const {
    auto rect = m_widget->rect();
    return Abs(Standard_Real(rect.right() - rect.left()) / Standard_Real(rect.top() - rect.bottom()));
}

void OccWindow::Size(Standard_Integer& width, Standard_Integer& height) const {
//    width = Abs(m_widget->rect().right());
//    height = Abs(m_widget->rect().bottom());
    auto rect = m_widget->rect();
    width = Abs(rect.right() - rect.left());
    height = Abs(rect.bottom() - rect.top());
}

void OccWindow::Position(
    Standard_Integer &x1,
    Standard_Integer &y1,
    Standard_Integer &x2,
    Standard_Integer &y2) const {

    auto rect = m_widget->rect();
    x1 = rect.left();
    x2 = rect.right();
    y1 = rect.top();
    y2 = rect.bottom();
}

IMPLEMENT_STANDARD_RTTIEXT(OccWindow, Aspect_Window)
