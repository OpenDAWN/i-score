#include "MessageNode.hpp"
#include <iscore/serialization/VisitorCommon.hpp>
template<typename T>
void toJsonValue(
        QJsonObject& object,
        const QString& name,
        const boost::optional<T>& value)
{
    if(value)
    {
        object[name] = marshall<JSONObject>(*value);
    }
}

template<typename T, std::size_t N>
QJsonArray toJsonArray(const std::array<T, N>& array)
{
    QJsonArray arr;
    for(std::size_t i = 0; i < N; i++)
    {
        arr.append(toJsonValue(array[i]));
    }

    return arr;
}

template<typename T, std::size_t N>
void fromJsonArray(const QJsonArray& array, std::array<T, N>& res)
{
    for(std::size_t i = 0; i < N; i++)
    {
        res[i] = fromJsonValue<T>(array[i]);
    }
}


template<typename T>
void fromJsonValue(
        const QJsonObject& object,
        const QString& name,
        boost::optional<T>& value)
{
    auto it = object.find(name);
    if(it != object.end())
    {
        value = unmarshall<iscore::Value>((*it).toObject());
    }
    else
    {
        value.reset();
    }
}




template<>
void Visitor<Reader<DataStream>>::readFrom(const std::array<PriorityPolicy, 3>& val)
{
    for(int i = 0; i < 3; i++)
        m_stream << val[i];
}

template<>
void Visitor<Writer<DataStream>>::writeTo(std::array<PriorityPolicy, 3>& val)
{
    for(int i = 0; i < 3; i++)
        m_stream >> val[i];
}

template<>
void Visitor<Reader<DataStream>>::readFrom(const ProcessStateData& val)
{
    m_stream << val.process << val.value;
}

template<>
void Visitor<Writer<DataStream>>::writeTo(ProcessStateData& val)
{
    m_stream >> val.process >> val.value;
}

template<>
void Visitor<Reader<JSONObject>>::readFrom(const ProcessStateData& val)
{
    m_obj["Process"] = toJsonValue(val.process);
    toJsonValue(m_obj, "Value", val.value);
}

template<>
void Visitor<Writer<JSONObject>>::writeTo(ProcessStateData& val)
{
    val.process = fromJsonValue<Id<Process>>(m_obj["Process"]);
    fromJsonValue(m_obj, "Value", val.value);
}


template<>
void Visitor<Reader<DataStream>>::readFrom(const StateNodeValues& val)
{
    m_stream << val.previousProcessValues << val.followingProcessValues << val.userValue << val.priorities;
}

template<>
void Visitor<Writer<DataStream>>::writeTo(StateNodeValues& val)
{
    m_stream >> val.previousProcessValues >> val.followingProcessValues >> val.userValue >> val.priorities;
}

template<>
void Visitor<Reader<JSONObject>>::readFrom(const StateNodeValues& val)
{
    m_obj["Previous"] = toJsonArray(val.previousProcessValues);
    m_obj["Following"] = toJsonArray(val.followingProcessValues);
    toJsonValue(m_obj, "User", val.userValue);
    m_obj["Priorities"] = toJsonArray(val.priorities);
}

template<>
void Visitor<Writer<JSONObject>>::writeTo(StateNodeValues& val)
{
    fromJsonArray(m_obj["Previous"].toArray(), val.previousProcessValues);
    fromJsonArray(m_obj["Following"].toArray(), val.followingProcessValues);
    fromJsonValue(m_obj, "User", val.userValue);
    fromJsonArray(m_obj["Priorities"].toArray(), val.priorities);
}


template<>
void Visitor<Reader<DataStream>>::readFrom(const StateNodeData& node)
{
    m_stream << node.name << node.values;
    insertDelimiter();
}

template<>
void Visitor<Writer<DataStream>>::writeTo(StateNodeData& node)
{
    m_stream >> node.name >> node.values;
    checkDelimiter();
}

template<>
void Visitor<Reader<JSONObject>>::readFrom(const StateNodeData& node)
{
    m_obj["Name"] = node.name;
    readFrom(node.values);
}

template<>
void Visitor<Writer<JSONObject>>::writeTo(StateNodeData& node)
{
    node.name = m_obj["Name"].toString();
    writeTo(node.values);
}
