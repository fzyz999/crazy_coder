#include "cpplexer.h"

QTextCharFormat LexerConfig::keywordsFormat;
QTextCharFormat LexerConfig::numberssFormat;
QTextCharFormat LexerConfig::functionsFormat;
QTextCharFormat LexerConfig::variablesFormat;
QTextCharFormat LexerConfig::commentsFormat;
QTextCharFormat LexerConfig::preprocessFormat;

void LexerConfig::setup()
{
    QSettings settings("settings.ini",QSettings::IniFormat);
    settings.beginGroup("LexerConfig");

    settings.beginGroup("keywords");
    keywordsFormat.setForeground(settings.value("Foreground",Qt::darkBlue).value<QBrush>());
    keywordsFormat.setFontWeight(settings.value("FontWeight",QFont::Bold).toInt());
    keywordsFormat.setFontItalic(settings.value("FontItalic",false).toBool());
    settings.endGroup();

    settings.beginGroup("comments");
    commentsFormat.setForeground(settings.value("Foreground",Qt::blue).value<QBrush>());
    commentsFormat.setFontWeight(settings.value("FontWeight",QFont::Normal).toInt());
    commentsFormat.setFontItalic(settings.value("FontItalic",true).toBool());
    settings.endGroup();

    settings.beginGroup("preprocess");
    preprocessFormat.setForeground(settings.value("Foreground",Qt::darkGreen).value<QBrush>());
    preprocessFormat.setFontWeight(settings.value("FontWeight",QFont::Normal).toInt());
    preprocessFormat.setFontItalic(settings.value("FontItalic",false).toBool());
    settings.endGroup();

    settings.endGroup();
}

void LexerConfig::shutdown()
{
    QSettings settings("settings.ini",QSettings::IniFormat);
    settings.beginGroup("LexerConfig");

    settings.beginGroup("keywords");
    settings.setValue("Foreground",keywordsFormat.foreground());
    settings.setValue("FontWeight",keywordsFormat.fontWeight());
    settings.setValue("FontItalic",keywordsFormat.fontItalic());
    settings.endGroup();

    settings.beginGroup("comments");
    settings.setValue("Foreground",commentsFormat.foreground());
    settings.setValue("FontWeight",commentsFormat.fontWeight());
    settings.setValue("FontItalic",commentsFormat.fontItalic());
    settings.endGroup();

    settings.beginGroup("preprocess");
    settings.setValue("Foreground",preprocessFormat.foreground());
    settings.setValue("FontWeight",preprocessFormat.fontWeight());
    settings.setValue("FontItalic",preprocessFormat.fontItalic());
    settings.endGroup();

    settings.endGroup();
}

void LexerConfig::setFormat(const QTextCharFormat &format, formatType type)
{
    switch (type)
    {
    case keywords:
        keywordsFormat=format;
        break;

    case numbers:
        numberssFormat=format;
        break;

    case functions:
        functionsFormat=format;
        break;

     case variables:
        variablesFormat=format;
        break;

    case comments:
        commentsFormat=format;
        break;

    case preprocess:
        preprocessFormat=format;
        break;

    default:
        qWarning()<<"LexerConfig::getFormat(formatType): undefined formatType!";
    }
}

QTextCharFormat LexerConfig::getFormat(formatType type)
{
    switch (type)
    {
    case keywords:
        return keywordsFormat;

    case numbers:
        return numberssFormat;

    case functions:
        return functionsFormat;

    case variables:
        return variablesFormat;

    case comments:
        return commentsFormat;

    case preprocess:
        return preprocessFormat;

    default:
        qWarning()<<"LexerConfig::getFormat(formatType): undefined formatType!";
        return QTextCharFormat();
    }
}

cppLexer::cppLexer(QTextDocument *parent):
    QSyntaxHighlighter(parent)
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

    QRegExp preprocessComment("^#[^\n]*");
    for(int index = preprocessComment.indexIn(text);index >= 0;)
    {
        int length = preprocessComment.matchedLength();
        setFormat(index, length, LexerConfig::getFormat(LexerConfig::preprocess));
        index = preprocessComment.indexIn(text, index + length);
    }
}
