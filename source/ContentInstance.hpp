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
#ifndef CONTENTINSTANCE_HPP_
#define CONTENTINSTANCE_HPP_

#include <QJsonObject>

#include "ContentItem.hpp"

class ContentData;

class ContentInstance : public ContentItem {
	public:
		ContentInstance(unsigned int id);
		explicit ContentInstance(const QJsonObject &jsonObject, ContentData &data);
		explicit ContentInstance(const QSqlQuery &sqlQuery, ContentData &data) : ContentItem("instances", sqlQuery) {}

		QString name() const { return get("name").toString(); }
		void setName(const QString &name) { set("name", name); updateDatabaseTable(); writeToDatabase(); }

		int engineVersionID() const { return get("engine_version").toInt(); }
		void setEngineVersionID(unsigned int id) { set("engine_version", id); updateDatabaseTable(); writeToDatabase(); }

		int modID() const { return get("mod").toInt(); }
		void setModID(unsigned int id) { set("mod", id); updateDatabaseTable(); writeToDatabase(); }
};

#endif // CONTENTINSTANCE_HPP_
