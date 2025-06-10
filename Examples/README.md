# **Qt Examples**

Let us analyze some example projects from the QTEXAMPLES.zip resource on Piazza.

## Button
```cpp
#include <QApplication>
#include <QPushButton>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
	
    QPushButton *button = new QPushButton("Hello");
    QObject::connect(button, SIGNAL(clicked()),
                     &app, SLOT(quit()));
    button->show();
	
    return app.exec();
}
```
In this example, we can see how a ```connect()``` call can be used to bind the ```clicked()``` event
of a button to a desired response, which is closing the app in this case.

___

## Button Group

```cpp
#include <QApplication>
#include <QPushButton>
#include <QButtonGroup>
#include <QLCDNumber>
#include <QHBoxLayout>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
	
    QWidget       window ; 
    QButtonGroup  buttongroup ; 
    QPushButton  *button[2]  ;
    QHBoxLayout   layout ;
    QLCDNumber    disp ; 
    QString s ;

    for(int i=0 ; i < 2 ; i++) {
      s = QString::number(i+1) ; 
      button[i] = new QPushButton(s);
      layout.addWidget(button[i]);
      buttongroup.addButton(button[i],i+1) ; 
    }
    QObject::connect(&buttongroup, SIGNAL(buttonClicked(int)),
                     &disp, SLOT(display(int))); 

    layout.addWidget(&disp);

    window.setLayout(&layout);
    window.show();
	
    return app.exec();
}
```

This example shows how we can create a group of objects easily using for-loops.

We can also see in ```QObject::connect(&buttongroup, SIGNAL(buttonClicked(int)), &disp, SLOT(display(int))); ``` how a group of objects can be connected to the desired response at the same time within a ```connect()``` call.

___

## Currency

```cpp
// main.cpp
#include <QtCore>
#include <QApplication>
#include "myclass.h"

int main(int argc,char *argv[])
{
   QApplication a(argc, argv); 
   MyClass my ; 

   my.show(); 

   return a.exec();
}
```

```cpp
// myclass.cpp
#include <QtGui>
#include "myclass.h"
#include <QRegExp>

MyClass::MyClass(QWidget *parent) : QLabel(parent)
{

     label = new QLabel("Connecting");
     QHBoxLayout *layout = new QHBoxLayout;

     layout->addWidget(label);
     setLayout(layout);

     manager = new QNetworkAccessManager(this) ; 

     connect(manager, SIGNAL(finished(QNetworkReply *)),this, SLOT(replyFinished(QNetworkReply *)));

     // Initiate retrieval of  the data from the web page.
     // When completed, slot replyFinished will be called. 
     manager->get(QNetworkRequest(QUrl("https://www.bitstamp.net/api/v2/ticker/btcusd")));
}

void MyClass::replyFinished(QNetworkReply *reply)  {

     QString str ; 
     int pos = 0;

     // read the data fetched from the web site 
     QString data = (QString) reply->readAll();

     // use pattern matching to extract the rate 
     QRegExp rx("\"last\": \"(\\d+\\.\\d+)\"");

     if ( rx.indexIn(data, pos) != -1 ) {
       str = QString("BTC/USD:  ") + rx.cap(1);    // rate found 
     }
     else {
       str = QString("Error") ; 
     }

     // set the text of the label 
     label->setText(str) ;
}
```


```cpp
// myclass.h
#include <QtGui>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>

class MyClass : public QLabel
{
    Q_OBJECT

    public:
      MyClass(QWidget *parent = 0);
	 
	 
    public slots:
      void replyFinished(QNetworkReply * reply) ; 

    private:
      QLabel *label;
      QNetworkAccessManager *manager ;
} ;
```

This project retrieves and displays the current BTC/USD exchange rate from the Bitstamp API. One thing to note is that we need ```QT += network``` in the project file in order to use Qt's netwrok module since classes like ```QNetworkAccessManager``` depend on it.

Qt is modular, and we need to explicitly declare which modules we want to use in each project. Without listing the appropriate modules in the project file, Qt will not be able to link the required libraries, leading to compilation and linking errors.

___

## Designer

In Qt it is possible to separate the visual and the backend logic of your programs. This allows the programmer to change the UI without needing to change the backend logic, allowing easier customization. This is done via the ```.ui``` file, usually generated using the Qt Designer.

The strength of the ```.ui``` file is that it is both machine- and human-readable, thanks to the fact that is uses the XML format.

___

## Linguist

In real life, apps are generally used by people who speak different languages. Creating a separate app for each language is cumbersome and wastes storage space. As an example of how having a different app for each supported language creates problems for users, Fallout 3 on Game Pass in the Microsoft Store (a role-playing video game about a post-apocalyptic society) takes 43 GB of space instead of the usual 8 GB on different platforms, because it downloads the game separately for English, French, German, Spanish, and Italian.

Qt allows dynamically loading translation files based on user preferences or system language to prevent this type of problems. We can see how Qt's modular structure is used for this goal in the provided Linguist project. The source translations files (```.ts```) are compiled, and the compiled translation files (```.qm```) are used by the project to supply the user with the correct language during app operations. This format allows the porgrammer to add support for different languages along the way.

___

## Spinslider

```cpp
#include <QApplication>
#include <QHBoxLayout>
#include <QSlider>
#include <QSpinBox>

int main(int argc, char *argv[])
{
      QApplication app(argc, argv);

      QWidget *window = new QWidget;
      window->setWindowTitle("Enter Your Age");

     QSpinBox *spinBox = new QSpinBox;
     QSlider *slider = new QSlider(Qt::Horizontal);
     spinBox->setRange(0, 130);
     slider->setRange(0, 130);

     QObject::connect(spinBox, SIGNAL(valueChanged(int)),
                      slider, SLOT(setValue(int)));
     QObject::connect(slider, SIGNAL(valueChanged(int)),
                      spinBox, SLOT(setValue(int)));
     spinBox->setValue(35);

     QHBoxLayout *layout = new QHBoxLayout;
     layout->addWidget(spinBox);
     layout->addWidget(slider);
     window->setLayout(layout);

     window->show();

     return app.exec();
}
```

This example illustrates how two-way bindings can be utilised to create synchronous objects.

```cpp
QObject::connect(spinBox, SIGNAL(valueChanged(int)), slider, SLOT(setValue(int)));
QObject::connect(slider, SIGNAL(valueChanged(int)), spinBox, SLOT(setValue(int)));
```

Thanks to the above code, the values maintained by the ```spinBox``` and ```slider``` remain equal at all times. This synchronization allows users to interact with either control — adjusting the value using the spinBox for precision or the slider for speed — while ensuring that both always reflect the same value. This improves flexibility and usability in the user interface.
