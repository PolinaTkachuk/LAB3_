#include"DatabaseJson.h"

//Данные характеризуются парой [значение, дата], хранятся в БД SQLite
Containerdata_ dataSql::getData (const QString& filePath)
{
    QString name_ = filePath;
    //ДОБАВЛЕНИЕ БД ИСХОДНОЙ
    QSqlDatabase  db=QSqlDatabase::addDatabase("QSQLITE");//указываем тип бд
    db.setConnectOptions("QSQLITE_OPEN_READONLY=1");//Возвращает строку используемых опций соединения.
    db.setDatabaseName(filePath);//полное имя базы данных (зависит от того, на какую мы нажали- передали в аргументе)
    //db.setDatabaseName("./dbase.BLOOD_SUGAR.sqlite");

      if(db.open())//проверка на успешное открытие бд
      {
          qDebug() << " База данных успешно открылась\n";//выводим об успешносм открытие сообщение

          //чтобы работать с данными бд, необходимо в имени бд убрать все символы начиная с начала до слеша
          //а также после точки , то есть оставить только название.
    //remove ( int position, int n )Удаляет n символов из строки, начиная с позиции с индексом position,возвращает ссылку на строку.
          name_.remove(0,name_.lastIndexOf('/') + 1);
          int dif= name_.size() - name_.indexOf('.');
          name_.remove(name_.indexOf('.'),dif);


          int i=0;
           QVector<Data_> data_;
          //делаем селект к выбранной базе данных с корректным кратким  именем
            QSqlQuery query("SELECT * FROM " + name_, db);
          while (query.next() && i < 8)//проходим по всем данным бд
          {
              i++;//учёт только значимых данных, после 8го идут незначимые
               //значение 1го столбца и 2го
              auto date_ = query.value(0).toString();
              auto value_ = query.value(1).toDouble();
              //фиксируем для графика дата и значение
              data_.push_back(qMakePair(date_, value_));

          }
           return data_;//возвращаем данные в нужном виде для графика
      }
      else
      {
          //"Ошибка подключения к БД: "
          qDebug() << db.lastError().text();
          return QVector<Data_>{};
      }
};

//Данные представлены JSON файлом. Формат данных [значение , дата].
Containerdata_ dataJSON::getData (const QString& filePath)
{
    QFile file; QString st;
     QVector <Data_> data;//эти данные и возвращаем
    file.setFileName(filePath);
    file.open(QIODevice::ReadOnly | QIODevice::Text);// открываем файл для чтения
    if (file.isOpen())//если открылся
    {
        st = file.readAll();
        file.close();
        // открываем как json документ
        auto json_ = QJsonDocument::fromJson(st.toUtf8());
        //рассматриваем объект json
        QJsonObject Obj_json = json_.object();
        //извлекаем значения в список
        QStringList val = Obj_json.keys();
        //формируем список с этими значениями
        QListIterator<QString> iterator(val);
         int i = 0;
           while (iterator.hasNext() && i < 8) //проходим по всем значимым значениям, незначимые после 8ми
           {
               QString key = iterator.next();
               double value = Obj_json.value(key).toDouble();// расчет значения
               data.push_back(Data_(key, value));//записываем ключ(дата) и значение для построения графиков
           }
        return data;
    }
    else
    {
        qDebug() << "Файл json не открылся";
        return QVector<Data_>{};
    }
 }
