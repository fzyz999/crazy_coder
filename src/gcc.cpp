#include "gcc.h"

gcc::gcc()
{
    gccPath="g++";
    setReadChannelMode(QProcess::MergedChannels);
}

QString gcc::version()
{
    QStringList arg;
    arg<<"-dumpversion";
    start(gccPath,arg);
    waitForFinished();
    return QString("g++ ")+QString::fromUtf8(readAllStandardOutput().constData());
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

    connect(this,SIGNAL(readyRead()),this,SLOT(onOutputReady()));
}

void gcc::onOutputReady()
{
    char str_msg[1024];
    while(readLine(str_msg,1024))
    {
        QString msg=QString::fromUtf8(str_msg);
        QRegExp regexp(".*:\\d+");
        int match=regexp.indexIn(msg);
        if (match!=-1)
        {
            QString tmp=msg.mid(match,regexp.matchedLength());
            regexp.setPattern("\\d+");
            match=regexp.indexIn(tmp);
            int lineNum=tmp.mid(match).toInt();
            compilerMessage compilerMsg;
            compilerMsg.lineNum=lineNum;
            compilerMsg.msg=msg;

            regexp.setPattern("\[-W.*\]");
            match=regexp.indexIn(msg);
            if(match==-1)   //case error
                compilerMsg.type=compilerMessage::error;
            else    //case warning
                compilerMsg.type=compilerMessage::warning;

            emit output(compilerMsg);
        }
        else
        {
            compilerMessage compilerMsg;
            compilerMsg.lineNum=-1;
            compilerMsg.msg=msg;
            compilerMsg.type=compilerMessage::others;
            emit output(compilerMsg);
        }
    }
}
