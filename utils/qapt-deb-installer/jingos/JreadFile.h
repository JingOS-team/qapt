/*
 * Copyright (C) 2021 Beijing Jingling Information System Technology Co., Ltd. All rights reserved.
 * 
 * Authors: 
 * Jiashu Yu <yujiashu@jingos.com>
 *
 */

#ifndef JREADFILE_H
#define JREADFILE_H

#include <QObject>
#include <QThread>

class JreadFile : public QObject
{
    Q_OBJECT
public:
    explicit JreadFile(QObject *parent = nullptr);
    void getIconList() ;
    void getFileList();
    void setPath(const QString &path);

signals:
    void iconReadSig(const QStringList &list);
    void fileReadSig(const QStringList &list);
    void strIconReadSig(const QString &strPath, const QString &strIcon);
private:
    QStringList fileList() const;
private:
    QString m_strPath;

};

#endif // JREADFILE_H
