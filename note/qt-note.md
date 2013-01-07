#Qt笔记#

##文本处理相关##
###QPlainTextEdit###
QPlainTextEdit是一个用于纯文本的文本编辑控件。它为打开大文档和快速响应用户输入做过优化。它所使用的技术和QTextEdit相似，但针对纯文本作了相应优化。

*选择一段文本*
要选择文本可以通过QTextCursor类(selection of text is handled by the QTextCursor class)。它提供了创建一个被选择段，检索所选段落和删除所选段落的功能。如果你想选中QPlainTextEdit中的某一段，只需创建一个QTextCursor，然后使用setCursor()让它可见。这个被选段落可以通过copy()拷贝到剪贴板或cut()剪切到剪贴板。可以通过selectAll()选择全文。

*获取QTextDocument*
QPlainText中可以通过document()方法访问其QTextDocument对象。QTextDocument是结构化的符文本文档的容器。



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


