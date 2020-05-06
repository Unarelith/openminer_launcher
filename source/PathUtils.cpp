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
#include <QStandardPaths>

#include "ContentData.hpp"
#include "PathUtils.hpp"
#include "Utils.hpp"

QString PathUtils::getInstancePath(const QString &instanceName) {
	QString appData = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
	return appData + "/instances/" + instanceName;
}

QString PathUtils::getEnginePath(int versionID) {
	QString appData = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
	return appData + "/versions/" + QString::number(versionID) + "/openminer";
}

QString PathUtils::getModPath(int modID, int versionID, const QString &modName) {
	QString appData = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
	return appData + "/mods/" + QString::number(modID) + "/" + QString::number(versionID) + "/" + modName;
}

void PathUtils::renameInstance(const QString &oldName, const QString &newName) {
	QString oldInstancePath = getInstancePath(oldName);
	QString newInstancePath = getInstancePath(newName);

	Utils::copyDirectory(oldInstancePath, newInstancePath);

	QDir oldInstanceDir{oldInstancePath};
	if (!oldInstanceDir.removeRecursively())
		qDebug() << "Failed to remove" << oldInstanceDir.path();

	QDir resourcesDir{newInstancePath + "/resources"};
	if (!resourcesDir.removeRecursively())
		qDebug() << "Failed to remove" << resourcesDir.path();

	QDir modsDir{newInstancePath + "/mods"};
	if (!modsDir.removeRecursively())
		qDebug() << "Failed to remove" << modsDir.path();
}

void PathUtils::reinstallInstance(const ContentInstance &instance, ContentData &data) {
	QString instancePath = getInstancePath(instance.name());
	QString versionPath = getEnginePath(instance.engineVersionID());
	Utils::copyDirectory(versionPath + "/resources", instancePath + "/resources");

	auto mods = instance.mods();
	for (auto &it : mods) {
		ContentMod *mod = data.getMod(it);
		QString modPath = PathUtils::getModPath(mod->id(), mod->latestVersionID(), mod->name());

		// FIXME: Use a mod string ID instead of the name
		Utils::copyDirectory(modPath, instancePath + "/mods/" + mod->name());
	}
}
