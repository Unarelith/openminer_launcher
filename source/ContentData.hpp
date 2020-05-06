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

#include "ContentInstance.hpp"
#include "ContentEngineVersion.hpp"
#include "ContentMod.hpp"
#include "ContentModVersion.hpp"
#include "ContentNewsArticle.hpp"
#include "ContentRepository.hpp"
#include "ContentUser.hpp"
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
		void updateUserList();
		void updateInstanceList();
		void updateNewsArticleList();
		void updateEngineVersionList();
		void updateModList();
		void updateModVersionList();
		void updateRepositoryList();

		const Database &database() const { return m_database; }

		const std::unordered_map<unsigned int, ContentUser> &userList() const { return m_userList; }
		const std::unordered_map<unsigned int, ContentInstance> &instanceList() const { return m_instanceList; }
		const std::unordered_map<unsigned int, ContentNewsArticle> &newsArticleList() const { return m_newsArticleList; }
		const std::unordered_map<unsigned int, ContentEngineVersion> &engineVersionList() const { return m_engineVersionList; }
		const std::unordered_map<unsigned int, ContentMod> &modList() const { return m_modList; }
		const std::unordered_map<unsigned int, ContentModVersion> &modVersionList() const { return m_modVersionList; }
		const std::unordered_map<unsigned int, ContentRepository> &repositoryList() const { return m_repositoryList; }

		ContentUser *getUser(unsigned int id) { return getItem(id, m_userList); }
		ContentInstance *getInstance(unsigned int id) { return getItem(id, m_instanceList); }
		ContentNewsArticle *getNewsArticle(unsigned int id) { return getItem(id, m_newsArticleList); }
		ContentEngineVersion *getEngineVersion(unsigned int id) { return getItem(id, m_engineVersionList); }
		ContentMod *getMod(unsigned int id) { return getItem(id, m_modList); }
		ContentModVersion *getModVersion(unsigned int id) { return getItem(id, m_modVersionList); }
		ContentRepository *getRepository(unsigned int id) { return getItem(id, m_repositoryList); }

		void setUser(unsigned int id, const ContentUser &user) { setItem(id, user, m_userList); }
		void setInstance(unsigned int id, const ContentInstance &instance) { setItem(id, instance, m_instanceList); }
		void setNewsArticle(unsigned int id, const ContentNewsArticle &newsArticle) { setItem(id, newsArticle, m_newsArticleList); }
		void setEngineVersion(unsigned int id, const ContentEngineVersion &engineVersion) { setItem(id, engineVersion, m_engineVersionList); }
		void setMod(unsigned int id, const ContentMod &mod) { setItem(id, mod, m_modList); }
		void setModVersion(unsigned int id, const ContentModVersion &modVersion) { setItem(id, modVersion, m_modVersionList); }
		void setRepository(unsigned int id, const ContentRepository &repository) { setItem(id, repository, m_repositoryList); }

		void removeInstance(unsigned int id) { m_instanceList.erase(id); }
		void removeRepository(unsigned int id) { m_repositoryList.erase(id); }

	signals:
		void stateChanged(const QString &state, int timeout = 0);
		void windowRefeshRequested();

		void databaseUpdateStarted();
		void databaseUpdateStopped();

	private:
		Database m_database;
		DatabaseThread *m_databaseThread = nullptr;

		std::unordered_map<unsigned int, ContentUser> m_userList;
		std::unordered_map<unsigned int, ContentInstance> m_instanceList;
		std::unordered_map<unsigned int, ContentNewsArticle> m_newsArticleList;
		std::unordered_map<unsigned int, ContentEngineVersion> m_engineVersionList;
		std::unordered_map<unsigned int, ContentMod> m_modList;
		std::unordered_map<unsigned int, ContentModVersion> m_modVersionList;
		std::unordered_map<unsigned int, ContentRepository> m_repositoryList;

		template<typename T>
		T *getItem(unsigned int id, std::unordered_map<unsigned int, T> &itemList) {
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
