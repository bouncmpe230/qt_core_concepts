#include <QApplication>
#include <QLabel>
#include <QTimer>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QLabel label("Hello Qt!");

    label.show();

    // Close the label (and app) after 1000 milliseconds (1 second)
    QTimer::singleShot(5000, &app, &QApplication::quit);

    return app.exec();
}
