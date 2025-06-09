# **Qt Core Concepts**

## **1. What is Qt?**

Qt is a cross-platform GUI framework.

## **What *exactly* is Qt?**

When you search about Qt online, you will inevitably see the sentence: “*Qt is a cross-platform application development framework for creating graphical user interfaces as well as cross-platform applications that run on various software and hardware platforms such as Linux, Windows, macOS, Android or embedded systems with little or no change in the underlying codebase while still being a native application with native capabilities and speed.*” show up in several places.

But what does that mean? And what is it about Qt that makes it the preferred GUI (Graphical User Interface) framework for over a million developers in over 70 industries?

* ```Qt is a cross-platform application development framework…``` — Qt is a framework -a collection of libraries and tools- that you can use to build a program, which you can then run on multiple operating systems.

* ```…creating graphical user interfaces…``` — Qt is mainly used to build GUIs. A GUI of a program is everything that the user sees during the operation of the program. Buttons, sliders, search bars; all are a part of the GUI.

* ```…cross-platform applications that run on various software and hardware platforms such as Linux, Windows, macOS, Android or embedded systems…``` — While GUIs are definitely what Qt is known for, you are not limited to only developing GUIs through Qt. You can also create programs that function on different operating systems.

* ```…with little or no change in the underlying codebase while still being a native application with native capabilities and speed.``` — One of Qt’s strengths is that you can run the same code on other platforms and expect the same results. And all of this is done at native speeds, as if the program was coded just for your chosen platform. Qt manages this by setting up its API calls through the system’s native APIs, no virtualization needed.

## **To Summarize...**

Qt lets developers write code once to build high-performance, native-looking applications that run on many platforms—without needing to rewrite the code for each one. It handles much of the platform-specific complexity internally while giving you full access to native features when needed.

Because Qt is written in C++ and exposes a C++ API, every fundamental you already know from the **C Core Concepts** (compilation, linking, pointers)  and the **C++ Core Concepts** (classes, constructors, inheritance, polymorphism, templates) transfer directly to Qt programming.

You are now almost ready to learn how Qt can be used to create programs. But first, let’s see some terms and what they are.

---

## **2. Qt Terms**

| Qt term                    | Explanation                                                                                                                                                                                                                                                                                                                 |
| -------------------------- | ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| **`QObject`**              | A base class, much like the `Animal` base in the inheritance example.                                                                                                                                                                                                                 |
| **Signals & Slots**        | Similar to *function pointers*. A **signal** is a typed function prototype emitted at runtime; a **slot** is an ordinary member function that can be invoked via an *auto-generated* function pointer — no manual casting needed.
| **Parent/child hierarchy** | Automatic memory management: when a `QObject` parent is destroyed, it deletes its children. This builds on the pointer & heap concepts covered in C and destructor rules in C++ .                                                          |
---

## **3. Your First Qt Application**


```pro
# greeter.pro
QT += widgets

SOURCES += \
    greeter.cpp
```

```QT += widgets``` — Need to include the Qt Widgets module in order to use GUI components like windows, labels, etc.

```SOURCES += greeter.cpp``` — Specifies the source file(s).

```cpp
// greeter.cpp
#include <QApplication>
#include <QLabel>
#include <QTimer>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QLabel label("Hello Qt!");

    label.show();

    // Close the label (and app) after 5000 milliseconds (5 second)
    QTimer::singleShot(5000, &app, &QApplication::quit);

    return app.exec();
}

```

```int main(int argc, char *argv[])``` — The entry point of the application, should be familiar from C.

