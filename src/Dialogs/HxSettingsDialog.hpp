#pragma once
#include "HxWidgetsGlobal.hpp"
#include <QDialog>
#include <QFrame>

class HxSettingsPage;
class HxSettingsGroup;

class HX_WIDGETS_EXPORT HxSettingsDialog : public QDialog
{
    Q_OBJECT
    HX_DECLARE_PRIVATE(HxSettingsDialog)
    HX_DECLARE_UI(HxSettingsDialog)
    using Super = QDialog;
public:
    explicit HxSettingsDialog(QWidget *parent = nullptr);
    ~HxSettingsDialog();
    void locateWidget(QWidget *widget);
protected:
    HxSettingsPage *addPage(const QString &text);
};

class HX_WIDGETS_EXPORT HxSettingsPage : public QFrame
{
    Q_OBJECT
    HX_DECLARE_PRIVATE(HxSettingsPage)
    HX_DECLARE_UI(HxSettingsPage)
    using Super = QFrame;
    Q_PROPERTY(int spacing READ spacing WRITE setSpacing)
public:
    explicit HxSettingsPage(QWidget *parent = nullptr);
    ~HxSettingsPage();
    QAction *toggleAction() const;
    HxSettingsGroup* addGroup(const QString& title = "");
    void addWidget(QWidget *widget);
    void addLayout(QLayout *layout);
    void setSpacing(int spacing);
    int spacing() const;

protected:
    void changeEvent(QEvent *ev) override;
};

class HxSettingsNavigationBar : public QFrame
{
    Q_OBJECT
    HX_DECLARE_PRIVATE(HxSettingsNavigationBar)
    HX_DECLARE_UI(HxSettingsNavigationBar)
    using Super = QFrame;
    Q_PROPERTY(int spacing READ spacing WRITE setSpacing)
public:
    explicit HxSettingsNavigationBar(QWidget *parent = nullptr);
    ~HxSettingsNavigationBar();
    void setSpacing(int spacing);
    int spacing() const;
protected:
    void actionEvent(QActionEvent *event) override;
};

class HX_WIDGETS_EXPORT HxSettingsGroup : public QFrame
{
    Q_OBJECT
    HX_DECLARE_PRIVATE(HxSettingsGroup)
    HX_DECLARE_UI(HxSettingsGroup)
    using Super = QFrame;
    Q_PROPERTY(int spacing READ spacing WRITE setSpacing)
public:
    explicit HxSettingsGroup(QWidget *parent = nullptr);
    ~HxSettingsGroup();
    void addRow(const QString &labelText, QWidget *widget);
    void setSpacing(int spacing);
    int spacing() const;

protected:
    void changeEvent(QEvent *ev) override;
};
