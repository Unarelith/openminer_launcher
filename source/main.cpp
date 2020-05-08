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
#include <QCommandLineParser>
#include <QFile>

#include "MainWindow.hpp"

int main(int argc, char **argv) {
	QApplication app(argc, argv);
	QApplication::setApplicationName("openminer_launcher");
	QApplication::setApplicationDisplayName("OpenMiner Launcher");
	QApplication::setApplicationVersion("0.0.1");

	qSetMessagePattern("[%{time hh:mm:ss}] [%{if-debug}D%{endif}%{if-info}I%{endif}%{if-warning}W%{endif}%{if-critical}C%{endif}%{if-fatal}F%{endif}] [%{appname}] %{message}");

	QFile style(":/theme-default");
	style.open(QIODevice::OpenModeFlag::ReadOnly);
	app.setStyleSheet(style.readAll());

	// QCommandLineParser parser;
	// parser.addHelpOption();
	// parser.addVersionOption();
	// parser.addOptions({
	// 	{{"a", "api-source"},
	// 		QCoreApplication::translate("main", "Use <source> as the server for the API."),
	// 		QCoreApplication::translate("main", "source")},
	// });
    //
	// parser.process(app);
    //
	// QString apiSource = parser.value("a");

	MainWindow win;
	app.exec();
}

