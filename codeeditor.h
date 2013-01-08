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

/*!
 * \class CodeEditorConfig
 * \brief CodeEditorConfig provides some functions to config CodeEditor widget.
 *
 * For example, you can change the background color of the line-number-area of the CodeEditor by calling CodeEditorConfig::setLinenumAreaColor() function.
 * All of the CodeEditor in a application should have a same appearance.So we need one CodeEditorConfig to config all CodeEditors. Thus, all member of CodeEditorConfig is static and CodeEditorConfig class can't be instantiated.
*/
class CodeEditorConfig
{
public:
    /*!
    * \fn static void setLinenumAreaColor(QColor &color)
    * \brief set the background color of the LinenumArea. The font color of it will be compute automatically according to the background color.
    * \param color the color of the background.
    */
    static void setLinenumAreaColor(QColor &color); //font color will be complute automatically
    /*!
    * \fn static const QColor& getLineumAreaBackgroundColor()
    * \brief get the background color of the LinenumArea.
    */
    static const QColor& getLineumAreaBackgroundColor();
    /*!
    * \fn static const QColor& getLineumAreaFontColor()
    * \brief get the font color of the LinenumArea.
    */
    static const QColor& getLineumAreaFontColor();

private:
    /*!
     * \fn CodeEditorConfig()
     * \brief a private constructor to prevent CodeEditorConfig class from being instantiated.
    */
    CodeEditorConfig();

    /*!
     * \brief linenumAreaBackgroundColor is the background color of line-number area.
     */
    static QColor linenumAreaBackgroundColor;
    /*!
     * \brief linenumAreaFontColor is the font color of line-number area.
     */
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
    bool open(const QString& filePath);
    bool save();
    bool close();
    bool is_filename_changed();
    void set_filename_changed(const bool &bl);
    QString get_file_name();

    bool isModified();
    void setModified(const bool& flag);

protected:
    void resizeEvent(QResizeEvent *e);

private:
    QFile file;
    bool isFileNameChanged;
    bool modified;

private slots:
    void lineNumberAreaWidth();
    void updateLineNumberArea(const QRect &rect, int dy);
    void onTextChanged();

private:
    LinenumArea *lineNumArea;
};

#endif // CODEEDITOR_H
