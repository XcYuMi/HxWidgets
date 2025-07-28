#pragma once
#include "HxWidgetsGlobal.hpp"
#include <QDialog>

class HX_WIDGETS_EXPORT HxDialog : public QDialog {
    Q_OBJECT
    using Super = QDialog;
    HX_DECLARE_PRIVATE(HxDialog)
public:
    explicit HxDialog(QWidget *parent = nullptr);
    virtual ~HxDialog();
};
