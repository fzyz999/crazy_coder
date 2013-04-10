#include "comipler.h"

compilerDockWidget::compilerDockWidget(QWidget *parent) :
    QDockWidget(parent)
{
    currCompilerMsgList=new QStandardItemModel(this);
    compilerOutput=new QListView(this);
    compilerOutput->setModel(currCompilerMsgList);

    QStringList horizontalHeaderLabels;
    horizontalHeaderLabels<<tr("compiler output")<<tr("line number");
    currCompilerMsgList->setHorizontalHeaderLabels(horizontalHeaderLabels);
    currCompilerMsgList->setColumnCount(2);

    compilerOutput->setEditTriggers(QAbstractItemView::NoEditTriggers);

    this->setWidget(compilerOutput);
    setObjectName("compilerDockWidget");

    cpl=NULL;
}

void compilerDockWidget::setCompiler(compiler *_cpl)
{
    if(cpl!=NULL)
    {
        disconnect(cpl,SIGNAL(readyRead()),this,SLOT(compilerOutputReady()));
    }

    cpl=_cpl;
    cpl->setReadChannelMode(QProcess::MergedChannels);
    this->setWindowTitle(_cpl->version());
    connect(cpl,SIGNAL(output(compilerMessage)),this,SLOT(compilerOutputReady(compilerMessage)));
    connect(cpl,SIGNAL(finished(int,QProcess::ExitStatus)),this,SLOT(compileFinished(int,QProcess::ExitStatus)));
}

void compilerDockWidget::compilerOutputReady(compilerMessage msg)
{
    qDebug()<<msg.msg;
    int row=currCompilerMsgList->rowCount();
    QList<QStandardItem*> cplmsg;
    cplmsg<<new QStandardItem(msg.msg)
         <<new QStandardItem(QString::number(msg.lineNum));
    currCompilerMsgList->insertRow(row,cplmsg);
}

void compilerDockWidget::compileFinished(int exitcode, QProcess::ExitStatus exitStatus)
{
    /*
    //compilerOutput->insertPlainText(QString::fromUtf8(cpl->readAll().constData()));
    if(exitStatus==QProcess::NormalExit)
        compilerOutput->insertPlainText(tr("compile finished\n"));
    else
        compilerOutput->insertPlainText(tr("compiler crashed\n exit code ")+QString::number(exitcode)+"\n");*/
}
