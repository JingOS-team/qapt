/*
 * Copyright (C) 2021 Beijing Jingling Information System Technology Co., Ltd. All rights reserved.
 * 
 * Authors: 
 * Jiashu Yu <yujiashu@jingos.com>
 *
 */

#include "JdebInstaller.h"
#include "InstallTreeModel.h"
#include "JreadFile.h"

#include <QApplication>
#include <QDebug>
#include <QThread>
#include <QDir>
#include <QtAlgorithms>
#include <QFileInfo>
#include <QStringBuilder>
#include <KLocalizedString>
#include <KProtocolManager>
#include <KMessageBox>
#include <KMessageDialog>
#include <KStandardGuiItem>
#include <KFormat>
#include <KTextEdit>



#include <apt-pkg/debversion.h>
#include <apt-pkg/pkgsystem.h>
#include <apt-pkg/version.h>
// LibQApt/DebconfKDE includes
#include <QApt/Backend>
#include <QApt/Config>
#include <QApt/DependencyInfo>
#include <QApt/Transaction>
#include <QApt/DebFile>
#include <QApt/Package>

#include <DebconfGui.h>



JdebInstaller::JdebInstaller(QObject *parent)
    : QObject(parent)
    ,m_pInstallTreeModel(new InstallTreeModel(this))
    ,m_strProgress("")
    ,m_eInstallProcess(InstallStart)
    ,m_bshowInclude(true)
    ,m_pJreadFile(new JreadFile())
    ,m_pThread(new QThread())
    , m_pBackend(new QApt::Backend(this))
    , m_pTrans(nullptr)
{

}
JdebInstaller::~JdebInstaller()
{
    delete m_pJreadFile;
    m_pThread->quit();
    m_pThread->wait();
    delete m_pThread;
}
JdebInstaller::InstallProcess JdebInstaller::installProcess()
{
    return m_eInstallProcess;
}

void JdebInstaller::setInstallProcess(JdebInstaller::InstallProcess type)
{
    m_eInstallProcess=type;
    emit installChangedProcess();
}


QString JdebInstaller::strIcon()
{
    return m_strIcon;
}

void JdebInstaller::setStrIcon(const QString &str)
{
    m_strIcon=str;
    emit strIconChanged(m_strIcon);
}

QString JdebInstaller::strDebName()
{
    return m_strDebName;
}

void JdebInstaller::setStrDebName(const QString &str)
{
    m_strDebName=str;
    emit strDebNameChanged(str);
}
QString JdebInstaller::statusText()
{
    return m_strStatusText;
}
void JdebInstaller::setStatusText(const QString &str)
{
    m_strStatusText=str;
    emit statusTextChanged(m_strStatusText);
}

bool JdebInstaller::showInclude()
{
    return m_bshowInclude;
}

void JdebInstaller::setShowInclude(bool flag)
{
     m_bshowInclude =flag;
     emit showIncludeChanged(m_bshowInclude);
}

QAbstractItemModel *JdebInstaller::treeModel()
{
    return m_pInstallTreeModel;
}

void JdebInstaller::setTreeModel(QAbstractItemModel *model)
{
    m_pInstallTreeModel=dynamic_cast<InstallTreeModel*>(model);
    emit treeModelChanged(m_pInstallTreeModel);
}

QString JdebInstaller::descriPtionText()
{
    return m_strDescriPtion;
}

void JdebInstaller::setDescriPtionText(const QString &str)
{
    m_strDescriPtion=str;
    emit descriPtionChangedText(m_strDescriPtion);
}

QString JdebInstaller::detailsText()
{
    return m_strDetails;
}

void JdebInstaller::setDetailsText(const QString &str)
{
    m_strDetails=str;
    emit detailsChangedText(m_strDetails);
}

QString JdebInstaller::includeFilesText()
{
    return m_strIncludeFiles;
}

void JdebInstaller::setIncludeFilesText(const QString &str)
{
    m_strIncludeFiles=str;
    emit includeFilesChangedText(str);
}

