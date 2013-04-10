#include "tabmanager.h"

TabManager::TabManager(QWidget *parent) :
    QTabWidget(parent)
{
    clear();

    connect(this,SIGNAL(tabCloseRequested(int)),this,SLOT(on_tab_close_requested(int)));
    connect(this,SIGNAL(currentChanged(int)),this,SLOT(on_current_tab_change(int)));

    setDocumentMode(true);
    setMovable(true);
    setTabsClosable(true);
    setElideMode(Qt::ElideRight);
    setTabPosition(QTabWidget::North);
    tab_count=1;
    new_file_count=1;
    isEditor=false;
}

void TabManager::open_file()
{
    QSettings settings("settings.ini",QSettings::IniFormat);
    QStringList file_name_list=QFileDialog::getOpenFileNames(this,tr("Open File"),settings.value("files/historyDir").toString());
    CodeEditor *tmp;
    QString name;

    int i;

    if(!file_name_list.isEmpty())
    {
        name=*file_name_list.begin();

        for(i=name.size()-1;i>=0;i--)
            if(name[i]=='/')
                break;

        settings.setValue("files/historyDir",name.left(i));
    }

    while(!file_name_list.isEmpty())
    {
        name=*file_name_list.begin();

        tmp=new CodeEditor(this);
        tmp->open(name);

        name=QFileInfo(name).fileName();
        tmp->setDocumentTitle(name);

        addTab(tmp,name);

        file_name_list.pop_front();
    }
}

void TabManager::new_file()
{
    CodeEditor *tmp=new CodeEditor(this);
    if(tmp!=NULL)
        addTab(tmp,tr("Untitled")+QString::number(new_file_count++));
    else
    {
        QErrorMessage error(this);
        error.showMessage(tr("out of memmory!"));
        error.exec();
    }
}

void TabManager::save()
{
    if(Q_LIKELY(this->isEditor))
    {
        CodeEditor *currentEditor=qobject_cast<CodeEditor *>(this->currentWidget());

        Q_ASSERT(currentEditor!=NULL);

        if(Q_UNLIKELY(!currentEditor->save()))
        {
            QErrorMessage error(this);
            error.showMessage(tr("Saving file failed!"));
            error.exec();
        }
    }
}

void TabManager::save_all()
{
    int i=count();
    while(i--)
    {
        CodeEditor *editor=qobject_cast<CodeEditor *>(widget(i-1));

        if(Q_LIKELY(editor!=NULL))
        {
            if(Q_UNLIKELY(!editor->save()))
            {
                QErrorMessage error(this);
                error.showMessage(tr("Saving file failed!"));
                error.exec();
            }
        }
    }
}

void TabManager::undo()
{
    if(Q_LIKELY(this->isEditor))
    {
        CodeEditor *currentEditor=qobject_cast<CodeEditor *>(this->currentWidget());

        Q_ASSERT(currentEditor!=NULL);
        currentEditor->undo();
    }
}

void TabManager::redo()
{
    if(Q_LIKELY(this->isEditor))
    {
        CodeEditor *currentEditor=qobject_cast<CodeEditor *>(this->currentWidget());

        Q_ASSERT(currentEditor!=NULL);
        currentEditor->redo();
    }
}

void TabManager::copy()
{
    if(Q_LIKELY(this->isEditor))
    {
        CodeEditor *currentEditor=qobject_cast<CodeEditor *>(this->currentWidget());

        Q_ASSERT(currentEditor!=NULL);
        currentEditor->copy();
    }
}

void TabManager::cut()
{
    if(Q_LIKELY(this->isEditor))
    {
        CodeEditor *currentEditor=qobject_cast<CodeEditor *>(this->currentWidget());

        Q_ASSERT(currentEditor!=NULL);
        currentEditor->cut();
    }
}

void TabManager::paste()
{
    if(Q_LIKELY(this->isEditor))
    {
        CodeEditor *currentEditor=qobject_cast<CodeEditor *>(this->currentWidget());

        Q_ASSERT(currentEditor!=NULL);
        currentEditor->paste();
    }
}

void TabManager::select_all()
{
    if(Q_LIKELY(this->isEditor))
    {
        CodeEditor *currentEditor=qobject_cast<CodeEditor *>(this->currentWidget());

        Q_ASSERT(currentEditor!=NULL);
        currentEditor->selectAll();
    }
}

void TabManager::on_tab_close_requested(int index)
{
    QWidget *tab=widget(index);

    if(tab!=NULL)
    {
        tab->setAttribute(Qt::WA_DeleteOnClose,true);
        if(tab->close())
            removeTab(index);
    }

}

void TabManager::on_current_tab_change(int index)
{
    CodeEditor* editor=qobject_cast<CodeEditor *>(widget(index));
    if(editor!=NULL)
        isEditor=true;
    else
        isEditor=false;
}

void TabManager::compile_current_file()
{
    if(Q_LIKELY(this->isEditor))
    {
        CodeEditor *currentEditor=qobject_cast<CodeEditor *>(this->currentWidget());

        Q_ASSERT(currentEditor!=NULL);
        currentEditor->compile();
    }
}

void TabManager::closeEvent(QCloseEvent *e)
{
    e->accept();//set the accept flag
    int i=count();
    while(i--)
    {
        QWidget *page=widget(i);

        if(!page->close())
            e->ignore();//clean the accept flag
    }
}

void TabManager::renameTabTitle(QString title)
{
    QWidget* page=qobject_cast<QWidget*>(sender());
    if(page!=NULL)
    {
        setTabText(indexOf(page),title);
    }
    else
        qWarning()<<"in TabManager::renameTabTitle(QString title): a wrong sender!";
}

void TabManager::tabRemoved(int index)
{

}

void TabManager::tabInserted(int index)
{
    CodeEditor* editor=qobject_cast<CodeEditor *>(widget(index));
    if(editor!=NULL)
    {
        connect(editor,SIGNAL(filenameChanged(QString)),this,SLOT(renameTabTitle(QString)));
    }
}
