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

#ifndef ATOMS3D_CAMERACONTROLLER_H
#define ATOMS3D_CAMERACONTROLLER_H

#include <Qt3DExtras/QAbstractCameraController>
#include <Qt3DCore/QTransform>
#include <QPoint>
#include <QSize>
#include <cmath>

class Atoms3DCameraController : public Qt3DExtras::QAbstractCameraController {
Q_OBJECT

Q_PROPERTY(QSize window_size READ window_size WRITE set_window_size NOTIFY window_size_changed)
Q_PROPERTY(float trackball_size READ trackball_size WRITE set_trackball_size NOTIFY trackball_size_changed)
Q_PROPERTY(float rotation_speed READ rotation_speed WRITE set_rotation_speed NOTIFY rotation_speed_changed)

public:

    explicit Atoms3DCameraController(Qt3DCore::QNode* parent = nullptr);
    ~Atoms3DCameraController() = default;

    QSize window_size() const;
    float trackball_size() const;
    float rotation_speed() const;

protected:

public slots:

    void set_window_size(QSize window_size);
    void set_trackball_size(float trackball_size);
    void set_rotation_speed(float rotation_speed);

signals:
    void window_size_changed(QSize window_size);
    void trackball_size_changed(float trackball_size);
    void rotation_speed_changed(float rotation_speed);

protected:

    void moveCamera(const Qt3DExtras::QAbstractCameraController::InputState& state, float dt) override;
    QVector3D project_to_trackball(const QPoint& screenCoords) const;
    void create_rotation(const QPoint& firstPoint, const QPoint& nextPoint, QVector3D& dir, float& angle);

private:

    QPoint m_mouse_last_position;
    QPoint m_mouse_current_position;
    QSize m_window_size;
    float m_trackball_radius;
    float m_rotation_speed;
    float m_trackball_size;
};

inline QSize Atoms3DCameraController::window_size() const {
    return m_window_size;
}

inline float Atoms3DCameraController::trackball_size() const {
    return m_trackball_size;
}

inline float Atoms3DCameraController::rotation_speed() const {
    return m_rotation_speed;
}

inline void Atoms3DCameraController::set_window_size(QSize window_size) {
    if (m_window_size == window_size) {
        return;
    }
    m_window_size = window_size;
    emit window_size_changed(m_window_size);
}

inline void Atoms3DCameraController::set_trackball_size(float trackball_size) {
    if (qFuzzyCompare(m_trackball_size, trackball_size)) {
        return;
    }
    m_trackball_size = trackball_size;
    emit trackball_size_changed(m_trackball_size);
}

inline void Atoms3DCameraController::set_rotation_speed(float rotation_speed) {
    if (std::abs(m_rotation_speed - rotation_speed) < 1.0E-6) {
        return;
    }
    m_rotation_speed = rotation_speed;
    emit rotation_speed_changed(m_rotation_speed);
}

#endif // CAMERACONTROLLER_H
