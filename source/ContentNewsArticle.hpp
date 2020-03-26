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
#ifndef CONTENTNEWSARTICLE_HPP_
#define CONTENTNEWSARTICLE_HPP_

#include <QDateTime>
#include <QJsonObject>

#include "ContentItem.hpp"

class ContentData;

class ContentNewsArticle : public ContentItem {
	public:
		explicit ContentNewsArticle(const QSqlQuery &sqlQuery, ContentData &data) : ContentItem("news_articles", sqlQuery) {}
		explicit ContentNewsArticle(const QJsonObject &jsonObject, ContentData &data) : ContentItem("news_articles") { loadFromJson(jsonObject, data); }

		void loadFromJson(const QJsonObject &jsonObject, ContentData &data);

		QString title() const { return get("title").toString(); }
		QString content() const { return get("content").toString(); }
		QDateTime date() const { return get("date").toDateTime(); }
		unsigned int user() const { return get("user").toUInt(); }
};

#endif // CONTENTNEWSARTICLE_HPP_
