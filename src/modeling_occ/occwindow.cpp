#include "occwindow.h"

OccWindow::OccWindow(QWidget* parent): m_widget{parent} {

    m_x_left = m_widget->rect().left();
    m_y_top = m_widget->rect().top();
    m_x_right = m_widget->rect().right();
    m_y_bottom = m_widget->rect().bottom();

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
    int mask{0};
    auto mode = Aspect_TOR_UNKNOWN;

    if (!m_widget->isMinimized()) {
        if (Abs(m_widget->rect().left() - m_x_left) > 2.) {
            mask |= 1;
        }
        if (Abs(m_widget->rect().right() - m_x_right) > 2.) {
            mask |= 2;
        }
        if (Abs(m_widget->rect().top() - m_y_top) > 2.) {
            mask |= 4;
        }
        if (Abs(m_widget->rect().bottom() - m_y_bottom) > 2.) {
            mask |= 8;
        }

        std::cout << "OccWindow::DoResize -> mask -> " << mask << "\n";

        switch (mask) {
            case 0:
                mode = Aspect_TOR_NO_BORDER;
                break;
            case 1:
                mode = Aspect_TOR_LEFT_BORDER;
                break;
            case 2:
                mode = Aspect_TOR_RIGHT_BORDER;
                break;
            case 4:
                mode = Aspect_TOR_TOP_BORDER;
                break;
            case 5:
                mode = Aspect_TOR_LEFT_AND_TOP_BORDER;
                break;
            case 6:
                mode = Aspect_TOR_TOP_AND_RIGHT_BORDER;
                break;
            case 8:
                mode = Aspect_TOR_BOTTOM_BORDER;
                break;
            case 9:
                mode = Aspect_TOR_BOTTOM_AND_LEFT_BORDER;
                break;
            case 10:
                mode = Aspect_TOR_RIGHT_AND_BOTTOM_BORDER;
                break;
            default:
                break;
        }
        m_x_left = m_widget->rect().left();
        m_x_right = m_widget->rect().right();
        m_y_top = m_widget->rect().top();
        m_y_bottom = m_widget->rect().bottom();
    }
    return mode;
}

Standard_Real OccWindow::Ratio() const {
    auto rect = m_widget->rect();
    return Abs(Standard_Real(rect.right() - rect.left()) / Standard_Real(rect.top() - rect.bottom()));
}

void OccWindow::Size(Standard_Integer &width, Standard_Integer &height) const {
    width = m_widget->rect().right();
    height = m_widget->rect().bottom();
}

void OccWindow::Position(
    Standard_Integer &x1,
    Standard_Integer &y1,
    Standard_Integer &x2,
    Standard_Integer &y2) const {

    x1 = m_widget->rect().left();
    x2 = m_widget->rect().right();
    y1 = m_widget->rect().top();
    y2 = m_widget->rect().bottom();
}

IMPLEMENT_STANDARD_RTTIEXT(OccWindow, Aspect_Window)
