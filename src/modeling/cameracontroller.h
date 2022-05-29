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

/// Atoms3DCameraController is a customized Qt3D camera controlelr
/// it's actually a Trackball or Arcball camera controller
///
/// For more knowledge about Object Mouse Trackball,
/// see https://www.khronos.org/opengl/wiki/Object_Mouse_Trackball
///
/// For information about implementation of an CUstom Camera Controller, refer to:
/// https://code.qt.io/cgit/qt/qt3d.git/tree/src/extras/defaults/qabstractcameracontroller.cpp
/// https://code.qt.io/cgit/qt/qt3d.git/tree/src/extras/defaults/qorbitcameracontroller.cpp

#ifndef MODELING_CAMERACONTROLLER_H
#define MODELING_CAMERACONTROLLER_H

#include <Qt3DExtras/QAbstractCameraController>
#include <Qt3DCore/QTransform>
#include <QPoint>
#include <QSize>
#include <cmath>

class Atoms3DCameraController : public Qt3DExtras::QAbstractCameraController {

Q_OBJECT
Q_PROPERTY(QSize window_size MEMBER m_window_size READ get_window_size WRITE set_window_size NOTIFY window_size_changed)
Q_PROPERTY(double trackball_radius MEMBER m_trackball_radius READ get_trackball_radius WRITE set_trackball_radius NOTIFY trackball_radius_changed)
Q_PROPERTY(double rotation_speed MEMBER m_rotation_speed READ get_rotation_speed WRITE set_rotation_speed NOTIFY rotation_speed_changed)

public:
    explicit Atoms3DCameraController(Qt3DCore::QNode* parent = nullptr);
    ~Atoms3DCameraController() = default;

    QSize get_window_size() const;
    double get_trackball_radius() const;
    double get_rotation_speed() const;

    void set_trackball_center(double x, double y);

public slots:
    void set_window_size(QSize window_size);
    void set_trackball_radius(double trackball_radius);
    void set_rotation_speed(double rotation_speed);

signals:
    void window_size_changed(QSize window_size);
    void trackball_radius_changed(double trackball_radius);
    void rotation_speed_changed(double rotation_speed);

protected:
    void moveCamera(const Qt3DExtras::QAbstractCameraController::InputState& state, float dt) override;

private:
    QPoint m_mouse_last_position;
    QPoint m_mouse_current_position;
    QSize m_window_size;
    /**
     * @brief m_trackball_center
     * The lower left corner is the origin (0,0).
     * And the coordinate of upper right ocrner is (1.0, 1.0).
     * by default m_trackball_center is (0.5, 0.5), which is
     * the center of the 3D screen window.
     */
    QVector2D m_trackball_center;
    double m_trackball_radius;
    double m_rotation_speed;
};

inline QSize Atoms3DCameraController::get_window_size() const {
    return m_window_size;
}

inline double Atoms3DCameraController::get_trackball_radius() const {
    return m_trackball_radius;
}

inline double Atoms3DCameraController::get_rotation_speed() const {
    return m_rotation_speed;
}

inline void Atoms3DCameraController::set_window_size(QSize window_size) {
    if (m_window_size == window_size) {
        return;
    } else {
        m_window_size = window_size;
    }
    emit window_size_changed(m_window_size);
}

inline void Atoms3DCameraController::set_trackball_radius(double trackball_radius) {
    if (std::abs(m_trackball_radius - trackball_radius) < 1.0E-6) {
        return;
    } else {
        m_trackball_radius = trackball_radius;
    }
    emit trackball_radius_changed(m_trackball_radius);
}

inline void Atoms3DCameraController::set_rotation_speed(double rotation_speed) {
    if (std::abs(m_rotation_speed - rotation_speed) < 1.0E-6) {
        return;
    } else {
        m_rotation_speed = rotation_speed;
    }
    emit rotation_speed_changed(m_rotation_speed);
}

/**
 * @brief Atoms3DCameraController::set_trackball_center
 * @param x within range of (0, 1)
 * @param y within range of (0, 1)
 */
inline void Atoms3DCameraController::set_trackball_center(double x, double y) {
    this->m_trackball_center[0] = x > 0 ? (x < 1 ? x : 1) : 0;
    this->m_trackball_center[1] = y > 0 ? (y < 1 ? y : 1) : 0;
}

#endif // MODELING_CAMERACONTROLLER_H
