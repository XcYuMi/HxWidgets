#pragma once
#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui {
class TestStarterWidget;
}
QT_END_NAMESPACE

class TestStarterWidget : public QWidget
{
    Q_OBJECT
    Ui::TestStarterWidget *ui;
public:
    TestStarterWidget(QWidget *parent = nullptr);
    ~TestStarterWidget();

private:
    void onButtonFlowLayoutClicked();
    void onActionTestAnchorLayout();
    void onActionTestNestedSplitter();
    void onActionTestRangeSlider();
    void onActionTestDoubleRangeSlider();
    void onActionTestToolBar();
    void onActionTestBadge();
    void onActionSettingsDialogSearch();
    void onActionTestItemWidgets();
};
