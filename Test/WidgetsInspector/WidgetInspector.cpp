#include "WidgetInspector.h"
#include "WidgetInspectorUi.h"

#include <Windows.h>
#include <WinUser.h>
#pragma comment(lib, "User32.lib")

#include "wingdi.h"
#pragma comment(lib, "Gdi32.lib")

#include <QApplication>
#include <QClipBoard>
#include <QTimer>
#include <QShortcut>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsObject>
#include <QGraphicsDropShadowEffect>

#include <QDebug>
#define DEBUG qDebug().noquote() << __FUNCTION__
#pragma execution_character_set("utf-8")

QString ColorName(const QColor& color, WidgetInspector::ColorNameFormat format = WidgetInspector::ColorNameFormat::HexRgb) {
    if (format == WidgetInspector::ColorNameFormat::HexRgb) {
        return color.name(QColor::HexRgb);
    } 
    
    return QString("%1, %2, %3").arg(color.red()).arg(color.green()).arg(color.blue());
}

struct WidgetInspector::WidgetInspectorPrivate {
    QPointer<QWidget> widget;
    QPointer<QWidget> displayWidget;
    QList<WidgetIndicator*> indicators;
    bool pickEnabled = false;
    QGraphicsItem* item{ nullptr };
};

QColor GetColorUnderCursor() {
    POINT cursorPos;
    ::GetCursorPos(&cursorPos);
    HDC hdc = ::GetDC(NULL);
    COLORREF colorRef = ::GetPixel(hdc, cursorPos.x, cursorPos.y);
    ::ReleaseDC(NULL, hdc);
    return QColor(GetRValue(colorRef), GetGValue(colorRef), GetBValue(colorRef));
}

struct {
    QWidgetList widgetHierarchy;
    QString objectName;
    QString className;
    QString inhertText;
    QPoint position;
    QSize size;
    QSize minSize;
    QSize maxSize;
    QMargins contentsMargins;
    QMargins layoutContentsMargins;
    QFont font;
    QString stylesheet;
    QColor color;
    QRect visibleRect;
    QString statusTip;
    bool enabled{ true };
    WId windowId;
} static gEditParam;

WidgetInspector::~WidgetInspector() { }

void WidgetInspector::attach(QWidget* widget) {
    static QPointer<WidgetInspector> singleton;
    if (!singleton) {
        singleton = new WidgetInspector;
    }
    singleton->setParent(widget);
}

WidgetInspector::WidgetInspector(QWidget *parent) : QWidget(parent) {
    ui.reset(new WidgetInspectorUi);
    ui->setupUi(this);

    p.reset(new WidgetInspectorPrivate);
    setWindowFlags(Qt::Window);

    const auto timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &WidgetInspector::OnTick);
    timer->start(1000/20);

    ui->comboboxColorFormat->addItem("HexRGB", QVariant::fromValue(ColorNameFormat::HexRgb));
    ui->comboboxColorFormat->addItem("RGB", QVariant::fromValue(ColorNameFormat::Rgb));
    ui->comboboxColorFormat->setCurrentText("HexRGB");

    qApp->installEventFilter(this);
}

void WidgetInspector::OnTick() {
    if (p->displayWidget && p->displayWidget->inherits("QGraphicsView")) {
        const auto graphicsView = qobject_cast<QGraphicsView*>(p->displayWidget);
        const auto& viewPos = graphicsView->mapFromGlobal(QCursor::pos());
        const auto item = graphicsView->itemAt(viewPos);

        if (p->item != item) {
            if (p->item) {
                p->item->setGraphicsEffect(nullptr);
            }
            p->item = item;
            if (p->item) {
                const auto effect = new QGraphicsDropShadowEffect;
                effect->setColor(Qt::red);
                effect->setBlurRadius(20);
                effect->setOffset(0, 0);
                p->item->setGraphicsEffect(effect);
            }
        }
        if (item != nullptr) {
            if (const auto object = item->toGraphicsObject()) {
                const auto& msg = QString("Mouse hovers graphics object: %1").arg(object->metaObject()->className());
                AppendMessage(msg);
            }
        }
    }

    if(p->pickEnabled) {
        const auto nouseOver = rect().contains(mapFromGlobal(QCursor::pos()));
        if(!nouseOver) {
            PickWidget();
        }
    }

    if(p->displayWidget) {
        RefreshWidgetProperties();
    }

    if (isVisible()) {
        const auto &color = GetColorUnderCursor();
        if (!ui->editUnderCursorColor->hasFocus() && gEditParam.color != color) {
            gEditParam.color = color;
            const auto &nameFormat = ui->comboboxColorFormat->currentData().value<ColorNameFormat>();
            const auto &colorText = ::ColorName(color, nameFormat);
            ui->editUnderCursorColor->setText(colorText.toUpper());
        }
    }  
}

