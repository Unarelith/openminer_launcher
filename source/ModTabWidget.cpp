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
#include <QNetworkReply>
#include <QProgressBar>
#include <QVBoxLayout>

#include "ContentData.hpp"
#include "ModTabWidget.hpp"
#include "PathUtils.hpp"

ModTabWidget::ModTabWidget(ContentData &data, QWidget *parent) : QWidget(parent), m_data(data) {
	m_modListWidget.setHeaderLabels({"", tr("ID"), tr("Name"), tr("Author"), tr("Latest installed"), tr("Latest version"), tr("Repository"), tr("Creation date"), tr("Updated"), "", ""});
	m_modListWidget.setSortingEnabled(true);
	m_modListWidget.setContextMenuPolicy(Qt::CustomContextMenu);
	m_modListWidget.sortItems(2, Qt::AscendingOrder);
	m_modListWidget.setColumnWidth(0, 64);
	m_modListWidget.setColumnWidth(9, 32);
	m_modListWidget.hideColumn(1);

	connect(&m_modListWidget, &QTreeWidget::itemSelectionChanged, this, &ModTabWidget::toggleButtons);

	m_installButton = new QPushButton{tr("Install")};
	m_removeButton = new QPushButton{tr("Remove")};

	m_installButton->setDisabled(true);
	m_removeButton->setDisabled(true);

	connect(m_installButton, &QPushButton::clicked, this, &ModTabWidget::downloadActionTriggered);
	connect(m_removeButton, &QPushButton::clicked, this, &ModTabWidget::removeActionTriggered);

	auto *sideBar = new QWidget;
	auto *buttonLayout = new QVBoxLayout{sideBar};
	buttonLayout->addWidget(m_installButton);
	buttonLayout->addWidget(m_removeButton);
	buttonLayout->addWidget(new QWidget, 1);

	QHBoxLayout *layout = new QHBoxLayout{this};
	layout->addWidget(&m_modListWidget);
	layout->addWidget(sideBar);
}

