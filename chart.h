#ifndef CHART_H
#define CHART_H

#include<QPieSeries>
#include<QBarSeries>
#include <QtCharts/QBarSet>
#include<QChart>
#include "IOC.h"
#include <QString>
#include <QPair>
#include <QVector>
#include<QChartView>
#include <QtCharts/QStackedBarSeries>
#include"DatabaseJson.h"

//typedef QPair<QString,double> Data_;//из контейнерa берем
//typedef QVector< QPair<float,QString> > ContainerData_;
//using Containerdata_ = QVector< QPair<QString, double> >;


// Определяем классы работы с графиками так, как показано на примерах в файле с ioc контейнером moodle
//определяем интерфейс
class IntrfaceDraw

{
private:
   Containerdata_ data_;
public:

    virtual ~IntrfaceDraw()=default;
    virtual QChart* Draw(const Containerdata_& data_) = 0;//построение графика на основе входных данных (бд,json)

};


class createPieChart: public IntrfaceDraw
{
    bool BlackWhiteCheck=false;
    QChartView *chartView=nullptr;
public:
    ~createPieChart() = default;
    QChart* Draw(const Containerdata_& data_) override;

};

class createBarChart: public IntrfaceDraw
{
    bool BlackWhiteCheck=false;
    QChartView *chartView=nullptr;
public:
    ~createBarChart() = default;
    QChart* Draw(const Containerdata_& data_) override;
};


class Chart
{
public:

    Chart() = default;
    virtual ~Chart() = default;

    QChart* getChart();

    void drawingChart(const QString& title, const Containerdata_& data);//рисование графика на основе переданных данных
    void ReDrawingChart(); //перерисовка графика

};


#endif // CHART_H
