#include "dbapi.h"
#include "mainwindow.h"

bool repetitions_allowed = true;

bool fatal_error(QString error)
{
    cout << error.toUtf8().data() << endl;
    return false;
}

bool new_db_from_txt(QString filename,bool are_repetitions_allowed)
{
    repetitions_allowed = are_repetitions_allowed;
    QTextCodec *codec = QTextCodec::codecForName("UTF-8");
    QTextCodec::setCodecForCStrings(codec);

    QString db_name = filename;
    if (db_name.endsWith(".txt") && db_name.length() > 4)
        db_name.remove(db_name.length() - 4,4);
    db_name.append(".sqlite");
    cout << db_name.toAscii().data() << endl;
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE",db_name);
    db.setDatabaseName(db_name);

    fstream inputf;
    inputf.open(filename.toUtf8().data(),ifstream::in);
    if (!inputf.is_open())
        return fatal_error("unable to open an initial txt");
    char buf[50];

    if (!db.open())
        return fatal_error("unable to open the database");

    QSqlQuery query = QSqlQuery(db);

    bool flag;
    flag = query.exec("CREATE TABLE 'suffixes' ("
               "'id' 'integer primary key autoincrement',"
               "'suffix' 'varchar(10)' )");

    flag = query.exec("CREATE TABLE 'words' ("
               "'id' 'integer primary key autoincrement', "
               "'word' 'varchar(50)', "
               "'suffix' 'integer', "
               "FOREIGN KEY ('suffix') REFERENCES 'suffixes' ('id') )");

    QString word;
    int i = 0;
    while(true)
    {
        i++;
        inputf >> buf;
        if (inputf.eof())
            break;
        word = QString(buf);

        add_word(word,db);

        if(!(i%100))
            cout << "words added:" << i << endl;
    }


    db.close();
    inputf.close();
    return true;
}

bool add_word(QString word,QSqlDatabase &db)
{
    word = word.toLower();
    if (word.length() < 2 || (repetitions_allowed && is_word_in_base(word,db)))
    {
        return true;
    }

    int suff_id = get_suffix_id(word.right(2),db);

    if(suff_id == -1)
    {
        return false;
    }

    QSqlQuery query = QSqlQuery(db);
    QString query_text("INSERT INTO words VALUES('%1','%2','%3')");
    query_text = query_text.arg(get_max_id(QString("words"),db,true) + 1).arg(word).arg(suff_id);

    return query.exec(query_text);
}

bool add_suffix(QString suff,QSqlDatabase &db)
{
    if (suff.length() != 2)
    {
        return false;
    }
    QString text("INSERT INTO suffixes VALUES('%1','%2')");
    text = text.arg(get_max_id(QString("suffixes"),db,true) + 1).arg(suff);
    QSqlQuery query(db);
    return query.exec(text);
}

bool is_word_in_base(QString word,QSqlDatabase &db)
{
    QString text("SELECT word FROM words WHERE word='%1'");
    text = text.arg(word);
    QSqlQuery query(db);
    query.exec(text);
    return query.next();
}

bool is_suffix_in_base(QString suff,QSqlDatabase &db)
{
    QString text("SELECT suffix FROM suffixes WHERE suffix='%1'");
    text = text.arg(suff);
    QSqlQuery query(db);
    query.exec(text);
    return query.next();
}

int get_suffix_id(QString suff,QSqlDatabase &db)
{
    int id = -1;
    QString text("SELECT id FROM suffixes WHERE suffix='%1'");
    text = text.arg(suff);
    QSqlQuery query(db);
    query.exec(text);
    if (query.next())
    {
        id = query.value(0).toInt();
    }
    if(id == -1)
    {
        if(add_suffix(suff,db))
        {
            id = get_max_id(QString("suffixes"),db,false);
        }
    }
    return id;
}

int get_max_id(QString tablename,QSqlDatabase &db,bool will_rec_be_added)
{
    static QMap<QString,int> max_id;
    if (!max_id.contains(tablename))
    {
        max_id[tablename] = 0;
        QString text("SELECT max(id) FROM '%1'");
        text = text.arg(tablename);
        QSqlQuery query(db);
        query.exec(text);
        if (query.next())
        {
            max_id[tablename] = query.value(0).toInt();
        }
    }
    if (will_rec_be_added)
    {
        return max_id[tablename]++;
    }
    else
    {
        return max_id[tablename];
    }
}

QList<QString> get_rhymes(QString db_name,QString suff,int max_number)
{
    QList<QString> rhymes;
    QSqlDatabase db = get_base(db_name);
    if (!db.open())
    {
        rhymes.push_back(QString("unable to open the database"));
    }
    else
        if(suff.length() == 2)
        {
            int id = 0;
            QSqlQuery query(db);
            QString suff_text("SELECT id FROM suffixes WHERE suffix='%1'");
            suff_text = suff_text.arg(suff);
            query.exec(suff_text);
            if(query.next())
            {
                id = query.value(0).toInt();
            }
            else
            {
                return rhymes; //if there are no such suffix in database - return empty list
            }

            QString words_text("SELECT word FROM words WHERE suffix=%1");
            words_text = words_text.arg(id);
            query.exec(words_text);
            int counter = 0;
            while(query.next() && counter < max_number)
            {
                rhymes.push_back(query.value(0).toString());
                counter++;
            }
            db.close();
        }
    return rhymes;
}

QSqlDatabase get_base(QString db_name)
{
    QSqlDatabase db;
    if (QSqlDatabase::contains(db_name))
    {
        db = db.database(db_name);
    }
    else
    {
        db = QSqlDatabase::addDatabase("QSQLITE",db_name);
        db.setDatabaseName(db_name);
    }
    return db;
}

int get_words_number(QString db_name)
{
    int number = 0;
    QSqlDatabase db = get_base(db_name);
    if (db.open())
    {
        QSqlQuery query(db);
        QString text("SELECT count(*) FROM words");
        query.exec(text);
        if (query.next())
        {
            number = query.value(0).toInt();
        }
        db.close();
    }
    return number;
}
