#include "dmframewidget.h"
#include "utils.h"
#include <QDebug>
#include <DFontSizeManager>
#include <DApplicationHelper>

#include "partitioninfo.h"
#include "partedproxy/dmdbushandler.h"

DmFrameWidget::DmFrameWidget(DiskInfoData data, QWidget *parent): DFrame(parent), m_infodata(data)
{
    m_parentPb = DApplicationHelper::instance()->palette(this);
    connect(DApplicationHelper::instance(), &DApplicationHelper::themeTypeChanged, this,
            &DmFrameWidget::slothandleChangeTheme);
}

void DmFrameWidget::setFrameData()
{
    PartitionInfo data = DMDbusHandler::instance()->getCurPartititonInfo();

    DiskInfoData temp;
    m_infodata = temp;
    for (QString strpoint : data.mountpoints)
        m_infodata.mountpoints.append(strpoint + "  ");
    m_infodata.unused = QString::number(Utils::sector_to_unit(data.sectors_unused, data.sector_size, SIZE_UNIT::UNIT_GIB), 'f', 2) + "GB";
    m_infodata.used = QString::number(Utils::sector_to_unit(data.sectors_used, data.sector_size, SIZE_UNIT::UNIT_GIB), 'f', 2) + "GB";
    m_infodata.fstype = Utils::FSTypeToString((FSType)data.fstype);
    m_infodata.partitionsize = QString::number(Utils::sector_to_unit(data.sector_end - data.sector_start, data.sector_size, SIZE_UNIT::UNIT_GIB), 'f', 2) + "GB";
    m_infodata.syslabel = data.filesystem_label;
    update();
}

void DmFrameWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    QWidget::paintEvent(event);
    painter.save();
    painter.setRenderHint(QPainter::Antialiasing);  // 反锯齿;
    QRect rect = this->rect();
    DGuiApplicationHelper::ColorType themeType = DGuiApplicationHelper::instance()->themeType();
    if (themeType == DGuiApplicationHelper::LightType) {
        m_parentPb = DApplicationHelper::instance()->palette(this);
        QBrush brush = DApplicationHelper::instance()->palette(this).itemBackground();
        painter.setBrush(brush);
        QColor outsidecolor(qRgba(0, 0, 0, 0.1));
        outsidecolor.setAlphaF(0.1);
        painter.setPen(outsidecolor);
        QRect rect = this->rect();
        rect.setWidth(rect.width() - 1);
        rect.setHeight(rect.height() - 1);
        painter.drawRoundedRect(rect, 15, 15);
        QRect paintRect = QRect(rect.topLeft().x() + 1, rect.topLeft().y() + (rect.height() / 3), rect.width() - 2, rect.height() / 3);
        m_parentPb = DApplicationHelper::instance()->palette(this);
        QBrush brush1 = DApplicationHelper::instance()->palette(this).textLively();
        painter.setBrush(brush1);
        painter.fillRect(paintRect, brush1);
        painter.drawLine(paintRect.width() / 2, rect.topLeft().y(), paintRect.width() / 2, rect.bottomLeft().y());
        QRect textRect = QRect(rect.width() / 2 - 80, rect.topLeft().y() + 10, 80, 40);
        QFont font = DFontSizeManager::instance()->get(DFontSizeManager::T8);
        QColor textcolor = this->palette().color(DPalette::Normal, DPalette::Text);
        QTextOption option;
        painter.setFont(font);
        painter.setPen(textcolor);
        painter.drawText(textRect, m_infodata.mountpoints, option);
        textRect.moveTo(rect.width() / 2 - 80, rect.topLeft().y() + 60);
        painter.drawText(textRect, m_infodata.unused, option);
        textRect.moveTo(rect.width() / 2 - 80, rect.topLeft().y() + 110);
        painter.drawText(textRect, m_infodata.used, option);
        textRect.moveTo(rect.width() - 80, rect.topLeft().y() + 10);
        painter.drawText(textRect, m_infodata.fstype);
        textRect.moveTo(rect.width() - 80, rect.topLeft().y() + 60);
        painter.drawText(textRect, m_infodata.partitionsize);
        textRect.moveTo(rect.width() - 80, rect.topLeft().y() + 110);
        painter.drawText(textRect, m_infodata.syslabel);
        painter.restore();
    } else if (themeType == DGuiApplicationHelper::DarkType) {
        m_parentPb = DApplicationHelper::instance()->palette(this);
        QColor color = m_parentPb.color(DPalette::Normal, DPalette::TextLively);
        color.setAlphaF(0.05);
        painter.setBrush(QBrush(color));
        QColor outsidecolor(qRgba(0, 0, 0, 0.1));
        outsidecolor.setAlphaF(0.1);
        painter.setPen(outsidecolor);
        QRect rect = this->rect();
        rect.setWidth(rect.width() - 1);
        rect.setHeight(rect.height() - 1);
        painter.drawRoundedRect(rect, 15, 15);
        QRect paintRect = QRect(rect.topLeft().x() + 1, rect.topLeft().y() + (rect.height() / 3), rect.width() - 2, rect.height() / 3);
        QColor midcolor = QColor("#252525");
        painter.setBrush(QBrush(midcolor));
        painter.fillRect(paintRect, midcolor);
        painter.drawLine(paintRect.width() / 2, rect.topLeft().y(), paintRect.width() / 2, rect.bottomLeft().y());
        QRect textRect = QRect(rect.width() / 2 - 80, rect.topLeft().y() + 10, 80, 40);
        QFont font = DFontSizeManager::instance()->get(DFontSizeManager::T8);
        QColor textcolor = this->palette().color(DPalette::Normal, DPalette::WindowText);
        QTextOption option;
        painter.setFont(font);
        painter.setPen(textcolor);
        painter.drawText(textRect, m_infodata.mountpoints, option);
        textRect.moveTo(rect.width() / 2 - 80, rect.topLeft().y() + 60);
        painter.drawText(textRect, m_infodata.unused, option);
        textRect.moveTo(rect.width() / 2 - 80, rect.topLeft().y() + 110);
        painter.drawText(textRect, m_infodata.used, option);
        textRect.moveTo(rect.width() - 80, rect.topLeft().y() + 10);
        painter.drawText(textRect, m_infodata.fstype);
        textRect.moveTo(rect.width() - 80, rect.topLeft().y() + 60);
        painter.drawText(textRect, m_infodata.partitionsize);
        textRect.moveTo(rect.width() - 80, rect.topLeft().y() + 110);
        painter.drawText(textRect, m_infodata.syslabel);
        painter.restore();
    }


}
void DmFrameWidget::slothandleChangeTheme()
{

    m_parentPb = Dtk::Gui::DGuiApplicationHelper::instance()->applicationPalette();

}