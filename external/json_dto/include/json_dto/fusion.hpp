#pragma once

#include <json_dto/pub.hpp>

#include <boost/fusion/include/define_struct.hpp>
#include <boost/fusion/include/at.hpp>


#include <boost/mpl/range_c.hpp>
#include <boost/mpl/for_each.hpp>

#define JSON_DTO_DEFINE_STRUCT(NAMESPACE_SEQ, NAME, ATTRIBUTES) \
    BOOST_FUSION_DEFINE_STRUCT(NAMESPACE_SEQ, NAME, ATTRIBUTES) \
    namespace json_dto \
    { \
    template<> struct is_custom_dto<BOOST_FUSION_ADAPT_STRUCT_NAMESPACE_DECLARATION((0)NAMESPACE_SEQ) NAME> : std::true_type {}; \
    template<> struct is_fusion_dto<BOOST_FUSION_ADAPT_STRUCT_NAMESPACE_DECLARATION((0)NAMESPACE_SEQ) NAME> : std::true_type {}; \
    } \


namespace json_dto
{

template <typename T>
struct is_fusion_dto : std::false_type
{};

template<typename JSON_IO, typename Member>
std::enable_if_t<is_nullable_t<Member>::value, void> fusion_serialize(JSON_IO &io,
                                                                    string_ref_t id,
                                                                    Member &member)
{
    io & json_dto::optional_null(id, member);
}

template<typename JSON_IO, typename Member>
std::enable_if_t<!is_nullable_t<Member>::value, void> fusion_serialize(JSON_IO &io,
                                                                     string_ref_t id,
                                                                     Member &member)
{
    io & json_dto::mandatory(id, member);
}

template<typename JSON_IO, typename DTO>
typename std::enable_if<is_fusion_dto<DTO>::value, void>::type
json_io(JSON_IO &io, DTO &msg)
{
    namespace fusion = boost::fusion;
    using range = boost::mpl::range_c<int, 0, boost::fusion::result_of::size<DTO>::value>;

    boost::mpl::for_each<range>([&io, &msg](auto index)
    {
        using type_index = decltype(index);
        auto name = fusion::extension::struct_member_name<DTO, type_index::value>::call();
        fusion_serialize(
            io, 
            {name, static_cast<unsigned>(std::strlen(name))},
            fusion::at<type_index>(msg));
    });
}

}