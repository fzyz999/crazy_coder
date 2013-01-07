#include "codeeditor.h"


QColor CodeEditorConfig::linenumAreaBackgroundColor=QColor(Qt::lightGray);
QColor CodeEditorConfig::linenumAreaFontColor=QColor(0xff-linenumAreaBackgroundColor.red(),
                                                     0xff-linenumAreaBackgroundColor.green(),
                                                     0xff-linenumAreaBackgroundColor.blue());
void CodeEditorConfig::setLinenumAreaColor(QColor &color)
{
    linenumAreaBackgroundColor=color;
    linenumAreaFontColor=QColor(0xff-linenumAreaBackgroundColor.red(),
                                0xff-linenumAreaBackgroundColor.green(),
                                0xff-linenumAreaBackgroundColor.blue());
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
    areaWidth=5+editor->fontMetrics().width(QLatin1Char('9'));
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
    lineNumArea=new LinenumArea(this);

    lineNumberAreaWidth();
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

    int space = 5 + fontMetrics().width(QLatin1Char('9')) * digits;

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
