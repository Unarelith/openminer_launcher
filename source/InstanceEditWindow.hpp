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
#ifndef INSTANCEEDITWINDOW_HPP_
#define INSTANCEEDITWINDOW_HPP_

#include <QDialog>
#include <QLineEdit>

class ContentData;
class ContentInstance;
class InstanceEditModTab;
class InstanceEditVersionTab;

class InstanceEditWindow : public QDialog {
	public:
		InstanceEditWindow(ContentData &data, ContentInstance *instance = nullptr, QWidget *parent = nullptr);

		void saveChanges();

	private:
		ContentData &m_data;
		ContentInstance *m_instance = nullptr;

		QLineEdit *m_nameEdit = nullptr;

		InstanceEditVersionTab *m_versionTab = nullptr;
		InstanceEditModTab *m_modTab = nullptr;
};

#endif // INSTANCEEDITWINDOW_HPP_
