﻿#include "HxSettingsDialog.hpp"
#include "HxWidgetsGlobalPrivate.hpp"

#include <QVBoxLayout>
#include <QFormLayout>
#include <QActionEvent>
#include <QAction>
#include <QScrollArea>
#include <QToolButton>
#include <QButtonGroup>
#include <QStackedWidget>
#include <QLabel>
#include <QActionGroup>
#include <QTextEdit>

QString widgetHierarchyText(QWidget *widget) {
    QWidgetList widgets;

    QWidget *parent = widget;
    while(parent != nullptr) {
        widgets.append(parent);

        parent = parent->parentWidget();
    }

    std::reverse(widgets.begin(), widgets.end());

    QStringList ancesstorInfos;
    for(const auto widgetPtr : widgets) {
        const auto &className = widgetPtr->metaObject()->className();
        const auto &objectName = widgetPtr->objectName();
        if(objectName.isEmpty()) {
            ancesstorInfos << QString("%1").arg(className);
        } else {
            ancesstorInfos << QString("%1:%2").arg(className).arg(objectName);
        }
    }
    const auto &text = ancesstorInfos.join("->");
    return text;
}

class HxSettingsDialog::HxSettingsDialogPrivate {
    HxSettingsDialog *q = nullptr;
public:
    void init(HxSettingsDialog *dialog) { q = dialog; }
public:
    QList<HxSettingsPage*> pages;
    QActionGroup* actionGroup = nullptr;
};

class HxSettingsDialog::HxSettingsDialogUi {
    HxSettingsDialog *_this = nullptr;
public:
    void setupUi(HxSettingsDialog *dialog);
    void retranslateUi() {}
public:
    HxSettingsNavigationBar *navigationBar = nullptr;
    QStackedWidget *pageStackedWidget = nullptr;
};

HxSettingsDialog::~HxSettingsDialog() {
}

HxSettingsDialog::HxSettingsDialog(QWidget *parent) : Super{parent} {
    d.reset(new HxSettingsDialogPrivate);
    d->init(this);
    ui.reset(new HxSettingsDialogUi);
    ui->setupUi(this);
    ui->retranslateUi();

    d->actionGroup = new QActionGroup(this);
    d->actionGroup->setExclusive(true);
}

void HxSettingsDialog::HxSettingsDialogUi::setupUi(HxSettingsDialog *dialog)
{
    _this = dialog;

    const auto layout = new QHBoxLayout(_this);
    layout->setContentsMargins(0,0,0,0);
    layout->setSpacing(0);

    navigationBar = new HxSettingsNavigationBar(_this);
    layout->addWidget(navigationBar);
    navigationBar->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);

    pageStackedWidget = new QStackedWidget(_this);
    layout->addWidget(pageStackedWidget, 1);
    pageStackedWidget->setObjectName("PageStackedWidget");
    pageStackedWidget->setFrameShape(QFrame::NoFrame);
}

HxSettingsPage *HxSettingsDialog::addPage(const QString &text) {
    const auto page = new HxSettingsPage(this);
    d->pages.append(page);
    page->setWindowTitle(text);

    const auto pageAction = page->toggleAction();
    ui->navigationBar->addAction(pageAction);
    ui->pageStackedWidget->addWidget(page);
    d->actionGroup->addAction(pageAction);

    connect(pageAction, &QAction::toggled, this, [=](bool checked) {
        if(checked)
            ui->pageStackedWidget->setCurrentWidget(page);
    });

    return page;
}

void HxSettingsDialog::locateWidget(QWidget *widget) {

}

class HxSettingsNavigationBar::HxSettingsNavigationBarPrivate {
public:
    int spacing = 0;
    QHash<QAction*, QToolButton*> actionButtonHash;
};

class HxSettingsNavigationBar::HxSettingsNavigationBarUi {
    HxSettingsNavigationBar *_this = nullptr;
public:
    void setupUi(HxSettingsNavigationBar *bar);
public:
    QScrollArea* contentsScrollArea = nullptr;
    HxSettingBoxLayoutFrame *contentsFrame = nullptr;
};

HxSettingsNavigationBar::~HxSettingsNavigationBar() {

}

HxSettingsNavigationBar::HxSettingsNavigationBar(QWidget *parent) : Super{parent} {
    d.reset(new HxSettingsNavigationBarPrivate);
    ui.reset(new HxSettingsNavigationBarUi);
    ui->setupUi(this);
}

