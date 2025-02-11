#include <QApplication>
#include <iscore/tools/IdentifiedObject.hpp>
#include <iscore/tools/ObjectPath.hpp>

ObjectPath ObjectPath::pathBetweenObjects(
        const QObject* const parent_obj,
        const QObject* target_object)
{
    std::vector<ObjectIdentifier> v;

    auto current_obj = target_object;
    auto add_parent_to_vector = [&v](const QObject * ptr)
    {
        if(auto id_obj = dynamic_cast<const IdentifiedObjectAbstract*>(ptr))
            v.push_back({id_obj->objectName(), id_obj->id_val() });
        else
            v.push_back({ptr->objectName(), {}});
    };

    QString debug_objectnames;
    // Recursively go through the object and all the parents
    while(current_obj != parent_obj)
    {
        debug_objectnames += current_obj->objectName() + "->";

        if(current_obj->objectName().isEmpty())
        {
            qDebug() << "Names: " << debug_objectnames;

            ISCORE_BREAKPOINT;
            throw std::runtime_error("ObjectPath::pathFromObject : an object in the hierarchy does not have a name.");
        }

        add_parent_to_vector(current_obj);

        current_obj = current_obj->parent();

        if(!current_obj)
        {
            ISCORE_BREAKPOINT;
            throw std::runtime_error("ObjectPath::pathFromObject : Could not find path to parent object");
        }
    }

    // Add the last parent (the one specified with parent_name)
    add_parent_to_vector(current_obj);

    // Search goes from top to bottom (of the parent hierarchy) instead
    std::reverse(std::begin(v), std::end(v));

    ObjectPath path{std::move(v)};
    path.m_cache = const_cast<QObject*>(target_object);

    return path;
}

QString ObjectPath::toString() const
{
    QString s;

    for(auto& obj : m_objectIdentifiers)
    {
        s += obj.objectName();

        if(obj.id())
        {
            s += ".";
            s += QString::number(*obj.id());
        }

        s += "/";
    }

    return s;
}


template<typename Container>
typename Container::value_type findById_weak_safe(const Container& c, int32_t id)
{
    auto it = std::find_if(std::begin(c),
                           std::end(c),
                           [&id](typename Container::value_type model)
    {
        return model->id_val() == id;
    });

    if(it != std::end(c))
    {
        return *it;
    }

    ISCORE_BREAKPOINT;
    throw std::runtime_error(QString("findById : id %1 not found in vector of %2").arg(id).arg(typeid(c).name()).toUtf8().constData());
}

template<typename Container>
typename Container::value_type findById_weak_unsafe(const Container& c, int32_t id)
{
    auto it = std::find_if(std::begin(c),
                           std::end(c),
                           [&id](typename Container::value_type model)
    {
        return model->id_val() == id;
    });

    if(it != std::end(c))
    {
        return *it;
    }

    return nullptr;
}



QObject* ObjectPath::find_impl() const
{
    auto parent_name = m_objectIdentifiers.at(0).objectName();
    std::vector<ObjectIdentifier> children(m_objectIdentifiers.size() - 1);
    std::copy(std::begin(m_objectIdentifiers) + 1,
              std::end(m_objectIdentifiers),
              std::begin(children));

    auto objs = qApp->findChildren<IdentifiedObjectAbstract*> (parent_name);
    NamedObject* obj = findById_weak_safe(objs, *m_objectIdentifiers.at(0).id());

    for(const auto& currentObjIdentifier : children)
    {
        if(currentObjIdentifier.id())
        {
            auto found_children = obj->findChildren<IdentifiedObjectAbstract*> (currentObjIdentifier.objectName(),
                            Qt::FindDirectChildrenOnly);

            obj = findById_weak_safe(found_children,
                           *currentObjIdentifier.id());
        }
        else
        {
            auto child = obj->findChild<NamedObject*> (currentObjIdentifier.objectName(),
                         Qt::FindDirectChildrenOnly);

            if(!child)
            {
                ISCORE_BREAKPOINT;
                throw std::runtime_error("ObjectPath::find  Error! Child not found");
            }

            obj = child;
        }
    }

    return obj;
}


QObject* ObjectPath::find_impl_unsafe() const
{
    auto parent_name = m_objectIdentifiers.at(0).objectName();
    std::vector<ObjectIdentifier> children(m_objectIdentifiers.size() - 1);
    std::copy(std::begin(m_objectIdentifiers) + 1,
              std::end(m_objectIdentifiers),
              std::begin(children));

    auto objs = qApp->findChildren<IdentifiedObjectAbstract*> (parent_name);
    NamedObject* obj = findById_weak_unsafe(objs, *m_objectIdentifiers.at(0).id());
    if(!obj)
        return nullptr;

    for(const auto& currentObjIdentifier : children)
    {
        if(currentObjIdentifier.id())
        {
            auto found_children = obj->findChildren<IdentifiedObjectAbstract*> (currentObjIdentifier.objectName(),
                            Qt::FindDirectChildrenOnly);

            obj = findById_weak_unsafe(found_children,
                           *currentObjIdentifier.id());

            if(!obj)
                return nullptr;
        }
        else
        {
            auto child = obj->findChild<NamedObject*> (currentObjIdentifier.objectName(),
                         Qt::FindDirectChildrenOnly);

            if(!child)
                return nullptr;

            obj = child;
        }
    }

    return obj;
}
