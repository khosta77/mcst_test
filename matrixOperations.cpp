#include "matrixOperations.hpp"

#include <cassert>

void test_empty()
{
    Matrix m;
    assert( m.empty() );
    assert( m.rows() == 0 );
    assert( m.cols() == 0 );
}

void test_make_use( const size_t row, const size_t col )
{
    Matrix m( row, col );
    assert( !m.empty() );
    assert( m.rows() == row );
    assert( m.cols() == col );

    for( size_t i = 0; i < row; ++i )
        for( size_t j = 0; j < col; ++j )
            m( i, j ) = ( row * col );

    for( size_t i = 0; i < row; ++i )
        for( size_t j = 0; j < col; ++j )
            assert( m( i, j ) == ( row * col ) );

    for( size_t i = 0; i < row; ++i )
        for( size_t j = 0; j < col; ++j )
            m( i, j ) = ( m( i, j ) + ( row * col ) );

    for( size_t i = 0; i < row; ++i )
        for( size_t j = 0; j < col; ++j )
            assert( m( i, j ) == ( 2 * row * col ) );
}

void test_out_of_range()
{
    Matrix m( 2, 1 );
    try
    {
        m( 100, 0 );
    }
    catch( const std::exception &emsg )
    {
        std::string result( emsg.what() );
        assert( ( result == "(100, 0) != [2, 1]" ) );
        result.clear();
    };
}

Matrix unpack( std::istream &in )
{
    size_t rows, cols;
    in >> rows >> cols;
    Matrix m( rows, cols );
    for( size_t i = 0; i < rows; ++i )
        for( size_t j = 0; j < cols; ++j )
            in >> m( i, j );
    return m;
}

void test_equall()
{
    std::stringstream input1, input2, input3;
    input1 << "2 2\n 1 2 3 4" << std::endl;
    input2 << "2 2\n 0 1 2 3" << std::endl;
    input3 << "2 1\n 0 1" << std::endl;
    Matrix m1 = unpack(input1);
    Matrix m2 = unpack(input2);
    Matrix m3 = unpack(input3);

    assert( ( m1 == m1 ) );
    assert( !( m1 == m2 ) );
    assert( !( m1 == m3 ) );
}

void test_constuct()
{
    std::stringstream input1, input2;
    input1 << "2 2\n 1 2 3 4" << std::endl;
    input2 << "2 2\n 0 1 2 3" << std::endl;
    Matrix m1 = unpack(input1);
    Matrix m2 = unpack(input2);

    Matrix m4(m1);
    assert( ( m1 == m4 ) );

    Matrix m5(std::move(m1));
    assert( ( m5 == m4 ) );

    Matrix m6;
    m6 = m2;
    assert( ( m6 == m2 ) );
    m2 = m6;
    assert( ( m6 == m2 ) );

    Matrix m7;
    m7 = std::move(m2);
    assert( ( m7 == m6 ) );
    assert( m2.empty() );

    m7 = std::move(m6);
    assert( m6.empty() );
    assert( m7( 0, 0 ) == 0 );
    assert( m7( 0, 1 ) == 1 );
    assert( m7( 1, 0 ) == 2 );
    assert( m7( 1, 1 ) == 3 );
}

void test_plus()
{
    std::stringstream input1, input2, input3;
    input1 << "2 2\n 1 2 3 4" << std::endl;
    input2 << "2 2\n 0 1 2 3" << std::endl;
    input3 << "2 1\n 0 1" << std::endl;
    Matrix m1 = unpack(input1);
    Matrix m2 = unpack(input2);
    Matrix m3 = unpack(input3);

    Matrix m4 = ( m1 + m2 );
    std::stringstream output;
    output << m4;
    assert( output.str() == "1 3 5 7" );
    assert( m4.rows() == 2 );
    assert( m4.cols() == 2 );

    try
    {
        Matrix m5 = ( m1 + m3 );
    }
    catch( const std::exception &emsg )
    {
        std::string result( emsg.what() );
        assert( ( result == "[2, 2] != [2, 1]" ) );
        result.clear();
    };
}

void test_minus()
{
    std::stringstream input1, input2, input3;
    input1 << "2 2\n 1 2 3 4" << std::endl;
    input2 << "2 2\n 0 1 2 3" << std::endl;
    input3 << "2 1\n 0 1" << std::endl;
    Matrix m1 = unpack(input1);
    Matrix m2 = unpack(input2);
    Matrix m3 = unpack(input3);

    Matrix m4 = ( m1 - m2 );
    std::stringstream output;
    output << m4;
    assert( output.str() == "1 1 1 1" );
    assert( m4.rows() == 2 );
    assert( m4.cols() == 2 );

    try
    {
        Matrix m5 = ( m1 - m3 );
    }
    catch( const std::exception &emsg )
    {
        std::string result( emsg.what() );
        assert( ( result == "[2, 2] != [2, 1]" ) );
        result.clear();
    };
}

void tests()
{
    test_empty();
    test_make_use( 1, 1 );
    test_make_use( 2, 2 );
    test_make_use( 3, 3 );
    test_make_use( 4, 6 );
    test_make_use( 6, 4 );
    test_out_of_range();
    test_equall();
    test_constuct();
    test_plus();
    test_minus();
}

int main()
{
    tests();
    return 0;
}
