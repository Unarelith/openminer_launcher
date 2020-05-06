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
 *  engineversionify it under the terms of the GNU Lesser General Public
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

#include <quazip/quazipfile.h>

#include "ContentData.hpp"
#include "EngineVersionTabWidget.hpp"

EngineVersionTabWidget::EngineVersionTabWidget(ContentData &data, QWidget *parent) : QWidget(parent), m_data(data) {
	m_versionListWidget.setHeaderLabels({"", tr("ID"), tr("Name"), tr("Repository"), tr("Creation date")});
	m_versionListWidget.setRootIsDecorated(false);
	m_versionListWidget.setSortingEnabled(true);
	m_versionListWidget.setContextMenuPolicy(Qt::CustomContextMenu);
	m_versionListWidget.sortItems(2, Qt::AscendingOrder);
	m_versionListWidget.setColumnWidth(0, 27);
	m_versionListWidget.hideColumn(1);
	m_versionListWidget.setSelectionMode(QAbstractItemView::NoSelection);
	m_versionListWidget.setFocusPolicy(Qt::NoFocus);

	connect(&m_versionListWidget, &QTreeWidget::customContextMenuRequested, this, &EngineVersionTabWidget::showContextMenu);

	QVBoxLayout *layout = new QVBoxLayout{this};
	layout->addWidget(&m_versionListWidget);
}

void EngineVersionTabWidget::update() {
	m_versionListWidget.clear();

	auto &versionList = m_data.engineVersionList();
	for (auto &it : versionList) {
		auto *item = new QTreeWidgetItem(&m_versionListWidget);
		if (it.second.state() == ContentEngineVersion::State::Available)
			item->setIcon(0, QIcon(":/checkbox_off"));
		else if (it.second.state() == ContentEngineVersion::State::Downloaded)
			item->setIcon(0, QIcon(":/checkbox_on"));

		item->setText(1, QString::number(it.second.id()));
		item->setText(2, it.second.name());
		item->setText(4, it.second.date().toString());

		QUuid repositoryUuid = it.second.get("repository_uuid").toUuid();
		ContentRepository *repository = m_data.getRepositoryFromUuid(repositoryUuid);
		if (repository)
			item->setText(3, repository->name());
		else
			item->setText(3, "N/A");
	}
}

void EngineVersionTabWidget::showContextMenu(const QPoint &pos) {
	QTreeWidgetItem *item = m_versionListWidget.itemAt(pos);
	if (!item) return;

	m_currentItem = item;

	ContentEngineVersion *engineVersion = m_data.getEngineVersion(m_currentItem->text(1).toUInt());
	if (engineVersion) {
		QMenu menu{this};

		if (engineVersion->state() == ContentEngineVersion::State::Available) {
			QAction *downloadAction = new QAction(tr("&Download"), this);
			downloadAction->setStatusTip(tr("Download engine version"));
			connect(downloadAction, &QAction::triggered, this, &EngineVersionTabWidget::downloadActionTriggered);
			menu.addAction(downloadAction);
		}
		else if (engineVersion->state() == ContentEngineVersion::State::Downloaded) {
			QAction *removeAction = new QAction(tr("&Remove"), this);
			removeAction->setStatusTip(tr("Remove engine version"));
			connect(removeAction, &QAction::triggered, this, &EngineVersionTabWidget::removeActionTriggered);
			menu.addAction(removeAction);
		}

		menu.exec(m_versionListWidget.mapToGlobal(pos));
	}
}

void EngineVersionTabWidget::downloadActionTriggered() {
	ContentEngineVersion *engineVersion = m_data.getEngineVersion(m_currentItem->text(1).toUInt());

	if (engineVersion) {
		QUuid repositoryUuid = engineVersion->get("repository_uuid").toUuid();

		QString path = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
		path += "/versions/" + repositoryUuid.toString(QUuid::WithoutBraces) + "/" + engineVersion->name() + "/";

		QDir dir;
		if (!dir.exists(path))
			dir.mkpath(path);

		m_session.download(engineVersion->doc(), path + "content.zip");

		QuaZip archive{path + "content.zip"};
		archive.open(QuaZip::mdUnzip);

		for(bool f = archive.goToFirstFile(); f; f = archive.goToNextFile()) {
			QString filePath = archive.getCurrentFileName();

			if (filePath.at(filePath.size() - 1) == '/') {
				QDir dir;
				dir.mkpath(path + filePath);
			}
		}

		for(bool f = archive.goToFirstFile(); f; f = archive.goToNextFile()) {
			QString filePath = archive.getCurrentFileName();

			if (filePath.at(filePath.size() - 1) != '/') {
				QuaZipFile file(archive.getZipName(), filePath);
				file.open(QIODevice::ReadOnly);

				QuaZipFileInfo info;
				file.getFileInfo(&info);

				QByteArray data = file.readAll();

				file.close();

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

		engineVersion->setState(ContentEngineVersion::State::Downloaded);

		update();
	}
}

void EngineVersionTabWidget::removeActionTriggered() {
	ContentEngineVersion *engineVersion = m_data.getEngineVersion(m_currentItem->text(1).toUInt());

	if (engineVersion) {
		QUuid repositoryUuid = engineVersion->get("repository_uuid").toUuid();
		QString path = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
		path += "/versions/" + repositoryUuid.toString(QUuid::WithoutBraces) + "/" + engineVersion->name() + "/";

		QDir dir{path};
		if (dir.exists())
			dir.removeRecursively();

		engineVersion->setState(ContentEngineVersion::State::Available);

		update();
	}
}


