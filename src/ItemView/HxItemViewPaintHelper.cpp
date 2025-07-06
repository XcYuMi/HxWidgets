#include "HxItemViewPaintHelper.h"
#include "HxWidgetsGlobalPrivate.hpp"

#include <QPointer>
#include <QApplication>
#include <QHoverEvent>
#include <QScrollBar>
#include <QPainterPath>
#include <QPersistentModelIndex>
#include <QPainter>
#include <QTableView>
#include <QtMath>

class HxItemViewPaintHelper::HxItemViewPaintHelperPrivate
{
public:
    QMap<QPointer<QAbstractItemView>, QPersistentModelIndex> viewHoverIndexMap;
    QHash<QAbstractItemView *, PaintOptions> viewPaintOptionHash;
};

HxItemViewPaintHelper::~HxItemViewPaintHelper() { }

HxItemViewPaintHelper::HxItemViewPaintHelper(QObject *parent) : QObject{parent} 
{
    d.reset(new HxItemViewPaintHelperPrivate);
}

HxItemViewPaintHelper *HxItemViewPaintHelper::instance() 
{
    static QPointer<HxItemViewPaintHelper> singleton;
    if(singleton.isNull()) 
    {
        singleton = new HxItemViewPaintHelper(QApplication::instance());
    }
    return singleton;
}

bool HxItemViewPaintHelper::viewportEvent(QAbstractItemView *view, QEvent *event) 
{
    const auto event_type = event->type();
    switch (event_type) 
    {
    case QEvent::HoverMove:
    case QEvent::HoverEnter:
        setViewHoverIndex(view, view->indexAt(static_cast<QHoverEvent*>(event)->pos()));
        break;
    case QEvent::HoverLeave:
        setViewHoverIndex(view, QModelIndex());
        break;
    case QEvent::Leave:
        setViewHoverIndex(view, QModelIndex()); // If we've left, no hover should be needed anymore
        break;
    default:
        break;
    }
    return false;
}

void HxItemViewPaintHelper::setViewHoverIndex(QAbstractItemView *view, const QModelIndex &index) {
    if(!d->viewHoverIndexMap.contains(view))
    {
        connect(view->verticalScrollBar(), &QScrollBar::valueChanged, this, [=](int value) {
            onViewVerticalScrollBarValueChanged(view, value);
        });
        connect(view->horizontalScrollBar(), &QScrollBar::valueChanged, this, [=](int value) {
            onViewHorizontalScrollBarValueChanged(view, value);
        });
    }
    auto previous = d->viewHoverIndexMap.value(view, QModelIndex());
    if(previous == index)
        return;
    auto &current = d->viewHoverIndexMap[view];
    current = QPersistentModelIndex(index);

    int extend = 1;
    QPainterPath path;

    const auto selection_behavior = view->selectionBehavior();
    if (selection_behavior != QAbstractItemView::SelectRows) 
    {
        view->update(previous); //update the old one
        view->update(current); //update the new one
    }
    else 
    {
        const auto viewport = view->viewport();
        if (previous.isValid())
            path.addRect(view->visualRect(previous));
        else
            qDebug();
        if(current.isValid())
            path.addRect(view->visualRect(current));
        const auto &rct = path.boundingRect().adjusted(-extend, -extend, extend, extend).toRect();
        viewport->update(rct);
    }
}

void HxItemViewPaintHelper::mouseMoveEvent(QAbstractItemView *view, State state, QMouseEvent *event)
{
    QPoint bottomRight = event->pos();
    if (state == State::ExpandingState || state == State::CollapsingState)
        return;
    if (state == State::DraggingState)
        return;
    QPersistentModelIndex index = view->indexAt(bottomRight);
    if (state == State::EditingState)
        return;
    setViewHoverIndex(view, index);
}

