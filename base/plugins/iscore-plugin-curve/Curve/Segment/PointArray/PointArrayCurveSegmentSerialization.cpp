#include "PointArrayCurveSegmentModel.hpp"
#include <iscore/serialization/DataStreamVisitor.hpp>
#include <iscore/serialization/JSONVisitor.hpp>
template<>
void Visitor<Reader<DataStream>>::readFrom(const PointArrayCurveSegmentModel& segmt)
{
    ISCORE_TODO;
}

template<>
void Visitor<Writer<DataStream>>::writeTo(PointArrayCurveSegmentModel& segmt)
{
    ISCORE_TODO;
}

template<>
void Visitor<Reader<JSONObject>>::readFrom(const PointArrayCurveSegmentModel& segmt)
{
    ISCORE_TODO;
}

template<>
void Visitor<Writer<JSONObject>>::writeTo(PointArrayCurveSegmentModel& segmt)
{
    ISCORE_TODO;
}

template<>
void Visitor<Reader<DataStream>>::readFrom(const PointArrayCurveSegmentData& segmt)
{
    ISCORE_TODO;
}

template<>
void Visitor<Writer<DataStream>>::writeTo(PointArrayCurveSegmentData& segmt)
{
    ISCORE_TODO;
}

template<>
void Visitor<Reader<JSONObject>>::readFrom(const PointArrayCurveSegmentData& segmt)
{
    ISCORE_TODO;
}

template<>
void Visitor<Writer<JSONObject>>::writeTo(PointArrayCurveSegmentData& segmt)
{
    ISCORE_TODO;
}
