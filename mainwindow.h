#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWidget>
#include <QFileSystemModel>
#include <QTreeView>
#include <QTableView>
#include <QPushButton>
#include <QCheckBox>
#include <QComboBox>
#include <QWidget>
#include <QLabel>
#include <QMessageBox>
#include <QBoxLayout>
#include <QFileDialog>
#include <QPdfWriter>
#include <QtPrintSupport/QPrinter>
#include <QPainter>
#include <QTextOption>
#include<QPagedPaintDevice>
#include <QtCharts/QChartView>
#include<QTableWidget>
#include"chart.h"
#include<QSplitter>
#include<QDir>

//виды диаграмм
enum TypeChart
{
    Pie,
    Bar

};

class MainWindow : public QWidget
{
    Q_OBJECT

private slots://слоты
    void PrintSlot();//печать в pdf
    void TypeChartSlot();//Перерисовка графика в зависимости от тип графика из списка
    void ChangeDirectory();// выбор директории, чтоб открыть папку с файлами бд или json. Левая часть окна
    void SelectionChangedSlot(const QItemSelection &, const QItemSelection &);
    void onComboboxChangedSlot();//прорисовка графика ссылаясь на тип
    void repaintCharts();//рисуем график, только если он доступен (т.е. выбран файл .sqlite .json и тип графика из имеющихся bar pie, выбор только 1го файла)

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    void connectSignals();//соединение всех сигналов-слотов
    QComboBox *createTypeBox();//для названий из выпад списка
    void DrawChart();// рисование графика на основе выбранного файла и отметки чекбокса

    QFileSystemModel *dirModel;
    QTableView *tableView;
    QLabel *label, *label_path;

    //раскладки
    QVBoxLayout *VertLayout;
    QHBoxLayout *HorizLayout;
    QHBoxLayout *HParent;
    QHBoxLayout *layoutWidgetH;
    QVBoxLayout *layoutFileV;
    QVBoxLayout *verticalL;
    QVBoxLayout *verticalR;
    QHBoxLayout *horizWrapper;

    //Разделители
    QSplitter* splitterL= new QSplitter(Qt::Horizontal);
    QSplitter* splitterR = new QSplitter(Qt::Vertical);;

    //Виджеты
    //кнопки, комбокс, чекбокс, лейбл
    QComboBox *typeChart; //список- тип графика
    QCheckBox *BlackWhiteCheck; //чекбокс черно-белый
    QPushButton *printChart;//кнопка печати
    QLabel * typeChart_label;//для подписик к списку
    QPushButton *btnDirectory;//кнопка выбор директории

    QString currentPath;//путь корневой папки
    QString filePath;
    QChartView *chartView;//представление графика
    QChart* chart;

    bool AvailableGraph = false;
};

#endif // MAINWINDOW_H
