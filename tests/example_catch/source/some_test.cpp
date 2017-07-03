#include <catch.hpp>
#include <tmock.hpp>

unsigned int Factorial1( unsigned int number ) {
    return number <= 1 ? number : Factorial1(number-1)*number;
}

TEST_CASE( "Factorials are computed once more", "[factorial]" ) {
    REQUIRE( Factorial1(1) == 1 );
    REQUIRE( Factorial1(2) == 2 );
    REQUIRE( Factorial1(3) == 6 );
    REQUIRE( Factorial1(10) == 3628800 );
}
