#include <lib_template/dummy.h>

#include <boost/any.hpp>

#include <plog/Log.h>


#include <mpir.h>
#include <boost/multiprecision/gmp.hpp>

#include <sstream>

namespace boost_mp = boost::multiprecision;
using int_type = boost_mp::number<boost_mp::backends::gmp_int,
                                  boost_mp::expression_template_option::et_off>;



Dummy::Dummy() 
{
    LOG_INFO << "Dummy lib constructed";
}
