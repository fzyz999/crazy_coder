#include "cpplexer.h"

QTextCharFormat LexerConfig::textFormat[LexerConfig::count];

QString LexerConfig::getTypeNameFromIndex(const int &index)
{
    switch (index)
    {
    case keywords:
        return QString("keywords");
        break;

    case numbers:
        return QString("numbers");
        break;

    case comments:
        return QString("comments");
        break;

    case preprocess:
        return QString("preprocess");
        break;

    case strings:
        return QString("strings");
        break;

    default:
        qWarning()<<"LexerConfig::getFormat(formatType): undefined formatType!";
        return QString("");
    }
}

LexerConfig::formatType LexerConfig::getFormatTypeFromIndex(const int &index)
{
    switch (index)
    {
    case keywords:
        return keywords;
        break;

    case numbers:
        return numbers;
        break;

    case comments:
        return comments;
        break;

    case preprocess:
        return preprocess;
        break;

    case strings:
        return strings;
        break;

    default:
        qWarning()<<"LexerConfig::getFormat(formatType): undefined formatType!";
        return count;
    }
}

LexerConfig::formatType LexerConfig::getFormatTypeFromName(const QString &name)
{
    if(name == QString("keywords"))
        return keywords;
    if(name == QString("numbers"))
        return numbers;
    if(name == QString("comments"))
        return comments;
    if(name == QString("preprocess"))
        return preprocess;
    if(name == QString("strings"))
        return strings;

    qWarning()<<"LexerConfig::getFormat(formatType): undefined formatType!";
    return count;
}

void LexerConfig::restore_settings()
{
    QSettings settings("settings.ini",QSettings::IniFormat);
    settings.beginGroup("LexerConfig");

    for(int i=0;i<LexerConfig::count;i++)
    {
        settings.beginGroup(getTypeNameFromIndex(i));
        textFormat[i].setForeground(settings.value("Foreground").value<QBrush>());
        textFormat[i].setFontWeight(settings.value("FontWeight").toInt());
        textFormat[i].setFontItalic(settings.value("FontItalic").toBool());
        settings.endGroup();
    }

    settings.endGroup();
}

void LexerConfig::save_settings()
{
    QSettings settings("settings.ini",QSettings::IniFormat);
    settings.beginGroup("LexerConfig");

    for(int i=0;i<LexerConfig::count;i++)
    {
        settings.beginGroup(getTypeNameFromIndex(i));
        settings.setValue("Foreground",textFormat[i].foreground());
        settings.setValue("FontWeight",textFormat[i].fontWeight());
        settings.setValue("FontItalic",textFormat[i].fontItalic());
        settings.endGroup();
    }

    settings.endGroup();
}

void LexerConfig::setFormat(const QTextCharFormat &format, formatType type)
{
    Q_ASSERT(type<count);
    textFormat[type]=format;
}

QTextCharFormat LexerConfig::getFormat(formatType type)
{
    Q_ASSERT(type<count);
    return textFormat[type];
}

cppLexer::cppLexer(QPlainTextEdit *parent):
    QSyntaxHighlighter(parent->document())
{
    init(parent);
}

cppLexer::cppLexer(QTextEdit *parent):
    QSyntaxHighlighter(parent->document())
{
    init(parent);
}

void cppLexer::init(QWidget* parent)
{
    QStringList datatype;
    datatype<<"double"<<"short"<<"bool"<<"int"<<"signed"
           <<"long"<<"enum"<<"static"<<"unsigned"<<"char"
          <<"void"<<"const"<<"float"<<"wchar_t"<<"char16_t"
         <<"char32_t"<<"constexpr"<<"auto"<<"thread_local";
    QStringList others;
    others<<"asm"<<"do"<<"if"<<"return"<<"typedef"
         <<"inline"<<"typeid"<<"dynamic_cast"<<"typename"<<"break"
        <<"else"<<"sizeof"<<"union"<<"case"<<"mutable"<<"catch"
       <<"explicit"<<"namespace"<<"static_cast"<<"using"<<"export	"
      <<"new"<<"struct"<<"virtual"<<"class"<<"extern"<<"operator"
     <<"switch"<<"false"<<"private"<<"template"<<"volatile"
    <<"const_cast"<<"protected"<<"this"<<"continue"<<"for"
    <<"public"<<"throw"<<"while"<<"default"<<"friend"
    <<"register"<<"true"<<"delete"<<"goto"<<"reinterpret_cast"
    <<"try"<<"alignas"<<"alignof"<<"decltype"<<"noexcept"
    <<"nullptr"<<"static_assert";

    foreach (QString pattern, datatype)
    {
        pattern.prepend("\\b");
        pattern.append("\\b");
        QRegExp exp(pattern);
        keywords_datatype.append(exp);
    }
    foreach (QString pattern, others)
    {
        pattern.prepend("\\b");
        pattern.append("\\b");
        QRegExp exp(pattern);
        keywords_others.append(exp);
    }
}

cppLexer::~cppLexer()
{
}

void cppLexer::highlightBlock(const QString &text)
{

    foreach (const QRegExp& expression, keywords_datatype)
    {
        int index = expression.indexIn(text);
        while (index >= 0)
        {
            int length = expression.matchedLength();
            setFormat(index, length, LexerConfig::getFormat(LexerConfig::keywords));
            index = expression.indexIn(text, index + length);
        }
    }

    foreach (const QRegExp& expression, keywords_others)
    {
        int index = expression.indexIn(text);
        while (index >= 0)
        {
            int length = expression.matchedLength();
            setFormat(index, length, LexerConfig::getFormat(LexerConfig::keywords));
            index = expression.indexIn(text, index + length);
        }
    }

    QRegExp singleLineComment("//[^\n]*");
    for(int index = singleLineComment.indexIn(text);index >= 0;)
    {
        int length = singleLineComment.matchedLength();
        setFormat(index, length, LexerConfig::getFormat(LexerConfig::comments));
        index = singleLineComment.indexIn(text, index + length);
    }

    QRegExp number("\\W-?(0x(\\d|[a-f]|[A-F])+|\\d+)(\\W|\\s)");
    for(int index = number.indexIn(text);index >= 0;)
    {
        int length = number.matchedLength();
        //match \\w at the begin of the string and (\\W|\\s) at the end of it.
        //so the real length of number of the string is length-2
        index++;
        length-=2;
        setFormat(index, length, LexerConfig::getFormat(LexerConfig::numbers));
        index = number.indexIn(text, index + length);
    }

    QRegExp string("\".*\"");
    string.setMinimal(true);
    for(int index = string.indexIn(text);index >= 0;)
    {
        int length = string.matchedLength();
        setFormat(index, length, LexerConfig::getFormat(LexerConfig::strings));
        index = string.indexIn(text, index + length);
    }

    QRegExp preprocess("^#[^\n]*");
    for(int index = preprocess.indexIn(text);index >= 0;)
    {
        int length = preprocess.matchedLength();
        setFormat(index, length, LexerConfig::getFormat(LexerConfig::preprocess));
        index = preprocess.indexIn(text, index + length);
    }
}
