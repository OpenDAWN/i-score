#include "DeviceExplorerMenuButton.hpp"
#include <Explorer/Explorer/DeviceExplorerModel.hpp>
#include <Device/QMenuView/qmenuview.h>
#include <QPushButton>
#include <QGridLayout>
DeviceExplorerMenuButton::DeviceExplorerMenuButton(DeviceExplorerModel *model)
{
    auto lay = new QGridLayout;
    this->setLayout(lay);
    lay->setSpacing(0);
    lay->setContentsMargins(0,0,0,0);

    // Menu button
    auto pb = new QPushButton {"/", this};

    auto menuview = new QMenuView {pb};
    menuview->setModel(reinterpret_cast<QAbstractItemModel*>(model));

    connect(menuview, &QMenuView::triggered,
            this, [&](const QModelIndex & m)
    { emit addressChosen(iscore::address(model->nodeFromModelIndex(m))); });

    pb->setMenu(menuview);
    this->layout()->addWidget(pb);
}
