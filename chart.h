#ifndef CHART_H
#define CHART_H

#include<QPieSeries>
#include <QtCharts/QBarSet>
#include<QChart>
#include "IOC.h"
#include"mainwindow.h"
#include <QString>
#include <QPair>
#include <QVector>

typedef QPair<float,QString> Data_;//из контейнерa берем
typedef QVector< QPair<float,QString> > ContainerData_;


// Определяем классы работы с графиками так, как показано на примерах в файле с ioc контейнером moodle
//определяем интерфейс
class IntrfaceDraw

{
private:
   ContainerData_ data_;

public:

    virtual ~IntrfaceDraw()=default;
    virtual void Draw(const ContainerData_& data_) = 0;//построение графика на основе входных данных (бд,json)

};


class createPieChart: public IntrfaceDraw
{
    ~createPieChart() = default;
    void Draw(const ContainerData_& data_) override;
};

class createBarChart: public IntrfaceDraw
{
    ~createBarChart() = default;
    void Draw(const ContainerData_& data_) override;
};



#endif // CHART_H
