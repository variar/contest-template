#include <gmock/gmock.h>

#include <boost/filesystem.hpp>

class Test : public ::testing::Test
{
};

TEST_F(Test, SetupTest)
{
}

TEST_F(Test, CheckTestData)
{
    EXPECT_TRUE(boost::filesystem::exists( "./test_data/example_test_gtest/data.txt" ));
}
