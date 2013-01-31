#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMenuBar>
#include <QMessageBox>
#include <QIcon>
#include "codeeditor.h"
#include "tabmanager.h"
#include "cpplexer.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void about();
    void aboutQt();

protected:
    void closeEvent(QCloseEvent *);

private:
    TabManager *tabManager;

    void create_menus();
    void save_settings();
    void restore_settings();
};

#endif // MAINWINDOW_H
