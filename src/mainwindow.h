#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMenuBar>
#include <QMessageBox>
#include <QIcon>
#include "codeeditor.h"
#include "tabmanager.h"
#include "cpplexer.h"
#include "comipler.h"
#include "editorconfigdlg.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void about();
    void aboutQt();
    void preference();

protected:
    void closeEvent(QCloseEvent *);

private:
    TabManager *tabManager;
    compilerDockWidget *compilerDock;

    void create_menus();
    void save_settings();
    void restore_settings();
};

#endif // MAINWINDOW_H
