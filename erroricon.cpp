#include "erroricon.h"

ErrorIcon::ErrorIcon(QWidget *parent) : QWidget(parent)
{
    framenum = 0;
    frame = ":icons/error_a.png";
    animation = new QTimer(this);
    animation->setInterval(300);
    animation->start();

    connect(animation, &QTimer::timeout, this, &ErrorIcon::changeFrame);
}

void ErrorIcon::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    QPixmap pframe(frame);
    painter.drawPixmap(0,0,pframe.width(),pframe.height(),pframe);
}

void ErrorIcon::changeFrame()
{
    if(framenum == 0)
    {
        frame = ":icons/error_a.png";
        framenum = 1;
    }
    else if(framenum == 1)
    {
        frame = ":icons/error_b.png";
        framenum = 0;
    }
    this->update();
}

ErrorIcon::~ErrorIcon()
{
    animation->stop();
}

