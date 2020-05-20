/*
* Copyright (C) 2019 ~ 2020 Deepin Technology Co., Ltd.
*
* Author:     zhangkai <zhangkai@uniontech.com>
* Maintainer: zhangkai <zhangkai@uniontech.com>
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* any later version.
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#include "partchartshowing.h"
#include <QDebug>
PartChartShowing::PartChartShowing(QWidget *parent) : QWidget(parent)
{

}

void PartChartShowing::getData(const QString &totalsize, const QVector<double>sizeInfo)
{
    int i = totalsize.lastIndexOf("G");
    total = totalsize.left(i).toDouble();
    if (total < 1) {
        total = total * 1024;
    }
    partsize = sizeInfo;
//    path = paintpath;
    qDebug() << total << partsize;
}



//PartChartShowing::PartChartShowing(int used, int trueused, int noused, QWidget *parent): m_used(used), m_tureused(trueused), m_noused(noused)
//{

//}

void PartChartShowing::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.save();
    painter.setRenderHint(QPainter::Antialiasing);
    QRect rect;
    rect.setX(this->x());
    rect.setY(this->y());
    rect.setWidth(this->width());
    QRect paintRect = QRect(0, 10, rect.width(), 35);
    QPainterPath paintpath;
    const int radius = 8;

    paintpath.moveTo(paintRect.bottomRight() - QPoint(0, radius));
    paintpath.lineTo(paintRect.topRight() + QPoint(0, radius));
    paintpath.arcTo(QRect(QPoint(paintRect.topRight() - QPoint(radius * 2, 0)),
                          QSize(radius * 2, radius * 2)),
                    0, 90);
    paintpath.lineTo(paintRect.topLeft() + QPoint(radius, 0));
    paintpath.arcTo(QRect(QPoint(paintRect.topLeft()), QSize(radius * 2, radius * 2)), 90, 90);
    paintpath.lineTo(paintRect.bottomLeft() - QPoint(0, radius));
    paintpath.arcTo(QRect(QPoint(paintRect.bottomLeft() - QPoint(0, radius * 2)),
                          QSize(radius * 2, radius * 2)),
                    180, 90);
    paintpath.lineTo(paintRect.bottomRight() - QPoint(radius, 0));
    paintpath.arcTo(QRect(QPoint(paintRect.bottomRight() - QPoint(radius * 2, radius * 2)),
                          QSize(radius * 2, radius * 2)),
                    270, 90);

    QColor fillColor = QColor(210, 210, 210); //this->palette().color(DPalette::Normal, DPalette::Highlight)
    painter.setBrush(QBrush(fillColor));
    painter.fillPath(paintpath, fillColor);
    painter.setPen(QPen(QColor(fillColor), 3));
    painter.drawLine(paintRect.topLeft().x() + radius, paintRect.topLeft().y() + radius, paintRect.topLeft().x() + radius + paintRect.width(), paintRect.bottomLeft().y() - radius);
    QColor tipColor = QColor(210, 210, 210);


    painter.setPen(tipColor);
    painter.drawRect(this->x() + 20, this->y() + 50, 10, 10);

    painter.setPen(this->palette().text().color());
    QRect textRect(this->x() + 35, this->y() + 45, 80, 50);
    painter.drawText(textRect, tr("Free space"));


    QWidget::paintEvent(event);

    update();

    addPaint(&painter);
    painter.restore();


}

void PartChartShowing::addPaint(QPainter *painter)
{

    painter->setPen(Qt::NoPen);
    painter->setRenderHint(QPainter::Antialiasing);
    QRect rect;
    rect.setX(this->x());
    rect.setY(this->y());
    rect.setWidth(this->width());
    QRect paintRect = QRect(0, 10, rect.width(), 35);
    QPainterPath path0, path1, path2, path3, path4, path5, path6, path7, path8, path9, path10, path11, path12;
    QVector<QPainterPath>path{path0, path1, path2, path3, path4, path5, path6, path7, path8, path9, path10, path11, path12};
    QPainterPath paintpath;
    const int radius = 8;
    double sum = 0.00;

    for (int i = 0; i < partsize.size(); i++) {
        sum = sum + partsize.at(i);
        if (i == 0) {
            path[0].moveTo(paintRect.topLeft() + QPoint(radius, 0));
            path[0].arcTo(QRect(QPoint(paintRect.topLeft()), QSize(radius * 2, radius * 2)), 90, 90);
            path[0].lineTo(paintRect.bottomLeft() - QPoint(0, radius));
            path[0].arcTo(QRect(QPoint(paintRect.bottomLeft() - QPoint(0, radius * 2)),
                                QSize(radius * 2, radius * 2)),
                          180, 90);
            path[0].lineTo(paintRect.bottomLeft() + QPoint(static_cast<int>((partsize.at(i) / total)*paintRect.width() + radius), 0));
            path[0].lineTo(paintRect.topLeft() + QPoint(static_cast<int>((partsize.at(i) / total)*paintRect.width() + radius), 0));
            path[0].lineTo(paintRect.topLeft() + QPoint(radius, 0));
            painter->fillPath(path[0], QBrush(QColor(this->palette().highlight().color())));
        } else if (sum < total && i > 0) {
            path[i].moveTo(path[i - 1].currentPosition() + QPoint(static_cast<int>((partsize.at(i - 1) / total)*paintRect.width()), 0));
            path[i].lineTo(path[i - 1].currentPosition() + QPoint(static_cast<int>((partsize.at(i - 1) / total)*paintRect.width() + (partsize.at(i) / total)*paintRect.width()), 0));
            path[i].lineTo(path[i - 1].currentPosition() + QPoint(static_cast<int>((partsize.at(i - 1) / total)*paintRect.width() + (partsize.at(i) / total)*paintRect.width()), paintRect.height() - 1));
            path[i].lineTo(path[i - 1].currentPosition() + QPoint(static_cast<int>((partsize.at(i - 1) / total)*paintRect.width()), paintRect.height() - 1));
            path[i].lineTo(path[i - 1].currentPosition() + QPoint(static_cast<int>((partsize.at(i - 1) / total)*paintRect.width()), 0));

            painter->fillPath(path[i], QBrush(QColor(this->palette().highlight().color())));
        } else if (sum >= total) {
            path[partsize.size() - 1].moveTo(paintRect.bottomRight() - QPoint(0, radius));
            path[partsize.size() - 1].lineTo(paintRect.topRight() + QPoint(0, radius));
            path[partsize.size() - 1].arcTo(QRect(QPoint(paintRect.topRight() - QPoint(radius * 2, 0)),
                                                  QSize(radius * 2, radius * 2)),
                                            0, 90);
            path[partsize.size() - 1].lineTo(path[partsize.size() - 2].currentPosition() + QPoint(static_cast<int>((partsize.at(partsize.size() - 2) / total))*paintRect.width(), 0));
            path[partsize.size() - 1].lineTo(path[partsize.size() - 2].currentPosition() + QPoint(static_cast<int>((partsize.at(partsize.size() - 2) / total)*paintRect.width()), paintRect.height() - 1));
            path[partsize.size() - 1].lineTo(paintRect.bottomRight() - QPoint(radius, 0));
            path[partsize.size() - 1].arcTo(QRect(QPoint(paintRect.bottomRight() - QPoint(radius * 2, radius * 2)),
                                                  QSize(radius * 2, radius * 2)),
                                            270, 90);
            painter->fillPath(path[partsize.size() - 1], QBrush(QColor(this->palette().highlight().color())));
        }
        curPoint = QPoint(static_cast<int>(path[i].currentPosition().x()), 0);
//        getCurRect = QRect(curPoint.x(), curPoint.y(), static_cast<int>((partsize.at(i) / total) * paintRect.width()), paintRect.height());
//        allRect.append(getCurRect);

    }
    getCurRect = QRect(0, 10, rect.width(), 35);
}

void PartChartShowing::mousePressEvent(QMouseEvent *event)
{

    if (event->button() == Qt::LeftButton) {
//        qDebug() << "121212";
        int x = event->pos().x();
        int y = event->pos().y();

        if (x > 0 && x < getCurRect.width() && y > 10 && y < 45) {
            qDebug() << "232323" ;
        }


        update();
    }


}
