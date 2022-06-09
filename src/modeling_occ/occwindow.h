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

#ifndef MODELING_OCCTWINDOW_H
#define MODELING_OCCTWINDOW_H

#include <QWidget>

#include <Aspect_Window.hxx>

class OccWindow : public Aspect_Window {
    DEFINE_STANDARD_RTTIEXT(OccWindow, Aspect_Window)
public:

    OccWindow(QWidget* parent = nullptr);

    ~OccWindow() {
        m_widget = nullptr;
    }

    Standard_EXPORT virtual Aspect_Drawable NativeHandle() const override;
    Standard_EXPORT virtual Aspect_Drawable NativeParentHandle() const override;
    Standard_EXPORT virtual Aspect_TypeOfResize DoResize() override;
    Standard_EXPORT virtual Standard_Boolean IsMapped() const override;
    Standard_EXPORT virtual Standard_Boolean DoMapping() const override;
    Standard_EXPORT virtual void Map() const override;
    Standard_EXPORT virtual void Unmap() const override;
    Standard_EXPORT virtual void Position(
        Standard_Integer& x1,
        Standard_Integer& y1,
        Standard_Integer& x2,
        Standard_Integer& y2
    ) const override;
    Standard_EXPORT virtual Standard_Real Ratio() const override;
    Standard_EXPORT virtual void Size(
        Standard_Integer& width,
        Standard_Integer& height
    ) const override;
    Standard_EXPORT virtual Aspect_FBConfig NativeFBConfig() const Standard_OVERRIDE {
        return NULL;
    }

protected:
    Standard_Integer m_x_left;
    Standard_Integer m_y_top;
    Standard_Integer m_x_right;
    Standard_Integer m_y_bottom;
    QWidget* m_widget;
};

DEFINE_STANDARD_HANDLE(OccWindow, Aspect_Window)

#endif // MODELING_OCCTWINDOW_H
