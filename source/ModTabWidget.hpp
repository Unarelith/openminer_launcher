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
#ifndef MODTABWIDGET_HPP_
#define MODTABWIDGET_HPP_

#include <QTreeWidget>

#include "Session.hpp"

class ContentData;

class ModTabWidget : public QWidget {
	public:
		ModTabWidget(ContentData &data, QWidget *parent = nullptr);

		void update();

	private:
		void showContextMenu(const QPoint &pos);

		void downloadActionTriggered();

		ContentData &m_data;

		QTreeWidget m_modListWidget;

		QTreeWidgetItem *m_currentItem = nullptr;

		Session m_session;
};

#endif // MODTABWIDGET_HPP_
