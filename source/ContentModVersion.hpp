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
#ifndef CONTENTMODVERSION_HPP_
#define CONTENTMODVERSION_HPP_

#include <QDateTime>
#include <QJsonObject>

#include "ContentItem.hpp"

class ContentData;
class ContentMod;

class ContentModVersion : public ContentItem {
	public:
		explicit ContentModVersion(const QSqlQuery &sqlQuery, ContentData &data);
		explicit ContentModVersion(const QJsonObject &jsonObject, ContentData &data) : ContentItem("mod_versions") { loadFromJson(jsonObject, data); }

		void loadFromJson(const QJsonObject &jsonObject, ContentData &data);

		QString name() const { return get("name").toString(); }
		QDateTime date() const { return get("date").toDateTime(); }
		QString doc() const { return get("doc").toString(); }
		unsigned int modID() const { return get("mod").toUInt(); }

		const ContentMod &mod() const { return *m_mod; }

		enum State {
			Available,
			Downloaded
		};

		State state() const { return (State)get("state").toUInt(); }
		void setState(State state) { set("state", state); writeToDatabase(); }

	private:
		ContentMod *m_mod = nullptr;
};

#endif // CONTENTMODVERSION_HPP_
