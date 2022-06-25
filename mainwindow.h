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
#include "IOC.h"

//виды диаграмм
enum TypeChart
{
    Pie,
    Bar

};

class MainWindow : public QMainWindow
{
    Q_OBJECT
private slots:

    //void ColoredSlot();
    //void on_selectionChangedSlot(const QItemSelection &selected, const QItemSelection &deselected);
    //void on_select_comboboxOnChangedSlot(const int index);

    //void selectInTableSlot(const QItemSelection &selected, const QItemSelection &deselected);
private:
    //void connectSignals();
public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
    bool Colored();

private:
     QComboBox *createTypeBox();//для названий из выпад списка

    QFileSystemModel *fileModel;
    QFileSystemModel *dirModel;
    QTreeView *treeView;
    QTableView *tableView;
    QPushButton *btnPrint;
    QCheckBox *checkbox;
    QComboBox *combobox;
    QLabel *label, *label_path;

    //раскладки
    QVBoxLayout *VertLayout;
    QHBoxLayout *HorizLayout;

    QSplitter* splitterH= new QSplitter(Qt::Horizontal);
    QSplitter* splitterV = new QSplitter(Qt::Vertical);;

    //DataTable new_dataTable; //табличное представление
    QComboBox *typeChart; //список- тип графика
    QCheckBox *BlackWhiteCheck; //чекбокс черно-белый
    QPushButton *printChart;//кнопка печати
    QLabel * typeChart_label;//для подписик к списку

    QChartView *chartView;
};

#endif // MAINWINDOW_H
