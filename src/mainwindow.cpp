#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    tabManager=new TabManager(this);
    setCentralWidget(tabManager);

    create_menus();
    restore_settings();
    LexerConfig::setup();
}

MainWindow::~MainWindow()
{
}

void MainWindow::create_menus()
{
    QMenu *fileMenu=menuBar()->addMenu(tr("&File"));

    fileMenu->addAction(tr("&new"),tabManager,SLOT(new_file()));
    fileMenu->addAction(tr("&open"),tabManager,SLOT(open_file()));
    fileMenu->addSeparator();
    fileMenu->addAction(tr("&save"),tabManager,SLOT(save()));
    fileMenu->addAction(tr("&save all"),tabManager,SLOT(save_all()));
    fileMenu->addSeparator();
    fileMenu->addAction(tr("&quit"),this,SLOT(close()));

}

void MainWindow::save_settings()
{
    QSettings settings("settings.ini",QSettings::IniFormat);
    settings.beginGroup("MainWindow");
    settings.setValue("Geometry",saveGeometry());
    settings.setValue("State",saveState());
    settings.endGroup();
}

void MainWindow::restore_settings()
{
    QSettings settings("settings.ini",QSettings::IniFormat);
    settings.beginGroup("MainWindow");
    restoreGeometry(settings.value("Geometry").toByteArray());
    restoreState(settings.value("State").toByteArray());
    settings.endGroup();
}

void MainWindow::closeEvent(QCloseEvent *)
{
    save_settings();
    LexerConfig::shutdown();
}
