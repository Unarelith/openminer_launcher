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
#ifndef CONTENTREPOSITORY_HPP_
#define CONTENTREPOSITORY_HPP_

#include <QUrl>
#include <QUuid>

#include "ContentItem.hpp"

class ContentData;

class ContentRepository : public ContentItem {
	public:
		ContentRepository(unsigned int id, const QString &name, const QString &url, const QString &uuid);
		explicit ContentRepository(const QSqlQuery &sqlQuery, ContentData &data) : ContentItem("repositories", sqlQuery) {}

		QString name() const { return get("name").toString(); }
		QUrl url() const { return get("url").toUrl(); }
		QUuid uuid() const { return get("uuid").toUuid(); }

		void setName(const QString &name) { set("name", name); updateDatabaseTable(); writeToDatabase(); }
		void setUrl(const QString &url) { set("url", url); updateDatabaseTable(); writeToDatabase(); }
		void setUuid(const QString &uuid) { set("uuid", uuid); updateDatabaseTable(); writeToDatabase(); }
};

#endif // CONTENTREPOSITORY_HPP_
