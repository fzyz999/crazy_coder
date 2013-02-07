#include "gcc.h"

gcc::gcc()
{
    gccPath="g++";
}

QString gcc::version()
{
    QStringList arg;
    arg<<"--version";
    start(gccPath,arg);
    waitForFinished();
    return QString::fromUtf8(readAllStandardOutput().constData());
}

void gcc::startCompile(const QString &filePath)
{
    QFileInfo fileInfo(filePath);

    qDebug()<<filePath;

    QStringList arg;
    arg<<filePath<<"-g"<<"-Wall"<<"-o";

#ifdef Q_OS_WIN32
    arg<<fileInfo.absolutePath()+fileInfo.completeBaseName()+".exe";     //windows
#else
    arg<<fileInfo.absolutePath()+"/"+fileInfo.completeBaseName();    //unix/unix-like system
#endif

    qDebug()<<arg;

    start(gccPath,arg);
    //waitForFinished();
}
