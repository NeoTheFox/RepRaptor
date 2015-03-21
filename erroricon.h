#ifndef ERRORICON_H
#define ERRORICON_H

#include <QWidget>
#include <QTimer>
#include <QPainter>

class ErrorIcon : public QWidget
{
    Q_OBJECT
public:
    explicit ErrorIcon(QWidget *parent = 0);
    ~ErrorIcon();
    QTimer *animation;
    QString frame;
    int framenum;

protected:
    virtual void paintEvent(QPaintEvent *);

signals:

public slots:
    void changeFrame();
};

#endif // ERRORICON_H