QString JdebInstaller::progressText()
{
    return m_strProgress;
}

void JdebInstaller::setProgressText(const QString &str)
{
    m_strProgress=str;
    emit progressChangedText(m_strProgress);
}

void JdebInstaller::setDebFileInfo(const QString &info)
{
    setStrIcon("/usr/share/icons/jing/SwiMachine/icon.svg");
    if (!m_pBackend->init()){
        initError();
        return;
    }
    QApt::FrontendCaps caps = (QApt::FrontendCaps)(QApt::DebconfCap);
    m_pBackend->setFrontendCaps(caps);

    QFileInfo fi(info);
    m_pDebFile = new QApt::DebFile(fi.absoluteFilePath());

    if (!m_pDebFile->isValid()) {
        m_strStatusText=i18nc("@info","It does not appear to be a valid Debian package file");
        setStatusText(m_strStatusText);
        setInstallProcess(InstallError);
        return;
    }
    m_oldCacheState=m_pBackend->currentCacheState();
    m_pJreadFile->setPath(info);
    m_pJreadFile->moveToThread(m_pThread);
    m_pThread->start();
    connect(this,&JdebInstaller::readDepIconSig,m_pJreadFile,&JreadFile::getIconList);
    connect(this,&JdebInstaller::readFileInfoSig,m_pJreadFile,&JreadFile::getFileList);
    connect(m_pJreadFile,&JreadFile::strIconReadSig,this,&JdebInstaller::onReadDepIconSig);
    connect(m_pJreadFile,&JreadFile::fileReadSig,this,&JdebInstaller::onReadFileInfoSig);
    emit readDepIconSig();

    setStrDebName("load icon: " +m_pDebFile->packageName());

    // Details tab widgets
    QString shortDesc = m_pDebFile->shortDescription();

    QString longDesc = m_pDebFile->longDescription();

    setDescriPtionText(shortDesc+"\n\n" + longDesc);
    QString strDetails;
    strDetails=QString(i18nc("@info","Version:"))+QString(m_pDebFile->version())+
            i18nc("@info","\nInstalled Size:")+QString(KFormat().formatByteSize(m_pDebFile->installedSize() * 1024))+"\n";
    strDetails=QString(i18nc("@info","Maintainer:"))+strDetails+m_pDebFile->maintainer()+i18nc("@info","\nCategory:")+
            m_pDebFile->section()+i18nc("@info","\nHomepage:")+m_pDebFile->homepage();
    setDetailsText(strDetails);
    emit readFileInfoSig();
    bool canInstall = checkDeb();
    if(!canInstall){
        setInstallProcess(InstallError);
        return;
    }
    setStatusText(m_strStatus);

    QList<QApt::Package *> excluded;
    excluded.append(m_pBackend->package(m_pDebFile->packageName()));
    auto changes = m_pBackend->stateChanges(m_oldCacheState, excluded);
    m_pInstallTreeModel->addPackages(changes);
    emit expandTreeViewSig();
}
void JdebInstaller::initError()
{
    QString details = m_pBackend->initErrorMessage();
    QString title = i18nc("@title:window", "Initialization error");
    m_strStatusText=i18nc("@info","Initialization error\nThe package system could not "
                    "be initialized, yourconfiguration may be broken.")+details;
//    KMessageBox::detailedError(this, m_strStatusText, details, title);
//    exit(-1);
    setStatusText(m_strStatusText);
    setInstallProcess(InstallError);
}
bool JdebInstaller::checkDeb()
{
    QStringList arches = m_pBackend->architectures();
    arches.append(QLatin1String("all"));
    QString debArch = m_pDebFile->architecture();

    // Check if we support the arch at all
    if (debArch != m_pBackend->nativeArchitecture()) {
        if (!arches.contains(debArch)) {
            // Wrong arch
            m_strStatus = i18nc("@info", "Error: Wrong architecture '%1'", debArch);
            m_strStatus.prepend(QLatin1String("<font color=\"#ff0000\">"));
            m_strStatus.append(QLatin1String("</font>"));
            return false;
        }
        // We support this foreign arch
        m_strForeignArch = debArch;
    }
    compareDebWithCache();
    QApt::PackageList conflicts = checkConflicts();
    if (!conflicts.isEmpty()) {
        return false;
    }
    QApt::Package *willBreak = checkBreaksSystem();
    if (willBreak) {
        m_strStatus = i18nc("@info Error string when installing would break an existing package",
                               "Error: Breaks the existing package \"%1\"",
                               willBreak->name());
        m_strStatus.prepend(QLatin1String("<font color=\"#ff0000\">"));
        m_strStatus.append(QLatin1String("</font>"));
        return false;
    }
    if (!satisfyDepends()) {
        // create status message
        m_strStatus = i18nc("@info", "Error: Cannot satisfy dependencies");
        m_strStatus.prepend(QLatin1String("<font color=\"#ff0000\">"));
        m_strStatus.append(QLatin1String("</font>"));
        return false;
    }
    int toInstall = m_pBackend->markedPackages().size();
    if (toInstall) {
        m_strStatus = i18ncp("@label A note saying that additional packages are needed",
                                "Requires the installation of %1 additional package.",
                                "Requires the installation of %1 additional packages",
                                toInstall);
    } else {
        m_strStatus = i18nc("@info", "All dependencies are satisfied.");
    }
    return true;
}
void JdebInstaller::compareDebWithCache()
{
    QApt::Package *pkg = m_pBackend->package(m_pDebFile->packageName());
    if (!pkg) {
        return;
    }
    QString version = m_pDebFile->version();
    int res = QApt::Package::compareVersion(m_pDebFile->version(), pkg->availableVersion());
    if (res == 0 && !pkg->isInstalled()) {
        m_strVersionTitle = i18nc("@info", "The same version is available in a software channel.");
        m_strVersionInfo = i18nc("@info", "It is recommended to install the software from the channel instead");
    } else if (res > 0) {
        m_strVersionTitle = i18nc("@info", "An older version is available in a software channel.");
        m_strVersionInfo = i18nc("@info", "It is recommended to install the version from the "
                                       "software channel, since it usually has more support.");
    } else if (res < 0) {
        m_strVersionTitle = i18nc("@info", "A newer version is available in a software channel.");
        m_strVersionInfo = i18nc("@info", "It is strongly advised to install the version from the "
                                       "software channel, since it usually has more support.");
    }
}

