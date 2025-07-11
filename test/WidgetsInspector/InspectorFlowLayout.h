#pragma once
#include <QLayout>
#include <QPointer>
#include <QStyle>

class MyFlowLayout : public QLayout {
    Q_OBJECT
    struct {
        int horSpacing{0};
        int verSpacing{0};
        QList<QLayoutItem *> itemList;
    } p;
public:
    virtual ~MyFlowLayout() {}
    MyFlowLayout() = default;
    MyFlowLayout(QWidget *parent) : QLayout{parent} {}

public:
    int horizontalSpacing() const;
    void setHorizontalSpacing(int spacing);
    int verticalSpacing() const;
    void setVerticalSpacing(int spacing);

public:
    void addItem(QLayoutItem *item) override;
    Qt::Orientations expandingDirections() const override { return {}; }
    bool hasHeightForWidth() const override { return true; }
    int heightForWidth(int width) const override;
    int count() const override;
    QLayoutItem *itemAt(int index) const override;
    QSize minimumSize() const override;
    void setGeometry(const QRect &rect) override;
    QSize sizeHint() const override { return minimumSize(); }
    QLayoutItem *takeAt(int index) override;

private:
    int doLayout(const QRect &rect, bool testOnly) const;
    int smartSpacing(QStyle::PixelMetric pm) const;
};



