#pragma once
#include "HxWidgetsGlobal.hpp"
#include <QDialog>
#include <QFrame>
#include <QBoxLayout>

class QBoxLayout;
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
public:
    explicit HxSettingsPage(QWidget *parent = nullptr);
    ~HxSettingsPage();
    QAction *toggleAction() const;
    HxSettingsGroup* addGroup(const QString& title = "");
    void addWidget(QWidget *widget);

protected:
    void changeEvent(QEvent *ev) override;
    void showEvent(QShowEvent *event) override;
};

class HxSettingsNavigationBar : public QFrame
{
    Q_OBJECT
    HX_DECLARE_PRIVATE(HxSettingsNavigationBar)
    HX_DECLARE_UI(HxSettingsNavigationBar)
    using Super = QFrame;
public:
    explicit HxSettingsNavigationBar(QWidget *parent = nullptr);
    ~HxSettingsNavigationBar();
protected:
    void actionEvent(QActionEvent *event) override;
    void showEvent(QShowEvent* event) override;
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

class HxSettingBoxLayoutFrame : public QFrame {
    Q_OBJECT
    using Super = QFrame;
    Q_PROPERTY(int spacing READ spacing WRITE setSpacing)
public:
    explicit HxSettingBoxLayoutFrame(QBoxLayout::Direction direction, QWidget *parent = nullptr);
    ~HxSettingBoxLayoutFrame();
    void setSpacing(int spacing);
    int spacing() const;

private:
    QBoxLayout *mLayout = nullptr;
    int mSpacing = 0;
};
