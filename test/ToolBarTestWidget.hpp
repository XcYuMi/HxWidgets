#include "HxWidgetsGlobal.hpp"
#include <QWidget>

class ToolBarTestWidget : public QWidget {
    Q_OBJECT
    HX_DECLARE_UI(ToolBarTestWidget)
public:
    explicit ToolBarTestWidget(QWidget *parent = nullptr);
    ~ToolBarTestWidget();
private:
    void onAddActionTriggered();
    void onAddSeparatorTriggered();
    void onAddStretchTriggered();
    void onAddSpacingTriggered();
};
