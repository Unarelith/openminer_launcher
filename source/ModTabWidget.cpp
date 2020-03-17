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
#include <QMenu>
#include <QVBoxLayout>

#include "ContentData.hpp"
#include "ModTabWidget.hpp"

ModTabWidget::ModTabWidget(QWidget *parent) : QWidget(parent) {
	m_modListWidget.setHeaderLabels({tr("ID"), tr("Name"), tr("Author"), tr("Latest version"), tr("Creation date")});
	// m_modListWidget.setRootIsDecorated(false);
	m_modListWidget.setSortingEnabled(true);
	m_modListWidget.setContextMenuPolicy(Qt::CustomContextMenu);

	connect(&m_modListWidget, &QTreeWidget::customContextMenuRequested, this, &ModTabWidget::showContextMenu);

	QVBoxLayout *layout = new QVBoxLayout{this};
	layout->addWidget(&m_modListWidget);
}

void ModTabWidget::update(ContentData &data) {
	m_modListWidget.clear();

	auto &modList = data.modList();
	for (auto &it : modList) {
		auto *item = new QTreeWidgetItem(&m_modListWidget);
		item->setText(0, QString::number(it.second.id()));
		item->setText(1, it.second.name());
		item->setText(2, QString::number(it.second.user()));
		item->setText(4, it.second.date());

		ContentModVersion *latestVersion = nullptr;
		for (auto &it : it.second.versions()) {
			ContentModVersion *version = data.getModVersion(it);

			auto *child = new QTreeWidgetItem(item);
			child->setText(0, QString::number(version->id()));
			child->setText(1, version->name());
			child->setText(4, version->date());

			if (!latestVersion || latestVersion->id() < version->id())
				latestVersion = version;
		}

		if (latestVersion)
			item->setText(3, latestVersion->name());
	}
}

void ModTabWidget::showContextMenu(const QPoint &pos) {
	QTreeWidgetItem *item = m_modListWidget.itemAt(pos);
	if (!item) return;

	QAction *downloadAction = new QAction(tr("&Download"), this);
	downloadAction->setStatusTip(tr("Download mod"));

	QMenu menu{this};
	menu.addAction(downloadAction);

	menu.exec(m_modListWidget.mapToGlobal(pos));
}

