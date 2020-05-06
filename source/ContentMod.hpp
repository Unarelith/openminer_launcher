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
#ifndef CONTENTMOD_HPP_
#define CONTENTMOD_HPP_

#include <QDateTime>
#include <QJsonObject>

#include "ContentItem.hpp"

class ContentData;
class ContentModVersion;

class ContentMod : public ContentItem {
	public:
		ContentMod() : ContentItem("mods") {}
		explicit ContentMod(const QSqlQuery &sqlQuery, ContentData &data) : ContentItem("mods", sqlQuery) {}

		void loadFromJson(const QJsonObject &jsonObject, ContentData &data);

		unsigned int rid() const { return get("rid").toUInt(); }
		QUuid repositoryUuid() const { return get("repository_uuid").toUuid(); }

		QString name() const { return get("name").toString(); }
		QDateTime date() const { return get("date").toDateTime(); }
		unsigned int user() const { return get("user").toUInt(); }

		void addVersion(unsigned int id);
		const std::vector<unsigned int> &versions() const { return m_versions; }

		int latestVersionID() const { return m_latestVersionID; }

	private:
		std::vector<unsigned int> m_versions;

		int m_latestVersionID = -1;
};

#endif // CONTENTMOD_HPP_