void HxItemViewPaintHelper::paintEvent(QAbstractItemView *view, QPainter *painter, QPaintEvent *event)
{
    Q_UNUSED(event);
    const auto selectionModel = view->selectionModel();
    if (selectionModel == nullptr)
        return;

    int radius = 4;
    const auto &selectedIndexes = selectionModel->selectedIndexes();

    painter->save();
    painter->setRenderHint(QPainter::Antialiasing);

    // 绘制选中背景
    if (!selectedIndexes.isEmpty())
    {
        bool showGrid = false;
        if (view->metaObject()->inherits(&QTableView::staticMetaObject))
        {
            const auto table = qobject_cast<QTableView *>(view);
            showGrid = table->showGrid();
        }

        // 获取选中Item矩形列表
        QList<QRect> rects;
        for (const auto &index : selectedIndexes)
        {
            auto rect = view->visualRect(index);
            if (showGrid)
                rect.adjust(-1, -1, 1, 1);
            if (rect.isValid())
                rects.append(rect);
        }

        const auto &path = createRoundedPathFromRects(rects, radius + 3 * showGrid);
        if (!path.isEmpty())
            painter->fillPath(path, QColor("#1990FF"));
    }

    // 绘制HoverRect
    const auto &pos = view->viewport()->mapFromGlobal(QCursor::pos());
    const auto &hoverIndex = view->indexAt(pos);
    if (!selectedIndexes.contains(hoverIndex))
    {
        QPainterPath path;
        const auto rect = view->visualRect(hoverIndex);
        path.addRoundedRect(rect, radius, radius);
        painter->fillPath(path, QColor("#616161"));
    }

    painter->restore();
}

QModelIndex HxItemViewPaintHelper::hoverIndex(QAbstractItemView *view) const
{
    return d->viewHoverIndexMap.value(view, QModelIndex());
}

void HxItemViewPaintHelper::addView(QAbstractItemView *view, const PaintOptions &options)
{
    d->viewPaintOptionHash.insert(view, options);
    connect(view, &QObject::destroyed, this, [=] {
        d->viewPaintOptionHash.remove(view);
    });

    if (isPaintItemBackgroundEnabled(view))
    {
        // 保证View有SelectionModel
        auto selectionModel = view->selectionModel();
        if (selectionModel == nullptr) {
            selectionModel = new QItemSelectionModel(view->model(), view);
            view->setSelectionModel(selectionModel);
        }
        
        connect(selectionModel, &QItemSelectionModel::selectionChanged, this, [=](const QItemSelection &selected, const QItemSelection &deselected) {
            onViewSelectionChanged(view, selected, deselected);
        });
    }
}

bool HxItemViewPaintHelper::isPaintItemBackgroundEnabled(QAbstractItemView *view) const
{
    if (!d->viewPaintOptionHash.contains(view))
        return false;
    else
    {
        const auto &options = d->viewPaintOptionHash.value(view);
        return options.DelegateBackgroundPaint;
    }
}

bool HxItemViewPaintHelper::isItemHoverPaintOptimizationEnabled(QAbstractItemView *view) const
{
    if (!d->viewPaintOptionHash.contains(view))
        return false;
    else
    {
        const auto &options = d->viewPaintOptionHash.value(view);
        return options.OptimizeHoverPaint;
    }
}

void HxItemViewPaintHelper::onViewVerticalScrollBarValueChanged(QAbstractItemView *view, int value)
{
    Q_UNUSED(value);
    const auto &posInVp = view->viewport()->mapFromGlobal(QCursor::pos());
    if (view->viewport()->rect().contains(posInVp)) 
    {
        const auto &index = view->indexAt(posInVp);
        setViewHoverIndex(view, index);
    }
}

void HxItemViewPaintHelper::onViewHorizontalScrollBarValueChanged(QAbstractItemView *view, int value) 
{
    Q_UNUSED(value);
    const auto &posInVp = view->viewport()->mapFromGlobal(QCursor::pos());
    if (view->viewport()->rect().contains(posInVp)) 
    {
        const auto &index = view->indexAt(posInVp);
        setViewHoverIndex(view, index);
    }
}

