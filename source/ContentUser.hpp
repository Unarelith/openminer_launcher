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
#ifndef CONTENTUSER_HPP_
#define CONTENTUSER_HPP_

#include <QJsonObject>

#include "ContentItem.hpp"

class ContentData;

class ContentUser : public ContentItem {
	public:
		ContentUser() : ContentItem("users") {}
		explicit ContentUser(const QSqlQuery &sqlQuery, ContentData &data) : ContentItem("users", sqlQuery) {}

		void loadFromJson(const QJsonObject &jsonObject, ContentData &data);

		unsigned int rid() const { return get("rid").toUInt(); }
		QUuid repositoryUuid() const { return get("repository_uuid").toUuid(); }

		QString name() const { return get("name").toString(); }
};


#endif // CONTENTUSER_HPP_
