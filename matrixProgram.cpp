#include "matrixProgram.hpp"

#include <cassert>

void test_plus()
{
    std::stringstream input, output;
    input << "3 3\n +\n 1 2 3 4 5 6 7 8 9\n 11 12 13 14 15 16 17 18 19" << std::endl;
    Operations oper;
    oper( input, output );
    assert( output.str() == "12 14 16 18 20 22 24 26 28\n" );
}

void test_minus()
{
    std::stringstream input, output;
    input << "3 3\n -\n 1 2 3 4 5 6 7 8 9\n 11 12 13 14 15 16 17 18 19" << std::endl;
    Operations oper;
    oper( input, output );
    assert( output.str() == "-10 -10 -10 -10 -10 -10 -10 -10 -10\n" );
}

void test_some()
{
    std::stringstream input, output;
    input << "3 3\n *\n 1 2 3 4 5 6 7 8 9\n 11 12 13 14 15 16 17 18 19" << std::endl;
    Operations oper;
    try
    {
        oper( input, output );
        throw;
    }
    catch( const std::exception& emsg )
    {
        std::string result( emsg.what() );
        assert( ( result == "unknown operation" ) );
    result.clear();
    };
}

void test_invalid_input()
{
    std::stringstream input, output;
    Operations oper;
    try
    {
        oper( input, output );
        throw;
    }
    catch( const std::exception& emsg )
    {
        std::string result( emsg.what() );
        assert( ( result == "Произошла ошибка при чтении из потока" ) );
        result.clear();
    };
}

void tests()
{
    test_plus();
    test_minus();
    test_some();
    test_invalid_input();
}

int main()
{
    tests();
    return 0;
}
