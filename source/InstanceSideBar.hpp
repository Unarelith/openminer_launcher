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
#ifndef INSTANCESIDEBAR_HPP_
#define INSTANCESIDEBAR_HPP_

#include <QPushButton>

class ContentData;
class InstanceListWidget;

class InstanceSideBar : public QWidget {
	Q_OBJECT

	public:
		InstanceSideBar(ContentData &data, InstanceListWidget &instanceListWidget, QWidget *parent = nullptr);

		void toggleButtons();

	signals:
		void windowRefeshRequested();
		void addInstanceButtonClicked();
		void editInstanceButtonClicked();
		void runInstanceButtonClicked();
		void deleteInstanceButtonClicked();

	private:
		ContentData &m_data;

		InstanceListWidget &m_instanceListWidget;

		QPushButton m_addInstanceButton{this};
		QPushButton m_editInstanceButton{this};
		QPushButton m_runInstanceButton{this};
		QPushButton m_deleteInstanceButton{this};
};

#endif // INSTANCESIDEBAR_HPP_
