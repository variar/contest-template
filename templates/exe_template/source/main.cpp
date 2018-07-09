#include <thread>
#include <cli11/cli11.hpp>

#include <lib_template/dummy.h>

#include <stlab/concurrency/default_executor.hpp>
#include <stlab/concurrency/future.hpp>

#include <gsl/gsl_util.h>

#include <mpir.h>
#include <boost/multiprecision/gmp.hpp>
#include <boost/multiprecision/cpp_int.hpp>
#include <boost/multiprecision/cpp_int.hpp>

#include <app_utils/logger.h>

int main(int argc, char *argv[])
{
    auto logger = app_utils::Logger(argc, argv,
                                    app_utils::LoggerChannel::File,
                                    app_utils::LoggerFlags::CrashTrace 
                                    | app_utils::LoggerFlags::Verbose);
    
    CLI::App cli{"dummy program to test externals"};

    cli.add_flag_function("-c,--crash",
             [](size_t) {int* a = nullptr; *a = 0;},
             "crash switch");
   
    try {
        cli.parse(argc, argv);
    } catch (const CLI::ParseError &e) {
        return cli.exit(e);
    }

    Dummy d;

   auto f_mpir = stlab::async(stlab::default_executor, []
    { 
        using namespace boost::multiprecision;
        mpq_rational v = 1;

        LOG_INFO << "MPIR start";
        // Do some arithmetic:
        for(unsigned i = 1; i <= 1000; ++i)
            v *= i;

        LOG_INFO << "MPIR done" << v;

        return v;
    });

    auto f_boost = f_mpir.then(stlab::default_executor,[] (auto){
        using namespace boost::multiprecision;
        cpp_rational v = 1;

        LOG_INFO << "BOOST start";
        // Do some arithmetic:
        for (unsigned i = 1; i <= 1000; ++i)
            v *= i;

        LOG_INFO << "BOOST done"; // prints 1000!
        
        return v;
    });

    auto f_mpf = f_boost.then(stlab::default_executor, [] (auto)
    {
        using namespace boost::multiprecision;

        mpf_float v = 1.0f;
        LOG_INFO << "MPIR float start";
        // Do some arithmetic:
        for(unsigned i = 1; i <= 1000; ++i)
            v *= i;

        LOG_INFO << "MPIR float done" << v;
        return v;
    });

    // Waiting just for illustrational purpose
    while (!f_mpf.get_try()) {
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }

    std::cout << "The answer is " << f_mpf.get_try().value() << "\n";

    {
        auto guard = gsl::finally([]() { LOG_INFO << "guard done"; });
    }

    return 0;
}
