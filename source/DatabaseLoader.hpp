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
#ifndef DATABASELOADER_HPP_
#define DATABASELOADER_HPP_

#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>
#include <QObject>
#include <QThread>

#include "Session.hpp"

class ContentData;

class DatabaseLoader : public QObject {
	Q_OBJECT

	public:
		DatabaseLoader(ContentData &data) : m_data(data) {}

		void update() const;

	signals:
		void updateStarted() const;
		void updateProgressed(int value) const;
		void updateFinished() const;

	private:
		void updateEngineVersions() const;
		void updateMods() const;
		void updateModVersions() const;

		ContentData &m_data;

		template<typename T, typename F1, typename F2>
		void updateModel(const QString &apiEndpoint, const F1 &getModel, const F2 &setModel) const {
			if (QThread::currentThread()->isInterruptionRequested())
				return;

			Session session;
			QJsonDocument json = session.get(apiEndpoint);
			QJsonArray array = json.array();
			if (array.isEmpty())
				return;

			for (const QJsonValue &value : array) {
				if (QThread::currentThread()->isInterruptionRequested())
					return;

				QJsonObject jsonObject = value.toObject();
				int id = jsonObject.value("id").toInt();

				T *model = getModel(id);

				if (model) {
					model->loadFromJson(jsonObject, m_data);
					model->updateDatabaseTable();
					model->writeToDatabase();
				}
				else {
					if (apiEndpoint == "/api/mod")
						qDebug() << "created" << model << "from DatabaseLoader";

					T model{jsonObject, m_data};
					model.updateDatabaseTable();
					model.writeToDatabase();

					setModel(model.id(), model);
				}
			}

			emit updateProgressed(25);
		}
};

#endif // DATABASELOADER_HPP_
