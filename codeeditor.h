#ifndef CODEEDITOR_H
#define CODEEDITOR_H

#include <QPainter>
#include <QPlainTextEdit>
#include <QTextBlock>


class CodeEditorConfig
{
public:
    static void setLineumAreaColor(QColor &color); //font color will be complute automatically
    static const QColor& getLineumAreaBackgroundColor();
    static const QColor& getLineumAreaFontColor();

private:
    CodeEditorConfig(); //the member functions in this class is like global functions in C program language.
    static QColor linenumAreaBackgroundColor;
    static QColor linenumAreaFontColor;
};

class CodeEditor;

class LinenumArea : public QWidget
{

public:
    LinenumArea(CodeEditor *parent);
    void setAreaWidth(const int &w);
    QSize sizeHint() const {
        return QSize(areaWidth, 0);
    }
    int getAreaWidth();

protected:
    void paintEvent(QPaintEvent *e);

private:
    int areaWidth;
    CodeEditor *editor;
};

class CodeEditor : public QPlainTextEdit
{
    Q_OBJECT

public:
    CodeEditor(QWidget *parent);
    void lineNumPaintEvent(QPaintEvent *e);

protected:
    void resizeEvent(QResizeEvent *e);

private slots:
    void lineNumberAreaWidth();
    void updateLineNumberArea(const QRect &rect, int dy);

private:
    LinenumArea *lineNumArea;
};

#endif // CODEEDITOR_H