void ModTabWidget::update() {
	m_modListWidget.clear();

	auto &modList = m_data.modList();
	for (auto &it : modList) {
		auto *item = new QTreeWidgetItem(&m_modListWidget);
		item->setText(1, QString::number(it.second.id()));
		item->setText(2, it.second.name());
		item->setText(7, it.second.date().toString());
		item->setText(8, it.second.hasBeenUpdated() ? "true" : "false");

		QUuid repositoryUuid = it.second.get("repository_uuid").toUuid();
		ContentRepository *repository = m_data.getRepositoryFromUuid(repositoryUuid);
		if (repository)
			item->setText(6, repository->name());
		else
			item->setText(6, "N/A");

		ContentUser *user = m_data.getUser(it.second.user());
		if (user)
			item->setText(3, user->name());

		auto *progressBar = new QProgressBar;
		progressBar->setRange(0, 100);
		m_modListWidget.setItemWidget(item, 9, progressBar);

		ContentModVersion *latestVersion = nullptr;
		ContentModVersion *latestInstalledVersion = nullptr;
		for (auto &it : it.second.versions()) {
			ContentModVersion *version = m_data.getModVersion(it);

			auto *child = new QTreeWidgetItem(item);
			if (version->state() == ContentModVersion::State::Available)
				child->setIcon(0, QIcon(":/checkbox_off"));
			else if (version->state() == ContentModVersion::State::Downloaded)
				child->setIcon(0, QIcon(":/checkbox_on"));
			else
				child->setIcon(0, QIcon(":/ask"));

			child->setText(1, "    " + QString::number(version->id()));
			child->setText(1, "    " + QString::number(version->id()));
			child->setText(2, "    " + version->name());
			child->setText(7, "    " + version->date().toString());
			child->setText(8, "    " + QString(version->hasBeenUpdated() ? "true" : "false"));

			auto *progressBar = new QProgressBar;
			progressBar->setRange(0, 100);
			m_modListWidget.setItemWidget(child, 9, progressBar);

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

	for (int i = 1 ; i < 9 ; ++i)
		m_modListWidget.resizeColumnToContents(i);

	toggleButtons();
}

ContentModVersion *ModTabWidget::getModVersionFromItem(QTreeWidgetItem *item, bool installed) {
	ContentModVersion *modVersion = nullptr;
	if (item->parent()) {
		modVersion = m_data.getModVersion(item->text(1).toUInt());
	}
	else {
		ContentMod *mod = m_data.getMod(item->text(1).toUInt());

		ContentModVersion *latestVersion = nullptr;
		for (auto &it : mod->versions()) {
			ContentModVersion *version = m_data.getModVersion(it);
			if ((!latestVersion || latestVersion->id() < version->id())
			&& (!installed || version->state() == ContentModVersion::State::Downloaded))
				latestVersion = version;
		}

		if (latestVersion)
			modVersion = latestVersion;
	}

	return modVersion;
}

void ModTabWidget::downloadActionTriggered() {
	QList<QTreeWidgetItem *> selectedItems = m_modListWidget.selectedItems();
	if (selectedItems.size() == 1) {
		ContentModVersion *modVersion = getModVersionFromItem(selectedItems.at(0));

		if (modVersion) {
			QNetworkReply *reply = m_session.downloadRequest(modVersion->doc());
			connect(reply, &QNetworkReply::finished, [this, reply]() { unzipFile(reply); });
			connect(reply, &QNetworkReply::downloadProgress, this, [this, reply](qint64 bytesReceived, qint64 bytesTotal) {
				updateProgressBar(reply, bytesReceived, bytesTotal);
			});

			m_downloads.emplace(reply, selectedItems.at(0));

			updateProgressBar(reply, 0, 1);
		}
	}
}

void ModTabWidget::removeActionTriggered() {
	QList<QTreeWidgetItem *> selectedItems = m_modListWidget.selectedItems();
	if (selectedItems.size() == 1) {
		ContentModVersion *modVersion = getModVersionFromItem(selectedItems.at(0), true);

		if (modVersion) {
			QString path = PathUtils::getModVersionPath(modVersion->mod(), *modVersion);

			QDir dir{path};
			if (dir.exists())
				dir.removeRecursively();

			modVersion->setState(ContentModVersion::State::Available);

			update();
		}
	}
}

void ModTabWidget::updateProgressBar(QNetworkReply *reply, qint64 bytesReceived, qint64 bytesTotal) {
	QProgressBar *progressBar = (QProgressBar *)m_modListWidget.itemWidget(m_downloads.at(reply), 9);
	if (progressBar)
		progressBar->setValue((float)bytesReceived / bytesTotal * 100.f);
}

void ModTabWidget::unzipFile(QNetworkReply *reply) {
	ContentModVersion *modVersion = getModVersionFromItem(m_downloads.at(reply));

	if (modVersion) {
		QString path = PathUtils::getModVersionPath(modVersion->mod(), *modVersion);

		QDir dir;
		if (!dir.exists(path))
			dir.mkpath(path);

		if (!m_session.saveFileToDisk(reply, path + "/content.zip")) {
			qDebug() << "Failed to save" << path + "/content.zip";
			return;
		}

		PathUtils::unzipFile(path + "/content.zip", true);

		modVersion->setState(ContentModVersion::State::Downloaded);

		update();

		m_downloads.erase(reply);
	}
}

void ModTabWidget::toggleButtons() {
	QList<QTreeWidgetItem *> selectedItems = m_modListWidget.selectedItems();
	if (selectedItems.size() == 1) {
		ContentModVersion *modVersion = getModVersionFromItem(selectedItems.at(0));

		m_installButton->setEnabled(modVersion->state() != ContentModVersion::State::Downloaded);
		m_removeButton->setEnabled(modVersion->state() == ContentModVersion::State::Downloaded);
	}
	else {
		m_installButton->setEnabled(false);
		m_removeButton->setEnabled(false);
	}
}

