#include <QtGui/QApplication>
#include "mainwindow.h"
#include "dbapi.h"
#include <cstring>

int main(int argc, char ** argv)
{
    if (argc > 1)
    {
        bool are_repetitions_allowed = true;
        for(int i = 1;i < argc;i++)
        {
            if (strstr(argv[i],"-n"))
            {
                are_repetitions_allowed = false;
            }
        }
        new_db_from_txt(argv[1],are_repetitions_allowed);
        exit(0);
    }

    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
