#include "mainwindow.h"
#include"IOC.h"
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
   btnPrint = new QPushButton("Печать Графика");
   btnDirectory = new QPushButton("Открыть Директорию");

   //определяем раскладки для виджетов и для файлов
   HParent = new QHBoxLayout(this);//основной для размещения всего Горизонтально
   VertLayout = new QVBoxLayout();// вертикально
   layoutWidgetH=new QHBoxLayout();// для виджетов
   layoutFileV=new QVBoxLayout();// для директорий

   verticalL = new QVBoxLayout();
   verticalR = new QVBoxLayout();

   HParent->addLayout(verticalL);
   HParent->addLayout(verticalR);

   horizWrapper = new QHBoxLayout();

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
    tableView->setModel(dirModel);
    splitterL->addWidget(tableView);
    splitterR->addWidget(chart_view);

    verticalL->addWidget(btnDirectory);
    verticalL->addWidget(splitterL);
    verticalR->addLayout(layoutWidgetH);
    verticalR->addWidget(splitterR);

    // create layout добавл виджетов на горизонт и верт раскладки
    layoutWidgetH->addWidget(typeChart_label);
    layoutWidgetH->addWidget(typeChart);
    layoutWidgetH->addWidget(BlackWhiteCheck);
    layoutWidgetH->addWidget(btnPrint);

    layoutWidgetH->addStretch();
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

     connect(btnDirectory, SIGNAL(clicked()), this, SLOT(ChangeDirectory()));
     connect(btnPrint, SIGNAL(clicked()), this, SLOT(PrintSlot()));

     connect(
                 selectionModel,
                 SIGNAL(selectionChanged(const QItemSelection &, const QItemSelection &)),
                 this,
                 SLOT(SelectionChangedSlot(const QItemSelection &, const QItemSelection &))
                 );
     connect(typeChart, SIGNAL(currentIndexChanged(int)), this, SLOT(onComboboxChangedSlot()));

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

void MainWindow::SelectionChangedSlot(const QItemSelection &selected, const QItemSelection &deselected) {
    Q_UNUSED(deselected);

    QModelIndexList indexes =  selected.indexes();
    if (indexes.count() < 1)
    {
        return;
    }

    QString filePath = "";
    filePath = dirModel->filePath(indexes.constFirst());

    bool isExpectedFile = true
            && (filePath.endsWith(".sqlite")
            || filePath.endsWith(".json"));
    if (!isExpectedFile)
    {
//        chartManipulation.chart->cleanSeries();
        isChartAvailable = false;
//        messageBox{"Expect .json or .sqlite"};
        return;
    }
    if (filePath.endsWith(".sqlite")) {
        gContainer.RegisterInstance<Idata, dataSql>();
    }
    else if (filePath.endsWith(".josn")) {
        gContainer.RegisterInstance<Idata, dataJSON>();
    }

    QString chartType = typeChart->currentText();
    if (chartType == "Pie") {
        gContainer.RegisterInstance<IntrfaceDraw, createPieChart>();
        isChartAvailable = true;
    }
    else if (chartType == "Bar") {
        gContainer.RegisterInstance<IntrfaceDraw, createBarChart>();
        isChartAvailable = true;
    }
    auto bamp = gContainer.GetObject<IntrfaceDraw>();
    auto data = gContainer.GetObject<Idata>();
    Containerdata_ gettedData = data->getData(filePath);
    bamp->Draw(gettedData);
//    bamp->Draw(gContainer.GetObject<Idata>()->getData(filePath);
    chartView->setChart(bamp->getChart());
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


void MainWindow::ChangeDirectory() {
    QFileDialog dialog{this};
    dialog.setFileMode(QFileDialog::Directory);
    QString currentPath;
    if ( dialog.exec() )
    {
        currentPath = dialog.selectedFiles().first();
//        labelsOutput.labelPath->setText(labelsOutput.serialize(currentPath));
    }
    tableView->setRootIndex(dirModel->setRootPath(currentPath));
}


MainWindow::~MainWindow()
{

}