void HxItemViewPaintHelper::updateIndexes(QAbstractItemView *view, const QModelIndexList &indexes)
{
    const int index_count = indexes.count(); Q_UNUSED(index_count);
    const auto selection_behavior = view->selectionBehavior();
    if (selection_behavior != QAbstractItemView::SelectRows)
    {
        for(const auto &index : indexes)
            view->update(index);
    }
    else
    {
        int extend = 3;
        QPainterPath path;
        const auto viewport = view->viewport();
        for (const auto &index : indexes)
        {
            if (index.isValid())
                path.addRect(view->visualRect(index));
        }
        
        const auto &rct = path.boundingRect().adjusted(-extend, -extend, extend, extend).toRect();
        viewport->update(rct);
    }
}

void HxItemViewPaintHelper::onViewSelectionChanged(QAbstractItemView *view, const QItemSelection &selected, const QItemSelection &deselected)
{
    const auto &selectedIndexes = selected.indexes();
    const auto &deselectedIndexes = deselected.indexes();

    QModelIndexList indexes;
    indexes << selectedIndexes;
    indexes << deselectedIndexes;
    updateIndexes(view, indexes);
}

QPainterPath HxItemViewPaintHelper::createRoundedPathFromRects(const QList<QRect> &rects, int radius)
{
    if (rects.isEmpty()) 
        return QPainterPath();

    // 计算所有矩形的并集
    QRegion region;
    for (const QRect &rect : rects) {
        region = region.united(rect);
    }

    // 将区域转换为路径
    QPainterPath regionPath;
    regionPath.addRegion(region);

    // 遍历区域的所有轮廓
    const auto &polygons = unitedPolygonsFromRects(rects);
    const int polygon_count = polygons.count(); Q_UNUSED(polygon_count);

    // 创建最终路径
    QPainterPath roundedPath;

    // 对每个轮廓进行圆角化处理
    for (const QPolygon &polygon : polygons) {
        QPainterPath contourPath = roundedPolygon(polygon, radius);
        roundedPath.addPath(contourPath);
    }

    // 确保所有矩形都包含在路径中
    if (roundedPath.isEmpty()) {
        for (const QRect &rect : rects) {
            roundedPath.addRoundedRect(rect, radius, radius);
        }
    }

    return roundedPath;
}

QList<QPolygon> HxItemViewPaintHelper::unitedPolygonsFromRects(const QList<QRect> &rects)
{
    QRegion region;
    for (const auto &rect : rects)
        region = region.united(rect);

    QPainterPath path;
    path.addRegion(region);

    const auto &polygonfs = path.toSubpathPolygons();

    QList<QPolygon> polygons;
    for (const auto &polygonf : polygonfs)
        polygons.append(polygonf.toPolygon());

    return polygons;
}

