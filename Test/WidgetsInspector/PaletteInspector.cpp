#include "PaletteInspector.hpp"

#include <QDebug>
#define DEBUG qDebug().noquote() << __FUNCTION__
#pragma execution_character_set("utf-8")

#include <QApplication>
#include <QPainter>

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QScrollArea>

#include <QMetaEnum>
#include <QTableView>
#include <QStandardItemModel>
#include <QStandardItem>
#include <QColorDialog>

#include "CustomizedWidgets.hpp"
#include <QLabel>
#include <QPushButton>
#include <QCheckBox>
#include <QLineEdit>
#include <QComboBox>

#include <Windows.h>
#include <WinUser.h>
#pragma comment(lib, "User32.lib")

#include "wingdi.h"
#pragma comment(lib, "Gdi32.lib")

using ColorRole = QPalette::ColorRole;
using ColorGroup = QPalette::ColorGroup;

enum class DataRole {
    Text = Qt::DisplayRole,
    Color = Qt::UserRole + 1,
};

class PaletteInspector::PaletteInspectorPrivate {
public:
    QStandardItemModel *paletteModel{ nullptr };
    QString stylesheet;

    struct {
        QColor color;
    } editParams;
};

class PaletteInspector::PaletteInspectorUi {
    PaletteInspector *_this{ nullptr };
public:
    void setupUi(PaletteInspector *widget);
    void retranslateUi();
public:
    QLabel *labelUnderCursorColor{ nullptr };
    QLineEdit *editUnderCursorColor{ nullptr };
    QComboBox *comboboxColorFormat{ nullptr };

    QLabel *labelUnderCursorPaletteColor{ nullptr };
    QLineEdit *editUnderCursorPaletteColor{ nullptr };

    QLabel *labelEnableAppStylesheet{ nullptr };
    QCheckBox *togglerEnableAppStylesheet{ nullptr };

    QTableView *paletteTable{ nullptr };
};

PaletteInspector::~PaletteInspector() {}

PaletteInspector::PaletteInspector(QWidget *parent) : QWidget{parent} {
    d.reset(new PaletteInspectorPrivate);

    ui.reset(new PaletteInspectorUi);
    ui->setupUi(this);
    ui->retranslateUi();

    initUi();
}

void PaletteInspector::initUi() {
    const auto meta_role = QMetaEnum::fromType<ColorRole>();
    int roleCount = meta_role.keyCount();
    QStringList roleLabels;
    for (int i = 0; i < meta_role.keyCount(); ++i) {
        const auto &role_name = meta_role.key(i);
        const auto role = ColorRole(meta_role.value(i));
        roleLabels.append(role_name);
    }

    const auto meta_group = QMetaEnum::fromType<ColorGroup>();
    int groupCount = meta_group.keyCount();
    QStringList groupLabels;
    for (int i = 0; i < meta_group.keyCount(); ++i) {
        const auto &group_name = meta_group.key(i);
        const auto group = ColorGroup(meta_group.value(i));
        groupLabels.append(group_name);
    }

    d->paletteModel = new QStandardItemModel(this);
    ui->paletteTable->setModel(d->paletteModel);
    ui->paletteTable->setSelectionBehavior(QAbstractItemView::SelectionBehavior::SelectItems);
    ui->paletteTable->setSelectionMode(QAbstractItemView::SelectionMode::SingleSelection);
    ui->paletteTable->setItemDelegate(new ColorItemDelegate);
    ui->paletteTable->setEditTriggers(QAbstractItemView::NoEditTriggers);

    d->paletteModel->setRowCount(roleCount);
    d->paletteModel->setColumnCount(groupCount);
    d->paletteModel->setHorizontalHeaderLabels(groupLabels);
    d->paletteModel->setVerticalHeaderLabels(roleLabels);

    for (int row = 0; row < roleCount; ++row) {
        for (int column = 0; column < groupCount; ++column) {
            const auto &index = d->paletteModel->index(row, column, QModelIndex());
            const auto item = d->paletteModel->itemFromIndex(index);

            const auto &color = QColor(Qt::green);
            const auto &colorName = color.name(QColor::HexArgb).toUpper();
            item->setData(QVariant::fromValue<QColor>(color), int(DataRole::Color));
            item->setText(colorName);
        }
    }

    /*/
    QSet<ColorGroup> hiddenGroups = { ColorGroup::NColorGroups, ColorGroup::Current, ColorGroup::All };
    for (int group : hiddenGroups) {
        ui->paletteTable->setColumnHidden(int(group), true);
    }
    ui->paletteTable->setColumnHidden(6, true);

    for (int row = roleCount - 2; row < roleCount; ++ row) {
        ui->paletteTable->setRowHidden(row, true);
    } //*/

    ui->comboboxColorFormat->addItem("HexRGB", QVariant::fromValue(ColorNameFormat::HexRgb));
    ui->comboboxColorFormat->addItem("RGB", QVariant::fromValue(ColorNameFormat::Rgb));
    ui->comboboxColorFormat->setCurrentText("HexRGB");
}

