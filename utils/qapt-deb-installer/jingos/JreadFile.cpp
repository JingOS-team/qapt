/*
 * Copyright (C) 2021 Beijing Jingling Information System Technology Co., Ltd. All rights reserved.
 * 
 * Authors: 
 * Jiashu Yu <yujiashu@jingos.com>
 *
 */

#include "JreadFile.h"
#include <QProcess>
#include <QStringList>
#include <QTemporaryFile>
#include <QDir>
#include <QLatin1String>
#include <QString>
#include <QStringBuilder>

JreadFile::JreadFile(QObject *parent) : QObject(parent)
{

}

QStringList JreadFile::fileList() const
{
    QTemporaryFile tempFile;
    if (!tempFile.open()) {
        return QStringList();
    }

    QString tempFileName = tempFile.fileName();
    QProcess dpkg;
    QProcess tar;

    // dpkg --fsys-tarfile filename
    QString program = QLatin1String("dpkg --fsys-tarfile ") + m_strPath;

    dpkg.setStandardOutputFile(tempFileName);
    dpkg.start(program);
    dpkg.waitForFinished();

    QString program2 = QLatin1String("tar -tf ") + tempFileName;
    tar.start(program2);
    tar.waitForFinished();

    QString files = tar.readAllStandardOutput();

    QStringList filesList = files.split('\n');
    filesList.removeFirst(); // First entry is the "./" entry
    filesList.removeAll(QLatin1String("")); // Remove empty entries

    // Remove non-file directory listings
    for (int i = 0; i < filesList.size() - 1; ++i) {
        if (filesList.at(i+1).contains(filesList.at(i))) {
            filesList[i] = QString(QLatin1Char(' '));
        }
        filesList.removeAll(QChar::fromLatin1(' '));
    }
    qSort(filesList);
    return filesList;
}
void JreadFile::getFileList()
{
    emit fileReadSig(fileList());
}

void JreadFile::setPath(const QString &path)
{
    m_strPath=path;
}
void JreadFile::getIconList()
{
    QStringList fileNames = fileList();
    QStringList iconsList;
    foreach (const QString &fileName, fileNames) {
        if (fileName.startsWith(QLatin1String("./usr/share/icons"))) {
            iconsList << fileName;
        }
    }

    // XPM as a fallback. It's really not pretty when scaled up
    if (iconsList.isEmpty()) {
        foreach (const QString &fileName, fileNames) {
            if (fileName.startsWith(QLatin1String("./usr/share/pixmaps"))) {
                iconsList << fileName;
            }
        }
    }
    qSort(iconsList);
    emit iconReadSig(iconsList);

    QString iconPath;
    if (!iconsList.isEmpty()) {
        iconPath = iconsList.last();
    }
    QDir tempDir = QDir::temp();
    tempDir.mkdir(QLatin1String("qapt-deb-installer"));

    QString destPath = QDir::tempPath() + QLatin1String("/qapt-deb-installer/");
    //iconPath, destPath

    //(const QString &fileName, const QString &destination)
    QTemporaryFile tempFile;
    if (tempFile.open()) {
        QString tempFileName = tempFile.fileName();

        // dpkg --fsys-tarfile filename
        QString program = QLatin1String("dpkg --fsys-tarfile ") + m_strPath;

        QProcess dpkg;
        dpkg.setStandardOutputFile(tempFileName);
        dpkg.start(program);
        dpkg.waitForFinished();

        QString program2 = QLatin1String("tar -xf") % tempFileName%
                QLatin1String(" -C ") % destPath % ' ' % iconPath;

        QProcess tar;
        tar.start(program2);
        tar.waitForFinished();

        tar.exitCode();
    }
    emit strIconReadSig(destPath,iconPath);
}
