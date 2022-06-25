#include"chart.h"


QChart* createPieChart::Draw(const Containerdata_& data_)
{
    QChart *chart = new QChart();
    chart->setTitle("Pie chart");
    Data_ data;

    for (int i = 0; i <data_.count(); i++) {
        QPieSeries *series = new QPieSeries(chart);
        series->append(
                               data_[i].first,
                               data_[i].second
                   );
           QPieSlice *slice = series->append(data_[i].first, data_[i].second);
            /*if (data==data_[i].first()) {
                slice->setLabelVisible();
                slice->setExploded();
              }*/
            if (BlackWhiteCheck)
            {
               //QConicalGradient::QConicalGradient(qreal cx, qreal cy, qreal angle)
                //Интерполировать цвета вокруг центральной точки (QConicalGradient)- круговой
                QConicalGradient gradient( chartView->height(), chartView->width(),360);
               // QLinearGradient gradient(0, 0, chartView->height(), chartView->width());
                gradient.setColorAt(0, Qt::black);
                gradient.setColorAt(1, Qt::white);

                QBrush brush(gradient);
                slice->setBrush(brush);
            }

        qreal pieSize = 1.0 /data_.count();
        qreal hPos = (pieSize / 2) + (i / (qreal)data_.count());
        series->setPieSize(pieSize);
        series->setHorizontalPosition(hPos);
        series->setVerticalPosition(0.5);
        chart->addSeries(series);
    }

    return chart;
}



QChart* createBarChart::Draw(const Containerdata_& data_)
{
    QChart *chart = new QChart();
    chart->setTitle("Bar chart");
    QBarSeries *series = new QBarSeries(chart);
       for (int i = 0; i < data_.count(); i++)
       {
           //QBarSet *set = new QBarSet("Bar set " + QString::number(i));
           QBarSet *set = new QBarSet(data_[i].first);
           if (BlackWhiteCheck)
           {
               QLinearGradient gradient(0, 0, chartView->width(), chartView->height());
               gradient.setColorAt(0, Qt::black);
               gradient.setColorAt(1, Qt::white);

               QBrush brush(gradient);
               set->setBrush(brush);

           }
           *set << data_[i].second;
           series->append(set);
       }

       chart->addSeries(series);
       chart->createDefaultAxes();

       return chart;
       //chartView->setChart(chart);

}
/*

 void Chart::ReDrawingChart()
 {
     gContainer.GetObject<IntrfaceDraw>()->drawChart();

 }*/
