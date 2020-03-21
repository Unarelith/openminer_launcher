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
#include <QLabel>
#include <QVBoxLayout>

#include "ContentData.hpp"
#include "InstanceWizardModPage.hpp"

InstanceWizardModPage::InstanceWizardModPage(ContentData &data, QWidget *parent) : QWizardPage(parent), m_data(data) {
	setTitle(tr("Select mods"));

	QLabel *label = new QLabel(tr("Please select the mods you want to use:"));
	label->setWordWrap(true);

	m_modListWidget = new InstanceWizardModList;
	m_modListWidget->setHeaderLabels({"", tr("ID"), tr("Name"), tr("Creation date")});
	m_modListWidget->setRootIsDecorated(false);
	m_modListWidget->setSortingEnabled(true);
	m_modListWidget->sortItems(2, Qt::AscendingOrder);
	m_modListWidget->setColumnWidth(0, 27);
	m_modListWidget->hideColumn(1);
	m_modListWidget->setSelectionMode(QAbstractItemView::SingleSelection);
	m_modListWidget->setFocusPolicy(Qt::NoFocus);

	registerField("mod*", m_modListWidget, "mod", SIGNAL(modSelectionChanged(int)));

	auto &modList = m_data.modList();
	for (auto &it : modList) {
		// FIXME: This should test the latest INSTALLED mod version, instead of the latest
		ContentModVersion *latestModVersion = m_data.getModVersion(it.second.latestVersionID());
		if (latestModVersion && latestModVersion->state() == ContentModVersion::State::Downloaded) {
			auto *item = new QTreeWidgetItem(m_modListWidget);
			if (latestModVersion->state() == ContentModVersion::State::Available)
				item->setIcon(0, QIcon(":/checkbox_off"));
			else if (latestModVersion->state() == ContentModVersion::State::Downloaded)
				item->setIcon(0, QIcon(":/checkbox_on"));

			item->setText(1, QString::number(it.second.id()));
			item->setText(2, it.second.name());
			item->setText(3, it.second.date().toString());
		}
	}

	QVBoxLayout *layout = new QVBoxLayout;
	layout->addWidget(label);
	layout->addWidget(m_modListWidget);

	setLayout(layout);
}

