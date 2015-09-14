#pragma once
#include <QAbstractItemModel>

#include "Document/Constraint/ConstraintModel.hpp"
#include "Document/Event/EventModel.hpp"
#include "Document/TimeNode/TimeNodeModel.hpp"
#include "Document/State/StateModel.hpp"
#include "Process/ScenarioModel.hpp"
#include <iscore/tools/VariantBasedNode.hpp>

using ScenarioElementBase = iscore::VariantBasedNode<
ScenarioModel*,
ConstraintModel*,
EventModel*,
StateModel*,
TimeNodeModel*>;
class ScenarioElement : public ScenarioElementBase
{
    public:
        using ScenarioElementBase::ScenarioElementBase;

};

namespace ScenarioNode
{
        template<typename Fun>
        auto apply(Fun fun, const QObject* node)
        {
            if(auto casted = dynamic_cast<const TimeNodeModel*>(node))
            {
                return fun(casted);
            }
            else if(auto casted = dynamic_cast<const StateModel*>(node))
            {
                return fun(casted);
            }
            else if(auto casted = dynamic_cast<const EventModel*>(node))
            {
                return fun(casted);
            }

            return fun(nullptr);
        }

        inline bool hasChild(nullptr_t obj, int row)
        {
            return false;
        }

        inline bool hasChild(const TimeNodeModel* obj, int row)
        {
            return obj->events().size() > row;
        }

        inline bool hasChild(const EventModel* obj, int row)
        {
            return obj->states().size() > row;
        }

        inline bool hasChild(const StateModel* obj, int row)
        {
            return false;
            return obj->states().rowCount({}) > row;
        }

        inline bool hasChild(const QObject* obj, int row)
        {
            return apply([&] (auto obj) {
                return hasChild(obj, row);
            }, obj);
        }


        inline QObject* childAt(nullptr_t obj, int row)
        {
            return nullptr;
        }

        inline QObject* childAt(const TimeNodeModel* obj, int row)
        {
            return &obj->parentScenario()->event(*(obj->events().begin() + row));
        }

        inline QObject* childAt(const EventModel* obj, int row)
        {
            return &obj->parentScenario()->state(*(obj->states().begin() + row));
        }

        inline QObject* childAt(const StateModel* obj, int row)
        {
            return nullptr;
        }

        inline auto childAt(const QObject* obj, int row)
        {
            return apply([&] (auto obj) {
                return childAt(obj, row);
            }, obj);
        }




        inline int indexOfChild(const TimeNodeModel* obj, const QObject* child)
        {
            auto cast = dynamic_cast<const EventModel*>(child);
            if(!cast)
                return -1;

            for(int i = 0; i < obj->events().size(); i++)
            {
                if(cast->id() == obj->events()[i])
                    return i;
            }

            return -1;
        }

        inline int indexOfChild(const EventModel* obj, const QObject* child)
        {
            auto cast = dynamic_cast<const StateModel*>(child);
            if(!cast)
                return -1;

            for(int i = 0; i < obj->states().size(); i++)
            {
                if(cast->id() == obj->states()[i])
                    return i;
            }

            return -1;
        }

        inline int indexOfChild(const StateModel* parent, const QObject* child)
        {
            return 0;
        }

        inline int indexOfChild(nullptr_t parent, const QObject* child)
        {
            return 0;
        }

        inline auto indexOfChild(const QObject* obj, const QObject* child)
        {
            return apply([&] (auto obj) {
                return indexOfChild(obj, child);
            }, obj);
        }



        inline int childCount(const TimeNodeModel* obj)
        {
            return obj->events().size();
        }
        inline int childCount(const EventModel* obj)
        {
            return obj->states().size();
        }

        inline int childCount(const StateModel* obj)
        {
            return 0;
        }

        inline int childCount(nullptr_t parent)
        {
            return 0;
        }

        inline auto childCount(const QObject* obj)
        {
            return apply([&] (auto obj) {
                return childCount(obj);
            }, obj);
        }



        inline QString name(const TimeNodeModel* obj)
        {
            return obj->metadata.name();
        }
        inline QString name(const EventModel* obj)
        {
            return obj->metadata.name();
        }

        inline QString name(const StateModel* obj)
        {
            return obj->metadata.name();
        }

        inline QString name(nullptr_t parent)
        {
            return "";
        }

        inline auto name(const QObject* obj)
        {
            return apply([&] (auto obj) {
                return name(obj);
            }, obj);
        }


        inline QVariant value(const TimeNodeModel* obj)
        {
            return {};
        }

        inline QVariant value(const EventModel* obj)
        {
            return obj->condition().toString();
        }

        inline QVariant value(const StateModel* obj)
        {
            return {};
        }

        inline QVariant value(nullptr_t parent)
        {
            return "";
        }

        inline auto value(const QObject* obj)
        {
            return apply([&] (auto obj) {
                return value(obj);
            }, obj);
        }
}


class ScenarioItemModel : public QAbstractItemModel
{
        QObject* m_root{};

    public:
        ScenarioItemModel(QObject* selected, QObject* parent);

        auto nodeToIndex(QModelIndex parent) const
        {
            const QObject* parentItem{};

            if (!parent.isValid())
                parentItem = m_root; // todo why ?
            else
                parentItem = static_cast<QObject*>(parent.internalPointer());

            return parentItem;
        }

        QModelIndex index(
                int row,
                int column,
                const QModelIndex& parent) const override
        {
            if (!hasIndex(row, column, parent))
                return QModelIndex();

            const QObject* parentItem = nodeToIndex(parent);

            if (ScenarioNode::hasChild(parentItem, row))
                return createIndex(row, column, ScenarioNode::childAt(parentItem, row));
            else
                return QModelIndex();
        }

        QModelIndex parent(const QModelIndex& index) const override
        {
            if (!index.isValid())
                return QModelIndex();

            auto childNode = nodeToIndex(index);
            auto parentNode = childNode->parent();

            if (!parentNode || parentNode == m_root)
                return QModelIndex();

            auto grandParentNode = parentNode->parent();
            if (!grandParentNode)
                return QModelIndex();

            const int rowParent = ScenarioNode::indexOfChild(grandParentNode, parentNode);
            assert(rowParent != -1);
            return createIndex(rowParent, 0, parentNode);
        }

        int rowCount(const QModelIndex& index) const override
        {
            if(index.column() > 0)
                return 0;

            return ScenarioNode::childCount(nodeToIndex(index));
        }

        int columnCount(const QModelIndex& parent) const override
        {
            return 2;
        }

        bool hasChildren(const QModelIndex &index) const
        {
            return ScenarioNode::childCount(nodeToIndex(index)) > 0;
        }

        QVariant data(const QModelIndex& index, int role) const override
        {
            qDebug(Q_FUNC_INFO);
            const int col = index.column();

            if(col < 0 || col > 1)
                return {};

            auto node = nodeToIndex(index);

            if(! node)
                return {};

            switch(col)
            {
                case 0:
                    if(role == Qt::DisplayRole)
                    {
                        return ScenarioNode::name(node);
                    }
                    break;
                case 1:
                {
                    if(role == Qt::DisplayRole)
                    {
                        return ScenarioNode::value(node);
                    }
                    break;
                }
                default:
                    break;
            }

            return {};
        }
};
