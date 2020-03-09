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
#include <QThread>
#include <QVariant>

#include "Database.hpp"

QString Database::s_path;

void Database::open(const QString &path) {
	s_path = path;

	if (!QSqlDatabase::isDriverAvailable("QSQLITE"))
		throw std::runtime_error("SQLite required!");
}

void Database::clear() const {
	for (QString table : getDatabase().tables()) {
		removeTable(table);
	}
}

void Database::addTable(const QString &name, const std::map<QString, QVariant> &fields) {
	QString queryString = "create table " + name + "(id INTEGER unique primary key";
	for (auto it : fields) {
		queryString += "," + it.first + " ";

		switch (it.second.type()) {
			case QVariant::Type::Int:
			case QVariant::Type::UInt:      queryString += "INTEGER"; break;
			case QVariant::Type::Bool:      queryString += "BOOLEAN"; break;
			case QVariant::Type::ByteArray:
			case QVariant::Type::Url:
			case QVariant::Type::String:    queryString += "TEXT"; break;
			case QVariant::Type::Date:      queryString += "DATE"; break;
			case QVariant::Type::DateTime:  queryString += "DATETIME"; break;
			default: break;
		}
	}
	queryString += ")";

	QSqlQuery query(queryString, getDatabase());
	if (!query.isActive())
		qWarning() << "Error: Failed to create database table " << name << ":" << query.lastError().text();
}

void Database::removeTable(const QString &name) {
	QSqlDatabase database = getDatabase();
	if (!database.tables().contains(name))
		return;

	QSqlQuery query(QString("drop table ") + name);
	if (!query.isActive())
		qWarning() << "Error: Failed to remove table " << name << " from database:" << query.lastError().text();
}

QSqlDatabase Database::getDatabase() {
	// Starting with Qt 5.11, sharing the same connection between threads is not allowed.
	// We use a dedicated connection for each thread requiring access to the database,
	// using the thread address as connection name.
	const QString thread_address = QString("0x") + QString::number((quintptr)QThread::currentThreadId(), 16);

 	// Check if there's already an open and valid connection to the database.
	QSqlDatabase db = QSqlDatabase::database(thread_address);
	if (db.isOpen() && db.isValid()) {
		return db;
	} else {
		qDebug() << "Creating new database connection for thread" << thread_address;
		db = QSqlDatabase::addDatabase(QString("QSQLITE"), thread_address);
		db.setDatabaseName(s_path);
		if (!db.open() || !db.isValid()) {
			qWarning() << "Error: " << db.lastError();
			throw std::runtime_error("Error: Failed to load database: " + s_path.toStdString());
		}
	}

	return db;
}

