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
#include <QDir>

#include "Utils.hpp"

void Utils::copyDirectory(const QString &src, const QString &dest) {
	QDir dir(src);
	if (!dir.exists())
		return;

	foreach (const QString &d, dir.entryList(QDir::Dirs | QDir::NoDotAndDotDot)) {
		QString destPath = dest + QDir::separator() + d;
		dir.mkpath(destPath);
		copyDirectory(src + QDir::separator() + d, destPath);
	}

	foreach (QString f, dir.entryList(QDir::Files)) {
		QFile::copy(src + QDir::separator() + f, dest + QDir::separator() + f);
	}
}

