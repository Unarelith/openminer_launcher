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
#include "ContentData.hpp"
#include "ContentNewsArticle.hpp"

void ContentNewsArticle::loadFromJson(const QJsonObject &jsonObject, ContentData &data) {
	QDateTime date = QDateTime::fromString(jsonObject.value("date").toString(), Qt::ISODate);
	date.setTimeSpec(Qt::UTC);

	set("rid", jsonObject.value("id").toInt());
	set("title", jsonObject.value("title").toString());
	set("content", jsonObject.value("content").toString());
	set("date", date);

	unsigned int authorRemoteID = jsonObject.value("user").toInt();
	const auto &userList = data.userList();
	for (auto &it : userList) {
		if (it.second.rid() == authorRemoteID)
			set("author", it.second.id());
	}

	m_hasBeenUpdated = true;
}

