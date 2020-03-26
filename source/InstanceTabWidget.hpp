/*
 * =====================================================================================
 *
 *  OpenMiner Launcher
 *
 *  Copyright (C) 2018-2020 Unarelith, Quentin Bazin <openminer@unarelith.net>
 *
 *  This file is part of OpenMiner.
 *
 *  OpenMiner is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  OpenMiner is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public License
 *  along with OpenMiner; if not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA  02110-1301 USA
 *
 * =====================================================================================
 */
#ifndef INSTANCETABWIDGET_HPP_
#define INSTANCETABWIDGET_HPP_

#include "InstanceListWidget.hpp"
#include "InstanceSideBar.hpp"

class ContentData;

class InstanceTabWidget : public QWidget {
	Q_OBJECT

	public:
		InstanceTabWidget(ContentData &data, QWidget *parent = nullptr);

		void update();

	signals:
		void windowRefeshRequested();

	private:
		ContentData &m_data;

		InstanceListWidget m_instanceListWidget{m_data};
		InstanceSideBar m_instanceSideBar{m_data, m_instanceListWidget};
};

#endif // INSTANCETABWIDGET_HPP_
