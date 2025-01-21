#ifndef MATRIXOPERATIONS_H_
#define MATRIXOPERATIONS_H_

#include <iostream>

#include <sstream>
#include <memory>
#include <cmath>
#include <limits>
#include <iomanip>
#include <numbers>

#include <exception>
#include <string>

#define STR(value) (std::to_string(value))

class MatrixException : public std::exception
{
public:
    explicit MatrixException(const std::string &msg) : m_msg(msg) {}
    const char *what() const noexcept override { return m_msg.c_str(); }
private:
    std::string m_msg;
};

class InvalidMatrixStream : public MatrixException
{
public:
    InvalidMatrixStream() : MatrixException("Произошла ошибка при чтении из потока") {}
};

class OutOfRange : public MatrixException
{
public:
    OutOfRange( size_t i, size_t j, size_t I, size_t J ) :
        MatrixException( "(" + STR(i) + ", " + STR(j) + ") != [" + STR(I) + ", " + STR(J) + "]" ) {}
};

class DimensionMismatch : public MatrixException
{
public:
    DimensionMismatch( size_t i, size_t j, size_t I, size_t J ) :
        MatrixException( "[" + STR(i) + ", " + STR(j) + "] != [" + STR(I) + ", " + STR(J) + "]" ) {}
};

class Matrix
{
private:
    using value_type = double;
    using size_type = size_t;
    using reference = value_type&;
    using const_reference = const value_type&;

    size_type _rows;
    size_type _cols;
    std::unique_ptr<value_type[]> _matrix;

    inline size_type iter( const size_type r, const size_type c ) { return ( c + ( r * _cols ) ); }
    inline size_type iter( const size_type r, const size_type c ) const { return ( c + ( r * _cols ) ); }

    inline void check( const size_type r, const size_type c ) const
    {
        if( ( ( r >= _rows ) or ( c >= _cols ) ) )
            throw OutOfRange( r, c, _rows, _cols );
    }

    inline void checkOperations( const Matrix &rhs ) const
    {
        if( ( ( _rows != rhs._rows ) or ( _cols != rhs._cols ) ) )
            throw DimensionMismatch( _rows, _cols, rhs._rows, rhs._cols );
    }

    inline void copy( std::unique_ptr<value_type[]> &left, const std::unique_ptr<value_type[]> &right )
    {
        for( size_type i = 0; i < _rows; ++i )
            for( size_type j = 0; j < _cols; ++j )
                left[iter( i, j )] = right[iter( i, j )];
    }

public:
    explicit Matrix( const size_t rows = 0, const size_t cols = 0 ) : _rows(rows), _cols(cols)
    {
        _matrix = std::make_unique<value_type[]>( ( _rows * _cols ) );
    }

    Matrix( const Matrix &rhs ) : _rows(rhs._rows), _cols(rhs._cols)
    {
        _matrix = std::make_unique<value_type[]>( ( _rows * _cols ) );
        copy( _matrix, rhs._matrix );
    }

    Matrix( Matrix &&rhs ) : _rows(rhs._rows), _cols(rhs._cols)
    {
        _matrix = std::move(rhs._matrix);
        rhs._rows = 0;
        rhs._cols = 0;
    }

    Matrix &operator=(const Matrix &rhs)
    {
        if( *this != rhs )
        {
            _matrix.reset();
            _rows = rhs._rows;
            _cols = rhs._cols;
            _matrix = std::make_unique<value_type[]>( ( _rows * _cols ) );
            copy( _matrix, rhs._matrix );
        }
        return *this;
    }

    Matrix &operator=( Matrix &&rhs )
    {
        if( *this != rhs )
        {
            _rows = rhs._rows;
            _cols = rhs._cols;
            _matrix.reset();
            _matrix = std::move(rhs._matrix); 
        }
        if(rhs._matrix)
            rhs._matrix.reset();
        rhs._rows = 0;
        rhs._cols = 0;
        return *this;
    }

    ~Matrix() { _matrix.reset(); }

    friend std::ostream& operator<<( std::ostream &os, const Matrix &matrix );

    inline size_type rows() const noexcept { return _rows; }
    inline size_type cols() const noexcept { return _cols; }

    inline bool empty() const noexcept { return ( ( _cols == 0 ) or ( _rows == 0 ) ); }

    reference operator()(size_type i, size_type j) {
        check( i, j );
        return (reference)_matrix[iter( i, j )];
    }

    const_reference operator()( size_type i, size_type j ) const
    {
        check( i, j );
        return (const_reference)_matrix[iter( i, j )];
    }

    bool operator==( const Matrix &rhs ) const
    {
        if( ( ( _rows != rhs._rows ) or ( _cols != rhs._cols ) ) )
            return false;
        for( size_type i = 0, I = ( _rows * _cols ); i < I; ++i )
            if( _matrix[i] != rhs._matrix[i] )
                return false;
        return true;
    }

    bool operator!=( const Matrix &rhs ) const { return !( *this == rhs ); }

    Matrix operator+( const Matrix &rhs ) const
    {
        checkOperations(rhs);

        Matrix new_matrix( _rows, _cols );
        for( size_t i = 0, size = ( _rows * _cols ); i < size; ++i )
            new_matrix._matrix[i] = ( _matrix[i] + rhs._matrix[i] );

        return new_matrix;
    }

    Matrix operator-( const Matrix &rhs ) const
    {
        checkOperations(rhs);

        Matrix new_matrix( _rows, _cols );
        for( size_t i = 0, size = ( _rows * _cols ); i < size; ++i )
            new_matrix._matrix[i] = ( _matrix[i] - rhs._matrix[i] );

        return new_matrix;
    }

};

std::ostream& operator<<( std::ostream &os, const Matrix &matrix )
{
    const size_t SIZE = ( ( matrix._cols - 1 ) + ( ( matrix._rows - 1 ) * matrix._cols ) );
    for( size_t i = 0; i < matrix._rows; ++i )
    {
        for( size_t j = 0; j < matrix._cols; ++j )
        {
            if constexpr( std::is_same_v<Matrix::value_type, double> )
            {
                // если без этого выводить double, то будет не красивый вывод в некоторых значениях
                os << std::setprecision(std::numeric_limits<double>::max_digits10)
                   << matrix._matrix[matrix.iter( i, j )];
            }
            else
            {
                os << matrix._matrix[matrix.iter( i, j )];
            }
            if( matrix.iter( i, j ) != SIZE )
                os << ' ';
        }
    }
    return os;
}

#endif  // MATRIXOPERATIONS_H_
