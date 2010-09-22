#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStatusBar>
#include <QTextEdit>
#include <QLineEdit>
#include <QComboBox>
#include <dbapi.h>
#include <QTextCursor>
#include <QTextTableCell>
#include <QTextCharFormat>
#include <QTextTableFormat>
#include <QGridLayout>
#include <QTextTableCellFormat>
#include <QDir>
#include <QStringList>
#include <QTimer>


class MainWindow : public QWidget {
    Q_OBJECT
public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
    QStatusBar * sbar;
    static MainWindow * main_win;

public slots:
    bool insert_rhymes();

private:
    QTimer * timer;
    QTextEdit * text_edit;
    QLineEdit * line_edit;
    QComboBox * combo_box;
    QString get_current_db_name();
};


class QStrVec: public QVector<int>, public QString
{
public:
    QStrVec(QString str,QVector<int> vect) : QVector<int>(vect),QString(str){}
};
bool less_than(QStrVec &,QStrVec &);
QStrVec get_overlaps(QString,QString);
QList<QStrVec> sorted_list(QString,QList<QString>);

#endif // MAINWINDOW_H
