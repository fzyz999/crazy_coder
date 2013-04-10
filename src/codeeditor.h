#ifndef CODEEDITOR_H
#define CODEEDITOR_H

#include <QFile>
#include <QTextStream>
#include <QPainter>
#include <QPlainTextEdit>
#include <QTextBlock>
#include <QSettings>
#include <QFileDialog>
#include <QMessageBox>
#include <QFont>
#include <QColor>

#include "cpplexer.h"
#include "gcc.h"

class cppLexer;

class CodeEditorConfig
{
public:
    static void setLinenumAreaColor(const QColor &color); //font color will be complute automatically
    static const QColor& getLineumAreaBackgroundColor();
    static const QColor& getLineumAreaFontColor();

    static void setEditorFont(const QFont &font);
    static const QFont& getEditorFont();

    static void restoreSettings();
    static void saveSettings();

private:
    CodeEditorConfig();

    static QColor linenumAreaBackgroundColor;
    static QColor linenumAreaFontColor;

    static QFont editorFont;
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
    enum languageMode
    {
        c,
        cpp
    };

    CodeEditor(QWidget *parent);
    void lineNumPaintEvent(QPaintEvent *e);
    bool open(const QString& filePath);
    bool save();

    void compile();

    static gcc cc;
    friend class cppLexer;

signals:
    void filenameChanged(QString newName);

protected:
    void resizeEvent(QResizeEvent *e);
    void closeEvent(QCloseEvent *e);

private:
    QFile file;
    bool modified;
    cppLexer *lexer;
    languageMode editorLanguageMode;

private slots:
    void lineNumberAreaWidth();
    void updateLineNumberArea(const QRect &rect, int dy);
    void onModificationChanged(bool changed);

private:
    LinenumArea *lineNumArea;

};

#endif // CODEEDITOR_H
