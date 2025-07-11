#pragma once
#include "WidgetInspector.h"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QFormLayout>
#include <QStackedLayout>

#include "InspectorFlowLayout.h"
#include "PaletteInspector.hpp"
#include "CustomizedWidgets.hpp"
#include <QApplication>
#include <QStyle>
#include <QSplitter>
#include <QScrollArea>

#include <QLabel>
#include <QLineEdit>
#include <QTextEdit>
#include <QComboBox>
#include <QPushButton>
#include <QToolButton>

#include "HxToolBar.hpp"

#pragma execution_character_set("utf-8")

class WidgetInspectorUi {
    WidgetInspector *mWidget{nullptr};
public:
    WidgetInspectorUi() = default;
    void setupUi(WidgetInspector *widget);
    void retranslateUi();
public:
    QLabel *labelObjectName{nullptr};
    QLineEdit *editObjectName{nullptr};
    QLabel *labelClassName{nullptr};
    QLineEdit *editClassName{nullptr};
    QLabel *labelInheritHierarchy{nullptr};
    QLineEdit *editInheritHierarchy{nullptr};
    QLabel *labelObjectHierarchy{nullptr};
    QScrollArea *areaObjectHierarchy{nullptr};
    MyFlowLayout *layoutObjectHierarchyContents{nullptr};
    QLabel *labelPosition{nullptr};
    QLineEdit *editPosition{nullptr};
    QLabel *labelSize{nullptr};
    QLineEdit *editSize{nullptr};
    QLabel *labelMinSize{ nullptr };
    QLineEdit *editMinSize{ nullptr };
    QLabel *labelMaxSize{ nullptr };
    QLineEdit *editMaxSize{ nullptr };
    QLabel *labelFont{nullptr};
    QLineEdit *editFont{nullptr};
    QLabel *labelStylsheet{nullptr};
    QTextEdit *editStylesheet{nullptr};
    QPushButton *buttonApplyQss{nullptr};

    QLabel *labelUnderCursorColor{nullptr};
    QLineEdit *editUnderCursorColor{nullptr};
    QComboBox *comboboxColorFormat{nullptr};

    QLabel *labelDebugMessage{nullptr};
    QTextEdit *editDebugMessage{nullptr};

    QLabel *labelPalette{ nullptr };
    PaletteInspector *inspectorPalette{ nullptr };

    QLabel *labelVisibleRect{nullptr};
    QLineEdit *editVisibleRect{nullptr};

    QLabel *labelStatusTip{ nullptr };
    QLineEdit *editStatusTip{ nullptr };

    QLabel *labelEnabled{ nullptr };
    QCheckBox *checkEnabled{ nullptr };

    QLabel *labelColor{ nullptr };
    ColorButton *buttonColor{ nullptr };

    QLabel *labelWindowId{ nullptr };
    QLineEdit *editWindowId{ nullptr };

    HxToolBar *mBottomToolBar{ nullptr };
    QToolButton *mButtonCopyAppStylesheet{ nullptr };

    QLabel *labelContentsMargins{ nullptr };
    QLineEdit *editContentsMargins{ nullptr };

    QLabel *labelLayoutContentsMargins{ nullptr };
    QLineEdit *editLayoutContentsMargins{ nullptr };
};

