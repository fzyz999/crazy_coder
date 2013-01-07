#简介#
　　Doxygen是一个将源代码中的特殊注释转化为文档（参考手册）的工具。

#官方文档#
　　[在线文档](http://www.doxygen.nl/manual/index.html)

#可处理语言#
C/C++
Java
Objective-C
Python
IDL (Corba, Microsoft及KDE-DCOP类型)
Fortran
VHDL
PHP
C#

#可产生的文件格式#
HTML
XML
LaTeX
RTF(MS-Word)
PostScript
Unix Man Page

#安装#
##Linux##
>Ubuntu: sudo apt-get install doxygen

#使用#
　　doxygen会将按特殊格式书写的注释转为文档
##注释风格##
　　通常代码可以附上一个注释块来对代码进行解释，一个注释块由一行或者多行组成。通常一个注释块包括一个简要说明（brief）和一个详细说明（detailed），这两部分都是可选的。 可以有多种方式标识出doxygen可识别的注释块。
>1.JavaDoc类型的多行注释。
>>/**
>>/...text...
>>*/
>2.QT样式的多行注释。
>>/*!
>>/...text...
>>*/
>3.使用至少两行c++注释块，在每行开始处加斜杠或惊叹号
>>///
>>/// …text….
>>///
>或
>>//!
>>//! …text….
>>//!

##简明描述##
　　简明描述有多种可行方式
　　在注释块头部使用\brief命令。这个命令在自然段结束时结束，空一行后紧接详细描述。例如：
>/*! \brief Brief description.
>*         Brief description continued.
>*
>*  Detailed description starts here.
>*/
　　其余方式详见doxygen文档。

