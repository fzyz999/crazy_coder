#ifndef GCC_H
#define GCC_H

#include <QFileInfo>
#include <QDebug>
#include "comipler.h"

class gcc : public compiler
{
public:
    gcc();
    void startCompile(const QString &filePath);
    QString version();

private:
    QString gccPath;
};

#endif // GCC_H