void WidgetInspector::PickWidget() {
    const auto &cursorPos = QCursor::pos();
    const auto widget = QApplication::widgetAt(cursorPos);
    SetCurrentWidget(widget);
}

void WidgetInspector::SetCurrentWidget(QWidget *widget) {
    if(p->widget == widget) {
        return ;
    }
    p->widget = widget;
    p->displayWidget = widget;

    QWidgetList widgetHierarchy;
    auto parentWidget = widget;
    while(parentWidget != nullptr) {
        widgetHierarchy.append(parentWidget);
        parentWidget = parentWidget->parentWidget();
    }

    if(!ui->areaObjectHierarchy->hasFocus() && gEditParam.widgetHierarchy != widgetHierarchy) {
        gEditParam.widgetHierarchy = widgetHierarchy;

        qDeleteAll(p->indicators);
        p->indicators.clear();

        for(const auto widget : widgetHierarchy) {
            const auto &parentClassName = widget->metaObject()->className();
            const auto &parentObjectName = widget->objectName();
            const auto &text =  QString("%1:%2").arg(parentClassName).arg(parentObjectName);

            const auto indicator = new WidgetIndicator;
            indicator->setText(text);
            indicator->SetWidget(widget);
            indicator->setChecked(widget == p->displayWidget);
            p->indicators.append(indicator);
            ui->layoutObjectHierarchyContents->addWidget(indicator);
            connect(indicator, &WidgetIndicator::clicked, this, [=] {
                if(indicator->isChecked()) {
                    p->displayWidget = indicator->Widget();
                    RefreshWidgetProperties();
                }
            });
        }
    }
}

QString ToString(const QSize &size) {
    return QString("%1x%2").arg(size.width()).arg(size.height());
}

QString ToString(const QPoint &point) {
    return QString("(%1,%2)").arg(point.x()).arg(point.y());
}

QString ToString(const QRect &rect) {
    return QString("[(%1,%2),%3x%4]").arg(rect.left()).arg(rect.top()).arg(rect.width()).arg(rect.height());
}

QString ToString(const QMargins &margins) {
    return QString("[left: %1, top: %2, right: %3, bottom: %4").arg(margins.left()).arg(margins.top()).arg(margins.right()).arg(margins.bottom());
}

