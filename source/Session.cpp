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
#include <iostream>

#include <QEventLoop>
#include <QSaveFile>
#include <QNetworkReply>
#include <QThread>

#include "Session.hpp"

Session::Session() {
}

QJsonDocument Session::get(const QString &url, const ParameterList &parameters) {
	QString parameterString{""};
	for (auto &parameter : parameters)
		parameterString += "&" + parameter.first + "=" + parameter.second;

	// qDebug() << "GET" << url + parameterString;

	// emit stateChanged("Downloading...");

	// QTime before = QTime::currentTime();

	QNetworkAccessManager &network = getNetworkAccessManager();

	QEventLoop waitReply;
	connect(&network, &QNetworkAccessManager::finished, &waitReply, &QEventLoop::quit);

	QNetworkReply *reply = network.get(QNetworkRequest(QUrl(url + parameterString)));

	waitReply.exec();

	QVariant statusCodeVariant = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
	int statusCode = statusCodeVariant.toInt();

	if (statusCode != 200) {
		qDebug() << "Error: Http request failed. Code:" << statusCode;
		emit stateChanged("Request failed. (" + QString::number(statusCode) + ")");
		emit httpError(statusCode);

		if (statusCode == 401) {
			QThread::currentThread()->requestInterruption();
			emit userLoginRequired();
		}
	}

	// std::cout << "GET request finished in " << before.msecsTo(QTime::currentTime()) << " ms" << std::endl;

	return QJsonDocument::fromJson(reply->readAll());
}

bool Session::download(const QUrl &url, const QString &path) {
	QNetworkAccessManager &network = getNetworkAccessManager();

	QEventLoop waitReply;
	connect(&network, &QNetworkAccessManager::finished, &waitReply, &QEventLoop::quit);

	QNetworkReply *reply = network.get(QNetworkRequest(url));

	waitReply.exec();

	QVariant statusCodeVariant = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
	int statusCode = statusCodeVariant.toInt();

	if (statusCode != 200) {
		std::cerr << "Error: Http request failed. Code: " << statusCode << std::endl;
		emit stateChanged("Request failed. (" + QString::number(statusCode) + ")");
		emit httpError(statusCode);

		if (statusCode == 401) {
			QThread::currentThread()->requestInterruption();
			emit userLoginRequired();
		}
	}
	else {
		QByteArray data = reply->readAll();
		QSaveFile file{path};
		if (file.open(QIODevice::WriteOnly)) {
			file.write(data);
			file.commit();
			return true;
		}
		else
			std::cerr << "Error: Failed to save file: " << path.toStdString() << std::endl;
	}

	return false;
}

QNetworkReply *Session::downloadRequest(const QUrl &url) {
	QNetworkRequest request(url);
	request.setAttribute(QNetworkRequest::RedirectPolicyAttribute, QNetworkRequest::NoLessSafeRedirectPolicy);

	return getNetworkAccessManager().get(request);
}

bool Session::saveFileToDisk(QNetworkReply *reply, const QString &path) const {
	QVariant statusCodeVariant = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
	int statusCode = statusCodeVariant.toInt();

	if (statusCode != 200) {
		std::cerr << "Error: Http request failed. Code: " << statusCode << std::endl;
		emit stateChanged("Request failed. (" + QString::number(statusCode) + ")");
		emit httpError(statusCode);

		if (statusCode == 401) {
			QThread::currentThread()->requestInterruption();
			emit userLoginRequired();
		}
	}
	else {
		QByteArray data = reply->readAll();
		QSaveFile file{path};
		if (file.open(QIODevice::WriteOnly)) {
			file.write(data);
			file.commit();
			return true;
		}
		else
			std::cerr << "Error: Failed to save file: " << path.toStdString() << std::endl;
	}

	return false;
}

QNetworkAccessManager &Session::getNetworkAccessManager() {
	const QString thread_address = QString("0x") + QString::number((quintptr)QThread::currentThreadId(), 16);
	auto it = m_networkManagers.find(thread_address);
	if (it != m_networkManagers.end()) {
		return *it->second;
	} else {
		// qDebug() << "Creating new network manager for thread" << thread_address;
		m_networkManagers.emplace(thread_address, new QNetworkAccessManager);
		return *m_networkManagers.at(thread_address);
	}
}

