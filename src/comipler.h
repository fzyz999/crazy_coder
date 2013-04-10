#ifndef COMIPLER_H
#define COMIPLER_H

#include <QDockWidget>
#include <QTextBrowser>
#include <QProcess>
#include <QDebug>
#include <QStandardItemModel>
#include <QListView>
#include <QHeaderView>

struct compilerMessage
{
    enum msgType
    {
        error,
        warning,
        others
    };

    msgType type;
    int lineNum;
    QString msg;
};

class compiler : public QProcess
{
    Q_OBJECT

public:
    virtual void startCompile(const QString& ){}
    virtual QString version(){return "";}


signals:
    void output(compilerMessage msg);

};

class compilerDockWidget : public QDockWidget
{
    Q_OBJECT
public:
    explicit compilerDockWidget(QWidget *parent = 0);
    void setCompiler(compiler *_cpl);
    
signals:
    
public slots:
    void compilerOutputReady(compilerMessage msg);
    void compileFinished(int exitcode,QProcess::ExitStatus exitStatus);

private:
    QStandardItemModel *currCompilerMsgList;
    QListView *compilerOutput;
    compiler *cpl;
};

#endif // COMIPLER_H
