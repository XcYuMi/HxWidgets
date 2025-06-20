﻿#include "TestStarterWidget.hpp"
#include "ui_TestStarterWidget.h"

#include "FlowLayoutTestWidget.hpp"
#include "AnchorLayoutTestWidget.hpp"
#include "NestedSplitterTestWidget.hpp"
#include "RangeSliderTestWidget.hpp"
#include "DoubleRangeSliderTestWidget.hpp"
#include "ToolBarTestWidget.hpp"

TestStarterWidget::~TestStarterWidget() {
    delete ui;
}

TestStarterWidget::TestStarterWidget(QWidget *parent) : QWidget(parent) , ui(new Ui::TestStarterWidget) {
    ui->setupUi(this);
    setWindowFlags(Qt::Widget | Qt::WindowCloseButtonHint);

    connect(ui->buttonFlowLayout, &QPushButton::clicked, this, &TestStarterWidget::onButtonFlowLayoutClicked);
    connect(ui->buttonAnchorLayout, &QPushButton::clicked, this, &TestStarterWidget::onActionTestAnchorLayout);
    connect(ui->buttonNestedSplitter, &QPushButton::clicked, this, &TestStarterWidget::onActionTestNestedSplitter);
    connect(ui->buttonRangeSlider, &QPushButton::clicked, this, &TestStarterWidget::onActionTestRangeSlider);
    connect(ui->buttonFloatRangeSlider, &QPushButton::clicked, this, &TestStarterWidget::onActionTestDoubleRangeSlider);
    connect(ui->buttonToolBar, &QPushButton::clicked, this, &TestStarterWidget::onActionTestToolBar);
}

void TestStarterWidget::onButtonFlowLayoutClicked() {
    const auto widget = new FlowLayoutTestWidget(this);
    widget->setWindowFlag(Qt::Window);
    widget->setAttribute(Qt::WA_DeleteOnClose);
    widget->show();
}

void TestStarterWidget::onActionTestAnchorLayout() {
    const auto widget = new AnchorLayoutTestWidget(this);
    widget->setWindowFlag(Qt::Window);
    widget->setAttribute(Qt::WA_DeleteOnClose);
    widget->show();
}

void TestStarterWidget::onActionTestNestedSplitter() {
    const auto widget = new NestedSplitterTestWidget(this);
    widget->setWindowFlag(Qt::Window);
    widget->setAttribute(Qt::WA_DeleteOnClose);
    widget->show();
}

void TestStarterWidget::onActionTestRangeSlider() {
    const auto widget = new RangeSliderTestWidget(this);
    widget->setWindowFlag(Qt::Window);
    widget->setAttribute(Qt::WA_DeleteOnClose);
    widget->show();
}

void TestStarterWidget::onActionTestDoubleRangeSlider() {
    const auto widget = new DoubleRangeSliderTestWidget(this);
    widget->setWindowFlag(Qt::Window);
    widget->setAttribute(Qt::WA_DeleteOnClose);
    widget->show();
}

void TestStarterWidget::onActionTestToolBar() {
    const auto widget = new ToolBarTestWidget(this);
    widget->setWindowFlag(Qt::Window);
    widget->setAttribute(Qt::WA_DeleteOnClose);
    widget->show();
}
