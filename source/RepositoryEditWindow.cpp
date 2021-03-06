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
#include <QFormLayout>
#include <QMessageBox>
#include <QPushButton>

#include "ContentData.hpp"
#include "RepositoryEditWindow.hpp"

RepositoryEditWindow::RepositoryEditWindow(ContentRepository *repository, ContentData &data, QWidget *parent)
	: m_data(data), m_repository(repository)
{
	setModal(true);

	m_nameEdit = new QLineEdit;
	m_urlEdit = new QLineEdit;

	if (repository) {
		m_nameEdit->setText(repository->name());
		m_urlEdit->setText(repository->url().toString());
	}

	auto *formLayout = new QFormLayout;
	formLayout->addRow("&Name:", m_nameEdit);
	formLayout->addRow("&URL:", m_urlEdit);

	m_okButton = new QPushButton{"&OK"};
	m_cancelButton = new QPushButton{"&Cancel"};

	connect(m_okButton, &QPushButton::clicked, this, &RepositoryEditWindow::saveRepository);
	connect(m_cancelButton, &QPushButton::clicked, this, &QDialog::close);

	auto *buttonLayout = new QHBoxLayout;
	buttonLayout->addWidget(m_okButton);
	buttonLayout->addWidget(m_cancelButton);

	auto *layout = new QVBoxLayout{this};
	layout->addLayout(formLayout);
	layout->addLayout(buttonLayout);
}

void RepositoryEditWindow::saveRepository() {
	QString name = m_nameEdit->text();
	QString url = m_urlEdit->text();
	if (!url.endsWith('/'))
		url.append('/');

	if (name.isEmpty()) {
		QMessageBox::critical(this, QApplication::applicationDisplayName(), "Name required");
		return;
	}

	if (url.isEmpty()) {
		QMessageBox::critical(this, QApplication::applicationDisplayName(), "URL required");
		return;
	}

	m_okButton->setEnabled(false);
	m_cancelButton->setEnabled(false);

	QJsonDocument doc = m_data.session().get(url + "api/");
	QUuid uuid = doc.object().value("uuid").toString();

	if (uuid.isNull()) {
		auto result = QMessageBox::question(this, QApplication::applicationDisplayName(), "Unable to get UUID. This repository won't work.\nDo you want to add it anyway?");
		if (result != QMessageBox::Yes) {
			m_okButton->setEnabled(true);
			m_cancelButton->setEnabled(true);

			return;
		}
	}

	if (m_repository) {
		m_repository->setName(name);
		m_repository->setUrl(url);
		m_repository->setUuid(uuid.toString());
	}
	else {
		ContentRepository repository(m_data.repositoryList().size(), name, url, uuid.toString());
		repository.updateDatabaseTable();
		repository.writeToDatabase();
		m_data.setRepository(m_data.repositoryList().size(), repository);
	}

	accept();
}

