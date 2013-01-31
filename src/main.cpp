/*!
 * \mainpage
 * \section intro_sec Introduction
 * Crazy Coder is an smart and light IDE
 * \section license_info License
 * This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
 */

#include <QApplication>
#include <QSettings>
#include <QTranslator>
#include <QLocale>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QCoreApplication::setOrganizationName("Crazy Coder Team");
    QCoreApplication::setApplicationName("Crazy Coder");
    QCoreApplication::setApplicationVersion("0.0.1");

    QTranslator translator;
    translator.load(QLocale::system().name());
    a.installTranslator(&translator);

    MainWindow w;
    w.show();
    
    return a.exec();
}
