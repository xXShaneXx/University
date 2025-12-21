#include "gtest/gtest.h"
#include "library.h"
#include "gtest/gtest.h"
#include "library.h"

TEST(LibraryTest, AddReader) 
{
    Library lib;
    lib.addReader("John", "Doe", 30);
    lib.addReader("Jane", "Smith", 25);

    testing::internal::CaptureStdout();
    lib.showReaders();
    std::string output = testing::internal::GetCapturedStdout();

    EXPECT_NE(output.find("John"), std::string::npos);
    EXPECT_NE(output.find("Doe"), std::string::npos);
    EXPECT_NE(output.find("Jane"), std::string::npos);
    EXPECT_NE(output.find("Smith"), std::string::npos);
}

TEST(LibraryTest, AddBook) 
{
    Library lib;
    lib.addBook("The Great Gatsby", "F. Scott Fitzgerald", "Fiction", 1925);
    lib.addBook("1984", "George Orwell", "Dystopian", 1949);

    testing::internal::CaptureStdout();
    lib.showBooks();
    std::string output = testing::internal::GetCapturedStdout();

    EXPECT_NE(output.find("The Great Gatsby"), std::string::npos);
    EXPECT_NE(output.find("1984"), std::string::npos);
}

TEST(LibraryTest, RemoveBook) 
{
    Library lib;
    lib.addBook("The Great Gatsby", "F. Scott Fitzgerald", "Fiction", 1925);
    lib.removeBook(0);

    testing::internal::CaptureStdout();
    lib.showBooks();
    std::string output = testing::internal::GetCapturedStdout();

    EXPECT_EQ(output.find("The Great Gatsby"), std::string::npos);
}

TEST(LibraryTest, AddSpecimen) 
{
    Library lib;
    lib.addBook("The Great Gatsby", "F. Scott Fitzgerald", "Fiction", 1925);
    lib.addSpecimen(0);
    lib.addSpecimen(0);

    const Book& book = lib.getBookById(0);
    testing::internal::CaptureStdout();
    book.show_info();
    std::string output = testing::internal::GetCapturedStdout();

    EXPECT_NE(output.find("Specimens: 2"), std::string::npos);
}

TEST(LibraryTest, RentBook) 
{
    Library lib;
    lib.addReader("John", "Doe", 30);
    lib.addBook("The Great Gatsby", "F. Scott Fitzgerald", "Fiction", 1925);
    lib.addSpecimen(0);

    bool success = lib.rentBook(0, 0);
    EXPECT_TRUE(success);

    testing::internal::CaptureStdout();
    lib.showBorrowedBooksForReader(0);
    std::string output = testing::internal::GetCapturedStdout();

    EXPECT_NE(output.find("The Great Gatsby"), std::string::npos);
}

TEST(LibraryTest, ReturnBook) 
{
    Library lib;
    lib.addReader("John", "Doe", 30);
    lib.addBook("The Great Gatsby", "F. Scott Fitzgerald", "Fiction", 1925);
    lib.addSpecimen(0);

    lib.rentBook(0, 0);
    bool success = lib.returnBook(0, 0);
    EXPECT_TRUE(success);

    testing::internal::CaptureStdout();
    lib.showBorrowedBooksForReader(0);
    std::string output = testing::internal::GetCapturedStdout();

    EXPECT_EQ(output.find("The Great Gatsby"), std::string::npos);
}

TEST(LibraryTest, FindBooks) 
{
    Library lib;
    lib.addBook("The Great Gatsby", "F. Scott Fitzgerald", "Fiction", 1925);
    lib.addBook("1984", "George Orwell", "Dystopian", 1949);

    std::vector<int> foundBooks = lib.findBooks("1984", "", 0, "");
    EXPECT_EQ(foundBooks.size(), 1);
    EXPECT_EQ(foundBooks[0], 1);
}

TEST(LibraryTest, ShowRentalHistoryForReader) 
{
    Library lib;
    lib.addReader("John", "Doe", 30);
    lib.addBook("The Great Gatsby", "F. Scott Fitzgerald", "Fiction", 1925);
    lib.addSpecimen(0);

    lib.rentBook(0, 0);
    lib.returnBook(0, 0);

    testing::internal::CaptureStdout();
    lib.showRentalHistoryForReader(0);
    std::string output = testing::internal::GetCapturedStdout();

    EXPECT_NE(output.find("The Great Gatsby"), std::string::npos);
    EXPECT_NE(output.find("Returned on"), std::string::npos);
}
