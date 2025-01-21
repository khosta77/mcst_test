#ifndef MATRIXPROGRAM_H_
#define MATRIXPROGRAM_H_

#include "matrixOperations.hpp"

class Operations
{
private:
    Matrix left;
    Matrix right;
    char operation;

    Matrix readmatrix( const size_t rows, const size_t cols, std::istream &in )
    {
        Matrix matrix( rows, cols );
        for( size_t i = 0; i < rows; ++i )
            for( size_t j = 0; j < cols; ++j )
                if( !( in >> matrix( i, j ) ) )
                    throw InvalidMatrixStream();
        return matrix;
    }

    void unpackstream( std::istream &in )
    {
        size_t rows = 0, cols = 0;
        if( !( in >> rows >> cols >> operation ) )
            throw InvalidMatrixStream();

        left = readmatrix( rows, cols, in );
        right = readmatrix( rows, cols, in );
    }

    void calc( std::ostream &out )
    {
        switch( operation )
        {
            case '+':
                out << ( left + right ) << std::endl; 
                break;
            case '-':
                out << ( left - right ) << std::endl;
                break;
            default:
                throw MatrixException( "unknown operation" );
        };
    }

public:
    Operations( int ) = delete;
    Operations( Operations&& ) = delete;
    Operations( const Operations& ) = delete;

    Operations &operator=( Operations&& ) = delete;
    Operations &operator=( const Operations& ) = delete;

    Operations() = default;
    ~Operations() = default;

    void operator()( std::istream& in, std::ostream& out )
    {
        unpackstream(in);
        calc(out);
    }
};

#endif // MATRIXPROGRAM_H_
