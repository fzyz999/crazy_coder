#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    CodeEditor *editor=new CodeEditor(this);
    editor->show();
    setCentralWidget(editor);
}

MainWindow::~MainWindow()
{
    
}
