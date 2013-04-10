#ifndef GCC_H
#define GCC_H

#include <QFileInfo>
#include <QRegExp>
#include <QDebug>
#include "comipler.h"

class gcc : public compiler
{
    Q_OBJECT

public:
    gcc();
    void startCompile(const QString &filePath);
    QString version();

public slots:
    void onOutputReady();

private:
    QString gccPath;
};

#endif // GCC_H
