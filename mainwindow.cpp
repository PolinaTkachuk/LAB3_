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

    //табличное представление

    tableView = new QTableView;
    tableView->setModel(fileModel);
    tableView->setRootIndex(fileModel->setRootPath(homePath));
    splitterH->addWidget(tableView);
    splitterV->addWidget(chart_view);


    // create layout добавл виджетов на горизонт и верт раскладки
    layoutWidgetH->addWidget(typeChart_label);
    layoutWidgetH->addWidget(typeChart);
    layoutWidgetH->addWidget(BlackWhiteCheck);
    layoutWidgetH->addWidget(printChart);
    layoutWidgetH->addStretch();
    layoutAllV->addLayout(layoutWidgetH);

    // Изначально- графики цветные
    BlackWhiteCheck->setChecked(false);

    //добавл раздилитель
    //???!!!!!!!!!!!!!!!!!!!!!!!!!!1
    //плавающий разделитель
    /*QSplitter *splitter = new QSplitter(parent);
    tableView = new QTableView;
    tableView->setModel(fileModel);
    splitter->addWidget(treeView);
    splitter->addWidget(tableView);
    */
     QItemSelectionModel *selectionModel = tableView->selectionModel();



}

QComboBox *MainWindow::createTypeBox()
{
    // type layout
    QComboBox *ComboBox_ = new QComboBox();
    ComboBox_->addItem("Pie", TypeChart::Pie);
    ComboBox_->addItem("Bar", TypeChart::Bar);

    return ComboBox_;
}


void MainWindow::PrintSlot()
{

    QFileDialog *fileDialog = new QFileDialog(this);
     // заголовок файла
    fileDialog-> setWindowTitle (tr ("Открыть изображение"));
    //путь к файлу определяем по умолчанию
    fileDialog->setDirectory(".");
             // Установить фильтр файлов, только файлы бд и json-данные
    fileDialog->setNameFilter(tr("Images(*.sqlite *.json)"));
 // Настройка позволяет выбрать несколько файлов, по умолчанию используется только один файл QFileDialog :: ExistingFiles
    fileDialog->setFileMode(QFileDialog::ExistingFiles);
             // Установить режим просмотра
    fileDialog->setViewMode(QFileDialog::Detail);
             // выводим путь ко всем выбранным файлам
    QStringList fileNames;
    if(fileDialog->exec())
    {
        fileNames = fileDialog->selectedFiles();
    }

    QPdfWriter writer_(fileNames[0] + ".pdf");//сохраняем в формате пдф

    writer_.setPageSize(QPageSize::A4);//размер страницы А4

    writer_.setResolution (300);//разрешение бумаги на 300, чтобы пиксель был 3508 * 2479
    // Добавляем контент с помощью QPainter
    QPainter painter(&writer_);

    //Отрисовка
    chartView->render(&painter);
    // chart->ReturnchartView()->render(&painter);
    painter.end();


}

void MainWindow::TypeChartSlot()
{
    QString chartType{typeChart->currentText()};
        if (chartType.compare("Pie") == 0)
        {
            gContainer.RegisterInstance<IntrfaceDraw,createPieChart>();
            //chart->Draw(gContainer.GetObject<IntrfaceDraw>()->getData(""));
            //chart-> ReDrawingChart();//перерисовка графика
            return;
        }
        if (chartType.compare("Bar") == 0)
        {
            gContainer.RegisterInstance<IntrfaceDraw,createBarChart>();
            //chart-> ReDrawingChart();//перерисовка из chart файла
            return;
        }

}

MainWindow::~MainWindow()
{

}
