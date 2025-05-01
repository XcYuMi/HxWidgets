#pragma once
#include "HxWidgetsGlobal.hpp"
#include <QObject>
#include <QWidget>
#include <QHash>
#include <QJsonObject>

HX_BEGIN_NAMESPACE

class SplitterHandler;

class HX_WIDGETS_EXPORT NestedSplitter : public QObject {
    Q_OBJECT
    HX_DECLARE_PRIVATE(NestedSplitter)
public:
    explicit NestedSplitter(QObject* parent = nullptr);
    virtual ~NestedSplitter();
    void setContainer(QWidget* container);
    void addWidget(QWidget* widget, Qt::Orientation orientation = Qt::Horizontal);
    void splitWidget(QWidget* existing, QWidget* newcomer, Qt::Orientation orientation);
    QJsonObject saveState() const;
    void restoreState(const QJsonObject& state);

signals:
    void layoutChanged();

protected:
    bool eventFilter(QObject* watched, QEvent* event) override;

private:
    //QWidget* m_container = nullptr;
    //QHash<QWidget*, QPair<QWidget*, Qt::Orientation>> m_relations;
    //<QPair<QWidget*, QWidget*>, SplitterHandler*> m_handlers;

    SplitterHandler* createHandler();
    void updateLayout();
    void calculateGeometry(QWidget* widget, const QRect& available);
    QWidget* findRootWidget() const;
    QRect calculateHandlerGeometry(QWidget* first, QWidget* second, Qt::Orientation orientation) const;
};


class SplitterHandler : public QWidget {
    Q_OBJECT
    HX_DECLARE_PRIVATE(SplitterHandler)
public:
    SplitterHandler(QWidget* parent);
    virtual ~SplitterHandler();
protected:
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void paintEvent(QPaintEvent *event) override;

signals:
    void geometryChanged();
};

HX_END_NAMESPACE
