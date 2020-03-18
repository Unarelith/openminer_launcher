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
#ifndef CONTENTITEM_HPP_
#define CONTENTITEM_HPP_

#include <QSqlQuery>
#include <QVariant>

class ContentItem {
	public:
		ContentItem(const QString &sqlTable);
		ContentItem(const QString &sqlTable, const QSqlQuery &sqlQuery);

		void updateDatabaseTable() const;
		void writeToDatabase();

		bool has(const QString &fieldName) const { return m_data.find(fieldName) != m_data.end(); }
		void set(const QString &fieldName, const QVariant &value);
		QVariant get(const QString &fieldName) const;

		unsigned int id() const { return m_id; }

	protected:
		unsigned int m_id;

	private:
		QString m_sqlTable;

		std::map<QString, QVariant> m_data;
};

#endif // CONTENTITEM_HPP_
