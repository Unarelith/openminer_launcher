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
#include <QLineEdit>
#include <QFormLayout>
#include <QRegExpValidator>

#include "InstanceWizardInfoPage.hpp"

InstanceWizardInfoPage::InstanceWizardInfoPage(QWidget *parent) : QWizardPage(parent) {
	setTitle(tr("Informations"));

	QLabel *label = new QLabel(tr("Please choose a name for your instance"));
	label->setWordWrap(true);

	QRegExp re("^[a-zA-Z0-9_]+$");
	QRegExpValidator *validator = new QRegExpValidator(re, this);

	// FIXME: Check if instance already exists with a custom validator
	QLineEdit *nameEdit = new QLineEdit;
	nameEdit->setValidator(validator);

	QFormLayout *layout = new QFormLayout;
	layout->addRow("Name:", nameEdit);
	setLayout(layout);

	registerField("instanceName*", nameEdit);
}

