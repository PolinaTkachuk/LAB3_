#ifndef CHART_H
#define CHART_H

#include<QPieSeries>
#include<QBarSeries>
#include <QtCharts/QBarSet>
#include<QChart>
#include "IOC.h"
#include"mainwindow.h"
#include <QString>
#include <QPair>
#include <QVector>
#include<QChartView>
#include <QtCharts/QStackedBarSeries>

typedef QPair<float,QString> Data_;//из контейнерa берем
//typedef QVector< QPair<float,QString> > ContainerData_;
using Containerdata_ = QVector< QPair<QString, double> >;


// Определяем классы работы с графиками так, как показано на примерах в файле с ioc контейнером moodle
//определяем интерфейс
class IntrfaceDraw

{
private:
   Containerdata_ data_;
   //QChartView *chartView=nullptr;
   //bool Colored=false;

public:

    virtual ~IntrfaceDraw()=default;
    virtual QChart* Draw(const Containerdata_& data_) = 0;//построение графика на основе входных данных (бд,json)
    //IntrfaceDraw(QWidget* parent = nullptr);
};


class createPieChart: public IntrfaceDraw
{
    bool Colored=false;
    QChartView *chartView=nullptr;
public:
    ~createPieChart() = default;
    QChart* Draw(const Containerdata_& data_) override;
};

class createBarChart: public IntrfaceDraw
{
    bool Colored=false;
    QChartView *chartView=nullptr;
public:
    ~createBarChart() = default;
    QChart* Draw(const Containerdata_& data_) override;
};



#endif // CHART_H
