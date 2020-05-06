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
#include <QScrollArea>

#include "ContentData.hpp"
#include "NewsTabWidget.hpp"

NewsTabWidget::NewsTabWidget(ContentData &data, QWidget *parent) : QWidget(parent), m_data(data) {
	auto *layoutWidget = new QWidget;
	m_layout = new QVBoxLayout(layoutWidget);

	auto *scrollArea = new QScrollArea;
	scrollArea->setWidget(layoutWidget);
	scrollArea->setWidgetResizable(true);

	auto *globalLayout = new QHBoxLayout(this);
	globalLayout->addWidget(scrollArea);
}

void NewsTabWidget::update() {
	QLayoutItem *item;
	while ((item = m_layout->takeAt(0)) != nullptr) {
		delete item->widget();
		delete item;
	}

	m_widgetList.clear();

    auto newsArticleList = m_data.newsArticleList();
	std::vector<ContentNewsArticle> newsArticles;
	std::transform(newsArticleList.begin(), newsArticleList.end(), std::back_inserter(newsArticles), [] (auto it) { return it.second; });
	std::sort(newsArticles.begin(), newsArticles.end(), [&] (auto &a, auto &b) { return a.date() > b.date(); });

	for (auto &newsArticle : newsArticles) {
		auto *widget = new NewsArticleWidget{this};
		widget->update(newsArticle, m_data);
		m_layout->addWidget(widget);
		m_widgetList.append(widget);
	}

	m_layout->addWidget(new QWidget, 1);
}

