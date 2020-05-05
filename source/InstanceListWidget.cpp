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
#include <QApplication>
#include <QMessageBox>
#include <QDir>
#include <QProcess>
#include <QStandardPaths>

#include "ContentData.hpp"
#include "InstanceEditWindow.hpp"
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

		QString appData = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
		QString enginePath = appData + "/versions/" + QString::number(instance->engineVersionID()) + "/";
		QString instancePath = appData + "/instances/" + instance->name() + "/";

		QStringList args;
		args << "--working-dir" << instancePath;

		QProcess *process = new QProcess{this};
		process->start(enginePath + "openminer/openminer", args);

		connect(this, &QWidget::destroyed, process, &QProcess::close);
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

		QString appData = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
		QString instancePath = appData + "/instances/" + instance->name() + "/";

		QDir instanceDir{instancePath};
		instanceDir.removeRecursively();

		instance->removeFromDatabase();
		m_data.removeInstance(instanceID);

		emit windowRefeshRequested();
	}
}

