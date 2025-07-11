#include "SettingsDialogSearchTestWidget.hpp"
#include "WidgetsTestGlobalPrivate.hpp"

#include <QVBoxLayout>
#include "HxToolBar.hpp"
#include <QLineEdit>
#include <QCheckBox>
#include <QDialog>
#include <QButtonGroup>
#include <QComboBox>
#include <QKeySequenceEdit>

class SettingsDialogSearchTestWidget::SettingsDialogSearchTestWidgetPrivate {
public:

};

class SettingsDialogSearchTestWidget::SettingsDialogSearchTestWidgetUi {
    SettingsDialogSearchTestWidget *_this = nullptr;
public:
    void setupUi(SettingsDialogSearchTestWidget *widget);
    void retranslateUi();
public:
    QLineEdit *searchEdit = nullptr;
    HxToolBar *toolBar = nullptr;
    QHash<QCheckBox*, QWidget*> togglerWidgetHash;
    QCheckBox *togglerDialogQQ = nullptr;
    QQSettingsDialog *dialogQQ = nullptr;
};

SettingsDialogSearchTestWidget::~SettingsDialogSearchTestWidget() {
}

SettingsDialogSearchTestWidget::SettingsDialogSearchTestWidget(QWidget *parent) : QWidget{parent} {
    d.reset(new SettingsDialogSearchTestWidgetPrivate);
    ui.reset(new SettingsDialogSearchTestWidgetUi);
    ui->setupUi(this);
    ui->retranslateUi();
}

void SettingsDialogSearchTestWidget::SettingsDialogSearchTestWidgetUi::setupUi(SettingsDialogSearchTestWidget *widget) {
    _this = widget;

    const auto layout = new QVBoxLayout(_this);
    layout->setAlignment(Qt::AlignCenter);
    layout->setContentsMargins(6,6,6,6);
    layout->setSpacing(6);

    searchEdit = new QLineEdit(_this);
    layout->addWidget(searchEdit);

    toolBar = new HxToolBar(_this);
    layout->addWidget(toolBar);

    const auto buttonGroup = new QButtonGroup(_this);
    buttonGroup->setExclusive(true);

    togglerDialogQQ = new QCheckBox(toolBar);
    toolBar->addWidget(togglerDialogQQ);
    buttonGroup->addButton(togglerDialogQQ);

    connect(togglerDialogQQ, &QCheckBox::toggled, _this, [=](bool checked) {
        if(dialogQQ == nullptr) {
            dialogQQ = new QQSettingsDialog(_this);
            togglerWidgetHash.insert(togglerDialogQQ, dialogQQ);
        }
        dialogQQ->setVisible(checked);
    });
}

void SettingsDialogSearchTestWidget::SettingsDialogSearchTestWidgetUi::retranslateUi() {
    _this->setWindowTitle(tr("设置对话框搜索测试"));
    searchEdit->setPlaceholderText(tr("输入关键字进行搜索"));
    togglerDialogQQ->setText(tr("QQ设置对话框"));
}

void QQSettingsDialog::initUi() {
    if(const auto page = addPage("通用")) {
        if(const auto group = page->addGroup("聊天")) {
            if(const auto selector = new QComboBox) {
                group->addRow("发送消息", selector);
                selector->addItem("Enter");
                selector->addItem("Ctrl + Enter");
            }
            if(const auto toggler = new QCheckBox) {
                group->addRow("双击会话列表打开独立聊天窗口", toggler);
            }
            if(const auto toggler = new QCheckBox) {
                group->addRow("使用QQ文档打开本地Office文件", toggler);
            }
            if(const auto toggler = new QCheckBox) {
                group->addRow("使用QQ文档打开在线文档", toggler);
            }
        }
        if(const auto group = page->addGroup("主面板")) {
            if(const auto toggler = new QCheckBox) {
                group->addRow("停靠在桌面边缘时自动隐藏", toggler);
            }
            if(const auto toggler = new QCheckBox) {
                group->addRow("在任务栏通知区域显示QQ图标", toggler);
            }
        }
        if(const auto group = page->addGroup("系统设置")) {
            if(const auto toggler = new QCheckBox) {
                group->addRow(R"(在系统右键菜单增加"通过QQ发送到我的手机和好友"选项)", toggler);
            }
        }
        if(const auto group = page->addGroup("其他")) {
            if(const auto toggler = new QCheckBox) {
                group->addRow(R"(透明效果)", toggler);
            }
        }
    }

    if(const auto page = addPage("消息通知")) {
        if(const auto group = page->addGroup("系统通知提醒")) {
        }
        if(const auto group = page->addGroup("消息提醒设置")) {
        }
        if(const auto group = page->addGroup("提示音")) {
        }
        if(const auto group = page->addGroup("通知开关")) {
        }
        if(const auto group = page->addGroup("群消息设置")) {
        }
    }
    if(const auto page = addPage("存储管理")) {

    }
    if(const auto page = addPage("快捷键")) {
        if(const auto group = page->addGroup("")) {
            if(const auto edit = new QKeySequenceEdit) {
                group->addRow("打开未读消息", edit);
            }
            if(const auto edit = new QKeySequenceEdit) {
                group->addRow("截图", edit);
            }
            if(const auto edit = new QKeySequenceEdit) {
                group->addRow("录屏", edit);
            }
        }
    }
    if(const auto page = addPage("权限管理")) {

    }
    if(const auto page = addPage("登录设置")) {

    }
    if(const auto page = addPage("超级调色盘")) {

    }
}
