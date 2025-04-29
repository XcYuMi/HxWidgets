#ifndef TESTSTARTERWIDGET_HPP
#define TESTSTARTERWIDGET_HPP

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui {
class TestStarterWidget;
}
QT_END_NAMESPACE

class TestStarterWidget : public QWidget
{
    Q_OBJECT

public:
    TestStarterWidget(QWidget *parent = nullptr);
    ~TestStarterWidget();

private:
    Ui::TestStarterWidget *ui;
};
#endif // TESTSTARTERWIDGET_HPP