void WidgetInspector::RefreshWidgetProperties() {
    if (!p->displayWidget) 
        return;

    if(p->pickEnabled) 
        ui->inspectorPalette->refreshProperties();

    const auto &widget = *(p->displayWidget.data());

    const auto &objectName = widget.objectName();
    if(!ui->editObjectName->hasFocus() && gEditParam.objectName != objectName) {
        ui->editObjectName->setText(objectName);
        gEditParam.objectName = objectName;
    }

    const auto &className = widget.metaObject()->className();
    if(!ui->editClassName->hasFocus() && gEditParam.className != className) {
        ui->editClassName->setText(className);
        gEditParam.className = className;
    }

    QStringList inheritList;
    auto metaObject = widget.metaObject();
    while(metaObject != nullptr) {
        const auto &className = metaObject->className();
        inheritList.append(className);
        metaObject = metaObject->superClass();
    }
    const auto &inhertText = inheritList.join(" -> ");
    if(!ui->editInheritHierarchy->hasFocus() && gEditParam.inhertText != inhertText) {
        ui->editInheritHierarchy->setText(inhertText);
        ui->editInheritHierarchy->setCursorPosition(0);
        gEditParam.inhertText = inhertText;
    }

    const auto &position = widget.pos();
    if(!ui->editPosition->hasFocus() && gEditParam.position != position) {
        ui->editPosition->setText(::ToString(position));
        gEditParam.position = position;
    }

    const auto &size = widget.size();
    if(!ui->editSize->hasFocus() && gEditParam.size != size) {
        ui->editSize->setText(::ToString(size));
        gEditParam.size = size;
    }

    const auto &min_size = widget.minimumSize();
    if (!ui->editMinSize->hasFocus() && gEditParam.minSize != min_size) {
        ui->editMinSize->setText(::ToString(min_size));
        gEditParam.minSize = min_size;
    }

    const auto &max_size = widget.maximumSize();
    if (!ui->editMaxSize->hasFocus() && gEditParam.maxSize != max_size) {
        ui->editMaxSize->setText(::ToString(max_size));
        gEditParam.maxSize = max_size;
    }

    const auto &contents_margins = widget.contentsMargins();
    if (!ui->editContentsMargins->hasFocus() && gEditParam.contentsMargins != contents_margins) {
        ui->editContentsMargins->setText(::ToString(contents_margins));
        gEditParam.contentsMargins = contents_margins;
    }

    if (widget.layout() == nullptr)
    {
        gEditParam.layoutContentsMargins = QMargins();
        ui->editLayoutContentsMargins->setText("null");
    }
    else
    {
        const auto layout_contents_margins = widget.layout()->contentsMargins();
        if (!ui->editLayoutContentsMargins->hasFocus() && gEditParam.layoutContentsMargins != layout_contents_margins) {
            ui->editLayoutContentsMargins->setText(::ToString(layout_contents_margins));
            gEditParam.layoutContentsMargins = layout_contents_margins;
        }
    }
    
    const auto &font = widget.font();
    if(!ui->editFont->hasFocus() && gEditParam.font != font) {
        gEditParam.font = font;

        QString familyText;
        if(font.families().isEmpty()) {
            familyText = font.family();
        } else {
            familyText = font.families().join("|");
        }
        QString sizeText;
        if(font.pixelSize() > 0) {
            sizeText = QString("%1px").arg(font.pixelSize());
        } else {
            sizeText = QString("%1pt").arg(font.pointSize());
        }

        QMap<QFont::Weight,QString> weightNames = {
            {QFont::Thin, "Thin"},
            {QFont::ExtraLight, "ExtraLight"},
            {QFont::Light, "Highlight"},
            {QFont::Normal, "Execute"},
            {QFont::Medium, "Medium"},
            {QFont::DemiBold, "DemiBold"},
            {QFont::Bold, "Bold"},
            {QFont::ExtraBold, "ExtraBold"},
            {QFont::Black, "Black"}
        };

        QString weightText;

        const auto weightType = QFont::Weight(font.weight());
        if(weightNames.contains(weightType)) {
            weightText = QString("%1(%2)").arg(weightNames.value(weightType)).arg(font.weight());
        } else {
            weightText = QString::number(font.weight());
        }
        const auto &info = QString("%1, %2, %3")
                               .arg(familyText)
                               .arg(sizeText)
                               .arg(weightText);
        ui->editFont->setText(info);
    }

    const auto &stylesheet = widget.styleSheet();
    if(!ui->editStylesheet->hasFocus() && gEditParam.stylesheet != stylesheet) {
        ui->editStylesheet->setPlainText(stylesheet);
        gEditParam.stylesheet = stylesheet;
    }

    if (!widget.testAttribute(Qt::WA_NativeWindow))
        ui->editWindowId->clear();
    else {
        const auto &id = widget.winId();
        if (gEditParam.windowId != id) {
            gEditParam.windowId = id;
            auto hex = QString("%1").arg(qint64(id), 8, 16, QChar('0')).toUpper();
            hex.prepend("0x");
            ui->editWindowId->setText(hex);
        }
    }
    
    const auto &visibleRect = widget.visibleRegion().boundingRect();
    if(!ui->editVisibleRect->hasFocus() && gEditParam.visibleRect != visibleRect) {
        gEditParam.visibleRect = visibleRect;
        ui->editVisibleRect->setText(::ToString(visibleRect));
    }

    const auto &status_tip = widget.statusTip();
    if (!ui->editStatusTip->hasFocus() && gEditParam.statusTip != status_tip) {
        gEditParam.statusTip = status_tip;
        ui->editStatusTip->setText(status_tip);
    }

    const auto &enabled = widget.isEnabled();
    if (!ui->checkEnabled->hasFocus() && gEditParam.enabled != enabled) {
        gEditParam.enabled = enabled;
        ui->checkEnabled->setChecked(enabled);
    }   

    const auto &color = getColorFromWidget(p->displayWidget, QCursor::pos());
    ui->buttonColor->setColor(color);
}

