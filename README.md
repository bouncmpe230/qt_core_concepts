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

# **What's Next?**

Qt offers features beyond standard C++ that are not covered in this page. For example, one can wonder how can a framework built upon C++ manage to handle signals and slots despite the fact that they are not a part of C++. In order to understand how, you need to first understand Qt's Meta-Object Compiler.

Meta-Object Compiler (simply ```moc```) is a code generator that processes header files and produces additional code that implements meta-object features of Qt. Qt's event system, signal/slot system, dynamic properties... exist all thanks to the ```moc```. Those interested can learn more about ```moc``` from the [official documentation page](https://doc.qt.io/qt-6/metaobjects.html). 
