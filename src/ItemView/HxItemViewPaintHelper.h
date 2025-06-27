#pragma once
#include "HxWidgetsGlobal.hpp"
#include <QObject>
#include <QScopedPointer>
#include <QAbstractItemView>
#include <QPainterPath>

class HX_WIDGETS_EXPORT HxItemViewPaintHelper : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY_MOVE(HxItemViewPaintHelper)
    HX_DECLARE_PRIVATE(HxItemViewPaintHelper)
public:
    enum State 
    {
        NoState,
        DraggingState,
        DragSelectingState,
        EditingState,
        ExpandingState,
        CollapsingState,
        AnimatingState
    };

    struct PaintOptions {
        bool OptimizeHoverPaint = false;
        bool DelegateBackgroundPaint = false;
    };
    ~HxItemViewPaintHelper();
    static HxItemViewPaintHelper *instance();
    bool viewportEvent(QAbstractItemView *view, QEvent * event);
    void mouseMoveEvent(QAbstractItemView *view, State state, QMouseEvent *event);
    void paintEvent(QAbstractItemView *view, QPainter *painter, QPaintEvent *event);
    QModelIndex hoverIndex(QAbstractItemView *view) const;
    void addView(QAbstractItemView *view, const PaintOptions &options);
    bool isPaintItemBackgroundEnabled(QAbstractItemView *view) const;
    bool isItemHoverPaintOptimizationEnabled(QAbstractItemView *view) const;

private:
    explicit HxItemViewPaintHelper(QObject *parent = nullptr);
    void setViewHoverIndex(QAbstractItemView *view, const QModelIndex &index);
    void onViewVerticalScrollBarValueChanged(QAbstractItemView *view, int value);
    void onViewHorizontalScrollBarValueChanged(QAbstractItemView *view, int value);
    void updateIndexes(QAbstractItemView *view, const QModelIndexList &indexes);
    void onViewSelectionChanged(QAbstractItemView *view, const QItemSelection &selected, const QItemSelection &deselected);

private:
    static QPainterPath createRoundedPathFromRects(const QList<QRect> &rects, int radius); // 圆角化正交多边形
    static QList<QPolygon> unitedPolygonsFromRects(const QList<QRect> &rects);
    static QPainterPath roundedPolygon(const QPolygon &polygon, int radius, bool closed = true);
    static QPainterPath roundedPolygonV2(const QPolygon &polygon, int radius, bool closed = true); 
};
