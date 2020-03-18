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
#ifndef INSTANCEWIZARDVERSIONPAGE_HPP_
#define INSTANCEWIZARDVERSIONPAGE_HPP_

#include <QDebug>
#include <QTreeWidget>
#include <QWizardPage>

class ContentData;
class ContentEngineVersion;

class InstanceWizardVersionList : public QTreeWidget {
	Q_OBJECT
	Q_PROPERTY(int engineVersion READ getEngineVersion)

	public:
		InstanceWizardVersionList(QWidget *parent = nullptr) : QTreeWidget(parent) {
			connect(this, &QTreeWidget::itemSelectionChanged, this, &InstanceWizardVersionList::selectEngineVersion);
		}

		int getEngineVersion() const { return m_engineVersion; }

	signals:
		void engineVersionChanged(int newEngineVersion);

	private:
		void selectEngineVersion() {
			QList<QTreeWidgetItem *> selectedItems = this->selectedItems();
			if (selectedItems.size() == 1)
				m_engineVersion = selectedItems.at(0)->text(1).toUInt();
			else if (selectedItems.size() == 0)
				m_engineVersion = -1;
			else
				qDebug() << "ERROR: Too many selected items";

			emit engineVersionChanged(m_engineVersion);
		}

		int m_engineVersion = -1;
};

class InstanceWizardVersionPage : public QWizardPage {
	public:
		InstanceWizardVersionPage(ContentData &data, QWidget *parent = nullptr);

	private:
		void selectEngineVersion();

		ContentData &m_data;

		InstanceWizardVersionList *m_versionListWidget = nullptr;
		ContentEngineVersion *m_engineVersion = nullptr;
};

#endif // INSTANCEWIZARDVERSIONPAGE_HPP_
