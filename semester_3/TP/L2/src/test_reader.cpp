#include "gtest/gtest.h"
#include "reader.h"
#include "gtest/gtest.h"
#include "reader.h"

TEST(ReaderTest, ConstructorTest) 
{
    Reader reader("John", "Doe", 30, 1);
    EXPECT_EQ(reader.getId(), 1);
}

TEST(ReaderTest, ShowInfoTest) 
{
    Reader reader("Jane", "Smith", 25, 2);
    testing::internal::CaptureStdout();
    reader.show_info();
    std::string output = testing::internal::GetCapturedStdout();
    std::string expected_output = "ID: 2\nName: Jane\nSurname: Smith\nAge: 25\n";
    EXPECT_EQ(output, expected_output);
}

TEST(ReaderTest, GetIdTest) 
{
    Reader reader("Alice", "Johnson", 40, 3);
    EXPECT_EQ(reader.getId(), 3);
}