void PaletteInspector::refreshProperties() {
    const auto &palette = qApp->palette();

    const auto &roleMetaEnum = QMetaEnum::fromType<ColorRole>();
    const auto &groupMetaEnum = QMetaEnum::fromType<ColorGroup>();

    for (int row = 0; row < roleMetaEnum.keyCount(); ++row) {
        const auto role = ColorRole(roleMetaEnum.value(row));
        if (role >= ColorRole::NColorRoles) {
            continue;
        }
            
        for (int column = 0; column < groupMetaEnum.keyCount(); ++column) {
            if (ui->paletteTable->isColumnHidden(column)) {
                //continue;
            }

            const auto group = ColorGroup(groupMetaEnum.value(column));
            const auto &index = d->paletteModel->index(row, column, QModelIndex());
            const auto item = d->paletteModel->itemFromIndex(index);
            if (ui->paletteTable->currentIndex() == index)
                continue;
            const auto &color = palette.color(group, role);
            const auto &colorName = color.name(QColor::HexArgb).toUpper();
            item->setData(QVariant::fromValue<QColor>(color), int(DataRole::Color));
            item->setText(colorName);
        }
    }

    if (isVisible()) {
        const auto &color = GetColorUnderCursor();
        if (!ui->editUnderCursorColor->hasFocus() && d->editParams.color != color) {
            d->editParams.color = color;
            const auto &nameFormat = ui->comboboxColorFormat->currentData().value<ColorNameFormat>();
            const auto &colorText = ColorName(color, nameFormat);
            ui->editUnderCursorColor->setText(colorText.toUpper());
        }

        const auto paletteRoles = paletteRolesFromColor(color);
        ui->editUnderCursorPaletteColor->setText(paletteRoles.join("、"));
    }
}

QColor PaletteInspector::GetColorUnderCursor() {
    POINT cursorPos;
    ::GetCursorPos(&cursorPos);
    HDC hdc = ::GetDC(NULL);
    COLORREF colorRef = ::GetPixel(hdc, cursorPos.x, cursorPos.y);
    ::ReleaseDC(NULL, hdc);
    return QColor(GetRValue(colorRef), GetGValue(colorRef), GetBValue(colorRef));
}

QString PaletteInspector::ColorName(const QColor &color, ColorNameFormat format) {
    if (format == ColorNameFormat::HexRgb) {
        return color.name(QColor::HexRgb);
    }

    return QString("%1, %2, %3").arg(color.red()).arg(color.green()).arg(color.blue());
}

QStringList PaletteInspector::paletteRolesFromColor(const QColor &color) {
    const auto &palette = qApp->palette();
    QStringList rolePairs;
    const auto &roleMetaEnum = QMetaEnum::fromType<ColorRole>();
    const auto &groupMetaEnum = QMetaEnum::fromType<ColorGroup>();

    for (int row = 0; row < roleMetaEnum.keyCount(); ++row) {
        const auto role = ColorRole(roleMetaEnum.value(row));
        if (role >= ColorRole::NColorRoles || role == ColorRole::NoRole)
            continue;
        const auto &roleName = roleMetaEnum.key(row);
        for (int column = 0; column < groupMetaEnum.keyCount(); ++column) {
            const auto group = ColorGroup(groupMetaEnum.value(column));
            if (group >= ColorGroup::NColorGroups || column == 6)
                continue;

            const auto &groupName = groupMetaEnum.key(column);
            const auto &paletteColor = palette.color(group, role);
            if(paletteColor == color) {
                rolePairs += QString("%1[%2]").arg(roleName).arg(groupName);
            }
        }
    }
    return rolePairs;
}

