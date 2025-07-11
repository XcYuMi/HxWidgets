#include "CustomizedWidgets.hpp"

#include <QDebug>
#define CONSOLE qDebug().noquote() << QString("[%1][%2]").arg(__FUNCTIONW__).arg(__LINE__)
#pragma execution_character_set("utf-8")

#include <QPainter>
#include <QStyleOptionButton>
#include <QPainterPath>
#include <QColorDialog>

QColor getHighContrastColor(const QColor &color, qreal f = 0.8) {
    Q_UNUSED(f);
    qreal r, g, b;
    color.getRgbF(&r, &g, &b);

    return QColor::fromRgbF(1.0 - r * f, 1.0 - g * f, 1.0 - b * f);
}

ColorButton::ColorButton(const QString &text, QWidget *parent) : QPushButton{text, parent} {
    connect(this, &ColorButton::clicked, this, &ColorButton::selectColor);
}

ColorButton::ColorButton(QWidget *parent)
    : QPushButton{parent}
{
    connect(this, &ColorButton::clicked, this, &ColorButton::selectColor);
}

void ColorButton::setColor(const QColor &color) {
    if (mColor == color) {
        return;
    }

    mColor = color;
    emit colorChanged(mColor);
    update();
}

void ColorButton::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event);
    QPainter painter(this);
    QStyleOptionButton option;
    option.initFrom(this);
    style()->drawControl(QStyle::CE_PushButtonBevel, &option, &painter, this);

    QPainterPath clipPath;
    clipPath.addRoundedRect(rect().adjusted(0,0,-0,-0), 2, 2);
    painter.save();
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setClipPath(clipPath);

    if(text().isEmpty()) {
        const auto rct = rect();
        painter.fillRect(rct, mColor);
    }

    if(text().isEmpty()) {
        const auto &name = mColor.name(QColor::HexArgb).toUpper();
        painter.setPen(getHighContrastColor(mColor));
        painter.drawText(rect(), Qt::AlignCenter, name);
    } else {
        painter.setPen(mColor);
        painter.drawText(rect(), Qt::AlignCenter, text());
    }
    painter.restore();
}

void ColorButton::selectColor() {
    const auto &new_color = QColorDialog::getColor(mColor, this, "Select Color", QColorDialog::ColorDialogOption::ShowAlphaChannel);
    setColor(new_color);
}