QString JdebInstaller::maybeAppendArchSuffix(const QString &pkgName, bool checkingConflicts)
{
    // Trivial cases where we don't append
    if (m_strForeignArch.isEmpty())
        return pkgName;

    QApt::Package *pkg = m_pBackend->package(pkgName);
    if (!pkg || pkg->architecture() == QLatin1String("all"))
        return pkgName;
    // Real multiarch checks
    QString multiArchName = pkgName % ':' % m_strForeignArch;
    QApt::Package *multiArchPkg = m_pBackend->package(multiArchName);

    // Check for a new dependency, we'll handle that later
    if (!multiArchPkg)
        return multiArchName;

    // Check the multi arch state
    QApt::MultiArchType type = multiArchPkg->multiArchType();

    // Add the suffix, unless it's a pkg that can satify foreign deps
    if (type == QApt::MultiArchForeign)
        return pkgName;

    // If this is called as part of a conflicts check, any not-multiarch
    // enabled package is a conflict implicitly
    if (checkingConflicts && type == QApt::MultiArchSame)
        return pkgName;
    return multiArchName;
}

QApt::PackageList JdebInstaller::checkConflicts()
{
    QApt::PackageList conflictingPackages;
    QList<QApt::DependencyItem> conflicts = m_pDebFile->conflicts();

    QApt::Package *pkg = 0;
    QString packageName;
    bool ok = true;
    foreach(const QApt::DependencyItem &item, conflicts) {
        foreach (const QApt::DependencyInfo &info, item) {
            packageName = maybeAppendArchSuffix(info.packageName(), true);
            pkg = m_pBackend->package(packageName);

            if (!pkg) {
                // FIXME: Virtual package, must check provides
                continue;
            }

            std::string pkgVer = pkg->version().toStdString();
            std::string depVer = info.packageVersion().toStdString();

            ok = _system->VS->CheckDep(pkgVer.c_str(),
                                       info.relationType(),
                                       depVer.c_str());

            if (ok) {
                // Group satisfied
                break;
            }
        }

        if (!ok && pkg) {
            conflictingPackages.append(pkg);
        }
    }
    return conflictingPackages;
}

