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
#ifndef MAINWINDOW_HPP_
#define MAINWINDOW_HPP_

#include <QMainWindow>
#include <QTabWidget>

#include "ContentData.hpp"
#include "Session.hpp"

#include "EngineVersionTabWidget.hpp"
#include "InstanceTabWidget.hpp"
#include "ModTabWidget.hpp"

class MainWindow : public QMainWindow {
	Q_OBJECT

	public:
		MainWindow(const QString &apiSource);

		void closeEvent(QCloseEvent *event) override;
		void keyPressEvent(QKeyEvent *event) override;

		void setupStatusBar();
		void setupTabs();

	private:
		void openDatabase();

		void connectObjects();
		void updateWidgets();

		ContentData m_contentData;

		QTabWidget m_tabWidget{this};

		InstanceTabWidget m_instanceTab{m_contentData};
		EngineVersionTabWidget m_engineVersionTab{m_contentData};
		ModTabWidget m_modTab{m_contentData};
};

#endif // MAINWINDOW_HPP_