```QApplication app(argc, argv);``` — Creates a QApplication object named app. This initializes the Qt application environment and processes any command-line arguments (Which we don't have any in this simple project).

```QLabel label("Hello Qt!");``` — Creates a QLabel widget named "label". This QLabel object is responsible for the text you see on the window.

```label.show();``` — In order for an object to be visible (and interactible, in the case of buttons and the like) ```show()``` needs to be called on the object.

```QTimer::singleShot(5000, &app, &QApplication::quit);``` — A timer that emits a single pulse after some time, invoking the specified slot (hence quitting in this example). Normally, Qt uses ```connect()``` blocks to connect signals with slots; however, ```singleShot``` does this connection automatically, no need for a separate ```connect()``` call.

```return app.exec();``` — Starts the Qt event loop, which waits for and dispatches GUI events (like mouse clicks, timer events, etc.). The program will stay in this loop until app.quit() is called, after which it returns and the program ends.

**Build & run**

```bash
qmake greeter.pro
make
open greeter.app 
```
---

## **4. Connect signals and slots with ```connect()```**

```connect()``` is an integral part of Qt programming, and is used for **event-driven programming**. It connects the **signals** (events) of one object (**sender**) to **slots** (functions) of another object (**receiver**).

A typical ```connect()``` call looks like this:
```connect(sender, SIGNAL(signal), receiver, SLOT(slot));```

This call declares the communication between the sender object’s signal, and the receiver object’s slot. When the sender object emits the specified signal, the receiver object’s function specified on the slot is called and executed.

Example code:

```connect(button, SIGNAL(clicked()), textBox, SLOT(changeText()));```

When the ```button``` object is clicked, a ```clicked()``` signal is emitted. This signal is communicated to the ```textBox``` object, and triggers its ```changeText()``` function.

Those familiar with network or robotic applications should notice that the Qt’s ```connect()``` call is conceptually similar to the Publish/Subscribe (pub/sub) model used in WebSockets and ROS. One can further explore this similarity by doing a search on **event-driven communication**.

---

## **5. Subclassing in Qt**

**Subclassing** is the basic act of defining a “derived” class that inherits the data members and member functions of its “base” class. But why is it important for us in Qt?

In a sense, Qt is built around inheritance. If you follow the inheritance tree, you will see that almost everything ultimately derives from QObject, QWidget, or QQuickItem. This means that if you want to add a signal or a slot to an object, or handle events in a different way than is inherited, you can simply *subclass* to achieve the desired outcome.

## **6. Subclassing example**

**Goal:** Make a label change its text when clicked.

##### Project `customlabel.pro`
```pro
QT += widgets

SOURCES += \
    main.cpp \
    mylabel.cpp

HEADERS += \
    mylabel.h
```

##### Header `mylabel.h`

```cpp
#ifndef MYLABEL_H
#define MYLABEL_H

#include <QLabel>
#include <QMouseEvent>

class MyLabel : public QLabel
{
    Q_OBJECT

public:
    explicit MyLabel(const QString &text, QWidget *parent = nullptr);

protected:
    void mousePressEvent(QMouseEvent *event) override;
};

#endif // MYLABEL_H

```

##### Source `mylabel.cpp`

```cpp
#include "mylabel.h"

MyLabel::MyLabel(const QString &text, QWidget *parent)
    : QLabel(text, parent)
{
    setAlignment(Qt::AlignCenter);
    setStyleSheet("font-size: 20px;");
}

void MyLabel::mousePressEvent(QMouseEvent *event)
{
    setText("Label clicked!");
    QLabel::mousePressEvent(event); // call base class if needed
}

```

##### Usage in `main.cpp`

```cpp
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

```

**What just happened?**
We **subclassed** `QLabel` to override `mousePressEvent`. This way, we were able to create our own label object that extends the one defined in Qt's base libraries.

Subclassing in Qt is a fundamental technique that allows developers to extend and customize the behavior of existing widgets beyond their default functionality. By creating subclasses of Qt's base classes like ```QLabel```, ```QPushButton```, or ```QWidget```, developers can override event handlers (such as mouse or keyboard events), define new properties, or introduce custom painting and logic.

Without the flexibility offered by subclassing, developers would be stuck using the same widgets with the same looks and functionalities. One wouldn't be able to disable an unwanted function of a widget, for example.

Thanks to subclassing, one can easily modify the offered elements to their needs with minimal work.

# **Qt Examples**

In this section we will analyze some example projects from the QTEXAMPLES.zip resource on Piazza.

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

# **What's Next?**

Qt offers features beyond standard C++ that are not covered in this page. For example, one can wonder how can a framework built upon C++ manage to handle signals and slots despite the fact that they are not a part of C++. In order to understand how, you need to first understand Qt's Meta-Object Compiler.

Meta-Object Compiler (simply ```moc```) is a code generator that processes header files and produces additional code that implements meta-object features of Qt. Qt's event system, signal/slot system, dynamic properties... exist all thanks to the ```moc```. Those interested can learn more about ```moc``` from the [official documentation page](https://doc.qt.io/qt-6/metaobjects.html). 