QApt::Package *JdebInstaller::checkBreaksSystem()
{
    QApt::PackageList systemPackages = m_pBackend->availablePackages();
    std::string debVer = m_pDebFile->version().toStdString();

    foreach (QApt::Package *pkg, systemPackages) {
        if (!pkg->isInstalled()) {
            continue;
        }
        // Check for broken depends
        foreach(const QApt::DependencyItem &item, pkg->depends()) {
            foreach (const QApt::DependencyInfo &dep, item) {
                if (dep.packageName() != m_pDebFile->packageName()) {
                    continue;
                }

                std::string depVer = dep.packageVersion().toStdString();

                if (!_system->VS->CheckDep(debVer.c_str(), dep.relationType(),
                                           depVer.c_str())) {
                    return pkg;
                }
            }
        }

        // Check for existing conflicts against the .deb
        // FIXME: Check provided virtual packages too
        foreach(const QApt::DependencyItem &item, pkg->conflicts()) {
            foreach (const QApt::DependencyInfo &conflict, item) {
                if (conflict.packageName() != m_pDebFile->packageName()) {
                    continue;
                }

                std::string conflictVer = conflict.packageVersion().toStdString();

                if (_system->VS->CheckDep(debVer.c_str(),
                                          conflict.relationType(),
                                          conflictVer.c_str())) {
                    return pkg;
                }
            }
        }
    }
    return 0;
}

bool JdebInstaller::satisfyDepends()
{
    QApt::Package *pkg = 0;
    QString packageName;

    foreach(const QApt::DependencyItem &item, m_pDebFile->depends()) {
        bool oneSatisfied = false;
        foreach (const QApt::DependencyInfo &dep, item) {
            packageName = maybeAppendArchSuffix(dep.packageName());
            pkg = m_pBackend->package(packageName);
            if (!pkg) {
                // FIXME: virtual package handling
                continue;
            }
            std::string debVersion = dep.packageVersion().toStdString();

            // If we're installed, see if we already satisfy the dependency
            if (pkg->isInstalled()) {
                std::string pkgVersion = pkg->installedVersion().toStdString();

                if (_system->VS->CheckDep(pkgVersion.c_str(),
                                          dep.relationType(),
                                          debVersion.c_str())) {
                    oneSatisfied = true;
                    break;
                }
            }

            // else check if cand ver will satisfy, then mark
            std::string candVersion = pkg->availableVersion().toStdString();

            if (!_system->VS->CheckDep(candVersion.c_str(),
                                       dep.relationType(),
                                       debVersion.c_str())) {
                continue;
            }

            pkg->setInstall();

            if (!pkg->wouldBreak()) {
                oneSatisfied = true;
                break;
            }
        }
        if (!oneSatisfied) {
            return false;
        }
    }
    return true;
}

