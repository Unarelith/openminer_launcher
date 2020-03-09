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
#ifndef CONTENTDATA_HPP_
#define CONTENTDATA_HPP_

#include <unordered_map>

#include "ContentMod.hpp"
#include "Database.hpp"
#include "DatabaseThread.hpp"

class ContentData : public QObject {
	Q_OBJECT

	public:
		ContentData();

		void openDatabase(const QString &path);
		void updateDatabase();
		void reloadDatabase();
		void stopDatabaseUpdate();

		void databaseUpdateFinished();

		void update();
		void updateModList();

		const Database &database() const { return m_database; }

		const std::unordered_map<unsigned int, ContentMod> &modList() const { return m_modList; }

		const ContentMod *getMod(unsigned int id) { return getItem(id, m_modList); }

		void setMod(unsigned int id, const ContentMod &mod) { setItem(id, mod, m_modList); }

	signals:
		void stateChanged(const QString &state, int timeout = 0);
		void windowRefeshRequested();

		void databaseUpdateStarted();
		void databaseUpdateStopped();

	private:
		Database m_database;
		DatabaseThread *m_databaseThread = nullptr;

		std::unordered_map<unsigned int, ContentMod> m_modList;

		template<typename T>
		const T *getItem(unsigned int id, const std::unordered_map<unsigned int, T> &itemList) {
			auto it = itemList.find(id);
			if (it == itemList.end())
				return nullptr;
			return &it->second;
		}

		template<typename T>
		void setItem(unsigned int id, const T &item, std::unordered_map<unsigned int, T> &itemList) {
			auto it = itemList.find(id);
			if (it == itemList.end())
				itemList.emplace(id, item);
			else
				it->second = item;
		}
};

#endif // CONTENTDATA_HPP_
