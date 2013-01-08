#include "codeeditor.h"


QColor CodeEditorConfig::linenumAreaBackgroundColor=QColor(Qt::lightGray);
QColor CodeEditorConfig::linenumAreaFontColor=QColor((0x7f+linenumAreaBackgroundColor.red())&0xff,
                                                     (0x7f+linenumAreaBackgroundColor.green())&0xff,
                                                     (0x7f+linenumAreaBackgroundColor.blue())&0xff);

void CodeEditorConfig::setLinenumAreaColor(QColor &color)
{
    linenumAreaBackgroundColor=color;
    linenumAreaFontColor=QColor((0xff+linenumAreaBackgroundColor.red())&0xff,
                                (0xff+linenumAreaBackgroundColor.green())&0xff,
                                (0xff+linenumAreaBackgroundColor.blue())&0xff);
}
const QColor& CodeEditorConfig::getLineumAreaBackgroundColor()
{
    return linenumAreaBackgroundColor;
}
const QColor& CodeEditorConfig::getLineumAreaFontColor()
{
    return linenumAreaFontColor;
}


LinenumArea::LinenumArea(CodeEditor *parent):
    QWidget(parent)
{
    editor=parent;
}

void LinenumArea::setAreaWidth(const int &w)
{
    areaWidth=w;
}

void LinenumArea::paintEvent(QPaintEvent *e)
{
    editor->lineNumPaintEvent(e);
}

int LinenumArea::getAreaWidth()
{
    return areaWidth;
}

CodeEditor::CodeEditor(QWidget *parent):
    QPlainTextEdit(parent)
{
    connect(this, SIGNAL(blockCountChanged(int)), this, SLOT(lineNumberAreaWidth()));
    connect(this, SIGNAL(updateRequest(QRect,int)), this, SLOT(updateLineNumberArea(QRect,int)));
    connect(this,SIGNAL(textChanged()),this,SLOT(onTextChanged()));
    lineNumArea=new LinenumArea(this);
    isFileNameChanged=false;
    modified=false;

    lineNumberAreaWidth();
}

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

void CodeEditor::onTextChanged()
{
    setModified(true);
}

void CodeEditor::setModified(const bool &flag)
{
    modified=flag;
}

bool CodeEditor::isModified()
{
    return modified;
}

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

bool CodeEditor::is_filename_changed()
{
    return isFileNameChanged;
}

QString CodeEditor::get_file_name()
{
    QFileInfo fileInfo(file);
    return fileInfo.fileName();
}

void CodeEditor::set_filename_changed(const bool &bl)
{
    isFileNameChanged=bl;
}


void CodeEditor::resizeEvent(QResizeEvent *e)
{
    QPlainTextEdit::resizeEvent(e);

    QRect cr = contentsRect();
    lineNumArea->setGeometry(QRect(cr.left(), cr.top(), lineNumArea->getAreaWidth(), cr.height()));
}

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

void CodeEditor::updateLineNumberArea(const QRect &rect, int dy)
{
    if (dy)
        lineNumArea->scroll(0, dy);
    else
        lineNumArea->update(0, rect.y(), lineNumArea->width(), rect.height());

    if (rect.contains(viewport()->rect()))
        lineNumberAreaWidth();
}

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
