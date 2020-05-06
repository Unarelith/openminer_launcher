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
#include <QFormLayout>
#include <QLineEdit>
#include <QPushButton>

#include "RepositoryEditWindow.hpp"

RepositoryEditWindow::RepositoryEditWindow(ContentRepository *repository, QWidget *parent) : m_repository(repository) {
	setModal(true);

	auto *formLayout = new QFormLayout;
	formLayout->addRow("&Name:", new QLineEdit);
	formLayout->addRow("&URL:", new QLineEdit);
	formLayout->addRow("UU&ID:", new QLineEdit);

	auto *okButton = new QPushButton{"&OK"};
	auto *cancelButton = new QPushButton{"&Cancel"};

	connect(okButton, &QPushButton::clicked, this, &QDialog::close); // FIXME
	connect(cancelButton, &QPushButton::clicked, this, &QDialog::close);

	auto *buttonLayout = new QHBoxLayout;
	buttonLayout->addWidget(okButton);
	buttonLayout->addWidget(cancelButton);

	auto *layout = new QVBoxLayout{this};
	layout->addLayout(formLayout);
	layout->addLayout(buttonLayout);
}