void JdebInstaller::transactionStatusChanged(QApt::TransactionStatus status)
{
    switch (status) {
    case QApt::SetupStatus:
    case QApt::WaitingStatus:
    case QApt::AuthenticationStatus:
        m_strStatusText=i18nc("@info"," Starting ");
        setStatusText(m_strStatusText);
        break;
    case QApt::WaitingMediumStatus:
    case QApt::WaitingLockStatus:
    case QApt::WaitingConfigFilePromptStatus:
        m_strStatusText=i18nc("@info"," Waiting ");
        setStatusText(m_strStatusText);
        break;
    case QApt::RunningStatus:
        setInstallProcess(Installing);
        m_strStatusText=i18nc("@info"," The same version is available in a software channel.\n "
                        "It is recommended to install the software from the channel instead");
        setStatusText(m_strStatusText);
        break;
    case QApt::LoadingCacheStatus:
        m_strStatusText=i18nc("@info"," Loading Software List ");
        setStatusText(m_strStatusText);
        break;
    case QApt::DownloadingStatus:
        m_strStatusText=i18nc("@info"," Downloading Packages");
        setStatusText(m_strStatusText);
        break;
    case QApt::CommittingStatus:
        break;
    case QApt::FinishedStatus:
        if (m_pTrans->role() == QApt::CommitChangesRole) {
            delete m_pTrans;
            // Dependencies installed, now go for the deb file
            m_pTrans = m_pBackend->installFile(*m_pDebFile);
            setupTransaction(m_pTrans);
            m_pTrans->run();
        }else if (m_pTrans->role() == QApt::InstallFileRole) {
            m_strProgress=m_strProgress+i18nc("@info","Done");
            setProgressText(m_strProgress);
            m_strProgress.clear();
            setInstallProcess(InstallEnd);
            m_strStatusText=i18nc("@info","Installation successful");
            setStatusText(m_strStatusText);
        }else {
        }
        break;
    default:
        break;
    }
}
void JdebInstaller::updateTerminal(const QString &message)
{
    m_strProgress=m_strProgress+message;
    setProgressText(m_strProgress);
}
void JdebInstaller::errorOccurred(QApt::ErrorCode error)
{
    QString text;
    QString title;
    QString details;

    switch (error) {
        case QApt::InitError: {
        m_strStatusText=i18nc("@info","Installation failed\nThe package system could not be initialized, your configuration may be broken");
        setStatusText(m_strStatusText);
            break;
        }
        case QApt::WrongArchError:
        m_strStatusText=i18nc("@info","Installation failed\nThis package is incompatible with your computer");
        setStatusText(m_strStatusText);
            break;
        default:
            break;
    }

}
void JdebInstaller::installDebFile()
{
    if (m_pBackend->markedPackages().size()) {
        m_pTrans = m_pBackend->commitChanges();
    } else {
        m_pTrans = m_pBackend->installFile(*m_pDebFile);
    }
    setupTransaction(m_pTrans);
    m_pTrans->run();
}

void JdebInstaller::setupTransaction(QApt::Transaction *trans)
{
    // Provide proxy/locale to the transaction
    if (KProtocolManager::proxyType() == KProtocolManager::ManualProxy) {
        trans->setProxy(KProtocolManager::proxyFor("http"));
    }

    trans->setLocale(QLatin1String(setlocale(LC_MESSAGES, 0)));

//    trans->setDebconfPipe(m_commitWidget->pipe());
//    m_commitWidget->setTransaction(m_pTrans);

    connect(m_pTrans, SIGNAL(statusChanged(QApt::TransactionStatus)),
            this, SLOT(transactionStatusChanged(QApt::TransactionStatus)));
    connect(m_pTrans, SIGNAL(errorOccurred(QApt::ErrorCode)),
            this, SLOT(errorOccurred(QApt::ErrorCode)));
    connect(m_pTrans, SIGNAL(statusDetailsChanged(QString)),
            this, SLOT(updateTerminal(QString)));
}
/* 读取图标信号 */
void JdebInstaller::onReadDepIconSig(const QString &strPath, const QString &strIcon)
{
    setStrDebName(m_pDebFile->packageName());
    if(strPath.isEmpty()||strIcon.isEmpty())
        return;
    QString finalPath = strPath + strIcon;
    if (QFile::exists(finalPath)) {
        setStrIcon(finalPath);
    }
}
/* 读取文件信息信号 */
void JdebInstaller::onReadFileInfoSig(const QStringList &fileList)
{
    QString filesString;
    foreach (const QString &file, fileList) {
        if (!file.trimmed().isEmpty()) {
            filesString.append(file + '\n');
        }
    }
    setShowInclude(false);
    setIncludeFilesText(filesString);
}
