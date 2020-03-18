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

#include "InstanceWizard.hpp"
#include "InstanceWizardVersionPage.hpp"
#include "InstanceWizardSummaryPage.hpp"

InstanceWizard::InstanceWizard(ContentData &data, QWidget *parent) : QWizard(parent), m_data(data) {
	addIntroPage();
	addPage(new InstanceWizardVersionPage{data, this});
	addPage(new InstanceWizardSummaryPage{data, this});

	setWindowTitle(tr("Instance Wizard"));
}

void InstanceWizard::addIntroPage() {
	QWizardPage *page = new QWizardPage{this};
	page->setTitle(tr("Introduction"));

	QLabel *label = new QLabel(tr(
		"This wizard will create an instance with the "
		"engine version, mod, game, texture pack "
		"that you decide to use."
	));

	label->setWordWrap(true);

	QVBoxLayout *layout = new QVBoxLayout;
	layout->addWidget(label);
	page->setLayout(layout);

	addPage(page);
}