void HxSettingsNavigationBar::HxSettingsNavigationBarUi::setupUi(HxSettingsNavigationBar *bar)
{
    _this = bar;
    _this->setFrameShape(QFrame::NoFrame);

    const auto layout = new QVBoxLayout(_this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);

    contentsScrollArea = new QScrollArea(_this);
    layout->addWidget(contentsScrollArea);
    contentsScrollArea->setWidgetResizable(true);
    contentsScrollArea->setFrameShape(QFrame::NoFrame);

    contentsFrame = new HxSettingBoxLayoutFrame(QBoxLayout::TopToBottom, _this);
    contentsScrollArea->setWidget(contentsFrame);
    contentsFrame->setObjectName("ContentsFrame");
    contentsFrame->layout()->setAlignment(Qt::AlignLeft | Qt::AlignTop);
}

void HxSettingsNavigationBar::actionEvent(QActionEvent *event) {
    Super::actionEvent(event);
    const auto action = event->action();
    const auto eventType = event->type();

    const auto UpdateActionButton = [=](QAction *action) {
        const auto button = d->actionButtonHash.value(action);
        if(button == nullptr)
            return;
        const auto& text = action->text();
        // 阻塞信号
        button->blockSignals(true);
        // 内容
        button->setText(action->text());
        button->setIcon(action->icon());
        button->setToolTip(action->toolTip());
        button->setStatusTip(action->statusTip());
        button->setWhatsThis(action->whatsThis());
        // 状态
        button->setCheckable(true);
        button->setChecked(action->isChecked());
        button->setEnabled(action->isEnabled());
        // 打开信号
        button->blockSignals(false);
    };

    if(eventType == QEvent::ActionAdded) {
        const auto button = new QToolButton(this);
        ui->contentsFrame->layout()->addWidget(button);
        d->actionButtonHash.insert(action, button);
        button->setObjectName("PageButton");
        button->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
        UpdateActionButton(action);
        connect(button, &QToolButton::clicked, action, [=] {
            action->trigger();
        });
    } else if(eventType == QEvent::ActionRemoved) {
        d->actionButtonHash.remove(action);
        const auto button = d->actionButtonHash.value(action);
        if (button == nullptr)
            return;
        ui->contentsFrame->layout()->removeWidget(button);
        button->deleteLater();  
    } else if(eventType == QEvent::ActionChanged) {
        UpdateActionButton(action);
    }
}

class HxSettingsPage::HxSettingsPagePrivate {
    HxSettingsPage *q = nullptr;
public:
    void init(HxSettingsPage *qptr) { q = qptr; }
public:
    int spacing = 0;
    QAction *toggleAction = nullptr;
};

class HxSettingsPage::HxSettingsPageUi {
    HxSettingsPage *_this = nullptr;
public:
    void setupUi(HxSettingsPage *page);
    void retranslateUi() {}
public:
    QScrollArea *contentsScrollArea = nullptr;
    HxSettingBoxLayoutFrame *contentsFrame = nullptr;
};

HxSettingsPage::~HxSettingsPage() {
}

HxSettingsPage::HxSettingsPage(QWidget *parent) : Super{parent} {
    d.reset(new HxSettingsPagePrivate);
    d->init(this);

    ui.reset(new HxSettingsPageUi);
    ui->setupUi(this);
    ui->retranslateUi();

    auto &action = d->toggleAction;
    action = new QAction(this);
    action->setCheckable(true);
}

QAction *HxSettingsPage::toggleAction() const {
    return d->toggleAction;
}

HxSettingsGroup *HxSettingsPage::addGroup(const QString &title)
{
    const auto group = new HxSettingsGroup(this);
    group->setWindowTitle(title);
    ui->contentsFrame->layout()->addWidget(group);
    return group;
}

void HxSettingsPage::addWidget(QWidget *widget)
{
    if(const auto layout = widget->layout()) {
        layout->setContentsMargins(0,0,0,0);
    }
    ui->contentsFrame->layout()->addWidget(widget);
}

void HxSettingsPage::changeEvent(QEvent *ev)
{
    Super::changeEvent(ev);
    if(ev->type() == QEvent::WindowTitleChange) {
        const auto &title = windowTitle();
        d->toggleAction->setText(title);
    } else if(ev->type() == QEvent::EnabledChange) {
        d->toggleAction->setEnabled(isEnabled());
    }
}

