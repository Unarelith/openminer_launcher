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
#include <QVBoxLayout>

#include "ContentData.hpp"
#include "InstanceEditVersionTab.hpp"

InstanceEditVersionTab::InstanceEditVersionTab(ContentData &data, ContentInstance *instance, QWidget *parent) : QWidget(parent), m_data(data) {
	m_versionListWidget = new QTreeWidget;
	m_versionListWidget->setHeaderLabels({"", tr("ID"), tr("Name"), tr("Creation date")});
	m_versionListWidget->setRootIsDecorated(false);
	m_versionListWidget->setSortingEnabled(true);
	m_versionListWidget->sortItems(2, Qt::AscendingOrder);
	m_versionListWidget->setColumnWidth(0, 27);
	m_versionListWidget->hideColumn(1);
	m_versionListWidget->setSelectionMode(QAbstractItemView::SingleSelection);
	m_versionListWidget->setFocusPolicy(Qt::NoFocus);

	connect(m_versionListWidget, &QTreeWidget::itemSelectionChanged, this, &InstanceEditVersionTab::updateSelectedVersion);

	auto &engineVersionList = data.engineVersionList();
	for (auto &it : engineVersionList) {
		if (it.second.state() == ContentEngineVersion::State::Downloaded) {
			auto *item = new QTreeWidgetItem(m_versionListWidget);
			if (it.second.state() == ContentEngineVersion::State::Available)
				item->setIcon(0, QIcon(":/checkbox_off"));
			else if (it.second.state() == ContentEngineVersion::State::Downloaded)
				item->setIcon(0, QIcon(":/checkbox_on"));

			item->setText(1, QString::number(it.second.id()));
			item->setText(2, it.second.name());
			item->setText(3, it.second.date().toString());

			if (instance && instance->engineVersionID() == (int)it.second.id()) {
				item->setSelected(true);
			}
		}
	}

	auto *layout = new QVBoxLayout{this};
	layout->addWidget(m_versionListWidget);
}

void InstanceEditVersionTab::updateSelectedVersion() {
	QList<QTreeWidgetItem *> selectedItems = m_versionListWidget->selectedItems();
	m_engineVersionID = (selectedItems.size() == 1) ? selectedItems.at(0)->text(1).toUInt() : -1;
}

