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

#include "cameracontroller.h"

#include <Qt3DRender/QCamera>
#include <Qt3DInput/QMouseHandler>
#include <Qt3DInput/QMouseDevice>
#include <Qt3DCore/QTransform>
#include <QtMath>
#include <QVector2D>
#include <Qt3DInput/QAction>
#include <Qt3DInput/QActionInput>

Atoms3DCameraController::Atoms3DCameraController(Qt3DCore::QNode* parent)
    : Qt3DExtras::QAbstractCameraController(parent) {

    m_trackball_radius = 1.0;
    m_rotation_speed = 2.0;
    m_trackball_size = 1.0;

    Qt3DInput::QMouseHandler *mouse_handler = new Qt3DInput::QMouseHandler(this);
    mouse_handler->setSourceDevice(this->mouseDevice());
    this->mouseDevice()->setSensitivity(0.001);

    QObject::connect(mouse_handler, &Qt3DInput::QMouseHandler::pressed,
        [this](Qt3DInput::QMouseEvent *pressed_event) {
            pressed_event->setAccepted(true);
            m_mouse_last_position = QPoint(pressed_event->x(), pressed_event->y());
            m_mouse_current_position = m_mouse_last_position;
        }
    );
    QObject::connect(mouse_handler, &Qt3DInput::QMouseHandler::positionChanged,
        [this](Qt3DInput::QMouseEvent *position_changed_event) {
            position_changed_event->setAccepted(true);
            m_mouse_current_position = QPoint(
                position_changed_event->x(),
                position_changed_event->y()
            );
        }
    );
}

QVector3D Atoms3DCameraController::project_to_trackball(const QPoint& screen_coords) const {
    float sx = screen_coords.x(), sy = m_window_size.height() - screen_coords.y();

    QVector2D point_2d(sx / m_window_size.width() - 0.5, sy / m_window_size.height() - 0.5f);

    float z = 0.0f;
    float r2 = m_trackball_size * m_trackball_size;
    if (point_2d.lengthSquared() <= r2 * 0.5) {
        z = sqrt(r2 - point_2d.lengthSquared());
    } else {
        z = r2 * 0.5f / point_2d.length();
    }
    QVector3D point_3d(point_2d, z);
    return point_3d;
}

float clamp(float x) {
    return x > 1? 1 : (x < -1? -1 : x);
}

void Atoms3DCameraController::create_rotation(const QPoint& first_point, const QPoint& next_point,
    QVector3D& dir, float& angle) {

    auto last_pos_3d = project_to_trackball(first_point).normalized();
    auto current_pos_3d = project_to_trackball(next_point).normalized();
    // Compute axis of rotation:
    dir = QVector3D::crossProduct(current_pos_3d, last_pos_3d);
    // Approximate rotation angle:
    angle = acos(clamp(QVector3D::dotProduct(current_pos_3d, last_pos_3d)));
}

void Atoms3DCameraController::moveCamera(const Qt3DExtras::QAbstractCameraController::InputState& state, float dt) {
    auto camera_obj = this->camera();

    if(camera_obj == nullptr) return;

    auto linear_speed = this->linearSpeed();

    if (state.leftMouseButtonActive) {
        QVector3D dir;
        float angle;
        create_rotation(m_mouse_last_position, m_mouse_current_position, dir, angle);

        auto current_rotation = camera_obj->transform()->rotation();

        auto rotated_axis = current_rotation.rotatedVector(dir);
        angle *= m_rotation_speed;

        auto rotation_matrix = QQuaternion::fromAxisAndAngle(rotated_axis, angle * M_1_PI * 180);
        camera_obj->rotateAboutViewCenter(rotation_matrix);

    } else if(state.middleMouseButtonActive) {
        auto offset = m_mouse_current_position - m_mouse_last_position;
        camera_obj->translate(
            QVector3D(
                -offset.x() / m_window_size.width() * 0.5 * linear_speed,
                offset.y() / m_window_size.height() * 0.5 * linear_speed,
                0
            )
        );
    } else if(std::abs(dt - 0.0) > 1.0e-5) {
        camera_obj->translate(
            QVector3D(
                state.txAxisValue * linear_speed,
                state.tyAxisValue * linear_speed,
                state.tzAxisValue * linear_speed
            ) * dt,
            Qt3DRender::QCamera::DontTranslateViewCenter
        );
    }
    m_mouse_last_position = m_mouse_current_position;
}
