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
#include "ContentInstance.hpp"

ContentInstance::ContentInstance(unsigned int id) : ContentItem("instances") {
	m_id = id;

	set("name", "");
	set("engine_version", -1);
	set("mods", "");
}

ContentInstance::ContentInstance(const QJsonObject &jsonObject, ContentData &) : ContentItem("instances") {
	m_id = jsonObject.value("id").toInt();

	set("name", jsonObject.value("name").toString());
	set("engine_version", jsonObject.value("engine_version").toInt());
	set("mods", jsonObject.value("mods").toString());
}

QList<int> ContentInstance::mods() const {
	QStringList mods = get("mods").toString().split(',');

	QList<int> modsID;
	for (auto &it : mods)
		modsID.append(it.toUInt());

	return modsID;
}

void ContentInstance::addMod(unsigned int id) {
	QString mods = get("mods").toString();
	set("mods", mods + (mods.isEmpty() ? "" : ",") + QString::number(id));

	updateDatabaseTable();
	writeToDatabase();
}

void ContentInstance::setMods(const QList<int> &mods) {
	QString modString;
	for (int modID : mods) {
		modString += (modString.isEmpty() ? "" : ",") + QString::number(modID);
	}

	set("mods", modString);
}