void WidgetInspector::AppendMessage(const QString &message) {
    ui->editDebugMessage->append(message);
}

void WidgetInspector::ApplyQss() {
    const auto &oldQss = p->widget->styleSheet();
    const auto &newQss = ui->editStylesheet->toPlainText();
    if(newQss == oldQss) {
        return ;
    }

    qApp->style()->unpolish(p->widget);
    p->widget->setStyleSheet(newQss);
    qApp->style()->polish(p->widget);
}

void WidgetInspector::CopyAppStylesheets()
{
    const auto &qss = qApp->styleSheet();
    const auto clipboard = qApp->clipboard();
    clipboard->setText(qss);
}

bool WidgetInspector::eventFilter(QObject *watched, QEvent *event) {
    const bool result = QWidget::eventFilter(watched, event);

    if(event->type() == QEvent::WinIdChange) {      
        QWidget *widget = qobject_cast<QWidget*>(watched);
        if (nullptr != widget && widget->isWindow()) {
            const QString class_name = watched->metaObject()->className();
            const auto& object_name = watched->objectName();
            const auto& message = QString("%1:: 类名: %2, 对象名: %3, 地址: %4, ID: %5")
                                      .arg("窗口ID改变")
                                      .arg(class_name)
                                      .arg(object_name.isEmpty() ? "\"\"" : object_name)
                                      .arg(size_t(watched), 16, 16, QChar('0')).arg(window()->effectiveWinId());
            if(object_name == "maximize")
                AppendMessage(message);
        }     
    } 

    if (event->type() == QEvent::Show && watched->isWidgetType() && !watched->inherits("QMenu") && !watched->inherits("QRollEffect")) {
        QWidget* widget = qobject_cast<QWidget*>(watched);
        if (widget->isWindow()) {
            QStringList objectHierarchy;

            QWidget* pWidget = widget;
            while (pWidget) {
                objectHierarchy.append(widget->metaObject()->className());
                pWidget = pWidget->parentWidget();
            }

            const auto& message = QString("顶级窗口显示::类名： %1, 对象名: %2, 标题： %3").arg(objectHierarchy.join("->")).arg(widget->objectName()).arg(widget->windowTitle());
            //AppendMessage(message);
        }
    }


    if(watched == ui->editStylesheet && event->type() == QEvent::Resize) {
        const auto &buttonSize = ui->buttonApplyQss->size();
        const auto &editRect = ui->editStylesheet->rect();
        const auto &buttonGeometry = QStyle::alignedRect(Qt::LeftToRight,
                                                         Qt::AlignBottom | Qt::AlignRight,
                                                         buttonSize,
                                                         editRect.adjusted(0,0,-3,-3));
        ui->buttonApplyQss->setGeometry(buttonGeometry);
    }

    if (event->type() == QEvent::EnabledChange)
    {
        QWidget *widget = qobject_cast<QWidget*>(watched);
        if (widget != nullptr)
        {
            const auto &class_name = watched->metaObject()->className();
            const auto &object_name = watched->objectName();
            const auto &message = QString("部件可用改变:: 类名=%1, 对象名=%2, 可用=%3").arg(class_name).arg(object_name).arg(widget->isEnabled());
            if(object_name == "Property_Button_RenameTemplate")
                AppendMessage(message);
        }    
    }

    return result;
}

