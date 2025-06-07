# **Qt Core Conceptss**

## **1. What even is Qt?**

When you search about Qt online, you will inevitably see the sentence: “*Qt is a cross-platform application development framework for creating graphical user interfaces as well as cross-platform applications that run on various software and hardware platforms such as Linux, Windows, macOS, Android or embedded systems with little or no change in the underlying codebase while still being a native application with native capabilities and speed.*” pop out of several places.

But what does that mean? And what is there about Qt that makes it the preferred GUI (Graphical User Interface) builder for over a million developers in over 70 industries?

```Qt is a cross-platform application development framework…``` — Qt is a framework -a collection of libraries and tools- that you can use to build a program, which you can then run on multiple operating systems.

```…creating graphical user interfaces…``` — Qt is mainly used to build GUIs. A GUI of a program is everything that the user sees during the operation of the program. Buttons, sliders, search bars; all are a part of the GUI.

```…cross-platform applications that run on various software and hardware platforms such as Linux, Windows, macOS, Android or embedded systems…``` — While GUIs are definitely what Qt is known for, you are not limited to only developing GUIs through Qt. You can also create programs that function on different operating systems.

```…with little or no change in the underlying codebase while still being a native application with native capabilities and speed.``` — One of Qt’s strengths is that you can run the same code on other platforms and expect the same results. And all of this is done at native speeds, as if the program was coded just for your chosen platform. Qt manages this by setting up its API calls through the system’s native APIs, no virtualization needed.

Qt lets developers write code once to build high-performance, native-looking applications that run on many platforms—without needing to rewrite the code for each one. It handles much of the platform-specific complexity internally while giving you full access to native features when needed.

Because Qt is written in C++ and exposes a C++ API, every fundamental you already know from the **C Core Concepts** (compilation, linking, pointers)  and the **C++ Core Concepts** (classes, constructors, inheritance, polymorphism, templates) transfers directly to Qt programming.

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

```cmake
cmake_minimum_required(VERSION 3.18)
project(QtHello LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)          # run moc automatically
set(CMAKE_AUTORCC ON)          # (for resources, not used here)
set(CMAKE_AUTOUIC ON)          # (for .ui files, not used here)

find_package(Qt6 REQUIRED COMPONENTS Core)

add_executable(QtHello
    main.cpp
)

target_link_libraries(QtHello
    Qt6::Core            # pulls in Qt6Core, pthread, etc.
)
```
```project(QtHello LANGUAGES CXX)```— Create a new project called QtHello and declare that it contains C++ (```CXX```) sources.

```find_package(Qt6 REQUIRED COMPONENTS Core)``` — Locates Qt 6 on the system.

```add_executable(QtHello main.cpp)``` — Declares the target that will be built and the source file it uses.

```cpp
// main.cpp
#include <QCoreApplication>
#include <QObject>
#include <QTimer>
#include <iostream>

class Greeter : public QObject {
    Q_OBJECT
public:
    Greeter(QObject* parent = nullptr) : QObject(parent) {}

public slots:              // slot = callable function
    void sayHello() {
        std::cout << "Hello from Qt!" << std::endl;
        QCoreApplication::quit();   // end event loop
    }
};

int main(int argc, char* argv[])
{
    QCoreApplication app(argc, argv);

    Greeter g;
    // QTimer emits the timeout() *signal* after 500 ms
    QTimer::singleShot(500, &g, &Greeter::sayHello);

    return app.exec();  // start event loop
}
```

```class Greeter : public QObject {``` — Defines a C++ class called *Greeter* that inherits signal/slot capability from QObject.

```Greeter(QObject* parent = nullptr) : QObject(parent) {}``` — A trivial constructor that forwards the parent pointer to QObject. If a parent is supplied, Qt handles deletion automatically when that parent is destroyed.

```public slots:``` — Introduces the **slot** section. Slots are regular member functions that can be invoked directly or via the signal/slot mechanism.

```void sayHello() {``` — The slot in question.

```std::cout << "Hello from Qt!" << std::endl;``` — Prints a greeting, familiar C++ code.

```QCoreApplication::quit();``` — Call that posts a “quit” event to the application event loop, causing ```app.exec()``` to return.

```int main(int argc, char* argv[])``` — Standard C++ entry point.

```QCoreApplication app(argc, argv);``` — Instantiates an application object.

```QTimer::singleShot(500, &g, &Greeter::sayHello);``` — A timer that emits a single pulse after a timer to the specified slot. Normally, Qt uses ```connect()``` blocks to connect signals with slots; however, ```singleShot``` does this connection automatically, no need for a separate ```connect()``` call.

```return app.exec();``` — Starts the event loop. The function waits here until ```quit()``` or ```exit()``` is called.

**Build & run**

```bash
mkdir build && cd build
cmake .. -DCMAKE_PREFIX_PATH=/path/to/Qt/6.x/gcc_64/lib/cmake
cmake --build .
./QtHello
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

## **5. Typical project layout**

```
MyApp/
 ├─ CMakeLists.txt
 ├─ main.cpp
 ├─ gui_main.cpp
 ├─ Main.qml
 ├─ resources.qrc   # embeds QML / images into the binary
 └─ include/
     └─ myclass.h   # your C++ headers
```
---
