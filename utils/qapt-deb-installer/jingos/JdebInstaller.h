/*
 * Copyright (C) 2021 Beijing Jingling Information System Technology Co., Ltd. All rights reserved.
 * 
 * Authors: 
 * Jiashu Yu <yujiashu@jingos.com>
 *
 */

#ifndef JDEBINSTALLER_H
#define JDEBINSTALLER_H

#include <QApt/DebFile>
#include <QApt/Globals>
#include <QApt/Package>
#include <QObject>
#include <QString>

namespace QApt {
    class Backend;
    class Transaction;
    class Backend;
    class DebFile;
}
namespace DebconfKde {
    class DebconfGui;
}

class InstallTreeModel;
class QAbstractItemModel;
class JreadFile;

class JdebInstaller : public QObject
{
    Q_OBJECT
public:
    explicit JdebInstaller(QObject *parent = nullptr);
    ~JdebInstaller();
    enum InstallProcess {
        InstallStart,
        Installing,
        InstallEnd,
        InstallError
    };
    Q_ENUM(InstallProcess)

    /*!
    *func:安装包类型供qml调用
    */
    Q_PROPERTY(InstallProcess installProcess READ installProcess WRITE setInstallProcess NOTIFY installChangedProcess)

    /*!
    *func:图片资源供qml调用
    */
    Q_PROPERTY(QString strIcon READ strIcon WRITE setStrIcon NOTIFY strIconChanged)

    /*!
    *func:dep包名供qml调用
    */
    Q_PROPERTY(QString strDebName READ strDebName WRITE setStrDebName NOTIFY strDebNameChanged)

    /*!
    *func:dep安装时信息供qml调用
    */
    Q_PROPERTY(QString statusText READ statusText WRITE setStatusText NOTIFY statusTextChanged)

    /*!
    *func:treeview的model供qml调用
    */
    Q_PROPERTY(QAbstractItemModel* treeModel READ treeModel WRITE setTreeModel NOTIFY treeModelChanged)

    /*!
    *func:设置descriPtion文本内容
    */
    Q_PROPERTY(QString descriPtionText READ descriPtionText WRITE setDescriPtionText NOTIFY descriPtionChangedText)

    /*!
    *func:设置details文本内容
    */
    Q_PROPERTY(QString detailsText READ detailsText WRITE setDetailsText NOTIFY detailsChangedText)

    /*!
    *func:设置includeFiles文本内容
    */
    Q_PROPERTY(QString includeFilesText READ includeFilesText WRITE setIncludeFilesText NOTIFY includeFilesChangedText)
    /*!
    *func:显示includeFiles文本内容或者加载界面
    */
    Q_PROPERTY(bool showInclude READ showInclude WRITE setShowInclude NOTIFY showIncludeChanged)
    /*!
    *func:设置progress文本内容
    */
    Q_PROPERTY(QString progressText READ progressText WRITE setProgressText NOTIFY progressChangedText)

   InstallProcess installProcess();
   void setInstallProcess(InstallProcess type);
   QString strIcon();
   void setStrIcon(const QString &str);
   QString strDebName();
   void setStrDebName(const QString &str);
   QString statusText();
   void setStatusText(const QString &str);
   bool showInclude();
   void setShowInclude(bool flag);

    /*!
    *func: 获取treeview的model
    *return[QStandardItemModel]:
    */
    QAbstractItemModel *treeModel();
    /*!
    *func:设置treeview的model
    *pragram[model]: 实例化的model
    */
    void setTreeModel(QAbstractItemModel *model);

    /*!
    *func: 获取descriPtion的内容
    *return[str]:
    */
    QString descriPtionText();
    /*!
    *func:设置descriPtion的内容
    *pragram[str]: 程序获取的descriPtion的内容
    */
    void setDescriPtionText(const QString &str);

    /*!
    *func: 获取details的内容
    *return[str]:
    */
    QString detailsText();
    /*!
    *func:设置details的内容
    *pragram[str]: 程序获取的details的内容
    */
    void setDetailsText(const QString &str);

    /*!
    *func: 获取includeFiles的内容
    *return[str]:
    */
    QString includeFilesText();
    /*!
    *func:设置includeFiles的内容
    *pragram[str]: 程序获取的includeFiles的内容
    */
    void setIncludeFilesText(const QString &str);

    /*!
    *func: 获取progress的内容
    *return[str]:
    */
    QString progressText();
    /*!
    *func:设置progress的内容
    *pragram[str]: 程序获取的progress的内容
    */
    void setProgressText(const QString &str);

    /*!
    *func:设置deb包info文件信息
    *pragram[str]: 程序获取的info文件信息
    */
    void setDebFileInfo(const QString &info);

signals:
    /*!
    *func:model改变的sig
    *pragram[model]:数据model
    */
    void treeModelChanged(QAbstractItemModel *model);
    void expandTreeViewSig();
    void descriPtionChangedText(const QString &str);
    void detailsChangedText(const QString &str);
    void includeFilesChangedText(const QString &str);
    void progressChangedText(const QString &str);
    void installChangedProcess();
    void strIconChanged(const QString &str);
    void strDebNameChanged(const QString &str);
    void statusTextChanged(const QString &str);
    void showIncludeChanged(bool flag);
    /* 读取图标信号 */
    void readDepIconSig();
    /* 读取文件信息信号 */
    void readFileInfoSig();

public slots:
    /*!
    *func:点击安装槽
    */
    void installDebFile();
private slots:
    void transactionStatusChanged(QApt::TransactionStatus status);
    void errorOccurred(QApt::ErrorCode error);
    void updateTerminal(const QString &message);
    /* 读取图标信号 */
    void onReadDepIconSig(const QString &strPath, const QString &strIcon);
    /* 读取文件信息信号 */
    void onReadFileInfoSig(const QStringList &fileList);

private:
    /*!
    *func:显示打开缓存错误信息
    */
    void initError();
    bool checkDeb();
    void compareDebWithCache();
    QString maybeAppendArchSuffix(const QString &pkgName, bool checkingConflicts = false);
    QApt::PackageList checkConflicts();
    QApt::Package *checkBreaksSystem();
    bool satisfyDepends();
    void setupTransaction(QApt::Transaction *trans);

private:
    /* treeView的model */
    InstallTreeModel *m_pInstallTreeModel;
    /* 安装debtab内容 */
    QString m_strDescriPtion;
    QString m_strDetails;
    QString m_strIncludeFiles;
    QString m_strProgress;
    InstallProcess m_eInstallProcess;
    QString m_strIcon;
    QString m_strDebName;
    QString m_strStatusText;
    bool m_bshowInclude;
    /* 文件读取类 */
    JreadFile *m_pJreadFile;
    QThread *m_pThread;

    // Backend stuff
    QApt::Backend *m_pBackend;
    QApt::DebFile *m_pDebFile;
    QApt::Transaction *m_pTrans;
    QString m_strForeignArch;

    //Misc
    QString m_strStatus;
    QString m_strVersionTitle;
    QString m_strVersionInfo;

    QApt::CacheState m_oldCacheState;
};

#endif // JDEBINSTALLER_H