inline void WidgetInspectorUi::setupUi(WidgetInspector *widget) {
    mWidget = widget;
    mWidget->setObjectName("Container");
    mWidget->setMinimumWidth(720);
    /*/
    mWidget->setStyleSheet(R"(
    QWidget#Container {
        background-color: #1A1A1A;
    }
    QScrollArea {
        background-color: transparent;
    }

    QAbstractItemView QHeaderView {
        background-color: transparent;
    }

    QAbstractItemView QHeaderView::section {
        background-color: transparent;
    }

    QAbstractItemView QHeaderView::section:checked {
        background-color: palette(highlight);
    }

    QTableView QTableCornerButton::section {
        background-color: transparent;
    }
    )"); //*/

    auto font = mWidget->font();
    font.setFamilies({"Consolas","MicroSoft YaHei UI"});
    font.setPixelSize(12);
    font.setStyleStrategy(QFont::PreferAntialias);
    font.setHintingPreference(QFont::PreferFullHinting);
    mWidget->setFont(font);

    const auto layoutPropert = new QFormLayout;
    layoutPropert->setContentsMargins(0,0,0,0);
    // ObjectName
    labelObjectName = new QLabel;
    editObjectName = new QLineEdit;
    layoutPropert->addRow(labelObjectName, editObjectName);
    // ClassName
    labelClassName = new QLabel;
    editClassName = new QLineEdit;
    layoutPropert->addRow(labelClassName, editClassName);
    // InheritHierarchy
    labelInheritHierarchy = new QLabel;
    editInheritHierarchy = new QLineEdit;
    layoutPropert->addRow(labelInheritHierarchy, editInheritHierarchy);
    // ObjectHierarchy
    labelObjectHierarchy = new QLabel;

    layoutObjectHierarchyContents = new MyFlowLayout;
    layoutObjectHierarchyContents->setContentsMargins(3,3,3,3);
    layoutObjectHierarchyContents->setHorizontalSpacing(3);
    layoutObjectHierarchyContents->setVerticalSpacing(3);

    auto objectHierarchyContentWidget = new QWidget;
    objectHierarchyContentWidget->setLayout(layoutObjectHierarchyContents);
    objectHierarchyContentWidget->setObjectName("Container");

    areaObjectHierarchy = new QScrollArea;
    areaObjectHierarchy->setWidgetResizable(true);
    areaObjectHierarchy->setWidget(objectHierarchyContentWidget);
    areaObjectHierarchy->setFixedHeight(64);
    layoutPropert->addRow(labelObjectHierarchy, areaObjectHierarchy);
    // Position
    labelPosition = new QLabel;
    editPosition = new QLineEdit;
    layoutPropert->addRow(labelPosition, editPosition);
    // Size
    labelSize = new QLabel;
    editSize = new QLineEdit;
    layoutPropert->addRow(labelSize, editSize);
    // Min Size
    labelMinSize = new QLabel;
    editMinSize = new QLineEdit;
    layoutPropert->addRow(labelMinSize, editMinSize);
    // Min Size
    labelMaxSize = new QLabel;
    editMaxSize = new QLineEdit;
    layoutPropert->addRow(labelMaxSize, editMaxSize);
    // ContentsMargins
    labelContentsMargins = new QLabel;
    editContentsMargins = new QLineEdit;
    layoutPropert->addRow(labelContentsMargins, editContentsMargins);
    // ContentsMargins
    labelLayoutContentsMargins = new QLabel;
    editLayoutContentsMargins = new QLineEdit;
    layoutPropert->addRow(labelLayoutContentsMargins, editLayoutContentsMargins);
    // Font
    labelFont = new QLabel;
    editFont = new QLineEdit;
    layoutPropert->addRow(labelFont, editFont);

    // VisibleRect
    labelVisibleRect = new QLabel;
    editVisibleRect = new QLineEdit;
    layoutPropert->addRow(labelVisibleRect, editVisibleRect);

    // Stylesheet
    labelStylsheet = new QLabel;
    editStylesheet = new QTextEdit;
    editStylesheet->setAcceptRichText(false);
    layoutPropert->addRow(labelStylsheet, editStylesheet);

    // StatusTip
    labelStatusTip = new QLabel;
    editStatusTip = new QLineEdit;
    layoutPropert->addRow(labelStatusTip, editStatusTip);

    // Enabled
    labelEnabled = new QLabel;
    checkEnabled = new QCheckBox;
    checkEnabled->setEnabled(false);
    layoutPropert->addRow(labelEnabled, checkEnabled);

    buttonApplyQss = new QPushButton(editStylesheet);
    QObject::connect(buttonApplyQss, &QPushButton::clicked, mWidget, &WidgetInspector::ApplyQss);
    editStylesheet->installEventFilter(mWidget);

    labelWindowId = new QLabel;
    editWindowId = new QLineEdit;
    editWindowId->setReadOnly(true);
    layoutPropert->addRow(labelWindowId, editWindowId);

    labelColor = new QLabel;
    buttonColor = new ColorButton;
    layoutPropert->addRow(labelColor, buttonColor);

    const auto propertyContentsWidget = new QWidget;
    propertyContentsWidget->setLayout(layoutPropert);
    propertyContentsWidget->setObjectName("Container");

    const auto propertyScrollArea = new QScrollArea;
    propertyScrollArea->setWidgetResizable(true);
    propertyScrollArea->setWidget(propertyContentsWidget);

    const auto layoutTool = new QFormLayout;
    
    labelUnderCursorColor = new QLabel;
    editUnderCursorColor = new QLineEdit;
    comboboxColorFormat = new QComboBox;
    auto layoutColor = new QHBoxLayout;
    layoutColor->addWidget(editUnderCursorColor);
    layoutColor->addWidget(comboboxColorFormat);
    layoutTool->addRow(labelUnderCursorColor, layoutColor);

    labelDebugMessage = new QLabel;
    editDebugMessage = new QTextEdit;
    layoutTool->addRow(labelDebugMessage, editDebugMessage);
    labelDebugMessage->hide();
    editDebugMessage->hide();

    labelPalette = new QLabel;
    inspectorPalette = new PaletteInspector;
    inspectorPalette->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    layoutTool->addRow(labelPalette, inspectorPalette);

    //layoutTool->addItem(new QSpacerItem(6, 6, QSizePolicy::Expanding, QSizePolicy::Expanding));
    auto panelTool = new QWidget;
    panelTool->setLayout(layoutTool);

    auto splitter =new QSplitter;
    splitter->setOpaqueResize(true);
    splitter->addWidget(propertyScrollArea);
    splitter->addWidget(panelTool);
    splitter->setStretchFactor(1, 1);

    mBottomToolBar = new HxToolBar(mWidget);
    mButtonCopyAppStylesheet = mBottomToolBar->addToolButton("Copy application stylesheets");
    const auto &icon = QIcon(qApp->style()->standardPixmap(QStyle::StandardPixmap::SP_DialogSaveButton));
    mButtonCopyAppStylesheet->setIcon(QIcon::fromTheme("edit-copy", icon));
    QObject::connect(mButtonCopyAppStylesheet, &QToolButton::clicked, mWidget, &WidgetInspector::CopyAppStylesheets);

    auto layout = new QVBoxLayout;
    layout->setContentsMargins(3,3,3,3);
    layout->addWidget(splitter, 1);
    layout->addWidget(mBottomToolBar);

    mWidget->setLayout(layout);
    retranslateUi();
    mWidget->adjustSize();
}

