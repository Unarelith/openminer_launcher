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
#include "DatabaseLoader.hpp"

using namespace std::placeholders;

void DatabaseLoader::update() const {
	emit updateStarted();

	const auto &repositoryList = m_data.repositoryList();
	for (auto &it : repositoryList) {
		emit stateChanged("Loading users from repository '" + it.second.name() + "'...");

		updateModel<ContentUser>(it.second, "/api/user",
				std::bind(&ContentData::getUser, &m_data, _1),
				std::bind(&ContentData::setUser, &m_data, _1, _2),
				std::bind(&ContentData::userList, &m_data));

		emit updateProgressed(20);
		emit stateChanged("Loading news from repository '" + it.second.name() + "'...");

		updateModel<ContentNewsArticle>(it.second, "/api/news",
				std::bind(&ContentData::getNewsArticle, &m_data, _1),
				std::bind(&ContentData::setNewsArticle, &m_data, _1, _2),
				std::bind(&ContentData::newsArticleList, &m_data));

		emit updateProgressed(40);
		emit stateChanged("Loading engine versions from repository '" + it.second.name() + "'...");

		updateModel<ContentEngineVersion>(it.second, "/api/version",
				std::bind(&ContentData::getEngineVersion, &m_data, _1),
				std::bind(&ContentData::setEngineVersion, &m_data, _1, _2),
				std::bind(&ContentData::engineVersionList, &m_data));

		emit updateProgressed(60);
		emit stateChanged("Loading mods from repository '" + it.second.name() + "'...");

		updateModel<ContentMod>(it.second, "/api/mod",
				std::bind(&ContentData::getMod, &m_data, _1),
				std::bind(&ContentData::setMod, &m_data, _1, _2),
				std::bind(&ContentData::modList, &m_data));

		emit updateProgressed(80);
		emit stateChanged("Loading mod versions from repository '" + it.second.name() + "'...");

		updateModel<ContentModVersion>(it.second, "/api/mod/version",
				std::bind(&ContentData::getModVersion, &m_data, _1),
				std::bind(&ContentData::setModVersion, &m_data, _1, _2),
				std::bind(&ContentData::modVersionList, &m_data));

		emit updateProgressed(100);
		emit stateChanged("Done.");
	}

	emit updateFinished();
}