void PaletteInspector::updatePaletteColor(const QColor &color, QPalette::ColorRole role, QPalette::ColorGroup group) {
    auto palette = qApp->palette();
    palette.setColor(group, role, color);

    for (const auto widget : qApp->allWidgets()) {
        widget->setPalette(palette);
    }

    qApp->style()->unpolish(qApp);
    qApp->setPalette(palette);
    qApp->style()->polish(qApp);
}

void PaletteInspector::onEnableAppStylesheetToggled(bool checked) {
    qApp->style()->unpolish(qApp);
    
    if (checked) {
        qApp->setStyleSheet(d->stylesheet);
    }
    else {
        d->stylesheet = qApp->styleSheet();
        qApp->setStyleSheet("");
    }

    qApp->style()->polish(qApp);
}

void PaletteInspector::onItemDoubleClicked(const QModelIndex &index) {
    const auto item = d->paletteModel->itemFromIndex(index);
    const auto &old_color = item->data(int(DataRole::Color)).value<QColor>();
    const auto &new_color = QColorDialog::getColor(old_color, this, "选择颜色", QColorDialog::ShowAlphaChannel);
    if (new_color.isValid() && new_color != old_color) {
        item->setData(new_color, int(DataRole::Color));
        item->setText(new_color.name(QColor::HexArgb).toUpper());

        const auto role = ColorRole(index.row());
        const auto group = ColorGroup(index.column());
        updatePaletteColor(new_color, role, group);
    }
}

void PaletteInspector::PaletteInspectorUi::setupUi(PaletteInspector *widget) {
    _this = widget;

    const auto layout = new QVBoxLayout(_this);

    const auto formLayout = new QFormLayout;
    layout->addLayout(formLayout);
    formLayout->setContentsMargins(0, 0, 0, 0);
    formLayout->setHorizontalSpacing(6);
    formLayout->setVerticalSpacing(3);

    labelUnderCursorColor = new QLabel;
    editUnderCursorColor = new QLineEdit;
    comboboxColorFormat = new QComboBox;
    auto layoutColor = new QHBoxLayout;
    layoutColor->setSpacing(6);
    layoutColor->addWidget(editUnderCursorColor);
    layoutColor->addWidget(comboboxColorFormat);
    formLayout->addRow(labelUnderCursorColor, layoutColor);

    labelUnderCursorPaletteColor = new QLabel;
    editUnderCursorPaletteColor = new QLineEdit;
    formLayout->addRow(labelUnderCursorPaletteColor, editUnderCursorPaletteColor);

    labelEnableAppStylesheet = new QLabel(_this);
    togglerEnableAppStylesheet = new QCheckBox(_this);
    formLayout->addRow(labelEnableAppStylesheet, togglerEnableAppStylesheet);
    togglerEnableAppStylesheet->setChecked(true);
    connect(togglerEnableAppStylesheet, &QCheckBox::toggled, _this, &PaletteInspector::onEnableAppStylesheetToggled);

    paletteTable = new QTableView(_this);
    layout->addWidget(paletteTable, 1);
    connect(paletteTable, &QTableView::doubleClicked, _this, &PaletteInspector::onItemDoubleClicked);
}

void PaletteInspector::PaletteInspectorUi::retranslateUi() {
    labelUnderCursorColor->setText("屏幕取色");
    labelUnderCursorPaletteColor->setText("同色的调色板颜色");
    labelEnableAppStylesheet->setText("启用APP样式表");
}

extern QColor getHighContrastColor(const QColor &color, qreal factor = 10.0);
void ColorItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const {
    const auto &color = index.data(int(DataRole::Color)).value<QColor>();
    const auto &color_name = color.name(QColor::HexArgb).toUpper();

    qApp->style()->drawPrimitive(QStyle::PE_PanelItemViewItem, &option, painter);

    int padding = 3;
    const auto &rect = option.rect.adjusted(padding, padding, -padding, -padding);
    painter->fillRect(rect, color);
    painter->setPen(true ? getHighContrastColor(color) : option.palette.color(QPalette::Light));
    painter->drawText(rect, Qt::AlignCenter, color_name);
}
