#pragma once
#include "HxWidgetsGlobal.hpp"
#include <QTableView>
#include <QScopedPointer>

class HX_WIDGETS_EXPORT HxTableView : public QTableView
{
    Q_OBJECT
    HX_DECLARE_PRIVATE(HxTableView)

public:
    explicit HxTableView(QWidget *parent = nullptr);
    virtual ~HxTableView();
    void setSelectionOnCheckTogglerBlocked(bool blocked);
    bool isSelectionOnCheckTogglerBlocked() const;
    void setPaintItemBackgroundEnabled(bool enabled);
    bool isPaintItemBackgroundEnabled() const;

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void showEvent(QShowEvent *event) override;
    void paintEvent(QPaintEvent *event) override;
    void initViewItemStyleOption(QStyleOptionViewItem *option, const QModelIndex &index) const;
};
