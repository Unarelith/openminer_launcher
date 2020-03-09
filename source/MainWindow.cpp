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
#include <QDebug>
#include <QDir>
#include <QFileInfo>
#include <QKeyEvent>
#include <QStandardPaths>

#include "MainWindow.hpp"

MainWindow::MainWindow() : QMainWindow(nullptr, Qt::Dialog) {
	setWindowTitle("OpenMiner Launcher");
	setFocusPolicy(Qt::ClickFocus);
	resize(1280, 720);

	m_tabWidget.addTab(&m_instanceTab, "Instances");
	// m_tabWidget.addTab(new QWidget, "News");
	// m_tabWidget.addTab(new QWidget, "Versions");
	// m_tabWidget.addTab(new QWidget, "Games");
	m_tabWidget.addTab(&m_modTab, "Mods");
	// m_tabWidget.addTab(new QWidget, "Textures");

	setCentralWidget(&m_tabWidget);

	openDatabase();
	updateWidgets();

	m_contentData.updateDatabase();

	show();
}

void MainWindow::closeEvent(QCloseEvent *event) {
	m_contentData.stopDatabaseUpdate();

	QMainWindow::closeEvent(event);
}

void MainWindow::keyPressEvent(QKeyEvent *event) {
	QMainWindow::keyPressEvent(event);

	if (event->key() == Qt::Key_Escape)
		close();
}

void MainWindow::openDatabase() {
	QString dirPath = QStandardPaths::writableLocation(QStandardPaths::CacheLocation);
	QString path = dirPath + "/openminer.sqlite";
	QFileInfo databaseInfo(path);
	if (databaseInfo.exists() && databaseInfo.isFile()) {
		m_contentData.openDatabase(path);
		m_contentData.update();
	}
	else {
		QDir dir(dirPath);
		if (!dir.mkpath(dirPath))
			qWarning() << "Error: Failed to create directory: " + dirPath;

		m_contentData.openDatabase(path);
	}
}

void MainWindow::connectObjects() {
	connect(&m_contentData, &ContentData::windowRefeshRequested, this, &MainWindow::updateWidgets);
}

void MainWindow::updateWidgets() {
	m_modTab.update(m_contentData);
}

