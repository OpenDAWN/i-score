#include "Client.hpp"

#include <iscore/serialization/DataStreamVisitor.hpp>
#include <iscore/serialization/JSONVisitor.hpp>

template<>
void Visitor<Reader<DataStream>>::readFrom(const Client& elt)
{
    readFrom(static_cast<const IdentifiedObject<Client>&>(elt));
    m_stream << elt.name();
    insertDelimiter();
}

template<>
void Visitor<Writer<DataStream>>::writeTo(Client& elt)
{
    QString s;
    m_stream  >> s;
    elt.setName(s);

    checkDelimiter();
}

template<>
void Visitor<Reader<JSONObject>>::readFrom(const Client& elt)
{
    readFrom(static_cast<const IdentifiedObject<Client>&>(elt));
    m_obj["Name"] = elt.name();
}

template<>
void Visitor<Writer<JSONObject>>::writeTo(Client& elt)
{
    elt.setName(m_obj["Name"].toString());
}
