#include <iostream>
#include <string>

// Standard single-line comment

/// Doxygen single-line comment
/** Doxygen multi-line comment
 * with multiple lines
 */

/*!
 * Another Doxygen multi-line comment
 * with exclamation mark
 */

/*
 * Standard multi-line comment
 * spanning multiple lines
 */

//! Doxygen single-line with exclamation

/// Another Doxygen single-line

/*
Standard multi-line without stars
//! But with Doxygen inside - should this be detected?
*/

/*
 * Multi-line with stars
 * *
 * *
 */

/**
 * Doxygen with code example
 * @code
 * int x = 5;
 * @endcode
 */

/*! Doxygen with params
 * @param x input value
 * @return output value
 */

int main() {
    std::cout << "Hello World"; // inline comment
    std::cout << "String with // comment inside" << std::endl;
    std::cout << "String with /* comment */ inside" << std::endl;
    std::cout << "String with /// doxygen inside" << std::endl;
    std::cout << "String with /** doxygen */ inside" << std::endl;
    std::cout << "String with /*! doxygen */ inside" << std::endl;

    // More comments
    /// More docs
    /** More docs */

    return 0;
}