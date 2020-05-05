/*
 * =====================================================================================
 *
 *  OpenMiner
 *
 *  Copyright (C) 2018-2020 Unarelith, Quentin Bazin <openminer@unarelith.net>
 *  Copyright (C) 2019-2020 the OpenMiner contributors (see CONTRIBUTORS.md)
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
#include <QFormLayout>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QPushButton>
#include <QRegExpValidator>
#include <QTabWidget>
#include <QVBoxLayout>

#include "ContentData.hpp"
#include "InstanceEditModTab.hpp"
#include "InstanceEditVersionTab.hpp"
#include "InstanceEditWindow.hpp"
#include "PathUtils.hpp"
#include "Utils.hpp"

InstanceEditWindow::InstanceEditWindow(ContentData &data, ContentInstance *instance, QWidget *parent)
	: QDialog(parent), m_data(data), m_instance(instance)
{
	setWindowTitle("Edit instance...");
	resize(640, 480);

	m_versionTab = new InstanceEditVersionTab{data, instance};
	m_modTab = new InstanceEditModTab{data, instance};

	auto *tabWidget = new QTabWidget;
	tabWidget->addTab(m_versionTab, "Engine");
	tabWidget->addTab(m_modTab, "Mods");

	QRegExp re("^[a-zA-Z0-9_]+$");
	QRegExpValidator *validator = new QRegExpValidator(re, this);

	m_nameEdit = new QLineEdit;
	m_nameEdit->setFocusPolicy(Qt::ClickFocus);
	m_nameEdit->setValidator(validator);
	if (instance)
		m_nameEdit->setText(instance->name());

	auto *formLayout = new QFormLayout;
	formLayout->addRow("&Name", m_nameEdit);

	auto *okButton = new QPushButton{"OK"};
	auto *cancelButton = new QPushButton{"Cancel"};

	connect(okButton, &QPushButton::clicked, this, &InstanceEditWindow::saveChanges);
	connect(cancelButton, &QPushButton::clicked, this, &QDialog::close);

	auto *hLayout = new QHBoxLayout;
	hLayout->addWidget(new QWidget, 1);
	hLayout->addWidget(okButton);
	hLayout->addWidget(cancelButton);

	auto *layout = new QVBoxLayout{this};
	layout->addLayout(formLayout);
	layout->addWidget(tabWidget);
	layout->addLayout(hLayout);
}

void InstanceEditWindow::saveChanges() {
	auto result = QMessageBox::question(this, "OpenMiner Launcher", "This operation will remove 'resources' and 'mods' folders.\nAre you sure?");
	if (result != QMessageBox::Yes)
		return;

	auto &instances = m_data.instanceList();
	for (auto &it : instances) {
		if ((!m_instance || it.second.id() != m_instance->id()) && it.second.name() == m_nameEdit->text()) {
			QMessageBox::critical(this, "OpenMiner Launcher", "An instance with this name already exists!");
			return;
		}
	}

	QString oldName = m_instance->name();
	m_instance->setName(m_nameEdit->text());
	m_instance->setEngineVersionID(m_versionTab->engineVersionID());
	m_instance->setMods(m_modTab->modList());

	PathUtils::renameInstance(oldName, m_instance->name());
	PathUtils::reinstallInstance(*m_instance, m_data);

	accept();

	QMessageBox::information(this, "OpenMiner Launcher", "Instance successfully edited!");
}

