#include "comipler.h"

compilerDockWidget::compilerDockWidget(QWidget *parent) :
    QDockWidget(parent)
{
    compilerOutput=new QTextBrowser(this);
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
    compilerOutput->insertPlainText(cpl->version());
    connect(cpl,SIGNAL(readyRead()),this,SLOT(compilerOutputReady()));
    connect(cpl,SIGNAL(finished(int,QProcess::ExitStatus)),this,SLOT(compileFinished(int,QProcess::ExitStatus)));
}

void compilerDockWidget::compilerOutputReady()
{
    QString tmp=QString::fromUtf8(cpl->readAll());
    qDebug()<<tmp;
    compilerOutput->insertPlainText(tmp);
}

void compilerDockWidget::compileFinished(int exitcode, QProcess::ExitStatus exitStatus)
{
    //compilerOutput->insertPlainText(QString::fromUtf8(cpl->readAll().constData()));
    if(exitStatus==QProcess::NormalExit)
        compilerOutput->insertPlainText(tr("compile finished\n"));
    else
        compilerOutput->insertPlainText(tr("compiler crashed\n exit code ")+QString::number(exitcode)+"\n");
}
