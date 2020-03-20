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

	updateModel<ContentEngineVersion>("/api/version",
			std::bind(&ContentData::getEngineVersion, &m_data, _1),
			std::bind(&ContentData::setEngineVersion, &m_data, _1, _2));

	updateModel<ContentMod>("/api/mod",
			std::bind(&ContentData::getMod, &m_data, _1),
			std::bind(&ContentData::setMod, &m_data, _1, _2));

	updateModel<ContentModVersion>("/api/mod/version",
			std::bind(&ContentData::getModVersion, &m_data, _1),
			std::bind(&ContentData::setModVersion, &m_data, _1, _2));

	emit updateFinished();
}

