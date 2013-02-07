#ifndef COMIPLER_H
#define COMIPLER_H

#include <QDockWidget>
#include <QTextBrowser>
#include <QProcess>
#include <QDebug>

class compiler : public QProcess
{
public:
    virtual void startCompile(const QString &filePath){}
    virtual QString version(){return "";}
};

class compilerDockWidget : public QDockWidget
{
    Q_OBJECT
public:
    explicit compilerDockWidget(QWidget *parent = 0);
    void setCompiler(compiler *_cpl);
    
signals:
    
public slots:
    void compilerOutputReady();
    void compileFinished(int exitcode,QProcess::ExitStatus exitStatus);

private:
    QTextBrowser *compilerOutput;
    compiler *cpl;
};

#endif // COMIPLER_H
