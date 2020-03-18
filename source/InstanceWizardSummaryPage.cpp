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
#include <QVariant>
#include <QVBoxLayout>

#include "ContentData.hpp"
#include "InstanceWizardSummaryPage.hpp"

InstanceWizardSummaryPage::InstanceWizardSummaryPage(ContentData &data, QWidget *parent) : QWizardPage(parent), m_data(data) {
	setTitle(tr("Summary"));

	m_label = new QLabel();

	m_label->setWordWrap(true);

	QVBoxLayout *layout = new QVBoxLayout;
	layout->addWidget(m_label);
	setLayout(layout);
}

void InstanceWizardSummaryPage::initializePage() {
	m_label->setText(tr(
		"A new instance will be created with:\n\n"
		"- OpenMiner v") + m_data.getEngineVersion(field("engineVersion").toUInt())->name());
}

