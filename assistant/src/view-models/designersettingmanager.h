/***************************************************************************
 *   This file is part of the Bosssoft Assistant Client Project                          *
 *   Copyright (C) 2023 by Bin Yan                                                                *
 *   yanbin@bosssoft.com.cn                                                                        *
 *                                                                                                                   *
 **                   GNU General Public License Usage                                      **
 *                                                                                                                  *
 *   This library is free software: you can redistribute it and/or modify        *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation, either version 3 of the License, or          *
 *   (at your option) any later version.                                                          *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>.   *
 *                                                                                                                   *
 **                  GNU Lesser General Public License                                      **
 *                                                                                                                   *
 *   This library is free software: you can redistribute it and/or modify         *
 *   it under the terms of the GNU Lesser General Public License as             *
 *   published by the Free Software Foundation, either version 3 of the       *
 *   License, or (at your option) any later version.                                         *
 *   You should have received a copy of the GNU Lesser General Public       *
 *   License along with this library.                                                                *
 *   If not, see <http://www.gnu.org/licenses/>.                                            *
 *                                                                                                                   *
 *   This library is distributed in the hope that it will be useful,                     *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of     *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the   *
 *   GNU General Public License for more details.                                          *
 ****************************************************************************/


#ifndef DESIGNERSETTINGMANAGER_H
#define DESIGNERSETTINGMANAGER_H

#include <QObject>
#include <QLocale>
#include <QApplication>
#include <QSettings>

class DesignerSettingManager : public QObject
{
    Q_OBJECT
public:
    explicit DesignerSettingManager(QObject *parent = 0);
    ~DesignerSettingManager();
public slots:
    void getAvailableLanguages(QList<QLocale::Language>* languages);
    QLocale::Language getCurrentDefaultLanguage();
    void currentDefaultLanguageChanged(QLocale::Language language);
private:
    QSettings* m_setting;
};

#endif // DESIGNERSETTINGMANAGER_H
