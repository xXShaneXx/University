#include "gtest/gtest.h"
#include "book.h"

// Test Book constructor
TEST(BookTest, Constructor) 
{
    Book book("The Great Gatsby", "F. Scott Fitzgerald", "Novel", 1925, 1);
    EXPECT_EQ(book.getTitle(), "The Great Gatsby");
    EXPECT_EQ(book.getAuthor(), "F. Scott Fitzgerald");
    EXPECT_EQ(book.getGenre(), "Novel");
    EXPECT_EQ(book.getYear(), 1925);
    EXPECT_EQ(book.getId(), 1);
}

// Test addSpecimen and getAvailableSpecimen
TEST(BookTest, AddAndGetAvailableSpecimen) 
{
    Book book("1984", "George Orwell", "Dystopian", 1949, 2);
    book.addSpecimen(1);
    book.addSpecimen(2);

    EXPECT_NE(book.getAvailableSpecimen(), nullptr);
    EXPECT_EQ(book.getAvailableSpecimen()->id, 1);

    // Rent first specimen and test availability of the next one
    book.getAvailableSpecimen()->isrented = true;
    EXPECT_NE(book.getAvailableSpecimen(), nullptr);
    EXPECT_EQ(book.getAvailableSpecimen()->id, 2);
}

// Test equality operator
TEST(BookTest, EqualityOperator) 
{
    Book book1("Pride and Prejudice", "Jane Austen", "Novel", 1813, 4);
    Book book2("Pride and Prejudice", "Jane Austen", "Novel", 1813, 5);
    Book book3("Emma", "Jane Austen", "Novel", 1815, 6);
    
    EXPECT_TRUE(book1 == book2);
    EXPECT_FALSE(book1 == book3);
}

int main(int argc, char **argv) 
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
