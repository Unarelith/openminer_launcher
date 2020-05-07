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
#include <QAction>
#include <QApplication>
#include <QDebug>
#include <QDir>
#include <QFileInfo>
#include <QKeyEvent>
#include <QMenuBar>
#include <QMessageBox>
#include <QProgressBar>
#include <QStandardPaths>
#include <QStatusBar>

#include "MainWindow.hpp"
#include "RepositoryWindow.hpp"

MainWindow::MainWindow() : QMainWindow(nullptr, Qt::Dialog) {
	setWindowTitle("OpenMiner Launcher");
	setFocusPolicy(Qt::ClickFocus);
	resize(1280, 720);

	openDatabase();

	setupStatusBar();
	setupMenuBar();
	setupTabs();

	connectObjects();

	show();
	updateWidgets();

	m_contentData.updateDatabase();
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
	connect(&m_instanceTab, &InstanceTabWidget::windowRefeshRequested, this, &MainWindow::updateWidgets);
}

void MainWindow::updateWidgets() {
	m_instanceTab.update();
	m_newsTab.update();
	m_engineVersionTab.update();
	m_modTab.update();
}

void MainWindow::setupTabs() {
	m_tabWidget.addTab(&m_instanceTab, "Instances");
	m_tabWidget.addTab(&m_newsTab, "News");
	m_tabWidget.addTab(&m_engineVersionTab, "Versions");
	// m_tabWidget.addTab(new QWidget, "Games");
	m_tabWidget.addTab(&m_modTab, "Mods");
	// m_tabWidget.addTab(new QWidget, "Textures");

	setCentralWidget(&m_tabWidget);
}

void MainWindow::setupStatusBar() {
	QProgressBar *dbUpdateBar = new QProgressBar;
	dbUpdateBar->hide();
	dbUpdateBar->setRange(0, 100);
	connect(&m_contentData, &ContentData::databaseUpdateStarted, dbUpdateBar, &QWidget::show);
	connect(&m_contentData, &ContentData::databaseUpdateStopped, dbUpdateBar, &QWidget::hide);

	QStatusBar *statusBar = QMainWindow::statusBar();
	statusBar->addPermanentWidget(dbUpdateBar);

	// connect(&m_session, &Session::stateChanged, statusBar, &QStatusBar::showMessage);
	connect(&m_contentData, &ContentData::stateChanged, statusBar, &QStatusBar::showMessage);
	connect(&m_contentData.database().loader(), &DatabaseLoader::stateChanged, statusBar, &QStatusBar::showMessage);
	connect(&m_contentData.database().loader(), &DatabaseLoader::updateProgressed, dbUpdateBar, &QProgressBar::setValue);
}

void MainWindow::setupMenuBar() {
	QAction *exitAction = new QAction(tr("&Exit"), this);
	exitAction->setShortcut(QKeySequence::Quit);
	exitAction->setStatusTip(tr("Exit the program"));
	connect(exitAction, &QAction::triggered, this, &MainWindow::close);

	QMenu *fileMenu = menuBar()->addMenu(tr("&File"));
	fileMenu->addAction(exitAction);

	QAction *repositoriesAction = new QAction(tr("&Repositories..."), this);
	repositoriesAction->setStatusTip(tr("View and edit repositories"));
	connect(repositoriesAction, &QAction::triggered, this, &MainWindow::openRepositoryWindow);

	QAction *updateAction = new QAction(tr("&Update"), this);
	updateAction->setShortcut(QKeySequence::Refresh);
	updateAction->setStatusTip("Update local database with online data");
	connect(updateAction, &QAction::triggered, &m_contentData, &ContentData::updateDatabase);

	// FIXME: Disabled because it deletes instances
	// QAction *reloadAction = new QAction(tr("&Reload"), this);
	// reloadAction->setShortcut(QKeySequence::fromString("Shift+F5"));
	// reloadAction->setStatusTip("Performs a full reload of the database");
	// connect(reloadAction, &QAction::triggered, &m_contentData, &ContentData::reloadDatabase);

	QAction *stopAction = new QAction(tr("&Stop"), this);
	stopAction->setShortcut(QKeySequence::fromString("Ctrl+C"));
	stopAction->setStatusTip("Stop database update");
	connect(stopAction, &QAction::triggered, &m_contentData, &ContentData::stopDatabaseUpdate);

	QMenu *contentMenu = menuBar()->addMenu(tr("&Content"));
	contentMenu->addAction(repositoriesAction);
	contentMenu->addSeparator();
	contentMenu->addAction(updateAction);
	// contentMenu->addAction(reloadAction);
	contentMenu->addAction(stopAction);

	QAction *aboutAction = new QAction(tr("&About"), this);
	aboutAction->setShortcut(QKeySequence::HelpContents);
	aboutAction->setStatusTip("About this program");
	connect(aboutAction, &QAction::triggered, this, &MainWindow::openAboutWindow);

	QAction *aboutQtAction = new QAction(tr("&About Qt"), this);
	aboutQtAction->setStatusTip("About Qt");
	connect(aboutQtAction, &QAction::triggered, qApp, &QApplication::aboutQt);

	QMenu *helpMenu = menuBar()->addMenu(tr("&Help"));
	helpMenu->addAction(aboutAction);
	helpMenu->addAction(aboutQtAction);
}

void MainWindow::openRepositoryWindow() {
	auto *window = new RepositoryWindow(m_contentData, this);
	window->show();
}

void MainWindow::openAboutWindow() {
	QMessageBox aboutBox(this);
	aboutBox.setWindowTitle("About OpenMiner Launcher");
	aboutBox.setTextFormat(Qt::RichText);
	aboutBox.setText("<h3>OpenMiner Launcher</h3>"
	                 "Made by Quentin Bazin (aka. Unarelith)<br/><br/>"
	                 "Thanks for using this project!<br/><br/>"
	                 "Feel free to create a ticket <a href='https://github.com/Unarelith/openminer_launcher/issues'>here</a> if you find some bugs.");
	aboutBox.setStandardButtons(QMessageBox::Ok);
	aboutBox.exec();
}

