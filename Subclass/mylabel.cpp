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
