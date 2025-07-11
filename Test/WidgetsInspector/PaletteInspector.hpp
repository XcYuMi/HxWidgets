#pragma once 
#include <QWidget>
#include <QScopedPointer>

class PaletteInspector : public QWidget {
    Q_OBJECT
    class PaletteInspectorUi;
    friend class PaletteInspectorUi;
    QScopedPointer<PaletteInspectorUi> ui;
    class PaletteInspectorPrivate;
    QScopedPointer<PaletteInspectorPrivate> d;

public:
    enum class ColorNameFormat {
        HexRgb,
        Rgb
    }; Q_ENUM(ColorNameFormat)

public:
    explicit PaletteInspector(QWidget *parent = nullptr);
    ~PaletteInspector();
    void refreshProperties();
    static QColor GetColorUnderCursor();
    static QString ColorName(const QColor &color, ColorNameFormat format = ColorNameFormat::HexRgb);
    static QStringList paletteRolesFromColor(const QColor &color);
    static void updatePaletteColor(const QColor &color, QPalette::ColorRole role, QPalette::ColorGroup = QPalette::ColorGroup::Active);

private:
    void onEnableAppStylesheetToggled(bool checked);
    void onItemDoubleClicked(const QModelIndex &index);

private:
    void initUi();
};

#include <QStyledItemDelegate>
class ColorItemDelegate : public QStyledItemDelegate {
    Q_OBJECT
public:
    ColorItemDelegate(QObject *parent = nullptr) : QStyledItemDelegate{ parent } {}
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const override {
        auto hint = QStyledItemDelegate::sizeHint(option, index);
        hint.setHeight(24);
        return hint;
    }
};