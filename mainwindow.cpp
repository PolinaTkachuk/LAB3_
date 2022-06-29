#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "IOC.h"


MainWindow::MainWindow(QWidget *parent)
    :   QWidget(parent)

{

   //Устанавливаем размер главного окна
   this->setGeometry(100, 100, 1200, 500);
   setWindowTitle("PrintChart"); // заголовк окна

   currentPath = QDir::homePath(); //задали имя для начальной папки
   filePath = "";
   // Определим  файловой системы:
   dirModel =  new QFileSystemModel(this);//модель для файловой системы
   dirModel->setFilter(QDir::NoDotAndDotDot | QDir::AllEntries); //задаем фильтры
   dirModel->setRootPath(currentPath); //указываем корневую папку

   //определяем кнопки
   printChart = new QPushButton("Печать Графика");
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

    typeChart_label = new QLabel("Выберите тип диаграммы:");
    typeChart = createTypeBox();//определяем 2типа

    //метод RegisterInstance будет возвращать один экземпляр  объекта  для каждого вызова
    //ранее объявили этот экземпляр IOCContainer gContainer;
    gContainer.RegisterInstance<IntrfaceDraw,createBarChart>();

    //черно-белый - определяем чекбокс
    BlackWhiteCheck = new QCheckBox("Черно-белый график");

    //работа с графиками задали представление
    chartView = new QChartView;
    chartView->setRenderHint(QPainter::Antialiasing);

    //табличное представление файлов слева
    tableView = new QTableView;
    tableView->setModel(dirModel);
    //добавл разделителей между файлами и представлением графика. окно на 2 части
    splitterL->addWidget(tableView);
    splitterR->addWidget(chartView);

    //добавл на раскладки виджеты
    verticalL->addWidget(btnDirectory);
    verticalL->addWidget(splitterL);
    verticalR->addLayout(layoutWidgetH);
    verticalR->addWidget(splitterR);

    layoutWidgetH->addWidget(typeChart_label);
    layoutWidgetH->addWidget(typeChart);
    layoutWidgetH->addWidget(BlackWhiteCheck);
    layoutWidgetH->addWidget(printChart);

    layoutWidgetH->addStretch();
    // Изначально- графики цветные
    BlackWhiteCheck->setChecked(false);

    //соединение всех сигналов-слотов
    connectSignals();

}
void MainWindow::connectSignals()
{
    //по щелчку-сигнал на кнопку ВЫБОР ДИРЕКТОРИИ используем слот-ChangeDirectory()
    connect(btnDirectory, SIGNAL(clicked()), this, SLOT(ChangeDirectory()));
    //по щелчку-сигнал на кнопку Печать Графика используе слот-PrintSlot()
    connect(printChart, SIGNAL(clicked()), this, SLOT(PrintSlot()));
    //сигнал -если выбрали другой файл для построения графика, то использ слот-SelectionChangedSlot
    //внутри SelectionChangedSlot обрабатываются ошибки и , если все корректно, идет построение верного графика на основе данных
    connect(
                tableView->selectionModel(),
                SIGNAL(selectionChanged(const QItemSelection &, const QItemSelection &)),
                this,
                SLOT(SelectionChangedSlot(const QItemSelection &, const QItemSelection &))
                );
    //сигнал-выбор пользователем другого типа графика из списка слот-onComboboxChangedSlot()
    connect(typeChart, SIGNAL(currentIndexChanged(int)), this, SLOT(onComboboxChangedSlot()));
    //сигнал-изменение отметки чекбокса ч/б слот-repaintCharts()
    connect(BlackWhiteCheck, SIGNAL(toggled(bool)), this, SLOT(repaintCharts()));

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

    if(AvailableGraph){
    QFileDialog *fileDialog = new QFileDialog(this);
     // заголовок файла
    fileDialog-> setWindowTitle (tr ("Сохранить в "));
    //путь к файлу определяем по умолчанию
    fileDialog->setDirectory(".");
    fileDialog->setAcceptMode(QFileDialog::AcceptSave);
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
    painter.end();
   }
    else {
        QMessageBox messageBox; // Окно-сообщение об ошибке
        messageBox.setText("График отсутствует. Печать не может быть выполнена.");
        messageBox.exec();
    }
}

