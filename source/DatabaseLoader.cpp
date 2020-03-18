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
#include <QJsonArray>
#include <QJsonDocument>
#include <QThread>

#include "ContentData.hpp"
#include "DatabaseLoader.hpp"
#include "Session.hpp"

void DatabaseLoader::update() const {
	emit updateStarted();

	updateEngineVersions();
	updateMods();
	updateModVersions();

	emit updateFinished();
}

void DatabaseLoader::updateEngineVersions() const {
	if (QThread::currentThread()->isInterruptionRequested())
		return;

	Session session;
	QJsonDocument json = session.get("/api/version");
	QJsonArray array = json.array();
	if (array.isEmpty())
		return;

	for (const QJsonValue &value : array) {
		if (QThread::currentThread()->isInterruptionRequested())
			return;

		ContentEngineVersion engineVersion(value.toObject(), m_data);
		engineVersion.updateDatabaseTable();
		engineVersion.writeToDatabase();

		m_data.setEngineVersion(engineVersion.id(), engineVersion);
	}

	emit updateProgressed(25);
}

void DatabaseLoader::updateMods() const {
	if (QThread::currentThread()->isInterruptionRequested())
		return;

	Session session;
	QJsonDocument json = session.get("/api/mod");
	QJsonArray array = json.array();
	if (array.isEmpty())
		return;

	for (const QJsonValue &value : array) {
		if (QThread::currentThread()->isInterruptionRequested())
			return;

		ContentMod mod(value.toObject(), m_data);
		mod.updateDatabaseTable();
		mod.writeToDatabase();

		m_data.setMod(mod.id(), mod);
	}

	emit updateProgressed(25);
}

void DatabaseLoader::updateModVersions() const {
	if (QThread::currentThread()->isInterruptionRequested())
		return;

	Session session;
	QJsonDocument json = session.get("/api/mod/version");
	QJsonArray array = json.array();
	if (array.isEmpty())
		return;

	for (const QJsonValue &value : array) {
		if (QThread::currentThread()->isInterruptionRequested())
			return;

		ContentModVersion modVersion(value.toObject(), m_data);
		modVersion.updateDatabaseTable();
		modVersion.writeToDatabase();

		m_data.setModVersion(modVersion.id(), modVersion);
	}

	emit updateProgressed(25);
}

