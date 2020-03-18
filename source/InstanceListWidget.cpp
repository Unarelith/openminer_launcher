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
#include "ContentData.hpp"
#include "InstanceListWidget.hpp"

InstanceListWidget::InstanceListWidget(ContentData &data, QWidget *parent) : QTreeWidget(parent), m_data(data) {
	setHeaderLabels({tr("ID"), tr("Name"), tr("Version")});
	setRootIsDecorated(false);
	setSortingEnabled(true);
	hideColumn(0);
}

void InstanceListWidget::update() {
	clear();

	auto &versionList = m_data.instanceList();
	for (auto &it : versionList) {
		auto *item = new QTreeWidgetItem(this);
		item->setText(0, QString::number(it.second.id()));
		item->setText(1, it.second.name());
		item->setText(2, m_data.getEngineVersion(it.second.engineVersionID())->name());
	}
}