void MainWindow::onComboboxChangedSlot() {
    QString chartType = typeChart->currentText();
    //определились с типом графика
    if (chartType == "Pie") {
        //используя внедрение зависимости создаем график типа pie
        gContainer.RegisterInstance<IntrfaceDraw, createPieChart>();
    }
    else if (chartType == "Bar") {
        //используя внедрение зависимости создаем график типа bar
        gContainer.RegisterInstance<IntrfaceDraw, createBarChart>();
    }
    //если все в порядке, прорисовываем его в зависимости от типа
    if (AvailableGraph) {
        DrawChart();
    }

}
void MainWindow::SelectionChangedSlot(const QItemSelection &selected, const QItemSelection &deselected) {
    Q_UNUSED(deselected);

  QModelIndexList indexes =  selected.indexes();//получаем индексы- сколько файлов выбрали
    if (indexes.count() != 1)//обрабатываем ошибку- необходимо выбрать только один график и на основе его данных строить график
    {
        QMessageBox messageBox;
        messageBox.setText("Выберите только один файл.");
        messageBox.exec();
        if (AvailableGraph){
            auto chart = gContainer.GetObject<IntrfaceDraw>();
            chart->ClearChart();//ощищаем поле для графика так как ошибка
            chart->getChart()->setTitle("");
        }
        AvailableGraph = false;// и устанавливаем, что график недоступен
        return;
    }

filePath = dirModel->filePath(indexes.constFirst());
//Проверка на расширения выбранного файла

if (filePath.endsWith(".sqlite")) {//корректно
    gContainer.RegisterInstance<Idata, dataSql>();//получ данные
}
else if (filePath.endsWith(".json")) {//корректно
    gContainer.RegisterInstance<Idata, dataJSON>();//получаем данные
}
else {
    QMessageBox messageBox;// Сообщение об ошибке
    messageBox.setText("Ошибка: выбран файл неверного формата. Корректный формат json или sqlite ");
    messageBox.exec();
    if (AvailableGraph) {
        auto chart = gContainer.GetObject<IntrfaceDraw>();
        chart->ClearChart();//очищаем поле графика
        chart->getChart()->setTitle("");
    }
    AvailableGraph = false;// и устанавливаем, что график недоступен
    return;
}
//проверка корректного типа графика
    QString chartType = typeChart->currentText();
    if (chartType == "Pie") {
        gContainer.RegisterInstance<IntrfaceDraw, createPieChart>();
       AvailableGraph= true;//все требования в норме, график доступен для построения
    }
    else if (chartType == "Bar") {
        gContainer.RegisterInstance<IntrfaceDraw, createBarChart>();
        AvailableGraph = true;//все требования в норме, график доступен для построения
    }
    else {
        QMessageBox messageBox;
        messageBox.setText("Некорректный тип графика.");
        messageBox.exec();
        if (AvailableGraph) {
            auto chart = gContainer.GetObject<IntrfaceDraw>();
            chart->ClearChart();//очищаем поле -убираем график
            chart->getChart()->setTitle("");
        }
       AvailableGraph = false;
    }
    DrawChart();

}

void MainWindow::TypeChartSlot()
{
    QString chartType{typeChart->currentText()};
    if (chartType.compare("Pie") == 0)//если тип pie
    {
        //используя внедрение зависимости создаем график типа pie
        gContainer.RegisterInstance<IntrfaceDraw,createPieChart>();
        return;
    }
    if (chartType.compare("Bar") == 0)//если тип bar
    {
        //используя внедрение зависимости создаем график типа bar
        gContainer.RegisterInstance<IntrfaceDraw,createBarChart>();
        return;
    }
}


void MainWindow::ChangeDirectory() {

    //выбираем нужную директорию
    QFileDialog dialog{this};
    dialog.setFileMode(QFileDialog::Directory);
    if ( dialog.exec() )
    {
        currentPath = dialog.selectedFiles().first();//выбор директории, путь до папки сохр
    }
    //отображаем в представление табличном директорию с выбранным путем
    tableView->setRootIndex(dirModel->setRootPath(currentPath));

}

void MainWindow::DrawChart() {

    //используем ioc(внедряем зависимость) обращение к интерфейсам- базовым классам работы с графиками и работы с данными
    auto obj_ = gContainer.GetObject<IntrfaceDraw>();
    auto data = gContainer.GetObject<Idata>();
    //получаем данные как пару [значение,дата] используя функцию из databasejson,указывем путь к файлу
    Containerdata_ gettedData = data->getData(filePath);
    //возвращением в объект результат прорисовки графика на основе входных полученных данных
    // и также учитывая нажат ли чекбокс черно-белый график
    obj_->Draw(gettedData, BlackWhiteCheck->isChecked());
    //получаем представление графика используя гет из chart
    chartView->setChart(obj_->getChart());
}

void MainWindow::repaintCharts() {
    if (AvailableGraph)
    {
        DrawChart();
    }
}

MainWindow::~MainWindow()
{

}
