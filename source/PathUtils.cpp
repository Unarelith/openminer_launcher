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

#include <quazip/quazipfile.h>

#include "ContentData.hpp"
#include "PathUtils.hpp"
#include "Utils.hpp"

QString PathUtils::getInstancePath(const QString &instanceName) {
	QString appData = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
	return appData + "/instances/" + instanceName;
}

QString PathUtils::getEngineVersionPath(const ContentEngineVersion &engineVersion) {
	QUuid repositoryUuid = engineVersion.repositoryUuid();

	QString appData = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
	return appData + "/versions/" + repositoryUuid.toString(QUuid::WithoutBraces) + "/" + engineVersion.name();
}

QString PathUtils::getModVersionPath(const ContentMod &mod, const ContentModVersion &modVersion) {
	QUuid repositoryUuid = modVersion.repositoryUuid();

	QString appData = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
	return appData + "/mods/" + repositoryUuid.toString(QUuid::WithoutBraces) + "/" + mod.name() + "/" + modVersion.name();
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
	QString engineVersionPath = getEngineVersionPath(*data.getEngineVersion(instance.engineVersionID()));
	Utils::copyDirectory(engineVersionPath + "/openminer/resources", instancePath + "/resources");

	auto mods = instance.mods();
	for (auto &it : mods) {
		ContentMod *mod = data.getMod(it);
		QString modVersionPath = PathUtils::getModVersionPath(*mod, *data.getModVersion(mod->latestVersionID()));

		Utils::copyDirectory(modVersionPath + "/" + mod->name(), instancePath + "/mods/" + mod->name());
	}
}

void PathUtils::unzipFile(const QString &path, bool removeAfterExtraction) {
	QFileInfo fileInfo{path};
	QuaZip archive{path};
	if (!archive.open(QuaZip::mdUnzip)) {
		qDebug() << "Failed to open archive. Error code:" << archive.getZipError();
		return;
	}

	for(bool f = archive.goToFirstFile(); f; f = archive.goToNextFile()) {
		QString filePath = archive.getCurrentFileName();

		if (filePath.at(filePath.size() - 1) == '/') {
			QDir dir;
			dir.mkpath(fileInfo.absolutePath() + QDir::separator() + filePath);
		}
		else {
			QuaZipFile file(archive.getZipName(), filePath);
			file.open(QIODevice::ReadOnly);

			QuaZipFileInfo info;
			file.getFileInfo(&info);

			QByteArray data = file.readAll();

			file.close();

			QDir dir;
			QFileInfo dirInfo{fileInfo.absolutePath() + QDir::separator() + filePath};
			if (!dir.exists(dirInfo.absolutePath())) {
				dir.mkpath(dirInfo.absolutePath());
			}

			QFile dstFile(fileInfo.absolutePath() + QDir::separator() + filePath);
			dstFile.open(QIODevice::WriteOnly);
			dstFile.write(data);
			if (info.getPermissions() != 0)
				dstFile.setPermissions(info.getPermissions());
			else
				dstFile.setPermissions(QFile::Permissions(0x6644));

			dstFile.close();
		}
	}

	archive.close();

	if (removeAfterExtraction) {
		QFile file{path};
		file.remove();
	}
}

