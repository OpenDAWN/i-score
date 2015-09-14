#include "ScenarioItemModel.hpp"
ScenarioItemModel::ScenarioItemModel(QObject* selected, QObject* parent):
    QAbstractItemModel{parent},
    m_root{selected}
{
    this->beginResetModel();
    this->endResetModel();
}
