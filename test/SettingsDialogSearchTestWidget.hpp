#pragma once
#include "HxWidgetsGlobal.hpp"
#include "HxSettingsDialog.hpp"

class SettingsDialogSearchTestWidget : public QWidget
{
    Q_OBJECT
    HX_DECLARE_PRIVATE(SettingsDialogSearchTestWidget)
    HX_DECLARE_UI(SettingsDialogSearchTestWidget)
public:
    explicit SettingsDialogSearchTestWidget(QWidget *parent = nullptr);
    ~SettingsDialogSearchTestWidget();
};

class QQSettingsDialog : public HxSettingsDialog {
    Q_OBJECT
public:
    explicit QQSettingsDialog(QWidget *parent = nullptr) : HxSettingsDialog{parent} { initUi(); }
    ~QQSettingsDialog() { }

public:
    void initUi();
};

