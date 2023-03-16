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


#ifndef UTILITY_H
#define UTILITY_H

#include <QCoreApplication>
#include <QDir>
#include <qsysinfo.h>
#include <QMessageBox>
#include <QTimer>
#include <QSettings>

namespace UTILITY
{
    inline QString getAbstractPath(const QString& relativedPath)
    {

           if(QDir::isAbsolutePath(relativedPath))
           {
               return relativedPath;
           }

           QString _path = relativedPath;
           if(relativedPath.startsWith("./") || relativedPath.startsWith(".\\"))
           {
              _path = relativedPath.mid(2, -1);
           }

           QDir _workspace(QCoreApplication::applicationDirPath());
           return _workspace.absoluteFilePath(_path);
    }

    inline QString getCpuArchitecture()
    {
        QString arch = QSysInfo::currentCpuArchitecture();
        if (arch.compare("i386") == 0)
            return "x86";
        if (arch.compare("x86_64") == 0)
            return "amd64";
        if (arch.compare("arm64") == 0)
            return "aarch64";
        if (arch.compare("mips64") == 0)
            return "mips64el";
        return  arch;
    }

    inline QString getPlatform()
    {
#ifdef Q_OS_WIN
        return "windows";
#else
        return "linux";
#endif
    }

    inline QString getOs()
    {
#ifdef Q_OS_WIN
        return "windows";
#elif defined Q_OS_MAC
        return "mac";
#elif defined Q_OS_LINUX
        return "linux";
#else
        return "";
#endif
    }

    inline int BsMessagebox(QWidget *parent, const QString &title,
                            const QString& text,
                            const QString& button0Text,
                            const QString& button1Text = QString(),
                            const QString& button2Text = QString())
    {
        QMessageBox* box = new QMessageBox(parent);
        box->setAttribute(Qt::WA_DeleteOnClose);
        box->setText(text);
        box->setWindowTitle(title);


        if (!button1Text.isEmpty()){
            if (!button2Text.isEmpty()){
                box->setStandardButtons(QMessageBox::Ok|QMessageBox::No|QMessageBox::Cancel);
                box->setButtonText(QMessageBox::Cancel, button1Text);
            }
            else
              box->setStandardButtons(QMessageBox::Ok|QMessageBox::No);
            box->setButtonText(QMessageBox::No, button1Text);
        }
        else
            box->setStandardButtons(QMessageBox::Ok);
        box->setButtonText(QMessageBox::Ok, button0Text);

        QTimer::singleShot(15*1000, box, &QMessageBox::close);
        return box->exec();
    }

    inline QVariant getConfigFromIni(const QString& file, const QString& key, const QVariant& defaultValue = "")
    {
        if (!QFileInfo::exists(file))
            return  defaultValue;
        QSettings sets(file, QSettings::IniFormat);
        if (!sets.contains(key))
            return  defaultValue;
        return  sets.value(key, defaultValue);
    }
}


#endif // UTILITY_H
