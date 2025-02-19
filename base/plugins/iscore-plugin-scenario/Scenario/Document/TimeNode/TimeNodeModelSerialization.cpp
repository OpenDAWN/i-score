#include "TimeNodeModel.hpp"
#include "Trigger/TriggerModel.hpp"

template<>
void Visitor<Reader<DataStream>>::readFrom(const TimeNodeModel& timenode)
{
    readFrom(static_cast<const IdentifiedObject<TimeNodeModel>&>(timenode));

    readFrom(timenode.metadata);
    readFrom(timenode.pluginModelList);

    m_stream << timenode.m_date
             << timenode.m_events
             << timenode.m_extent;

    m_stream << timenode.trigger()->active()
             << timenode.trigger()->expression();

    insertDelimiter();
}

template<>
void Visitor<Writer<DataStream>>::writeTo(TimeNodeModel& timenode)
{
    writeTo(timenode.metadata);
    timenode.pluginModelList = iscore::ElementPluginModelList{*this, &timenode};

    bool a;
    iscore::Trigger t;
    m_stream >> timenode.m_date
             >> timenode.m_events
             >> timenode.m_extent
             >> a
             >> t;

    timenode.m_trigger = new TriggerModel{Id<TriggerModel>(0), &timenode};
    timenode.trigger()->setExpression(t);
    timenode.trigger()->setActive(a);

    checkDelimiter();
}

template<>
void Visitor<Reader<JSONObject>>::readFrom(const TimeNodeModel& timenode)
{
    readFrom(static_cast<const IdentifiedObject<TimeNodeModel>&>(timenode));
    m_obj["Metadata"] = toJsonObject(timenode.metadata);

    m_obj["Date"] = toJsonValue(timenode.date());
    m_obj["Events"] = toJsonArray(timenode.m_events);
    m_obj["Extent"] = toJsonValue(timenode.m_extent);

    m_obj["PluginsMetadata"] = toJsonValue(timenode.pluginModelList);

    QJsonObject trig;
    trig["Active"] = timenode.m_trigger->active();
    trig["Expression"] = toJsonObject(timenode.m_trigger->expression());
    m_obj["Trigger"] = trig;
}

template<>
void Visitor<Writer<JSONObject>>::writeTo(TimeNodeModel& timenode)
{
    timenode.metadata = fromJsonObject<ModelMetadata>(m_obj["Metadata"].toObject());

    timenode.m_date = fromJsonValue<TimeValue> (m_obj["Date"]);
    timenode.m_extent = fromJsonValue<VerticalExtent>(m_obj["Extent"]);

    fromJsonValueArray(m_obj["Events"].toArray(), timenode.m_events);

    timenode.m_trigger =  new TriggerModel{Id<TriggerModel>(0), &timenode};

    Trigger t;
    fromJsonObject(m_obj["Trigger"].toObject()["Expression"].toObject(),t);
    timenode.m_trigger->setExpression(t);
    timenode.m_trigger->setActive(m_obj["Trigger"].toObject()["Active"].toBool());

    Deserializer<JSONValue> elementPluginDeserializer(m_obj["PluginsMetadata"]);
    timenode.pluginModelList = iscore::ElementPluginModelList{elementPluginDeserializer, &timenode};
}
