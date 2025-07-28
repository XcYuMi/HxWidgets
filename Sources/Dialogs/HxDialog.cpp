#include "HxDialog.hpp"
#include "HxWidgetsGlobalPrivate.hpp"

class HxDialog::HxDialogPrivate {
    HxDialog *q = nullptr;
public:
    HxDialogPrivate(HxDialog *qptr) { q = qptr; }
};

HxDialog::~HxDialog() {

}

HxDialog::HxDialog(QWidget *parent) : Super{parent} {
    d.reset(new HxDialogPrivate(this));
}
