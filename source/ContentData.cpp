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
#include <QApplication>
#include <QDebug>
#include <QJsonArray>
#include <QProgressDialog>
#include <QSqlRecord>
#include <QTimer>

#include "ContentData.hpp"

ContentData::ContentData() : m_database(*this) {
	connect(&m_database.loader(), &DatabaseLoader::updateFinished, this, &ContentData::update);
	connect(&m_database.loader(), &DatabaseLoader::updateFinished, this, &ContentData::windowRefeshRequested);
}

void ContentData::openDatabase(const QString &path) {
	m_database.open(path);
}

void ContentData::updateDatabase() {
	stopDatabaseUpdate();

	emit databaseUpdateStarted();

	m_databaseThread = new DatabaseThread(&m_database);
	connect(m_databaseThread, &QThread::finished, this, &ContentData::databaseUpdateFinished);
	m_databaseThread->start();
}

void ContentData::reloadDatabase() {
	stopDatabaseUpdate();
	m_database.clear();

	updateDatabase();
}

void ContentData::stopDatabaseUpdate() {
	if (m_databaseThread && m_databaseThread->isRunning()) {
		emit stateChanged("Stopping database update...");

		m_databaseThread->requestInterruption();
		m_databaseThread->wait();

		emit stateChanged("Database update stopped.");
		emit databaseUpdateStopped();

		m_databaseThread = nullptr;
	}
}

void ContentData::databaseUpdateFinished() {
	emit databaseUpdateStopped();

	// Wait 30 minutes and update database
	QTimer::singleShot(30 * 60 * 1000, [this] {
		updateDatabase();
	});
}

void ContentData::update() {
	updateModList();
	updateModVersionList();

	emit windowRefeshRequested();
}

void ContentData::updateModList() {
	m_modList.clear();

	QSqlQuery query("SELECT * FROM mods", Database::getDatabase());
	while (query.next()) {
		m_modList.emplace(query.value(0).toUInt(), ContentMod{query, *this});
	}
}

void ContentData::updateModVersionList() {
	m_modVersionList.clear();

	QSqlQuery query("SELECT * FROM mod_versions", Database::getDatabase());
	while (query.next()) {
		m_modVersionList.emplace(query.value(0).toUInt(), ContentModVersion{query, *this});
	}
}

