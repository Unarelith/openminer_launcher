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
#ifndef SESSION_HPP_
#define SESSION_HPP_

#include <QJsonDocument>
#include <QNetworkAccessManager>

class Session : public QObject {
	Q_OBJECT

	public:
		Session();

		using ParameterList = std::map<QString, QString>;
		QJsonDocument get(const QString &apiEndpoint, const ParameterList &parameters = {}) const;

		bool download(const QUrl &url, const QString &path) const;

		bool isLoggedIn() const { return m_isLoggedIn; }

		static QString baseUrl;

	signals:
		void httpError(int errorCode) const;
		void userLoginRequired() const;

		void userLoggedIn();
		void userLoggedOut();

		void stateChanged(const QString &state, int timeout = 0) const;

	private:
		bool m_isLoggedIn = false;

		QNetworkAccessManager *m_network = nullptr;
};

#endif // SESSION_HPP_
