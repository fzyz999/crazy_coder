#ifndef EDITORCONFIGDLG_H
#define EDITORCONFIGDLG_H

#include <QWidget>
#include <QListWidgetItem>
#include <QTextCharFormat>
#include <QColorDialog>
#include <QGraphicsColorizeEffect>
#include "cpplexer.h"
#include "codeeditor.h"

namespace Ui {
class editorConfigDlg;
}

class editorConfigDlg : public QWidget
{
    Q_OBJECT

    struct configInfo
    {

    };
    
public:
    explicit editorConfigDlg(QWidget *parent = 0);
    ~editorConfigDlg();

private slots:
    void on_listWidget_itemSelectionChanged();
    void on_checkBox_blod_stateChanged(int arg1);
    void on_checkBox_italic_stateChanged(int arg1);

    void on_pushButton_fgc_clicked();

    void on_fontComboBox_currentFontChanged(const QFont &f);

    void on_spinBox_valueChanged(int arg1);

private:
    Ui::editorConfigDlg *ui;
    cppLexer *lexer;
    QGraphicsColorizeEffect *effect;

};

#endif // EDITORCONFIGDLG_H
