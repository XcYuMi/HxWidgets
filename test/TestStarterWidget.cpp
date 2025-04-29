#include "TestStarterWidget.hpp"
#include "ui_TestStarterWidget.h"

TestStarterWidget::TestStarterWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::TestStarterWidget)
{
    ui->setupUi(this);
}

TestStarterWidget::~TestStarterWidget()
{
    delete ui;
}
