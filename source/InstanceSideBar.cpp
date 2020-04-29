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

#include "InstanceListWidget.hpp"
#include "InstanceSideBar.hpp"
#include "InstanceWizard.hpp"

InstanceSideBar::InstanceSideBar(ContentData &data, InstanceListWidget &instanceListWidget, QWidget *parent)
	: QWidget(parent), m_data(data), m_instanceListWidget(instanceListWidget)
{

	m_addInstanceButton.setText(tr("Add instance"));
	m_editInstanceButton.setText(tr("Edit instance"));
	m_runInstanceButton.setText(tr("Run instance"));
	m_deleteInstanceButton.setText(tr("Delete instance"));

	m_editInstanceButton.setEnabled(false);
	m_runInstanceButton.setEnabled(false);
	m_deleteInstanceButton.setEnabled(false);

	connect(&m_addInstanceButton, &QPushButton::clicked, this, &InstanceSideBar::openWizard);
	connect(&m_editInstanceButton, &QPushButton::clicked, this, &InstanceSideBar::openEditWindow);
	connect(&m_runInstanceButton, &QPushButton::clicked, this, &InstanceSideBar::runInstanceButtonClicked);
	connect(&m_deleteInstanceButton, &QPushButton::clicked, this, &InstanceSideBar::deleteInstanceButtonClicked);

	QVBoxLayout *layout = new QVBoxLayout(this);
	layout->addWidget(&m_addInstanceButton);
	layout->addWidget(&m_editInstanceButton);
	layout->addWidget(&m_runInstanceButton);
	layout->addWidget(&m_deleteInstanceButton);
	layout->addWidget(new QWidget, 1);
}

void InstanceSideBar::toggleButtons() {
	if (m_instanceListWidget.selectedItems().empty()) {
		m_editInstanceButton.setEnabled(false);
		m_runInstanceButton.setEnabled(false);
		m_deleteInstanceButton.setEnabled(false);
	}
	else {
		m_editInstanceButton.setEnabled(true);
		m_runInstanceButton.setEnabled(true);
		m_deleteInstanceButton.setEnabled(true);
	}
}

void InstanceSideBar::openWizard() {
	InstanceWizard *wizard = new InstanceWizard{m_data, this};
	wizard->setModal(true);
	wizard->show();

	connect(wizard, &InstanceWizard::windowRefeshRequested, this, &InstanceSideBar::windowRefeshRequested);
}

#include "InstanceEditWindow.hpp"

void InstanceSideBar::openEditWindow() {
	InstanceEditWindow *window = new InstanceEditWindow{m_data, this};
	window->setModal(true);
	window->show();
}