QPainterPath HxItemViewPaintHelper::roundedPolygon(const QPolygon &polygon, int radius, bool closed)
{
    Q_UNUSED(closed);
    enum class PolygonCloseMode {
        None,    // 不封闭，仅描边（角部圆角过渡）
        Angle,   // 封闭，首尾角不圆角，中间角圆角
        Radius   // 封闭，所有角（含首尾）圆角
    };

    struct JointData {
        QPointF center;
        QPointF moveInPoint;
        qreal startAngle;
        qreal sweepAngle;
        QPointF moveOutPoint;
    };

    // 修正角度计算（保持 Qt 的坐标系一致性）
    const auto &CalculateAngle = [=](const QVector2D &vec) -> qreal {
        return qRadiansToDegrees(-qAtan2(vec.y(), vec.x())); // 不再取负
    };

    // 修正角度跨度计算（处理跨 360° 的情况）
    const auto &CalculateSweepAngle = [=](qreal startAngle, qreal endAngle) -> qreal {
        qreal span = endAngle - startAngle;
        if (span > 180) span -= 360;
        if (span < -180) span += 360;
        return span;
        };

    // 计算角平分线上点的坐标
    const auto &CalculatePointP = [=](const QVector2D &A, const QVector2D &O, const QVector2D &B, qreal R) -> QVector2D {
        QVector2D OA = A - O;
        QVector2D OB = B - O;

        // 单位化向量
        qreal O_A = OA.length();
        qreal O_B = OB.length();

        // 计算角平分线的方向向量
        QVector2D OD = (OA * O_B + OB * O_A) / (O_A + O_B);
        qreal O_D = OD.length();

        // 计算P点坐标
        qreal COS = QVector2D::dotProduct(OA, OD) / (O_A * O_D);
        qreal SIN = qSqrt(1 - COS * COS);
        qreal L = R / SIN;
        QVector2D P = O + OD.normalized() * L;
        return P;
    };

    const auto &GetArcParam = [=](const QVector2D &A, const QVector2D &B, const QVector2D &C, qreal r) -> JointData {
        QVector2D P = CalculatePointP(A, B, C, r);
        QVector2D O = B;
        QVector2D OA = A - O;
        QVector2D OC = C - O;
        QVector2D OP = P - O;

        // 计算切点 Q 和 R
        QVector2D Q = O + OA * QVector2D::dotProduct(OA, OP) / OA.lengthSquared();
        QVector2D R = O + OC * QVector2D::dotProduct(OC, OP) / OC.lengthSquared();

        // 计算角度（修正后的逻辑）
        qreal angle_PQ = CalculateAngle(Q - P);
        qreal angle_PR = CalculateAngle(R - P);
        qreal sweepAngle = CalculateSweepAngle(angle_PQ, angle_PR);

        JointData joint;
        joint.center = P.toPointF();
        joint.moveInPoint = Q.toPointF();
        joint.moveOutPoint = R.toPointF();
        joint.startAngle = angle_PQ;
        joint.sweepAngle = sweepAngle;
        return joint;
    };

    const bool polygonClosed = true;
    const auto closeMode = PolygonCloseMode::Radius;

    QPainterPath path;
    if (polygon.size() < 3 || radius == 0) {
        // 半径为零或点数不足时直接绘制多边形
        path.addPolygon(polygon);
        return path;
    }

    QPointF startPoint;
    qreal R = radius;

    // 根据模式构造点序列
    QVector<QPointF> points;
    for (int i = 0; i < polygon.size(); ++i) {
        points.append(polygon[i]);
    }

    // Radius模式需要首尾连接，额外添加前两个点
    if (closeMode == PolygonCloseMode::Radius) {
        if(!polygonClosed)
            points.append(polygon[0]);
        points.append(polygon[1]);
    }
    else { //把画笔点到第一个点
        startPoint = polygon.first();
        path.moveTo(startPoint);
    }

    // 遍历每个顶点计算圆角
    for (int i = 1; i < points.size() - 1; ++i) {
        QVector2D A(points[i - 1]), B(points[i]), C(points[i + 1]);
        JointData joint = GetArcParam(A, B, C, R);

        if (startPoint.isNull()) {
            startPoint = joint.moveInPoint;
            path.moveTo(startPoint);
        }
        else {
            path.lineTo(joint.moveInPoint);
        }

        // 绘制圆弧
        QRectF rect(0, 0, R * 2, R * 2);
        rect.moveCenter(joint.center);
        path.arcTo(rect, joint.startAngle, joint.sweepAngle);
    }

    if (closeMode != PolygonCloseMode::Radius) { //把线连到最后一点个点
        path.lineTo(polygon.last());
    }

    // 封闭路径（Angle和Radius模式）
    if (closeMode != PolygonCloseMode::None) {
        path.closeSubpath();
    }

    return path;
}

QPainterPath HxItemViewPaintHelper::roundedPolygonV2(const QPolygon &polygon, int radius, bool closed)
{
    Q_UNUSED(closed);
    QPainterPath path;
    path.addRoundedRect(polygon.boundingRect(), radius, radius);
    return path;
}
