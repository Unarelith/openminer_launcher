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
#include <QVBoxLayout>

#include "ContentNewsArticle.hpp"
#include "NewsArticleWidget.hpp"

NewsArticleWidget::NewsArticleWidget(QWidget *parent) : QWidget(parent) {
	setObjectName("newsArticleWidget");

	m_titleWidget = new QLabel;
	m_titleWidget->setTextInteractionFlags(Qt::TextBrowserInteraction);
	m_titleWidget->setOpenExternalLinks(true);
	m_titleWidget->setWordWrap(true);

	auto titleFont = m_titleWidget->font();
	titleFont.setBold(true);
	m_titleWidget->setFont(titleFont);

	auto* line = new QFrame;
	line->setFrameShape(QFrame::HLine);
	line->setFrameShadow(QFrame::Sunken);

	m_contentWidget = new QLabel;
	m_contentWidget->setTextInteractionFlags(Qt::TextBrowserInteraction);
	m_contentWidget->setOpenExternalLinks(true);
	m_contentWidget->setWordWrap(true);

	auto contentFont = m_contentWidget->font();
	contentFont.setPixelSize(13);
	m_contentWidget->setFont(contentFont);

	auto* line2 = new QFrame;
	line2->setFrameShape(QFrame::HLine);
	line2->setFrameShadow(QFrame::Sunken);

	m_dateWidget = new QLabel;
	m_dateWidget->setAlignment(Qt::AlignRight);

	auto *layout = new QVBoxLayout(this);
	layout->addWidget(m_titleWidget);
	layout->addWidget(line);
	layout->addWidget(m_contentWidget);
	layout->addWidget(line2);
	layout->addWidget(m_dateWidget);
}

void NewsArticleWidget::update(const ContentNewsArticle &article) {
	m_dateWidget->setText(article.date().toString("dddd dd MMMM HH:mm:ss"));

	m_titleWidget->setText(article.title());
	m_contentWidget->setText(article.content());
}

