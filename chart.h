#ifndef CHART_H
#define CHART_H

#include<QPieSeries>
#include<QBarSeries>
#include <QtCharts/QBarSet>
#include<QChart>
#include <QString>
#include <QPair>
#include <QVector>
#include<QChartView>
#include <QtCharts/QStackedBarSeries>
#include"DatabaseJson.h"//подкл, чтоб построение графиков было на основе входных данных

// Определяем классы работы с графиками так, как показано на примерах в файле с ioc контейнером moodle
//определяем общий интерфейс
class IntrfaceDraw

{
private:
   Containerdata_ data_;// данные для графика вектор пары [значение,дата]
   QChart* chart = new QChart();//создание графика
public:

    virtual ~IntrfaceDraw()=default;
    virtual void Draw(const Containerdata_& data_, bool isBW) = 0;//построение графика на основе входных данных (бд,json) и цвет чб
    QChart *getChart() {return chart;}//получение графика
    void ClearChart();//очистка серий

};


class createPieChart: public IntrfaceDraw
{
public:
    ~createPieChart() = default;
    void Draw(const Containerdata_& data_, bool isBW) override;//построение графика типа pie
};

class createBarChart: public IntrfaceDraw
{
public:
    ~createBarChart() = default;
    void Draw(const Containerdata_& data_, bool isBW) override;//построение графика типа bar
};

#endif // CHART_H
