#ifndef CODEEDITOR_H
#define CODEEDITOR_H

#include <QPainter>
#include <QPlainTextEdit>
#include <QTextBlock>

/*!
 * \class CodeEditorConfig
 * \brief CodeEditorConfig provides some functions to config CodeEditor widget.
 *
 * For example, you can change the background color of the line-number-area of the CodeEditor by calling CodeEditorConfig::setLinenumAreaColor() function.
 * All of the CodeEditor in a application should have a same appearance.So we need one CodeEditorConfig to config all CodeEditors. Thus, all member of CodeEditorConfig is static and CodeEditorConfig class can't be instantiated.
 *
 * \fn static void setLinenumAreaColor(QColor &color)
 * \brief set the background color of the LinenumArea. The font color of it will be compute automatically according to the background color.
 * \param color the color of the background.
*/

class CodeEditorConfig
{
public:
    static void setLinenumAreaColor(QColor &color); //font color will be complute automatically
    static const QColor& getLineumAreaBackgroundColor();
    static const QColor& getLineumAreaFontColor();

private:
    CodeEditorConfig(); //to prevent CodeEditorConfig class from being instantiated.
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
