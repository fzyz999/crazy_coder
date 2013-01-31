#Qt笔记#

##文本处理相关##
###QPlainTextEdit###
QPlainTextEdit是一个用于纯文本的文本编辑控件。它为打开大文档和快速响应用户输入做过优化。它所使用的技术和QTextEdit相似，但针对纯文本作了相应优化。

*选择一段文本*

要选择文本可以通过QTextCursor类(selection of text is handled by the QTextCursor class)。它提供了创建一个被选择段，检索所选段落和删除所选段落的功能。如果你想选中QPlainTextEdit中的某一段，只需创建一个QTextCursor，然后使用setCursor()让它可见。这个被选段落可以通过copy()拷贝到剪贴板或cut()剪切到剪贴板。可以通过selectAll()选择全文。

*获取QTextDocument*

QPlainText中可以通过document()方法访问其QTextDocument对象。QTextDocument是结构化的符文本文档的容器。

###QSyntaxHighlighter###
QSyntaxHighlighter类可以用于高亮有特定格式的用户数据（最典型的应用就是源代码高亮）。QSyntaxHighlighter会自动高亮QTextEdit(或QPlainTextEdit)的一部分文本。自定义高亮规则首先需要继承QSyntaxHighlighter类，然后重载highlightBlock()函数。这个函数是一个纯虚函数，也就是说，如果不进行重载，QSyntaxHighlighter什么都不会做。
接着，实例化你的QSyntaxHighlighter的子类，将QTextEdit或QTextDocument传给它。例如：

        QTextEdit *editor = new QTextEdit;
        MyHighlighter *highlighter = new MyHighlighter(editor->document());

此后，你的highlightBlock()会在需要的时候被自动地调用。例如：

        void MyHighlighter::highlightBlock(const QString &text)
        {
            QTextCharFormat myClassFormat;
            myClassFormat.setFontWeight(QFont::Bold);
            myClassFormat.setForeground(Qt::darkMagenta);
            QString pattern = "\\bMy[A-Za-z]+\\b";
       
            QRegExp expression(pattern);
            int index = text.indexOf(expression);
            while (index >= 0) {
                int length = expression.matchedLength();
                setFormat(index, length, myClassFormat);
                index = text.indexOf(expression, index + length);
            }
        }

*何时会被自动调用？传入的text是什么？*

使用一下代码进行实验：

        Highlighter::Highlighter(QTextDocument *parent):
            QSyntaxHighlighter(parent)
        {
        }
        
        void Highlighter::highlightBlock(const QString &text)
        {
            qDebug()<<text;
        }
        
然后在相应的QTextEdit中输入：

        return 0;
        
Qt会输出以下调试信息（前面的序号代表输出顺序）：

        1 "" 
        2 "r" 
        3 "re" 
        4 "ret" 
        5 "retu" 
        6 "retur" 
        7 "return" 
        8 "return " 
        9 "return 0" 
        10 "return 0;"
        
**由此可知每次你编辑某一行的内容时，会自动调用highllightBlock()，而你编辑的行的全部内容会作为text传入。**

##图形绘制相关##
绘图是GUI程序必不可少的操作。Qt提供了强大的绘图功能，以满足开发需求。
###QPainter###
QPainter类提供经过高度优化的函数以实现GUI程序所需的大部分绘图操作。它可以绘制任何东西，从简单的线到复杂的图形。还包括文本和图片。它可以向所有继承QPaintDevice类的类进行绘图操作。
也就是说，QPaintDevice相当于一块画布，而QPainter就像一个多功能画笔。那么，那些东西可以作为画布呢？通过QPaintDevice的文档可以发现，继承了它类还是挺多的(文档原文Inherited by: QCustomRasterPaintDevice,QGLFramebufferObject, QGLPixelBuffer, QImage, QPicture, QPixmap, QPrinter, QSvgGenerator, and QWidget)。这就表明，QPainter可以在QWidget上绘制图形，也可以直接在图片上绘制新的图形，还可以在OpenGL的画布上绘制图形。
>*注意*
>一般情况下，如果要在QWidget上绘制，QPainter需要在QWidget的paintEvent成员函数中被实例化，而在函数结束时被析构。

>*可行的应用*
>对图片进行编辑操作
>在QWidget上绘制特殊内容，以实现新的控件
>将QWidget上的内容存成图片


