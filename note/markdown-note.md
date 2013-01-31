#为何使用Markdown？#
Markdown易于学习也易于编辑，且可以转成PDF、HTML等多种格式，github也支持显示markdown。很方便

#基本语法#
##标题##
标题可以使用=（最高阶标题）或-（第二阶标题）例如：

    H1
    ==
    H2
    --
    
或者在行手插入1～6个#。
如：
    #H1
    ##H2
    ######H6
如果为了美观，可以在行尾加入#（数量任意，标题阶数仅由行首决定）

##区块引用##
Markdown使用>表示引用（类似e-mail）。
建立区块仅需在每一行开头加上>，或者整段开头加上>，且可以嵌套引用（如：>>这是一个嵌套引用）
例如：
>\>这是引用。
>>\>>这是嵌套引用

##强调##
强调可以使用*或者_或者**或者__
例如：

    *single asterisks*
    _single underscores_
    **double asterisks**
    __double underscores__

##分割线##
你可以在一行中用三个以上的星号、减号、底线来建立一个分隔线，行内不能有其他东西。你也可以在星号或是减号中间插入空格。下面每种写法都可以建立分隔线：

    * * *
    ***
    *****
    - - -
    ---------------------------------------

#链接#
链接文字都是用\[方括号\]来标记。

要建立一个行内式的链接，只要在方块括号后面紧接着圆括号并插入网址链接即可，如果你还想要加上链接的 title 文字，只要在网址后面，用双引号把 title 文字包起来即可，例如：

    This is [an example](http://example.com/ "Title") inline link.
    [This link](http://example.net/) has no title attribute.

会产生：

><p>This is <a href="http://example.com/" title="Title">
an example</a> inline link.</p>

><p><a href="http://example.net/">This link</a> has no
title attribute.</p>


#更多请参考#
[Markdown语法说明](http://wowubuntu.com/markdown/index.html)


