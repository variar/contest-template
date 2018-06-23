#include <lib_template/dummy.h>

#include <plog/Log.h>

#include <sstream>

#include <json_dto/fusion.hpp>

#include <vector>
#include <string>

JSON_DTO_DEFINE_STRUCT(
    (json), A,
    (int, id)
    (json_dto::nullable_t<float>, x)
    (json_dto::nullable_t<float>, y)
)


JSON_DTO_DEFINE_STRUCT(
    (json), B,
    (int, id1)
    (json_dto::nullable_t<std::string>, str)
)

using A_or_B = json_dto::cpp17::variant<json::A, json::B>;


JSON_DTO_DEFINE_STRUCT(
    (json), C,
    (std::vector<A_or_B>, data)
)

Dummy::Dummy() 
{
    LOG_INFO << "Dummy lib constructed";

    std::string testJson = "{ \"data\" : [ "\
                                "{\"id2\" : 1, \"x\" : 0.1, \"y\" : 0.5},"\
                                "{\"id1\" : 1, \"str\" : \"y\"}"\
                            "]}";


    auto parsedData = json_dto::from_json<json::C>(testJson.c_str());
    auto s2 = json_dto::to_json(parsedData);

    LOG_INFO << "s2 " << s2;
}
