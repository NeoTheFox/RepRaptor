#include "erroricon.h"

ErrorIcon::ErrorIcon(QWidget *parent) : QWidget(parent)
{
    framenum = 0;
    frame = ":icons/error_a.png";
    animation.setInterval(300);
    animation.start();

    connect(&animation, SIGNAL(timeout()), this, SLOT(changeFrame()));
}

void ErrorIcon::paintEvent(QPaintEvent *pe)
{
    QPainter painter(this);
    painter.drawPixmap(0,0,128,87,QPixmap(frame));
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

}

