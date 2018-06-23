#pragma once
#include <json_dto/pub.hpp>

#include <boost/hana/define_struct.hpp>
#include <boost/hana/tuple.hpp>
#include <boost/hana/at_key.hpp>
#include <boost/hana/for_each.hpp>
#include <boost/hana/keys.hpp>

#include <boost/core/demangle.hpp>

#include <absl/types/variant.h>

namespace json_dto
{

template<typename JSON_IO, typename Member>
std::enable_if_t<is_nullable_t<Member>::value, void> hana_serialize(JSON_IO &io,
                                                                    string_ref_t id,
                                                                    Member &member)
{
    io & json_dto::optional_null(id, member);
}

template<typename JSON_IO, typename Member>
std::enable_if_t<!is_nullable_t<Member>::value, void> hana_serialize(JSON_IO &io,
                                                                     string_ref_t id,
                                                                     Member &member)
{
    io & json_dto::mandatory(id, member);
}

template<typename JSON_IO, typename HanaFoldable>
std::enable_if_t<boost::hana::Struct<HanaFoldable>::value, void> json_io(JSON_IO &io,
                                                                         HanaFoldable &msg)
{
    namespace hana = boost::hana;

    hana::for_each(hana::keys(msg), [&msg, &io](auto key)
    {
        hana_serialize(io,
                       {hana::to<char const *>(key), static_cast<unsigned int>(hana::length(key))},
                       hana::at_key(msg, key));
    });
}

struct json_o_visitor
{
public:
    json_o_visitor(json_output_t &output)
            : m_output(output) {}

    template<typename T>
    void operator()(T &dto)
    {
        json_io(m_output, dto);
    }

private:
    json_output_t &m_output;
};

template<typename ...Types>
void
json_io(json_output_t &io, absl::variant<Types...> &var)
{
    absl::visit(json_o_visitor{io}, var);
}

template <typename T>
std::string const& name_of(boost::hana::basic_type<T>) {
    static std::string name = boost::core::demangle(typeid(T).name());
    return name;
}

template<typename ...Types>
void
json_io(json_input_t &io, absl::variant<Types...> &var)
{
    boost::hana::tuple<boost::hana::type<Types>...> types;
    std::vector<std::string> exceptions;
    auto result = boost::hana::fold_left(types, false, [&io, &var, &exceptions](auto hasValue, auto memberType) {
        if (hasValue) {
            return hasValue;
        }


        try {
            using ValueT = typename decltype(memberType)::type;
            ValueT valueHolder;
            json_io(io, valueHolder);
            var = valueHolder;
            return true;
        }
        catch (const ex_t& e) {
            std::stringstream ss;
            ss << "error reading value of type " << name_of(memberType) << ": " << e.what();
            exceptions.push_back(ss.str());
            return false;
        }
    });

    if (result == false)
    {
        std::stringstream ss;
        bool isFirst = true;
        for (const auto& e: exceptions)
        {
            ss << (isFirst ? "[" : "] or [")  << e;
            isFirst = false;
        }
        ss << "]";
        throw ex_t{ss.str()};
    }
}
}