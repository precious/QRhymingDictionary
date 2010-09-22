#include "mainwindow.h"

MainWindow * MainWindow::main_win = NULL;

MainWindow::MainWindow(QWidget *parent) :
    QWidget(parent)
{
    sbar = new QStatusBar();
    main_win = this;
    setFixedSize(640,480);
    this->setWindowTitle(tr("QRhymingDictionary"));
    line_edit = new QLineEdit;
    combo_box = new QComboBox;
    timer = new QTimer;
    timer->setInterval(300); //mseconds
    timer->setSingleShot(true);
    QObject::connect(line_edit,SIGNAL(textChanged(QString)),timer,SLOT(start()));
    QObject::connect(timer,SIGNAL(timeout()),this,SLOT(insert_rhymes()));
    QObject::connect(combo_box,SIGNAL(currentIndexChanged(int)),this,SLOT(insert_rhymes()));
    text_edit = new QTextEdit;

    QGridLayout *grid_layout_main = new QGridLayout;
    QGridLayout *grid_layout_2nd = new QGridLayout;
    grid_layout_2nd->addWidget(line_edit,0,0);
    grid_layout_2nd->addWidget(combo_box,0,1);
    grid_layout_main->addItem(grid_layout_2nd,0,0);
    grid_layout_main->addWidget(text_edit,1,0);
    setLayout(grid_layout_main);

    QDir dir = QDir::current();
    QStringList filter;
    filter.append("*.sqlite");
    QStringList list_of_db_files = dir.entryList(filter,QDir::Files,QDir::Name);
    int number = 0;
    QString text;
    for(QList<QString>::iterator it = list_of_db_files.begin();it < list_of_db_files.end();it++)
    {
        text = *it;
        number = get_words_number(*it);
        if(text.endsWith(QString(".sqlite")))
        {
            text.chop(7);
        }
        text.append(QString(" (%1 words)").arg(number));

        combo_box->addItem(text,*it);
    }
}

MainWindow::~MainWindow()
{
    main_win = NULL;
}

bool MainWindow::insert_rhymes()
{
    QString word = line_edit->text();
    text_edit->clear();
    QList<QStrVec> rhymes = sorted_list(word,get_rhymes(get_current_db_name(),word.right(2),10000));
    if(rhymes.isEmpty())
    {
        return false;
    }
    QTextCursor cursor = text_edit->textCursor();
    cursor.beginEditBlock();
    int columns = 4;

    double d_rows = rhymes.length() / (columns * 1.0);
    int rows = (qRound(d_rows) < d_rows)? qRound(d_rows) + 1: qRound(d_rows);

    QTextTableFormat table_format;
    table_format.setAlignment(Qt::AlignLeft);
    table_format.setBorderStyle(QTextFrameFormat::BorderStyle_None);
    QVector<QTextLength> constraints;
    for(int i = 0;i < columns;i++)
    {
         constraints.append(QTextLength(QTextLength::PercentageLength, 100.0/columns));
    }
    table_format.setColumnWidthConstraints(constraints);

    QTextTable *table = cursor.insertTable(rows,columns,table_format);
    QTextCharFormat red;
    red.setForeground(QBrush(QColor(255,0,0)));
    QTextCharFormat black;
    black.setForeground(QBrush(QColor(0,0,0)));
    for(int current_row = 0;current_row < rows;current_row++)
    {
        for(int current_col = 0;current_col < columns;current_col++)
        {
            if(!rhymes.isEmpty())
            {
                QStrVec &ptr = rhymes.first();
                QTextCursor cellCursor = table->cellAt(current_row, current_col).firstCursorPosition();

                for(int i = 0;i < ptr.QString::size();i++)
                {
                    if(ptr.QVector<int>::contains(i))
                    {
                        cellCursor.setCharFormat(red);
                        cellCursor.insertText(QString(QString(ptr)[i]));
                    }
                    else
                    {
                        cellCursor.setCharFormat(black);
                        cellCursor.insertText(QString(QString(ptr)[i]));
                    }
                }
                rhymes.removeFirst();
            }
            else
            {
                break;
            }
        }
    }
    cursor.endEditBlock();
    return true;
}

QString MainWindow::get_current_db_name()
{
    return combo_box->itemData(combo_box->currentIndex()).toString();
}

QStrVec get_overlaps(QString word,QString rhyme)
{
    QVector<int> to_replace;
    int wordlen = word.length();
    int rhymlen = rhyme.length();
    int min = qMin(wordlen,rhymlen);
    for(int i = 1;i <= min;i++)
    {
        if (word[wordlen - i] == rhyme[rhymlen - i])
        {
            to_replace.append(rhymlen - i);
        }
    }
    return QStrVec(rhyme,to_replace);
}

bool less_than(QStrVec &left,QStrVec &right)
{
    return left.QVector<int>::size() > right.QVector<int>::size();
}

QList<QStrVec> sorted_list(QString word,QList<QString> rhymes)
{
    QList<QStrVec> list;
    while(!rhymes.isEmpty())
    {
        list.append(get_overlaps(word,rhymes.takeFirst()));
    }
    qSort(list.begin(),list.end(),less_than);
    return list;
}
