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
#ifndef DATABASE_HPP_
#define DATABASE_HPP_

#include <QObject>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>

#include "DatabaseLoader.hpp"

class Database : public QObject {
	public:
		Database(ContentData &data) : m_loader(data) {}

		void open(const QString &path);
		void clear() const;

		static void addTable(const QString &name, const std::map<QString, QVariant> &fields);
		static void removeTable(const QString &name);

		static QSqlDatabase getDatabase();

		const DatabaseLoader &loader() const { return m_loader; }

	private:
		static QString s_path;

		DatabaseLoader m_loader;
};

#endif // DATABASE_HPP_
