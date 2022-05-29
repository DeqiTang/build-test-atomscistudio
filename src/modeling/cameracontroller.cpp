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
#include <armadillo>

Atoms3DCameraController::Atoms3DCameraController(Qt3DCore::QNode* parent)
    : Qt3DExtras::QAbstractCameraController(parent) {

    m_trackball_center[0] = 0.5;
    m_trackball_center[1] = 0.5;
    m_trackball_radius = 1.0;
    m_rotation_speed = 2.0;

    Qt3DInput::QMouseHandler* mouse_handler = new Qt3DInput::QMouseHandler(this);
    mouse_handler->setSourceDevice(this->mouseDevice());
    this->mouseDevice()->setSensitivity(0.001);

    QObject::connect(mouse_handler, &Qt3DInput::QMouseHandler::pressed,
        [&](Qt3DInput::QMouseEvent* pressed_event) {
            pressed_event->setAccepted(true);
            m_mouse_last_position = QPoint(pressed_event->x(), pressed_event->y());
            m_mouse_current_position = m_mouse_last_position;
        }
    );
    QObject::connect(mouse_handler, &Qt3DInput::QMouseHandler::positionChanged,
        [&](Qt3DInput::QMouseEvent* position_changed_event) {
            position_changed_event->setAccepted(true);
            m_mouse_current_position = QPoint(
                position_changed_event->x(),
                position_changed_event->y()
            );
        }
    );
}

void Atoms3DCameraController::moveCamera(const Qt3DExtras::QAbstractCameraController::InputState& state, float dt) {
    auto camera_obj = this->camera();
    auto linear_speed = this->linearSpeed();

    if(nullptr == camera_obj) {
        return;
    }

    /**
     * map from the 2d screen coordination system to the
     * virtual 3D trackball system.
     */
    const auto map_to_trackball = [&](const QPoint& screen_point) {
        double screen_x = screen_point.x();
        // The y axis of Qt screen set the upper left corner as origin.
        double screen_y = m_window_size.height() - screen_point.y();

        // with the center of the 3d window as the center of coordinate system
        QVector2D point_2d(
            screen_x / m_window_size.width() - m_trackball_center[0],
            screen_y / m_window_size.height() - m_trackball_center[1]
        );

        double z = 0.0;
        double radius_pow_2 = std::pow(m_trackball_radius, 2);
        auto point_2d_length = point_2d.length();
        auto point_2d_length_squared = point_2d.lengthSquared();
        if (point_2d_length_squared <= radius_pow_2 * 0.5) {
            z = std::sqrt(radius_pow_2 - point_2d_length_squared);
        } else {
            z = radius_pow_2 * 0.5 / point_2d_length;
        }
        QVector3D point_3d(point_2d, z);
        return point_3d;
    };

    if (true == state.leftMouseButtonActive) {
        QVector3D rot_dir;
        double rot_angle;

        auto last_pos_3d = map_to_trackball(m_mouse_last_position).normalized();
        auto current_pos_3d = map_to_trackball(m_mouse_current_position).normalized();
        // compute axis of rotation:
        rot_dir = QVector3D::crossProduct(current_pos_3d, last_pos_3d);
        // approximate rotation angle:
        double dot_of_pos = QVector3D::dotProduct(current_pos_3d, last_pos_3d);
        rot_angle = std::acos(
            dot_of_pos > 1 ? 1 : (dot_of_pos < -1 ? -1 : dot_of_pos)
        );

        auto current_rotation = camera_obj->transform()->rotation();

        auto rotated_axis = current_rotation.rotatedVector(rot_dir);
        rot_angle *= m_rotation_speed;

        auto rotation_matrix = QQuaternion::fromAxisAndAngle(
            rotated_axis,
            rot_angle * 1 / arma::datum::pi * 180
        );
        camera_obj->rotateAboutViewCenter(rotation_matrix);

    } else if (true == state.middleMouseButtonActive) {
        auto position_change = m_mouse_current_position - m_mouse_last_position;
        camera_obj->translate(
            QVector3D(
                -position_change.x() / m_window_size.width() * 0.5 * linear_speed,
                position_change.y() / m_window_size.height() * 0.5 * linear_speed,
                0
            )
        );
    } else if (true == state.rightMouseButtonActive) {

    }
    //
    if(std::abs(dt - 0.0) > 1.0e-5) {
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
