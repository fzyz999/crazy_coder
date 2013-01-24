#include "codeeditor.h"

/*!
 * \class CodeEditorConfig
 * \brief CodeEditorConfig provides some functions to config CodeEditor widget.
 *
 * For example, you can change the background color of the line-number-area of the CodeEditor by calling CodeEditorConfig::setLinenumAreaColor() function.
 * All of the CodeEditor in a application should have a same appearance.So we need one CodeEditorConfig to config all CodeEditors. Thus, all member of CodeEditorConfig is static and CodeEditorConfig class can't be instantiated.
 */

/*!
 * \brief linenumAreaBackgroundColor is the background color of line-number area.
 */
QColor CodeEditorConfig::linenumAreaBackgroundColor=QColor(Qt::lightGray);
QColor
/*!
 * \brief linenumAreaFontColor is the font color of line-number area.
 */
CodeEditorConfig::linenumAreaFontColor=QColor((0x7f+linenumAreaBackgroundColor.red())&0xff,
                                                     (0x7f+linenumAreaBackgroundColor.green())&0xff,
                                                     (0x7f+linenumAreaBackgroundColor.blue())&0xff);
/*!
 * \brief editorFont is the font of the editor
 */
QFont CodeEditorConfig::editorFont("mono");

/*!
 * \brief a private constructor to prevent CodeEditorConfig class from being instantiated.
 *
 * One CodeEditorConfig is enough to configure all CodeEditors. So all CodeEditors,which are in the same application, have same colors and same fonts and so on.
 */
CodeEditorConfig::CodeEditorConfig()
{
}

/*!
 *\brief set the background color of the LinenumArea. The font color of it will be compute automatically according to the background color.
* \param color the color of the background.
*/
void CodeEditorConfig::setLinenumAreaColor(QColor &color)
{
    linenumAreaBackgroundColor=color;
    linenumAreaFontColor=QColor((0xff+linenumAreaBackgroundColor.red())&0xff,
                                (0xff+linenumAreaBackgroundColor.green())&0xff,
                                (0xff+linenumAreaBackgroundColor.blue())&0xff);
}
/*!
 * \brief get the background color of the LinenumArea.
 * \return return the background color of the LinenumArea.
 */
const QColor& CodeEditorConfig::getLineumAreaBackgroundColor()
{
    return linenumAreaBackgroundColor;
}
/*!
 * \brief get the font color of the LinenumArea.
 * \return return  the font color of the LinenumArea.
 */
const QColor& CodeEditorConfig::getLineumAreaFontColor()
{
    return linenumAreaFontColor;
}

/*!
 * \brief set the font of the editor.
 * \param font the font of the editor will be set.
 */
void CodeEditorConfig::setEditorFont(QFont &font)
{
    editorFont=font;
}
/*!
 * \brief get the font color of the editor.
 */
const QFont& CodeEditorConfig::getEditorFont()
{
    return editorFont;
}

/*!
 * \class LinenumArea
 * \brief LinenumArea is the line-number panel of the CodeEditor.
 *
 * it shows line number
 * \todo code fold feature is needed
 */
LinenumArea::LinenumArea(CodeEditor *parent):
    QWidget(parent)
{
    editor=parent;
}

/*!
 * \brief set the area width of the LinenumArea widget.
 *
 * this functions will be called by CodeEditor::lineNumberAreaWidth() when the width of LinenumArea widget should be changed(For example, when more digits is needed to be shown, linenumArea widget's width will be recompuated and be changed)
 * \param w the new width of LinenumArea
 */
void LinenumArea::setAreaWidth(const int &w)
{
    areaWidth=w;
}

/*!
 * \brief when recieving a paint event, LinenumArea widget will directly call the Code Editor::lineNumPaintEvent() function.
 * \param e a QPaintEvent type pointer
 */
void LinenumArea::paintEvent(QPaintEvent *e)
{
    editor->lineNumPaintEvent(e);
}
/*!
 * \brief get the width of the LinenumArea widget.
 * \return return the width of the LinenumArea widget.
 */
int LinenumArea::getAreaWidth()
{
    return areaWidth;
}

/*!
 * \class CodeEditor
 * \brief CodeEditor class is a code editor widget with lexer and LinenumArea widget.
 */

/*!
 * \brief initialize the CodeEditor
 *
 * First connect signals(blockCountChanged,updateRequest,textChanged) with slots
 * Then set the lineNumArea and lexer
 * Last set editor font and call lineNumberAreaWidth() functions to set the width of the LinenumArea widget
 * \todo only support a simple cpp lexer(in fact, it's only a simple highlighter),pascal lexer is needed.
 * \param parent
 */
CodeEditor::CodeEditor(QWidget *parent):
    QPlainTextEdit(parent)
{
    connect(this, SIGNAL(blockCountChanged(int)), this, SLOT(lineNumberAreaWidth()));
    connect(this, SIGNAL(updateRequest(QRect,int)), this, SLOT(updateLineNumberArea(QRect,int)));
    connect(this,SIGNAL(textChanged()),this,SLOT(onTextChanged()));

    lineNumArea=new LinenumArea(this);
    isFileNameChanged=false;
    modified=false;
    lexer=new cppLexer(this->document());
    editorLanguageMode=CodeEditor::cpp;

    lineNumberAreaWidth();
    setFont(CodeEditorConfig::getEditorFont());
}

/*!
 * \brief open a file
 * \param filePath the path of the file which will be opened.
 * \return true if successed, otherwise return false.
 */
bool CodeEditor::open(const QString &filePath)
{
    file.setFileName(filePath);
    if(file.open(QIODevice::ReadWrite))
    {
        QTextStream dataIn(&file);
        setPlainText(dataIn.readAll());
        modified=false;

        file.close();
        return true;
    }

    return false;
}

