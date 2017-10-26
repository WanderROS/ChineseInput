﻿#include "xytooltips.h"
#include <QPainter>
#include <QApplication>
#include <QDesktopWidget>
#include <QTimer>

XYToolTips *XYToolTips::mopInstance = NULL;
XYToolTips::XYToolTips(QWidget *parent)
    : XYBorderShadowWidget(parent)
{
    this->setWindowFlags(Qt::FramelessWindowHint
                   | Qt::WindowStaysOnTopHint
                   | Qt::Tool);
#if QT_VERSION >= 0x050000
    this->setWindowFlags(this->windowFlags() | Qt::WindowDoesNotAcceptFocus);
#endif

    moTimer.setInterval(2000);
    moTimer.setSingleShot(true);
    connect(&moTimer, SIGNAL(timeout()), this, SLOT(close()));
    setFocus();
}

XYToolTips::~XYToolTips()
{

}

XYToolTips *XYToolTips::getInstance()
{
    if (mopInstance == NULL)
    {
        mopInstance = new XYToolTips;
    }
    return mopInstance;
}

void XYToolTips::showToolTips(const QString &tooltips)
{
    QPoint pos = QCursor::pos();
    showToolTips(tooltips, pos);
}

void XYToolTips::showToolTips(const QString &tooltips, const QPoint &pos)
{
    if (tooltips.trimmed().isEmpty())
    {
        return;
    }
    XYToolTips *toolwd = getInstance();
    if (!toolwd->isHidden())
    {
        toolwd->close();
    }
    toolwd->msToolTips = tooltips;
    QFontMetrics metrics(qApp->font());
    int w = metrics.width(tooltips) + 25;
    int h = metrics.height() + 25;
    toolwd->show();

    QDesktopWidget *top = QApplication::desktop();
    QPoint f_pos = pos;
    if (top->width() < w + f_pos.x())
    {
        f_pos.setX(top->width() - w - 5);
    }
    if (top->height() < h + f_pos.y())
    {
        f_pos.setY(top->height() - h - 5);
    }
    toolwd->move(f_pos);
    toolwd->moTimer.start();
}

bool XYToolTips::close()
{
    moTimer.stop();
    return QWidget::close();
}

void XYToolTips::paintEvent(QPaintEvent *event)
{
    QFontMetrics metrics(qApp->font());
    int w = metrics.width(msToolTips);
    int h = metrics.height();

    this->setFixedSize(w + 30, h + 30);
    XYBorderShadowWidget::paintEvent(event);
    QPainter painter(this);

    painter.drawText(QRect( (width() - w) / 2,
                           (height() - h) / 2,
                           width(),
                           height()),
                     msToolTips);
}

void XYToolTips::focusOutEvent(QFocusEvent *event)
{
    close();
}

void XYToolTips::clicked(const QPoint &point)
{
    focusOutEvent(NULL);
}