void HxSettingsPage::HxSettingsPageUi::setupUi(HxSettingsPage *page) {
    _this = page;
    _this->setFrameShape(QFrame::NoFrame);

    const auto layout = new QVBoxLayout(_this);
    layout->setContentsMargins(0,0,0,0);
    layout->setSpacing(0);

    contentsScrollArea = new QScrollArea(_this);
    layout->addWidget(contentsScrollArea);
    contentsScrollArea->setWidgetResizable(true);
    contentsScrollArea->setFrameShape(QFrame::NoFrame);

    contentsFrame = new HxSettingBoxLayoutFrame(QBoxLayout::TopToBottom, _this);
    contentsScrollArea->setWidget(contentsFrame);
    contentsFrame->setObjectName("ContentsFrame");
    contentsFrame->layout()->setAlignment(Qt::AlignLeft | Qt::AlignTop);
}

class HxSettingsGroup::HxSettingsGroupPrivate {
public:
    int spacing = 0;
};

class HxSettingsGroup::HxSettingsGroupUi {
    HxSettingsGroup *_this = nullptr;
public:
    void setupUi(HxSettingsGroup *group);
    void retranslateUi() {}
public:
    QLabel *labelTitle = nullptr;
    QFrame *contentsFrame = nullptr;
    QFormLayout *contentsLayout = nullptr;
};

HxSettingsGroup::~HxSettingsGroup() {
}

void HxSettingsGroup::addRow(const QString &labelText, QWidget *widget)
{
    const auto label = new QLabel(this);
    label->setObjectName("SettiingsFiledLabel");
    label->setText(labelText);

    const auto fieldLayout = new QHBoxLayout;
    fieldLayout->setContentsMargins(0, 0, 0, 0);
    fieldLayout->setSpacing(0);
    fieldLayout->setAlignment(Qt::AlignVCenter | Qt::AlignRight);
    fieldLayout->addWidget(widget);

    ui->contentsLayout->addRow(label, fieldLayout);
}

void HxSettingsGroup::setSpacing(int spacing) {
    if(d->spacing == spacing)
        return ;
    d->spacing = spacing;
    ui->contentsLayout->setSpacing(spacing);
}

int HxSettingsGroup::spacing() const {
    return d->spacing;
}

void HxSettingsGroup::changeEvent(QEvent *ev)
{
    if(ev->type() == QEvent::WindowTitleChange) {
        const auto &title = windowTitle();
        ui->labelTitle->setText(title);
        ui->labelTitle->setVisible(!title.isEmpty());
    }
}

HxSettingsGroup::HxSettingsGroup(QWidget *parent) : Super{parent} {
    d.reset(new HxSettingsGroupPrivate);
    ui.reset(new HxSettingsGroupUi);
    ui->setupUi(this);
}

void HxSettingsGroup::HxSettingsGroupUi::setupUi(HxSettingsGroup *group)
{
    _this = group;
    _this->setFrameShape(QFrame::NoFrame);
    _this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    const auto layout = new QVBoxLayout(_this);
    layout->setContentsMargins(0,0,0,0);
    layout->setSpacing(0);

    labelTitle = new QLabel(_this);
    layout->addWidget(labelTitle);
    labelTitle->setObjectName("TitleLabel");

    contentsFrame = new QFrame(_this);
    layout->addWidget(contentsFrame, 1);
    contentsFrame->setObjectName("ContentsFrame");
    contentsFrame->setFrameShape(QFrame::NoFrame);

    contentsLayout = new QFormLayout(contentsFrame);
    contentsLayout->setContentsMargins(0, 0, 0, 0);
    contentsLayout->setHorizontalSpacing(6);
    contentsLayout->setVerticalSpacing(0);
}

HxSettingBoxLayoutFrame::~HxSettingBoxLayoutFrame() {

}

HxSettingBoxLayoutFrame::HxSettingBoxLayoutFrame(QBoxLayout::Direction direction, QWidget *parent) : Super{parent} {
    setFrameShape(QFrame::NoFrame);

    mLayout = new QBoxLayout(direction, this);
    mLayout->setContentsMargins(0,0,0,0);
    mLayout->setSpacing(mSpacing);
}

void HxSettingBoxLayoutFrame::setSpacing(int spacing) {
    if(mSpacing == spacing) {
        return;
    }
    mSpacing = spacing;
    mLayout->setSpacing(mSpacing);
}

int HxSettingBoxLayoutFrame::spacing() const {
    return mSpacing;
}
