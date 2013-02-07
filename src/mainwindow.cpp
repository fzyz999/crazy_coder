#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowIcon(QIcon(":/images/logo.png"));


    QString locale = QLocale::system().name();

    tabManager=new TabManager(this);
    setCentralWidget(tabManager);

    compilerDock=new compilerDockWidget(this);
    addDockWidget(Qt::BottomDockWidgetArea,compilerDock);
    compilerDock->setCompiler(&CodeEditor::cc);

    create_menus();

    restore_settings();
    LexerConfig::restore_settings();
    CodeEditorConfig::restore_settings();
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
    fileMenu->addAction(tr("save all"),tabManager,SLOT(save_all()));
    fileMenu->addSeparator();
    fileMenu->addAction(tr("&quit"),this,SLOT(close()));

    QMenu *editMenu=menuBar()->addMenu(tr("&Edit"));
    editMenu->addAction(tr("&undo"),tabManager,SLOT(undo()));
    editMenu->addAction(tr("&redo"),tabManager,SLOT(redo()));
    editMenu->addSeparator();
    editMenu->addAction(tr("&cut"),tabManager,SLOT(cut()));
    editMenu->addAction(tr("&copy"),tabManager,SLOT(copy()));
    editMenu->addAction(tr("&paste"),tabManager,SLOT(paste()));
    editMenu->addSeparator();
    editMenu->addAction(tr("&preference"),this,SLOT(preference()));

     QMenu *buildMenu=menuBar()->addMenu(tr("&Build"));
     buildMenu->addAction(tr("&compile"),tabManager,SLOT(compile_current_file()));

    QMenu *helpMenu=menuBar()->addMenu(tr("&Help"));
    helpMenu->addAction(tr("&about"),this,SLOT(about()));
    helpMenu->addAction(tr("about Qt"),this,SLOT(aboutQt()));
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
    LexerConfig::save_settings();
    CodeEditorConfig::save_settings();
}

void MainWindow::about()
{
    QMessageBox::about(this,tr("About"),tr("Crazy Coder is a light IDE for OIer/ACMer"));
}

void MainWindow::aboutQt()
{
    QMessageBox::aboutQt(this,tr("about Qt"));
}

void MainWindow::preference()
{
    tabManager->addTab(new editorConfigDlg(this),tr("preference"));
}
