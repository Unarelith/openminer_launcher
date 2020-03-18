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
#include <QDebug>
#include <QSqlError>
#include <QSqlRecord>

#include "Database.hpp"
#include "ContentItem.hpp"

ContentItem::ContentItem(const QString &sqlTable) {
	m_sqlTable = sqlTable;
}

ContentItem::ContentItem(const QString &sqlTable, const QSqlQuery &sqlQuery) {
	m_sqlTable = sqlTable;

	m_id = sqlQuery.value(0).toUInt();

	for (int i = 1 ; i < sqlQuery.record().count() ; ++i) {
		set(sqlQuery.record().fieldName(i), sqlQuery.value(i));
	};
}

void ContentItem::updateDatabaseTable() const {
	QSqlDatabase database = Database::getDatabase();
	if (database.tables().contains(m_sqlTable)) {
		// FIXME: Change record instead of dropping the entire table
		if ((unsigned int)database.record(m_sqlTable).count() != m_data.size() + 1) {
			qDebug() << "Removing deprecated table:" << m_sqlTable;
			qDebug() << "Record: " << database.record(m_sqlTable);
			Database::removeTable(m_sqlTable);
		}
	}

	if (!database.tables().contains(m_sqlTable)) {
		Database::addTable(m_sqlTable, m_data);
	}
}

void ContentItem::writeToDatabase() {
	QString insertQueryStringLeft = "insert or replace into " + m_sqlTable + "(id";
	QString insertQueryStringRight = ") values(?";
	for (auto it : m_data) {
		insertQueryStringLeft += "," + it.first;
		insertQueryStringRight += ",?";
	}
	insertQueryStringRight += ")";

	QSqlQuery insertQuery(Database::getDatabase());
	insertQuery.prepare(insertQueryStringLeft + insertQueryStringRight);

	unsigned int i = 0;
	insertQuery.bindValue(i++, m_id);
	for (auto it : m_data) {
		insertQuery.bindValue(i++, it.second);
	}

	if (!insertQuery.exec() && insertQuery.lastError().nativeErrorCode() != "19") {
		qWarning() << "Error: Failed to insert an element to table " << m_sqlTable << ":" << insertQuery.lastError().text();
		qWarning() << "       Query:" << insertQuery.lastQuery();
	}
}

void ContentItem::set(const QString &fieldName, const QVariant &value) {
	m_data[fieldName] = value;
}

QVariant ContentItem::get(const QString &fieldName) const {
	auto it = m_data.find(fieldName);
	if (it == m_data.end())
		return {};

	return it->second;
}

