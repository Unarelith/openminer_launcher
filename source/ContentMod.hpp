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

#include <QJsonObject>

#include "ContentItem.hpp"

class ContentMod : public ContentItem {
	public:
		explicit ContentMod(const QJsonObject &jsonObject);
		explicit ContentMod(const QSqlQuery &sqlQuery) : ContentItem("mods", sqlQuery) {}

		QString name() const { return get("name").toString(); }
		unsigned int user() const { return get("user").toUInt(); }
};

#endif // CONTENTMOD_HPP_
