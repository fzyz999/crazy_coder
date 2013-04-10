#include "editorconfigdlg.h"
#include "ui_editorconfigdlg.h"

editorConfigDlg::editorConfigDlg(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::editorConfigDlg)
{
    ui->setupUi(this);

    setLayout(ui->verticalLayout_3);
    lexer=new cppLexer(ui->textBrowser);

    effect=new QGraphicsColorizeEffect(this);
    ui->listWidget->setCurrentItem(ui->listWidget->item(0));
    ui->pushButton_fgc->setGraphicsEffect(effect);
    ui->fontComboBox->setCurrentFont(CodeEditorConfig::getEditorFont());
    ui->spinBox->setValue(CodeEditorConfig::getEditorFont().pointSize());

}

editorConfigDlg::~editorConfigDlg()
{
    delete ui;
}

void editorConfigDlg::on_listWidget_itemSelectionChanged()
{
    QListWidgetItem *item=ui->listWidget->selectedItems().first();
    //only one items can be selected

    LexerConfig::formatType type=LexerConfig::getFormatTypeFromIndex(ui->listWidget->row(item));
    QTextCharFormat format=LexerConfig::getFormat(type);

    effect->setColor(format.foreground().color());
    ui->pushButton_fgc->setFont(format.font());
    ui->checkBox_blod->setChecked(format.font().bold());
    ui->checkBox_italic->setChecked(format.font().italic());
}

void editorConfigDlg::on_checkBox_blod_stateChanged(int arg1)
{
    QListWidgetItem *item=ui->listWidget->selectedItems().first();
    //only one items can be selected

    LexerConfig::formatType type=LexerConfig::getFormatTypeFromIndex(ui->listWidget->row(item));
    QTextCharFormat format=LexerConfig::getFormat(type);

    if(arg1)
        format.setFontWeight(QFont::Bold);
    else
        format.setFontWeight(QFont::Normal);

    LexerConfig::setFormat(format,type);

    effect->setColor(format.foreground().color());
    ui->pushButton_fgc->setFont(format.font());
    lexer->rehighlight();
}

void editorConfigDlg::on_checkBox_italic_stateChanged(int arg1)
{
    QListWidgetItem *item=ui->listWidget->selectedItems().first();
    //only one items can be selected

    LexerConfig::formatType type=LexerConfig::getFormatTypeFromIndex(ui->listWidget->row(item));
    QTextCharFormat format=LexerConfig::getFormat(type);

    if(arg1)
        format.setFontItalic(true);
    else
        format.setFontItalic(false);

    LexerConfig::setFormat(format,type);

    effect->setColor(format.foreground().color());
    ui->pushButton_fgc->setFont(format.font());
    lexer->rehighlight();
}

void editorConfigDlg::on_pushButton_fgc_clicked()
{
    QColor color=QColorDialog::getColor();
    if(color.isValid())
    {
        QListWidgetItem *item=ui->listWidget->selectedItems().first();
        //only one items can be selected

        LexerConfig::formatType type=LexerConfig::getFormatTypeFromIndex(ui->listWidget->row(item));
        QTextCharFormat format=LexerConfig::getFormat(type);

        format.setForeground(QBrush(color));

        LexerConfig::setFormat(format,type);

        effect->setColor(format.foreground().color());
        ui->pushButton_fgc->setFont(format.font());
        lexer->rehighlight();
    }
}

void editorConfigDlg::on_fontComboBox_currentFontChanged(const QFont &f)
{
    CodeEditorConfig::setEditorFont(f);
}

void editorConfigDlg::on_spinBox_valueChanged(int arg1)
{
    QFont f=CodeEditorConfig::getEditorFont();
    f.setPointSize(arg1);
    CodeEditorConfig::setEditorFont(f);
}
