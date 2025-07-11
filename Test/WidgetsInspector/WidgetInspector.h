#pragma once
#include <QWidget>
#include <QPointer>

class WidgetInspectorUi;

class WidgetInspector : public QWidget {
    Q_OBJECT
    explicit WidgetInspector(QWidget* parent = nullptr);
public:
    enum class ColorNameFormat {
        HexRgb,
        Rgb
    }; Q_ENUM(ColorNameFormat)

    class WidgetIndicator;
    class ScrollArea;
    
    ~WidgetInspector();
    static void attach(QWidget *widget);
    static QColor getColorFromWidget(QWidget *widget, const QPoint &globalPos);

private slots:
    void OnTick();
    void PickWidget();
    void RefreshWidgetProperties();
    void AppendMessage(const QString &message);
    void ApplyQss();
    void CopyAppStylesheets();

protected:
    bool eventFilter(QObject *watched, QEvent *event) override;
    void changeEvent(QEvent* event) override;

private:
    void SetCurrentWidget(QWidget *widget);

private:
    QScopedPointer<WidgetInspectorUi> ui;
    struct WidgetInspectorPrivate;
    QScopedPointer<WidgetInspectorPrivate> p;
    friend class WidgetInspectorUi;
};

#include <QScrollArea>
class WidgetInspector::ScrollArea : public QScrollArea {
    Q_OBJECT
public:
    explicit ScrollArea(QWidget *widget = nullptr) : QScrollArea{widget} {}
public:
    QSize sizeHint() const override;
    QSize minimumSizeHint() const override;
};

#include <QCheckBox>
class WidgetInspector::WidgetIndicator : public QCheckBox {
    Q_OBJECT
    struct {
        QWidget *widget = nullptr;
    } p;
public:
    explicit WidgetIndicator(QWidget *parent = nullptr) : QCheckBox{parent} {
        setAutoExclusive(true);
    }
    void SetWidget(QWidget *widget) { 
        p.widget = widget; 
    }
    QWidget *Widget() const { 
        return p.widget; 
    }
};
