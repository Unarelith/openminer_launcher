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
#include <QDir>
#include <QMenu>
#include <QStandardPaths>
#include <QVBoxLayout>

#include <quazip5/quazipfile.h>

#include "ContentData.hpp"
#include "ModTabWidget.hpp"

ModTabWidget::ModTabWidget(ContentData &data, QWidget *parent) : QWidget(parent), m_data(data) {
	m_modListWidget.setHeaderLabels({"", tr("ID"), tr("Name"), tr("Author"), tr("Latest installed"), tr("Latest version"), tr("Creation date"), tr("Updated")});
	// m_modListWidget.setRootIsDecorated(false);
	m_modListWidget.setSortingEnabled(true);
	m_modListWidget.setContextMenuPolicy(Qt::CustomContextMenu);
	m_modListWidget.sortItems(2, Qt::AscendingOrder);
	m_modListWidget.setColumnWidth(0, 64);
	m_modListWidget.hideColumn(1);
	m_modListWidget.setSelectionMode(QAbstractItemView::NoSelection);
	m_modListWidget.setFocusPolicy(Qt::NoFocus);

	connect(&m_modListWidget, &QTreeWidget::customContextMenuRequested, this, &ModTabWidget::showContextMenu);

	QVBoxLayout *layout = new QVBoxLayout{this};
	layout->addWidget(&m_modListWidget);
}

void ModTabWidget::update() {
	m_modListWidget.clear();

	auto &modList = m_data.modList();
	for (auto &it : modList) {
		auto *item = new QTreeWidgetItem(&m_modListWidget);
		// item->setText(0, " 0");
		item->setText(1, QString::number(it.second.id()));
		item->setText(2, it.second.name());
		item->setText(6, it.second.date().toString());
		item->setText(7, it.second.hasBeenUpdated() ? "true" : "false");

		ContentUser *user = m_data.getUser(it.second.user());
		if (user)
			item->setText(3, user->name());

		ContentModVersion *latestVersion = nullptr;
		ContentModVersion *latestInstalledVersion = nullptr;
		for (auto &it : it.second.versions()) {
			ContentModVersion *version = m_data.getModVersion(it);

			auto *child = new QTreeWidgetItem(item);
			if (version->state() == ContentModVersion::State::Available)
				child->setIcon(0, QIcon(":/checkbox_off"));
			else if (version->state() == ContentModVersion::State::Downloaded)
				child->setIcon(0, QIcon(":/checkbox_on"));

			child->setText(1, "    " + QString::number(version->id()));
			child->setText(2, "    " + version->name());
			child->setText(6, "    " + version->date().toString());
			child->setText(7, "    " + QString(version->hasBeenUpdated() ? "true" : "false"));

			if (!latestVersion || latestVersion->id() < version->id())
				latestVersion = version;

			if ((!latestInstalledVersion || latestInstalledVersion->id() < version->id())
			&& version->state() == ContentModVersion::State::Downloaded)
				latestInstalledVersion = version;
		}

		if (latestInstalledVersion) {
			item->setIcon(0, QIcon(":/checkbox_on"));
			item->setText(4, latestInstalledVersion->name());
		}
		else
			item->setIcon(0, QIcon(":/checkbox_off"));

		if (latestVersion)
			item->setText(5, latestVersion->name());
	}
}

void ModTabWidget::showContextMenu(const QPoint &pos) {
	QTreeWidgetItem *item = m_modListWidget.itemAt(pos);
	if (!item) return;

	m_currentItem = item;

	QAction *downloadAction = new QAction(tr("&Download"), this);
	downloadAction->setStatusTip(tr("Download mod"));
	connect(downloadAction, &QAction::triggered, this, &ModTabWidget::downloadActionTriggered);

	QMenu menu{this};
	menu.addAction(downloadAction);

	menu.exec(m_modListWidget.mapToGlobal(pos));
}

void ModTabWidget::downloadActionTriggered() {
	ContentModVersion *modVersion = nullptr;
	if (m_currentItem->parent()) {
		modVersion = m_data.getModVersion(m_currentItem->text(1).toUInt());
	}
	else {
		ContentMod *mod = m_data.getMod(m_currentItem->text(1).toUInt());

		ContentModVersion *latestVersion = nullptr;
		for (auto &it : mod->versions()) {
			ContentModVersion *version = m_data.getModVersion(it);
			if (!latestVersion || latestVersion->id() < version->id())
				latestVersion = version;
		}

		if (latestVersion)
			modVersion = latestVersion;
	}

	if (modVersion) {
		QString path = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
		path += "/mods/" + QString::number(modVersion->modID()) + "/" + QString::number(modVersion->id()) + "/";

		QDir dir;
		if (!dir.exists(path))
			dir.mkpath(path);

		if (m_session.download(modVersion->doc(), path + "content.zip")) {
			QuaZip archive{path + "content.zip"};
			if (!archive.open(QuaZip::mdUnzip)) {
				qDebug() << "ERROR: Failed to open archive at" << path + "content.zip";
				return;
			}

			for(bool f = archive.goToFirstFile(); f; f = archive.goToNextFile()) {
				QString filePath = archive.getCurrentFileName();

				QuaZipFile file(archive.getZipName(), filePath);
				file.open(QIODevice::ReadOnly);

				QuaZipFileInfo info;
				file.getFileInfo(&info);

				QByteArray data = file.readAll();

				file.close();

				if (filePath.at(filePath.size() - 1) == '/') {
					QDir dir;
					dir.mkpath(path + filePath);
				}
				else {
					QFile dstFile(path + filePath);
					dstFile.open(QIODevice::WriteOnly);
					dstFile.write(data);
					dstFile.setPermissions(info.getPermissions());
					dstFile.close();
				}
			}

			archive.close();

			QFile file{path + "content.zip"};
			file.remove();

			modVersion->setState(ContentModVersion::State::Downloaded);

			update();
		}
	}
}

