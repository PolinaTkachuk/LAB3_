#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    :  QWidget(parent),
    BlackWhiteCheck(new QCheckBox("Черно-белый график")),//задаем чекбокс
    printChart(new QPushButton("Печать графика")) //задаем кнопку печати

{
   //Устанавливаем размер главного окна
   this->setGeometry(100, 100, 1500, 500);
   setWindowTitle("PrintChart"); // заголовк окна
   QString homePath = QDir::homePath();
   // Определим  файловой системы:
   dirModel =  new QFileSystemModel(this);
   dirModel->setFilter(QDir::NoDotAndDotDot | QDir::AllDirs);
   dirModel->setRootPath(homePath);

   fileModel = new QFileSystemModel(this);
   fileModel->setFilter(QDir::NoDotAndDotDot | QDir::Files);

   fileModel->setRootPath(homePath);
   //Показать как дерево, пользуясь готовым видом:

   treeView = new QTreeView();
   treeView->setModel(dirModel);

   treeView->expandAll();

   QSplitter *splitter = new QSplitter(parent);
   tableView = new QTableView;
   tableView->setModel(fileModel);
   splitter->addWidget(treeView);
   splitter->addWidget(tableView);

    // Изначально- графики цветные
    //BlackWhiteCheck->setChecked(true);
}




void connectSignals()
{
     //connect(BlackWhiteCheck,SIGNAL(clicked()),this,SLOT(ColoredSlot());
}

/*
bool Colored()
{
    if(BlackWhiteCheck->isChecked()) return true;
    return false;
}*/
MainWindow::~MainWindow()
{

}

