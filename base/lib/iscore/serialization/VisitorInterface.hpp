#pragma once
#include <type_traits>
/**
 *
 * This file contains the base types for the serialization mechanism
 * in i-score.
 */

class AbstractVisitor
{
};

template<typename VisitorType>
class Visitor : public AbstractVisitor
{
};

template<typename T>
class Reader
{
};

template<typename T>
class Writer
{
};

template<typename T>
using Serializer = Visitor<Reader<T>>;

template<typename T>
using Deserializer = Visitor<Writer<T>>;

using SerializationIdentifier = int;

/**
 * @brief The VisitorVariant struct
 *
 * Allows to pass visitor of multiple types in a function,
 * which is necessary when crossing plug-ins bounds if we don't
 * want to break their interface the day we change the available
 * serialization formats.
 */
struct VisitorVariant
{
        AbstractVisitor& visitor;
        const SerializationIdentifier identifier;
};

/**
 * Will allow a template to be selected if the given member
 * is a deserializer.
 */
template<typename DeserializerVisitor>
using enable_if_deserializer = typename std::enable_if_t<std::decay<DeserializerVisitor>::type::is_visitor_tag::value>;


// Declaration of common friends for classes that serialize themselves
#define ISCORE_SERIALIZE_FRIENDS(Type, Serializer) \
    friend void Visitor<Reader< Serializer >>::readFrom< Type > (const Type &); \
    friend void Visitor<Writer< Serializer >>::writeTo< Type > (Type &);


// Inherit from this to have
// the type treated as a value in the serialization context. Useful
// for choice between JSONObject / JSONValue
template<typename T>
struct is_value_tag {
        static const constexpr bool value = false;
};
#define ISCORE_DECL_VALUE_TYPE(Type) \
    template<> struct is_value_tag<Type> { static const constexpr bool value = true; };

template<typename T>
struct is_value_t
{
    static const constexpr bool value{
        std::is_arithmetic<T>::value
     || std::is_enum<T>::value
     || is_value_tag<T>::value
    };
};

