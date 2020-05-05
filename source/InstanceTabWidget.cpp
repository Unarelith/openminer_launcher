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
#include <QHBoxLayout>

#include "InstanceTabWidget.hpp"

InstanceTabWidget::InstanceTabWidget(ContentData &data, QWidget *parent) : QWidget(parent), m_data(data) {
	QHBoxLayout *layout = new QHBoxLayout{this};;
	layout->addWidget(&m_instanceListWidget);
	layout->addWidget(&m_instanceSideBar);

	connect(&m_instanceListWidget, &InstanceListWidget::windowRefeshRequested, this, &InstanceTabWidget::windowRefeshRequested);
	connect(&m_instanceListWidget, &InstanceListWidget::itemSelectionChanged, &m_instanceSideBar, &InstanceSideBar::toggleButtons);
	connect(&m_instanceSideBar, &InstanceSideBar::windowRefeshRequested, this, &InstanceTabWidget::windowRefeshRequested);
	connect(&m_instanceSideBar, &InstanceSideBar::addInstanceButtonClicked, &m_instanceListWidget, &InstanceListWidget::addInstance);
	connect(&m_instanceSideBar, &InstanceSideBar::editInstanceButtonClicked, &m_instanceListWidget, &InstanceListWidget::editInstance);
	connect(&m_instanceSideBar, &InstanceSideBar::runInstanceButtonClicked, &m_instanceListWidget, &InstanceListWidget::runInstance);
	connect(&m_instanceSideBar, &InstanceSideBar::deleteInstanceButtonClicked, &m_instanceListWidget, &InstanceListWidget::deleteInstance);
}

void InstanceTabWidget::update() {
	m_instanceListWidget.update();
}

