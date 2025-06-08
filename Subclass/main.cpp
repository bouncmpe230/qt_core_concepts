#include <QApplication>
#include "mylabel.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    MyLabel label("Click me!");
    label.resize(200, 100);
    label.show();

    return app.exec();
}
