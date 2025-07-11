#pragma once

#include <QPushButton>

class ColorButton : public QPushButton {
    Q_OBJECT
public:
    ColorButton(const QString &text, QWidget *parent = nullptr);
    explicit ColorButton(QWidget *parent = nullptr);
    void setColor(const QColor &color);
    QColor color() const { return mColor; }
protected:
    void paintEvent(QPaintEvent *event) override;
signals:
    void colorChanged(const QColor &color);
private:
    void selectColor();
private:
    QColor mColor;
};