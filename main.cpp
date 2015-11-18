#include <QCoreApplication>
#include <QTimer>
#include "twit_func.h"

using namespace std;

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    twit_func m(0, &a);
    QTimer::singleShot(0, &m, SLOT(run()));
    return a.exec();
}

