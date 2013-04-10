#ifndef CPPLEXER_H
#define CPPLEXER_H

#include <QObject>
#include <QString>
#include <QTextDocument>
#include <QTextCharFormat>
#include <QStringList>
#include <QSyntaxHighlighter>
#include <QDebug>
#include <QCompleter>
#include <QAbstractItemView>

#include "codeeditor.h"

class CodeEditor;

class LexerConfig : public QObject
{
    Q_OBJECT
public:
    enum formatType
    {
        keywords,
        numbers,
        comments,
        preprocess,
        strings,
        count   //the number of format-types
    };

    static void restoreSettings();
    static void saveSettings();

    static QTextCharFormat getFormat(formatType type);
    static QString getTypeNameFromIndex(const int &index);
    static formatType getFormatTypeFromName(const QString &name);
    static formatType getFormatTypeFromIndex(const int &index);
    static void setFormat(const QTextCharFormat& format, formatType type);

signals:
    void needRehighlight();

private:
    LexerConfig(){}
    static QTextCharFormat textFormat[count];
};

class cppLexer : public QSyntaxHighlighter
{
    Q_OBJECT

public:
    cppLexer(QPlainTextEdit *parent);
    cppLexer(QTextEdit *parent);
    ~cppLexer();

protected:
    void highlightBlock(const QString &text);

private:
    void init(QWidget* parent);
    QList<QRegExp> keywords_datatype;
    QList<QRegExp> keywords_others;
    QCompleter *completer;
};

#endif // CPPLEXER_H