inline void WidgetInspectorUi::retranslateUi() {
#if false
    mWidget->setWindowTitle(WidgetInspector::tr("Widget Inspector"));
    labelObjectName->setText(WidgetInspector::tr("Object Name"));
    labelClassName->setText(WidgetInspector::tr("Class Name"));
    labelInheritHierarchy->setText(WidgetInspector::tr("Inherit Hierarchy"));
    labelObjectHierarchy->setText(WidgetInspector::tr("Object Hierarchy"));
    labelPosition->setText(WidgetInspector::tr("Position"));
    labelSize->setText(WidgetInspector::tr("Size"));
    labelFont->setText(WidgetInspector::tr("Font"));
    labelStylsheet->setText(WidgetInspector::tr("Stylesheet"));
    buttonApplyQss->(WidgetInspector::tr("Set stylesheet"));
    labelUnderCursorColor->setText(WidgetInspector::tr("Screen Color"));
    labelDebugMessage->setText(WidgetInspector::tr("Debug Messages"));
    labelVisibleRect->setText(WidgetInspector::tr("Visible Rect"));
#else
    mWidget->setWindowTitle("部件监视器");
    labelObjectName->setText("对象名称");
    labelClassName->setText("类型名称");
    labelInheritHierarchy->setText("类型层级");
    labelObjectHierarchy->setText("窗口层级");
    labelPosition->setText("部件位置");
    labelSize->setText("部件大小");
    labelMinSize->setText("最小尺寸");
    labelMaxSize->setText("最大尺寸");
    labelContentsMargins->setText("内容边距");
    labelLayoutContentsMargins->setText("布局边距");
    labelFont->setText("字体");
    labelStylsheet->setText("部件样式");
    buttonApplyQss->setText("设置样式");
    labelWindowId->setText("窗口ID");

    labelUnderCursorColor->setText("屏幕颜色");
    labelDebugMessage->setText("调试消息");
    labelVisibleRect->setText("可视区域");
    labelStatusTip->setText("状态提示");
    labelEnabled->setText("可用");
    labelPalette->setText("调色板");
    labelColor->setText("部件颜色");

    mButtonCopyAppStylesheet->setText("复制App样式表");
#endif
}
