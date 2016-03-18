#include "pulsegenerator.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    PulseGenerator w;
    w.show();

    return a.exec();
}
