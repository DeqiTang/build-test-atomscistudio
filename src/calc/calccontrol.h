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


#ifndef CALCCONTROL_H
#define CALCCONTROL_H

#include <QWidget>
#include <QtWidgets/QHBoxLayout>

class CalcControl : public QWidget {
    Q_OBJECT
public:
    explicit CalcControl(QWidget *parent = nullptr);

        QHBoxLayout* m_hlayout;
signals:

};

#endif // CALCCONTROL_H
