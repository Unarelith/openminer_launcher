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
#ifndef INSTANCEWIZARDMODPAGE_HPP_
#define INSTANCEWIZARDMODPAGE_HPP_

#include <QDebug>
#include <QTreeWidget>
#include <QWizardPage>

class ContentData;
class ContentMod;

class InstanceWizardModList : public QTreeWidget {
	Q_OBJECT
	Q_PROPERTY(int mod READ getMod)

	public:
		InstanceWizardModList(QWidget *parent = nullptr) : QTreeWidget(parent) {
			connect(this, &QTreeWidget::itemSelectionChanged, this, &InstanceWizardModList::selectMod);
		}

		int getMod() const { return m_mod; }

	signals:
		void modSelectionChanged(int newMod);

	private:
		void selectMod() {
			QList<QTreeWidgetItem *> selectedItems = this->selectedItems();
			if (selectedItems.size() == 0)
				qDebug() << "ERROR: Too many selected items";
			else
				m_mod = selectedItems.at(0)->text(1).toUInt();

			emit modSelectionChanged(m_mod);
		}

		int m_mod;
};

class InstanceWizardModPage : public QWizardPage {
	public:
		InstanceWizardModPage(ContentData &data, QWidget *parent = nullptr);

	private:
		void selectMods();

		ContentData &m_data;

		InstanceWizardModList *m_modListWidget = nullptr;
		ContentMod *m_mod = nullptr;
};

#endif // INSTANCEWIZARDMODPAGE_HPP_
