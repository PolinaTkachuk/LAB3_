#include "mainwindow.h"
//#include"IOC.h"
#include "ui_mainwindow.h"

//Иницализация не нулевым значением

//int IOCContainer::s_typeId = 121;

MainWindow::MainWindow(QWidget *parent)
    :  QMainWindow(parent)

   // typeChart_lable(new QLabel("Выберите тип диаграммы:")),
   // BlackWhiteCheck(new QCheckBox("Черно-белый график")),//задаем чекбокс
   // printChart(new QPushButton("Печать графика")) //задаем кнопку печати

{

        // declare buttons
   //     auto buttonChooseDirectory = new QPushButton{"Choose directory", this};
   //     auto buttonWritePdf = new QPushButton{"Save to PDF", this};




   ////////////////
   //Устанавливаем размер главного окна
   this->setGeometry(100, 100, 1500, 500);
   setWindowTitle("PrintChart"); // заголовк окна

   QString homePath = QDir::homePath(); //задали имя для начальной папки
   // Определим  файловой системы:
   dirModel =  new QFileSystemModel(this);//модель для файловой системы
   dirModel->setFilter(QDir::NoDotAndDotDot | QDir::AllDirs); //задаем фильтры
   dirModel->setRootPath(homePath); //указываем корневую папку

   //определяем раскладки для виджетов и для файлов
   auto layoutAllH= new QHBoxLayout(this);//основной для размещения всего Горизонтально
   auto layoutAllV= new QVBoxLayout();// вертикально
   auto layoutWidgetH=new QHBoxLayout();// для виджетов
   auto layoutFileV=QVBoxLayout();// для директорий

   //работаем с выпадающим списком

    typeChart_label = new QLabel{"Выберите тип диаграммы:",this};
    typeChart = createTypeBox();//определяем 2типа


    // рисуем по умолчанию график типа bar.
    //метод RegisterInstance будет возвращать один экземпляр  объекта  для каждого вызова
    //ранее объявили этот экземпляр IOCContainer gContainer;
    gContainer.RegisterInstance<IntrfaceDraw,createBarChart>();

    //черно-белый - определяем чекбокс
    BlackWhiteCheck = new QCheckBox("Черно-белый график");

    //работа с графиками задали представление
    //QChart* chart_;
   // Idata.chart_=new Chart{};

    QChartView *chart_view = new QChartView;
    chart_view->setRenderHint(QPainter::Antialiasing);

   ////////////

   //инициализ кнопки
   typeChart = createTypeBox();
   BlackWhiteCheck = new QCheckBox("Черно-белый график");
   printChart = new QPushButton("Печать графика");
   typeChart_label = new QLabel("Выберите тип диаграммы:");


   //connectSignals();
   // create layout добавл виджетов на горизонт и верт раскладки
   VertLayout = new QVBoxLayout();
   HorizLayout = new QHBoxLayout();
   HorizLayout->addWidget(typeChart_label);
   HorizLayout->addWidget(typeChart);
   HorizLayout->addWidget(BlackWhiteCheck);
   HorizLayout->addWidget(printChart);
   HorizLayout->addStretch();
   VertLayout->addLayout(HorizLayout);

   // Изначально- графики цветные
   BlackWhiteCheck->setChecked(true);



   fileModel = new QFileSystemModel(this);
   fileModel->setFilter(QDir::NoDotAndDotDot | QDir::Files);

   fileModel->setRootPath(homePath);
   //Показать как дерево, пользуясь готовым видом:

   treeView = new QTreeView();
   treeView->setModel(dirModel);//связываем представление-дерево с моделью

   treeView->expandAll();

   //плавающий разделитель
   QSplitter *splitter = new QSplitter(parent);
   tableView = new QTableView;
   tableView->setModel(fileModel);
   splitter->addWidget(treeView);
   splitter->addWidget(tableView);


}

QComboBox *MainWindow::createTypeBox()
{
    // type layout
    QComboBox *ComboBox_ = new QComboBox();
    ComboBox_->addItem("Pie", TypeChart::Pie);
    ComboBox_->addItem("Bar", TypeChart::Bar);

    return ComboBox_;
}




MainWindow::~MainWindow()
{

}
