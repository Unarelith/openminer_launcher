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

#include <QApplication>
#include <QMessageBox>
#include <QDir>
#include <QProcess>

#include "ContentData.hpp"
#include "InstanceEditWindow.hpp"
#include "InstanceListWidget.hpp"
#include "PathUtils.hpp"

InstanceListWidget::InstanceListWidget(ContentData &data, QWidget *parent) : QTreeWidget(parent), m_data(data) {
	setHeaderLabels({tr("ID"), tr("Name"), tr("Version"), tr("Mods")});
	setRootIsDecorated(false);
	setSortingEnabled(true);
	hideColumn(0);
}

void InstanceListWidget::update() {
	clear();

	auto &instanceList = m_data.instanceList();
	for (auto &it : instanceList) {
		auto *item = new QTreeWidgetItem(this);
		item->setText(0, QString::number(it.second.id()));
		item->setText(1, it.second.name());

		auto *engineVersion = m_data.getEngineVersion(it.second.engineVersionID());
		if (engineVersion) {
			QString versionString = engineVersion->name();

			auto *repo = m_data.getRepositoryFromUuid(engineVersion->repositoryUuid());
			if (repo)
				versionString += " (" + repo->name() + ")";

			item->setText(2, versionString);
		}

		const auto &mods = it.second.mods();
		QString modString;
		for (auto &it : mods) {
			auto *mod = m_data.getMod(it);
			if (!modString.isEmpty())
				modString += ", ";

			modString += mod->name();

			auto *repo = m_data.getRepositoryFromUuid(mod->repositoryUuid());
			if (repo)
				modString += " (" + repo->name() + ")";
		}

		item->setText(3, modString);
	}

	for (int i = 0 ; i < 4 ; ++i)
		resizeColumnToContents(i);
}

void InstanceListWidget::addInstance() {
	InstanceEditWindow *window = new InstanceEditWindow{m_data, nullptr, this};
	window->setModal(true);
	window->show();

	connect(window, &QDialog::accepted, this, &InstanceListWidget::windowRefeshRequested);
}

void InstanceListWidget::editInstance() {
	ContentInstance *instance = nullptr;

	QList<QTreeWidgetItem *> selectedItems = this->selectedItems();
	if (selectedItems.size() == 1) {
		unsigned int instanceID = selectedItems.at(0)->text(0).toUInt();
		instance = m_data.getInstance(instanceID);
	}

	InstanceEditWindow *window = new InstanceEditWindow{m_data, instance, this};
	window->setModal(true);
	window->show();

	connect(window, &QDialog::accepted, this, &InstanceListWidget::windowRefeshRequested);
}

void InstanceListWidget::runInstance() {
	QList<QTreeWidgetItem *> selectedItems = this->selectedItems();
	if (selectedItems.size() == 1) {
		unsigned int instanceID = selectedItems.at(0)->text(0).toUInt();
		ContentInstance *instance = m_data.getInstance(instanceID);

		qDebug().noquote().nospace() << "Running instance '" << instance->name() << "'...";

		QString enginePath = PathUtils::getEngineVersionPath(*m_data.getEngineVersion(instance->engineVersionID()));
		QString instancePath = PathUtils::getInstancePath(instance->name());

		QStringList args;
		args << "--working-dir" << instancePath;

		QString cwd = QDir::currentPath();
		QDir::setCurrent(enginePath + "/openminer");

		QProcess *process = new QProcess{this};
		process->setProcessChannelMode(QProcess::MergedChannels);
		process->start(enginePath + "/openminer/openminer", args);

		QDir::setCurrent(cwd);

		connect(this, &QWidget::destroyed, process, &QProcess::close);
		connect(process, &QProcess::readyRead, [process] () {
			std::cout << process->readAll().constData();
		});
	}
}

void InstanceListWidget::deleteInstance() {
	QList<QTreeWidgetItem *> selectedItems = this->selectedItems();
	if (selectedItems.size() == 1) {
		unsigned int instanceID = selectedItems.at(0)->text(0).toUInt();
		ContentInstance *instance = m_data.getInstance(instanceID);

		auto result = QMessageBox::question(this, QApplication::applicationDisplayName(),
				"Are you sure you want to remove '" + instance->name() + "'?");
		if (result != QMessageBox::Yes)
			return;

		QString instancePath = PathUtils::getInstancePath(instance->name());

		QDir instanceDir{instancePath};
		instanceDir.removeRecursively();

		instance->removeFromDatabase();
		m_data.removeInstance(instanceID);

		emit windowRefeshRequested();
	}
}

