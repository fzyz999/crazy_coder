#ifndef CPPLEXER_H
#define CPPLEXER_H

#include <QObject>
#include <QString>
#include <QTextDocument>
#include <QTextCharFormat>
#include <QStringList>
#include <QSyntaxHighlighter>
#include <QDebug>

#include "codeeditor.h"

class CodeEditor;

class LexerConfig
{
public:
    enum formatType
    {
        keywords,
        numbers,
        comments,
        preprocess
    };

    static void setup();
    static void shutdown();

    static QTextCharFormat getFormat(formatType type);
    static void setFormat(const QTextCharFormat& format, formatType type);

private:
    LexerConfig(){}
    static QTextCharFormat keywordsFormat;
    static QTextCharFormat numbersFormat;
    static QTextCharFormat commentsFormat;
    static QTextCharFormat preprocessFormat;
};

class cppLexer : public QSyntaxHighlighter
{
    Q_OBJECT

public:
    cppLexer(QTextDocument *parent);
    ~cppLexer();

protected:
    void highlightBlock(const QString &text);

private:
    QList<QRegExp> keywords_datatype;
    QList<QRegExp> keywords_others;

};

#endif // CPPLEXER_H
