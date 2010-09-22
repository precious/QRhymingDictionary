#ifndef DBAPI_H
#define DBAPI_H

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QVariant>
#include <fstream>
#include <iostream>
#include <QTextCodec>
#include <QSqlField>
#include <QSqlRecord>
#include <QSqlDriver>
#include <QMap>

using namespace std;


QSqlDatabase get_base(QString);
int get_words_number(QString);
bool add_word(QString,QSqlDatabase &);
bool add_suffix(QString,QSqlDatabase &);
bool is_word_in_base(QString,QSqlDatabase &);
bool is_suffix_in_base(QString,QSqlDatabase &);
int get_suffix_id(QString,QSqlDatabase &);
bool new_db_from_txt(QString,bool);
bool fatal_error(QString);
int get_max_id(QString,QSqlDatabase &,bool);
QList<QString> get_rhymes(QString,QString,int);

#endif // DBAPI_H
