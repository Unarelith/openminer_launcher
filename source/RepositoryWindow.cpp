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
#include <QPushButton>
#include <QVBoxLayout>

#include "ContentData.hpp"
#include "RepositoryEditWindow.hpp"
#include "RepositoryWindow.hpp"

RepositoryWindow::RepositoryWindow(ContentData &data, QWidget *parent) : QDialog(parent), m_data(data) {
	setModal(true);

	m_repositoryList = new QTreeWidget;
	m_repositoryList->setHeaderLabels({tr("ID"), tr("Name"), tr("URL"), tr("UUID")});
	m_repositoryList->setRootIsDecorated(false);
	m_repositoryList->setSortingEnabled(true);
	m_repositoryList->hideColumn(0);

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
	layout->addWidget(m_repositoryList);
	layout->addLayout(horizontalLayout);

	update();
}

void RepositoryWindow::update() {
	m_repositoryList->clear();

	auto &repositoryList = m_data.repositoryList();
	for (auto &it : repositoryList) {
		auto *item = new QTreeWidgetItem(m_repositoryList);
		item->setText(0, QString::number(it.second.id()));
		item->setText(1, it.second.name());
		item->setText(2, it.second.url().toString());
		item->setText(3, it.second.uuid().toString());
	}
}

void RepositoryWindow::addRepository() {
	auto *window = new RepositoryEditWindow{nullptr, m_data, this};
	connect(window, &QDialog::accepted, this, &RepositoryWindow::update);
	window->show();
}

void RepositoryWindow::editRepository() {
	QList<QTreeWidgetItem *> selectedItems = m_repositoryList->selectedItems();
	if (selectedItems.size() == 1) {
		unsigned int repositoryID = selectedItems.at(0)->text(0).toUInt();
		ContentRepository *repository = m_data.getRepository(repositoryID);

		auto *window = new RepositoryEditWindow{repository, m_data, this};
		connect(window, &QDialog::accepted, this, &RepositoryWindow::update);
		window->show();
	}
}

void RepositoryWindow::removeRepository() {
	QList<QTreeWidgetItem *> selectedItems = m_repositoryList->selectedItems();
	if (selectedItems.size() == 1) {
		auto result = QMessageBox::question(this, QApplication::applicationDisplayName(), "Do you really want to remove this repository?");
		if (result != QMessageBox::Yes)
			return;

		unsigned int repositoryID = selectedItems.at(0)->text(0).toUInt();
		m_data.removeRepository(repositoryID);

		update();
	}
}

