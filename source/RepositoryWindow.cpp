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
#include <QPushButton>
#include <QTreeWidget>
#include <QVBoxLayout>

#include "ContentData.hpp"
#include "RepositoryEditWindow.hpp"
#include "RepositoryWindow.hpp"

RepositoryWindow::RepositoryWindow(ContentData &data, QWidget *parent) : QDialog(parent), m_data(data) {
	setModal(true);

	auto *repositoryList = new QTreeWidget;
	repositoryList->setHeaderLabels({tr("ID"), tr("Name"), tr("URL"), tr("UUID")});
	repositoryList->setRootIsDecorated(false);
	repositoryList->setSortingEnabled(true);
	repositoryList->hideColumn(0);

	auto &repositories = data.repositoryList();
	for (auto &it : repositories) {
		auto *item = new QTreeWidgetItem(repositoryList);
		item->setText(0, QString::number(it.second.id()));
		item->setText(1, it.second.name());
		item->setText(2, it.second.url().toString());
		item->setText(3, it.second.uuid().toString());
	}

	auto *addButton = new QPushButton{"Add"};
	auto *editButton = new QPushButton{"Edit"};
	auto *removeButton = new QPushButton{"Remove"};

	connect(addButton, &QPushButton::clicked, this, &RepositoryWindow::addRepository);
	connect(editButton, &QPushButton::clicked, this, &RepositoryWindow::editRepository);
	connect(removeButton, &QPushButton::clicked, this, &RepositoryWindow::removeRepository);

	auto *horizontalLayout = new QHBoxLayout;
	horizontalLayout->addWidget(addButton);
	horizontalLayout->addWidget(editButton);
	horizontalLayout->addWidget(removeButton);

	auto *layout = new QVBoxLayout(this);
	layout->addWidget(repositoryList);
	layout->addLayout(horizontalLayout);
}

void RepositoryWindow::addRepository() {
	auto *window = new RepositoryEditWindow{nullptr, this};
	window->show();
}

void RepositoryWindow::editRepository() {
	auto *window = new RepositoryEditWindow{nullptr, this}; // FIXME
	window->show();
}

void RepositoryWindow::removeRepository() {
	// TODO: Confirmation dialog
	// TODO: Remove the repository from m_data
}

