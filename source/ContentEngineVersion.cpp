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
#include <QSysInfo>

#include "ContentData.hpp"
#include "ContentEngineVersion.hpp"

void ContentEngineVersion::loadFromJson(const QJsonObject &jsonObject, ContentData &data) {
	QDateTime date = QDateTime::fromString(jsonObject.value("date").toString(), Qt::ISODate);
	date.setTimeSpec(Qt::UTC);

	set("rid", jsonObject.value("id").toInt());
	set("name", jsonObject.value("name").toString());
	set("date", date);

	QString windowsUrl = jsonObject.value("windows_url").toString();
	if (windowsUrl.isEmpty())
		windowsUrl = jsonObject.value("windows_zip").toString();
	set("windows_url", windowsUrl);

	QString linuxUrl = jsonObject.value("linux_url").toString();
	if (linuxUrl.isEmpty())
		linuxUrl = jsonObject.value("linux_zip").toString();
	set("linux_url", linuxUrl);

	QString macosUrl = jsonObject.value("macos_url").toString();
	if (macosUrl.isEmpty())
		macosUrl = jsonObject.value("macos_zip").toString();
	set("macos_url", linuxUrl);

	ContentEngineVersion *version = data.getEngineVersion(m_id);
	if (version)
		set("state", version->state());
	else
		set("state", State::Available);

	m_hasBeenUpdated = true;
}

QString ContentEngineVersion::fileUrl() const {
	QString product = QSysInfo::productType() ;
	if (product == "osx")
		return macosUrl();
	else if (product == "winrt" || product == "windows")
		return windowsUrl();
	else
		return linuxUrl();
}

