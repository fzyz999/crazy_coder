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
    currentEditor=NULL;

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
    if(Q_LIKELY(currentEditor!=NULL))
    {
        if(Q_UNLIKELY(!currentEditor->save()))
        {
            QErrorMessage error(this);
            error.showMessage(tr("Saving file failed!"));
            error.exec();
        }
        else if(currentEditor->is_filename_changed())
        {
            setTabText(currentIndex(),currentEditor->get_file_name());
            currentEditor->set_filename_changed(false);
        }
    }
}

void TabManager::save_all()
{
    for(int i=count();i>0;i--)
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
            else if(editor->is_filename_changed())
            {
                setTabText(i-1,editor->get_file_name());
            }
        }
    }
}

void TabManager::undo()
{
    if(Q_LIKELY(currentEditor!=NULL))
        currentEditor->undo();
}

void TabManager::redo()
{
    if(Q_LIKELY(currentEditor!=NULL))
        currentEditor->redo();
}

void TabManager::copy()
{
    if(Q_LIKELY(currentEditor!=NULL))
        currentEditor->copy();
}

void TabManager::cut()
{
    if(Q_LIKELY(currentEditor!=NULL))
        currentEditor->cut();
}

void TabManager::paste()
{
    if(Q_LIKELY(currentEditor!=NULL))
        currentEditor->paste();
}

void TabManager::select_all()
{
    if(Q_LIKELY(currentEditor!=NULL))
        currentEditor->selectAll();
}

void TabManager::on_tab_close_requested(int index)
{
    CodeEditor *tab=qobject_cast<CodeEditor *>(widget(index));

    if(tab->close())
    {
            removeTab(index);
            tab->deleteLater();
    }
}

void TabManager::on_current_tab_change(int index)
{
    currentEditor=qobject_cast<CodeEditor *>(widget(index));
}
