#ifndef CODEEDITOR_H
#define CODEEDITOR_H

#include <QPlainTextEdit>

class CodeEditor : public QPlainTextEdit
{
public:
    CodeEditor();
    CodeEditor(QWidget *parent);

private:
    QWidget *linenumarea;

protected:
    void paintEvent(QPaintEvent *e);
};

#endif // CODEEDITOR_H