QColor WidgetInspector::getColorFromWidget(QWidget *widget, const QPoint &globalPos)
{
    if (!widget || !widget->isVisible())
        return QColor();

    // 将全局坐标转换为部件本地坐标
    QPoint localPos = widget->mapFromGlobal(globalPos);
    if (!widget->rect().contains(localPos))
        return QColor();

    // 定义捕获区域大小（8x8）
    const int captureSize = 8;
    const int halfSize = captureSize / 2;

    // 计算以鼠标为中心的捕获区域（部件坐标系）
    QRect sourceRect(
        localPos.x() - halfSize,
        localPos.y() - halfSize,
        captureSize,
        captureSize
    );

    // 确保区域在部件范围内
    sourceRect = sourceRect.intersected(widget->rect());
    if (sourceRect.isEmpty())
        return QColor();

    // 根据 DPI 缩放调整 Pixmap 大小
    qreal dpr = widget->devicePixelRatioF();
    QSize targetSize = sourceRect.size() * dpr;

    // 创建离屏 Pixmap（仅需捕获区域大小）
    QPixmap pixmap(targetSize);
    pixmap.setDevicePixelRatio(dpr);
    pixmap.fill(Qt::transparent);

    // 将部件的指定区域渲染到 Pixmap（排除子部件）
    QPainter painter(&pixmap);
    widget->render(
        &painter,
        QPoint(-sourceRect.x(), -sourceRect.y()),  // 偏移量：将部件坐标系映射到 Pixmap
        sourceRect,                                // 捕获区域
        QWidget::DrawWindowBackground | QWidget::IgnoreMask
    );
    painter.end();

    // 转换为 QImage 并读取中心点颜色
    QImage image = pixmap.toImage();
    QPoint centerPos = QPoint(
        (sourceRect.width() - 1) / 2,  // 中心点坐标（例如 8x8 的中心为 (3,3)）
        (sourceRect.height() - 1) / 2
    );

    // 调整高 DPI 缩放后的坐标
    QPoint adjustedPos = centerPos * dpr;

    if (adjustedPos.x() >= 0 && adjustedPos.x() < image.width() &&
        adjustedPos.y() >= 0 && adjustedPos.y() < image.height()) {
        return image.pixelColor(adjustedPos);
    }

    return QColor();
}

QSet<QWidget*> attachedWidgetSet;
void WidgetInspector::changeEvent(QEvent* event) {
    QWidget::changeEvent(event);
    if (event->type() == QEvent::ParentChange) {
        const auto parent_widget = parentWidget();
        if (parent_widget) {
            setWindowFlag(Qt::Window);
            if (!attachedWidgetSet.contains(parent_widget)) {
                attachedWidgetSet.insert(parent_widget);
                const auto shortCutShowOrHide = new QShortcut(parentWidget());
                shortCutShowOrHide->setKey(QKeySequence(Qt::ALT + Qt::Key_H));
                shortCutShowOrHide->setContext(Qt::ApplicationShortcut);
                connect(shortCutShowOrHide, &QShortcut::activated, this, [=] {
                    setVisible(!isVisible());
                });

                const auto shortCut = new QShortcut(parentWidget());
                shortCut->setKey(QKeySequence(Qt::ALT + Qt::Key_I));
                shortCut->setContext(Qt::ApplicationShortcut);
                connect(shortCut, &QShortcut::activated, this, [=] {
                    p->pickEnabled = !p->pickEnabled;
                });
            }           
        }     
    }
}

QSize WidgetInspector::ScrollArea::sizeHint() const {
    auto hint = QScrollArea::sizeHint();
    DEBUG << hint;
    if(widget()) {
        hint.rheight() = 28;
    } else {
        hint.rheight() = 28;
    }
    DEBUG << hint;
    return hint;
}

QSize WidgetInspector::ScrollArea::minimumSizeHint() const {
    auto hint = QScrollArea::minimumSizeHint();
    DEBUG << hint;
    if(widget()) {
        hint.rheight() = 28;
    } else {
        hint.rheight() = 28;
    }
    DEBUG << hint;
    return hint;
}
