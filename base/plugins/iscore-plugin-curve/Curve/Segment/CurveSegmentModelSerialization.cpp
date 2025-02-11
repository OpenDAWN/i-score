#include "CurveSegmentModelSerialization.hpp"
#include "CurveSegmentModel.hpp"
#include "CurveSegmentList.hpp"
#include <iscore/serialization/VisitorCommon.hpp>
#include <iscore/tools/std/StdlibWrapper.hpp>
#include <core/application/ApplicationComponents.hpp>
template<>
void Visitor<Reader<DataStream>>::readFrom(const CurveSegmentData& segmt)
{
    m_stream << segmt.id
             << segmt.start << segmt.end
             << segmt.previous << segmt.following
             << segmt.type;

    auto& csl = context.components.factory<DynamicCurveSegmentList>();
    auto segmt_fact = csl.list().get(segmt.type);

    ISCORE_ASSERT(segmt_fact);
    segmt_fact->serializeCurveSegmentData(segmt.specificSegmentData, this->toVariant());

    insertDelimiter();
}

template<>
void Visitor<Writer<DataStream>>::writeTo(CurveSegmentData& segmt)
{
    m_stream >> segmt.id
             >> segmt.start >> segmt.end
             >> segmt.previous >> segmt.following
             >> segmt.type;

    auto& csl = context.components.factory<DynamicCurveSegmentList>();
    auto segmt_fact = csl.list().get(segmt.type);
    ISCORE_ASSERT(segmt_fact);
    segmt.specificSegmentData = segmt_fact->makeCurveSegmentData(this->toVariant());

    checkDelimiter();
}


template<>
void Visitor<Reader<DataStream>>::readFrom(const std::vector<CurveSegmentData>& segmt)
{
    readFrom_vector_obj_impl(*this, segmt);
}

template<>
void Visitor<Writer<DataStream>>::writeTo(std::vector<CurveSegmentData>& segmt)
{
    writeTo_vector_obj_impl(*this, segmt);
}

template<>
void Visitor<Reader<DataStream>>::readFrom(const CurveSegmentModel& segmt)
{
    // To allow recration using createProcess
    readFrom(segmt.key());

    // Save the parent class
    readFrom(static_cast<const IdentifiedObject<CurveSegmentModel>&>(segmt));

    // Save this class (this will be loaded by writeTo(*this) in CurveSegmentModel ctor
    m_stream << segmt.previous() << segmt.following()
             << segmt.start() << segmt.end();

    // Save the subclass
    segmt.serialize(toVariant());

    insertDelimiter();
}

template<>
void Visitor<Writer<DataStream>>::writeTo(CurveSegmentModel& segmt)
{
    m_stream >> segmt.m_previous >> segmt.m_following
             >> segmt.m_start >> segmt.m_end;

    // Note : don't call setStart/setEnd here since they
    // call virtual methods and this may be called from
    // CurveSegmentModel's constructor.

    // Note : delimiter checked in createCurveSegment
}

template<>
void Visitor<Reader<JSONObject>>::readFrom(const CurveSegmentModel& segmt)
{
    // To allow recration using createProcess
    m_obj["Name"] = toJsonValue(segmt.key());

    // Save the parent class
    readFrom(static_cast<const IdentifiedObject<CurveSegmentModel>&>(segmt));

    // Save this class (this will be loaded by writeTo(*this) in CurveSegmentModel ctor
    m_obj["Previous"] = toJsonValue(segmt.previous());
    m_obj["Following"] = toJsonValue(segmt.following());
    m_obj["Start"] = toJsonValue(segmt.start());
    m_obj["End"] = toJsonValue(segmt.end());

    // Save the subclass
    segmt.serialize(toVariant());
}

template<>
void Visitor<Writer<JSONObject>>::writeTo(CurveSegmentModel& segmt)
{
    segmt.m_previous = fromJsonValue<Id<CurveSegmentModel>>(m_obj["Previous"]);
    segmt.m_following = fromJsonValue<Id<CurveSegmentModel>>(m_obj["Following"]);
    segmt.m_start = fromJsonValue<Curve::Point>(m_obj["Start"]);
    segmt.m_end = fromJsonValue<Curve::Point>(m_obj["End"]);
}




CurveSegmentModel*createCurveSegment(
        const DynamicCurveSegmentList& csl,
        Deserializer<DataStream>& deserializer,
        QObject* parent)
{
    CurveSegmentFactoryKey name;
    deserializer.writeTo(name);

    auto fact = csl.list().get(name);
    auto model = fact->load(deserializer.toVariant(), parent);

    deserializer.checkDelimiter();
    return model;
}

CurveSegmentModel*createCurveSegment(
        const DynamicCurveSegmentList& csl,
        Deserializer<JSONObject>& deserializer,
        QObject* parent)
{
    auto fact = csl.list().get(fromJsonValue<CurveSegmentFactoryKey>(deserializer.m_obj["Name"]));
    auto model = fact->load(deserializer.toVariant(), parent);

    return model;
}


CurveSegmentModel*createCurveSegment(
        const DynamicCurveSegmentList& csl,
        const CurveSegmentData& dat,
        QObject* parent)
{
    auto fact = csl.list().get(dat.type);
    auto model = fact->load(dat, parent);

    return model;

}
