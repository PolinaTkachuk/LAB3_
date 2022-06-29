#include"chart.h"

void IntrfaceDraw::ClearChart() {
    //Удаляет все серии, добавленные на график. очистка графика.
    chart->removeAllSeries();
}

void createPieChart::Draw(const Containerdata_& data_, bool isBW)
{
    QChart *chart = getChart();
    chart->setTitle("Pie chart");
    Data_ data;
    QPieSeries *series = new QPieSeries(chart);
    for (int i = 0; i <data_.count(); i++) {
      QPieSlice *slice = series->append(data_[i].first, data_[i].second);

            if (isBW)
            {
                //меняем цвет на чб, используя рандомную раскладку оттенков серого от белого к черному
                int bw = (rand() % 255 + rand() % 255 + rand() % 255) / 3;
                // присваиваем элементам диаграммы эти цветовые данные
                slice->setBrush(QColor(bw, bw, bw));
            }
    }
        //очистка графика от серий старых
        chart->removeAllSeries();
        //добавление новых серий(долей) на график
        chart->addSeries(series);

}



void createBarChart::Draw(const Containerdata_& data_, bool isBW)
{
    //QChart *chart = new QChart();
    QChart *chart = getChart();
    chart->setTitle("Bar chart");
    QBarSeries *series = new QBarSeries(chart);
       for (int i = 0; i < data_.count(); i++)
       {
           //QBarSet *set = new QBarSet("Bar set " + QString::number(i));
           QBarSet *set = new QBarSet(data_[i].first);
           if (isBW)
           {
               //меняем цвет на чб, используя рандомную раскладку оттенков серого от белого к черному
               int bw = (rand() % 255 + rand() % 255 + rand() % 255) / 3;
                // присваиваем элементам диаграммы эти цветовые данные
               set->setBrush(QColor(bw, bw, bw));
               /*
               QLinearGradient gradient(0, 0, chartView->width(), chartView->height());
               gradient.setColorAt(0, Qt::black);
               gradient.setColorAt(1, Qt::white);

               QBrush brush(gradient);
               set->setBrush(brush);
               */

           }
           *set << data_[i].second;
           series->append(set);
       }
       chart->removeAllSeries();
       chart->addSeries(series);
       chart->createDefaultAxes();

}
