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
#include <QTreeWidget>
#include <QVBoxLayout>

#include "ContentData.hpp"
#include "InstanceEditModTab.hpp"

InstanceEditModTab::InstanceEditModTab(ContentData &data, QWidget *parent) : QWidget(parent) {
	auto *modListWidget = new QTreeWidget;
	modListWidget->setHeaderLabels({"", tr("ID"), tr("Name"), tr("Creation date")});
	modListWidget->setRootIsDecorated(false);
	modListWidget->setSortingEnabled(true);
	modListWidget->sortItems(2, Qt::AscendingOrder);
	modListWidget->setColumnWidth(0, 27);
	modListWidget->hideColumn(1);
	modListWidget->setSelectionMode(QAbstractItemView::MultiSelection);
	modListWidget->setFocusPolicy(Qt::NoFocus);

	auto &modList = data.modList();
	for (auto &it : modList) {
		// FIXME: This should test the latest INSTALLED mod version, instead of the latest version
		ContentModVersion *latestModVersion = data.getModVersion(it.second.latestVersionID());
		if (latestModVersion && latestModVersion->state() == ContentModVersion::State::Downloaded) {
			auto *item = new QTreeWidgetItem(modListWidget);
			if (latestModVersion->state() == ContentModVersion::State::Available)
				item->setIcon(0, QIcon(":/checkbox_off"));
			else if (latestModVersion->state() == ContentModVersion::State::Downloaded)
				item->setIcon(0, QIcon(":/checkbox_on"));

			item->setText(1, QString::number(it.second.id()));
			item->setText(2, it.second.name());
			item->setText(3, it.second.date().toString());
		}
	}

	auto *layout = new QVBoxLayout{this};
	layout->addWidget(modListWidget);
}

