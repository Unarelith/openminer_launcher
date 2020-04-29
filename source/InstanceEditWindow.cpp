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
#include <QFormLayout>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QTabWidget>
#include <QVBoxLayout>

#include "InstanceEditModTab.hpp"
#include "InstanceEditVersionTab.hpp"
#include "InstanceEditWindow.hpp"

InstanceEditWindow::InstanceEditWindow(ContentData &data, QWidget *parent) : QDialog(parent) {
	setWindowTitle("Edit instance...");
	resize(640, 480);

	auto *tabWidget = new QTabWidget;
	tabWidget->addTab(new InstanceEditVersionTab{data}, "Engine");
	tabWidget->addTab(new InstanceEditModTab{data}, "Mods");

	auto *formLayout = new QFormLayout;
	formLayout->addRow("&Name", new QLineEdit);

	auto *hLayout = new QHBoxLayout;
	hLayout->addWidget(new QWidget, 1);
	hLayout->addWidget(new QPushButton{"OK"});
	hLayout->addWidget(new QPushButton{"Cancel"});

	auto *layout = new QVBoxLayout{this};
	layout->addLayout(formLayout);
	layout->addWidget(tabWidget);
	layout->addLayout(hLayout);
}

