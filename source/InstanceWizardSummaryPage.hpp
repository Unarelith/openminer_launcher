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
#ifndef INSTANCEWIZARDSUMMARYPAGE_HPP_
#define INSTANCEWIZARDSUMMARYPAGE_HPP_

#include <QLabel>
#include <QWizardPage>

class ContentData;

class InstanceWizardSummaryPage : public QWizardPage {
	public:
		InstanceWizardSummaryPage(ContentData &data, QWidget *parent = nullptr);

	private:
		void initializePage() override;

		ContentData &m_data;

		QLabel *m_label = nullptr;
};

#endif // INSTANCEWIZARDSUMMARYPAGE_HPP_