/*!
 * \brief save file
 * \return true if successed, otherwise return false.
 */
bool CodeEditor::save()
{
    if(file.fileName().isEmpty())
    {
        QSettings settings("settings.ini",QSettings::IniFormat);
        file.setFileName(QFileDialog::getSaveFileName(
                             this,tr("Save File"),
                             settings.value("files/historyDir").toString()));

        //if getSaveFileName() is canceled by user
        if(file.fileName().isEmpty())
        {
            isFileNameChanged=false;
            return false;
        }

        isFileNameChanged=true;
    }

    if(file.open(QIODevice::WriteOnly))
    {
        QTextStream dataOut(&file);
        dataOut<<toPlainText();
    }
    else
        return false;

    file.close();
    modified=false;
    return true;
}

/*!
 * \brief Because the text has been modified, modified should be true.
 */
void CodeEditor::onTextChanged()
{
    setModified(true);
}

/*!
 * \brief set whether the text has been modified
 * \param flag Has the text been modified? true or false
 */
void CodeEditor::setModified(const bool &flag)
{
    modified=flag;
}

/*!
 * \brief is the text modified
 * \return true if the text has been modified, otherwise return false.
 */
bool CodeEditor::isModified()
{
    return modified;
}

/*!
 * \brief save the unsaved file when close.
 * \return true if the file has been saved, otherwise return false.
 */
bool CodeEditor::close()
{
    if(isModified())
    {
        QMessageBox msgbox(this);

        msgbox.setText(tr("The document has been modified."));
        msgbox.setInformativeText(tr("Do you want to save your changes?"));
        msgbox.setStandardButtons(QMessageBox::Save|QMessageBox::Discard|QMessageBox::Cancel);
        msgbox.setDefaultButton(QMessageBox::Save);

        int ret=msgbox.exec();

        switch (ret)
        {
        case QMessageBox::Save:
            // Save was clicked
            if(!save())
            {
                //Save file failed
                msgbox.setText(tr("Saving file failed!"));
                msgbox.setInformativeText(tr("Please save it again"));
                msgbox.setStandardButtons(QMessageBox::Ok);
                msgbox.setDefaultButton(QMessageBox::Ok);
                msgbox.setIcon(QMessageBox::Warning);

                msgbox.exec();
                return false;
            }

            break;
        case QMessageBox::Discard:
            // Don't Save was clicked
            return true;

            break;
        case QMessageBox::Cancel:
            // Cancel was clicked
            return false;

            break;
        default:
            // should never be reached
            qWarning("codeeditor.cpp: switch(ret) reached an unexcepted line!");
            break;
        }
    }

    return true;
}

/*!
 * \brief is filename changed
 * \return true if the filename has been changed, otherwise return false.
 */
bool CodeEditor::is_filename_changed()
{
    return isFileNameChanged;
}

/*!
 * \brief returns the name of the file, excluding the path.
 * \return the name of the file
 */
QString CodeEditor::get_file_name()
{
    QFileInfo fileInfo(file);
    return fileInfo.fileName();
}

/*!
 * \brief set whether the filename has been changed
 * \param flag Has the filename has been changed? true or false
 */
void CodeEditor::set_filename_changed(const bool &bl)
{
    isFileNameChanged=bl;
}

/*!
 * \brief resizeEvent
 * \param e QResizeEvent
 */
void CodeEditor::resizeEvent(QResizeEvent *e)
{
    QPlainTextEdit::resizeEvent(e);

    QRect cr = contentsRect();
    lineNumArea->setGeometry(QRect(cr.left(), cr.top(), lineNumArea->getAreaWidth(), cr.height()));
}

/*!
 * \brief CodeEditor::lineNumberAreaWidth compute the width of the lineNumArea. The computing base on the number of the digits of blockCount().
 */
void CodeEditor::lineNumberAreaWidth()
{
    int digits = 1;
    int max = qMax(1, blockCount());
    while (max >= 10)
    {
        max /= 10;
        digits++;
    }

    int space = 10 + fontMetrics().width(QLatin1Char('9')) * digits;

    lineNumArea->setAreaWidth(space);

    setViewportMargins(space, 0, 0, 0);
}

/*!
 * \brief update the lineNumArea
 * \param rect
 * \param dy
 */
void CodeEditor::updateLineNumberArea(const QRect &rect, int dy)
{
    if (dy)
        lineNumArea->scroll(0, dy);
    else
        lineNumArea->update(0, rect.y(), lineNumArea->width(), rect.height());

    if (rect.contains(viewport()->rect()))
        lineNumberAreaWidth();
}

/*!
 * \brief draw the line number
 * \param e a QPaintEvent
 */
void CodeEditor::lineNumPaintEvent(QPaintEvent *e)
{
    QPainter painter(lineNumArea);
    painter.fillRect(e->rect(),CodeEditorConfig::getLineumAreaBackgroundColor());

    QTextBlock block=firstVisibleBlock();
    int blockNumber=block.blockNumber();
    int top = (int) blockBoundingGeometry(block).translated(contentOffset()).top();
    int bottom = top + (int) blockBoundingRect(block).height();

    while (block.isValid() && top <= e->rect().bottom())
    {
        if (block.isVisible() && bottom >= e->rect().top())
        {
            QString number = QString::number(blockNumber + 1);
            painter.setPen(CodeEditorConfig::getLineumAreaFontColor());
            painter.drawText(0, top, lineNumArea->width(), fontMetrics().height(),
                             Qt::AlignRight, number);
        }

        block = block.next();
        top = bottom;
        bottom = top + (int) blockBoundingRect(block).height();
        blockNumber++;
    }
}
