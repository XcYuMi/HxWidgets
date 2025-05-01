#include "TestStarterWidget.hpp"
#include "ui_TestStarterWidget.h"

#include "FlowLayoutTestWidget.hpp"

TestStarterWidget::TestStarterWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::TestStarterWidget)
{
    ui->setupUi(this);
    connect(ui->buttonFlowLayout, &QPushButton::clicked, this, &TestStarterWidget::onButtonFlowLayoutClicked);
}

TestStarterWidget::~TestStarterWidget()
{
    delete ui;
}

void TestStarterWidget::onButtonFlowLayoutClicked() {
    const auto widget = new FlowLayoutTestWidget(this);
    widget->setWindowFlag(Qt::Window);
    widget->setAttribute(Qt::WA_DeleteOnClose);
    widget->show();
}
