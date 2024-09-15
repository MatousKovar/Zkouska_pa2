/**
 *@author Matouš Kovář <kovarm46@fit.cvut.cz>
 *@date 6/14/23
 */

#ifndef __PROGTEST__
#include <cassert>
#include <cmath>
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <list>
#include <forward_list>
#include <algorithm>
#include <memory>
#include <stdexcept>
using namespace std;
#endif /* __PROGTEST__ */

template <typename T_>
class CDiagonalMatrix
{
public:

    CDiagonalMatrix(int n, int k)
    {
        if ( k > n)
            throw out_of_range("invalid matrix size");
        m_K = k;
        m_N = n;
        m_Diagonal = vector<T_>(n);

        for (size_t i = 1 ; i < k; i++ )
        {
            int vectorSize = n - i;
            auto leftVector = vector<T_>(vectorSize);
            auto rightVector = vector<T_>(vectorSize);
            m_LeftDiagonal.push_back(leftVector);
            m_RightDiagonal.push_back(rightVector);
        }

    }
    T_& operator () (int row, int col)
    {

        if ( ( abs(row - col ) ) >= m_K )
            throw out_of_range("invalid index ( " + to_string(row) + ", " + to_string(col) + " )" );
        if ( row == col )
            return m_Diagonal[col];
        //leva polovina
        else if ( row > col )
        {
            int vectorIndex = abs ( row - col ) -1 ;
            return m_LeftDiagonal[vectorIndex][col]
            ;
        }
        else
        {
            int vectorIndex = abs( row - col ) - 1;
            return m_RightDiagonal[vectorIndex][col - vectorIndex -1 ];
        }
    }

    bool exists ( int row, int col) const
    {
        if ( ( abs(row - col ) ) >= m_K )
            return false;
        return true;
        /*if ( row == col )
            return m_Diagonal[col];
            //leva polovina
        else if ( row > col )
        {
            int vectorIndex = abs ( row - col ) -1 ;
            return m_LeftDiagonal[vectorIndex][col]
                    ;
        }
        else
        {
            int vectorIndex = abs( row - col ) - 1;
            return m_RightDiagonal[vectorIndex][col - vectorIndex -1 ];
        }*/
    }
    void reshape(int k )
    {
        if ( k > m_N)
            throw out_of_range("invalid matrix size");
        if ( k > m_K )
        {
            int diagonalsToAdd = k - m_K;
            int sizeStart = m_N - m_LeftDiagonal.size() - 1;
            for ( size_t i = 0 ; i < diagonalsToAdd ; i++)
            {
                vector<T_> leftAdd = vector<T_>(sizeStart);
                vector<T_> rightAdd = vector<T_>(sizeStart);
                m_LeftDiagonal.push_back(leftAdd);
                m_RightDiagonal.push_back(rightAdd);
                sizeStart--;
            }
        }
        else if (k == m_K)
            return;
        else
        {
            int diagonalsToDelete = m_K - k;
            for (size_t i = 0 ; i < diagonalsToDelete ; i++)
            {
                m_LeftDiagonal.pop_back();
                m_RightDiagonal.pop_back();
            }
        }
        m_K = k;
    }

private:
    vector<T_> m_Diagonal;
    vector <vector<T_>> m_LeftDiagonal;
    vector <vector<T_>> m_RightDiagonal;
    int m_N;
    int m_K;
};

#ifndef __PROGTEST__
class CTestCnt
{
public:
    CTestCnt                                ( int                                   val = 0 )
            : m_Val ( val )
    {
        c_Inst ++;
    }
    CTestCnt                                ( const CTestCnt                      & src ) noexcept
            : m_Val ( src . m_Val )
    {
        c_Inst ++;
    }
    CTestCnt                                ( CTestCnt                           && src ) noexcept
            : m_Val ( src . m_Val )
    {
        c_Inst ++;
    }
    ~CTestCnt                               ( void ) noexcept
    {
        c_Inst --;
    }
    CTestCnt                         & operator =                              ( CTestCnt                              src ) noexcept
    {
        swap ( src . m_Val, m_Val );
        return *this;
    }
    inline static size_t               c_Inst                                  = 0;
private:
    int                                m_Val;
};

int main ( void )
{
    CDiagonalMatrix<int> m1 ( 5, 4 );

    for ( int i = 0; i < 5; i ++ )
        m1 ( i, i ) = 10 + i;
    for ( int i = 0; i < 4; i ++ )
    {
        m1 ( i, i + 1 ) = 20 + i;
        m1 ( i + 1, i ) = 30 + i;
    }
    for ( int i = 0; i < 3; i ++ )
    {
        m1 ( i, i + 2 ) = 40 + i;
        m1 ( i + 2, i ) = 50 + i;
    }
    m1 ( 0, 3 ) = 60;
    m1 ( 1, 4 ) = 61;
    m1 ( 3, 0 ) = 70;
    m1 ( 4, 1 ) = 71;

    CDiagonalMatrix<int> m2 ( m1 );

    for ( int i = 0; i < 5; i ++ )
        assert ( m2 ( i, i ) == 10 + i );
    for ( int i = 0; i < 4; i ++ )
    {
        assert ( m2 ( i, i + 1 ) == 20 + i );
        assert ( m2 ( i + 1, i ) == 30 + i );
    }
    for ( int i = 0; i < 3; i ++ )
    {
        assert ( m2 ( i, i + 2 ) == 40 + i );
        assert ( m2 ( i + 2, i ) == 50 + i );
    }
    assert ( m2 ( 0, 3 ) == 60 );
    assert ( m2 ( 1, 4 ) == 61 );
    assert ( m2 ( 3, 0 ) == 70 );
    assert ( m2 ( 4, 1 ) == 71 );


    m2 ( 0, 0 ) = 100;
    assert ( m1 ( 0, 0 ) == 10 );
    assert ( m2 ( 0, 0 ) == 100 );


    assert ( m1 . exists ( 4, 1 ) && m1 ( 4, 1 ) == 71 );
    m1 . reshape ( 2 );
    assert ( m1 . exists ( 0, 1 ) && m1 ( 0, 1 ) == 20 );
    assert ( ! m1 . exists ( 0, 2 ) );
    try
    {
        m1 ( 0, 0 ) = m1 ( 0, 2 );
        assert ( "Missing an exception" == nullptr );
    }
    catch ( const std::out_of_range & e )
    {
        assert ( e . what () == "invalid index ( 0, 2 )" );
    }


    m1 . reshape ( 4 );
    assert ( m1 . exists ( 0, 1 ) && m1 ( 0, 1 ) == 20 );
    assert ( m1 . exists ( 0, 2 ) && m1 ( 0, 2 ) == 0 );

    try
    {
        m1 . reshape ( 6 );
        assert ( "Missing an exception" == nullptr );
    }
    catch ( const std::out_of_range & e )
    {
        assert ( e . what () == "invalid matrix size" );
    }

    try
    {
        CDiagonalMatrix<int> m3 ( 5, 6 );
        assert ( "Missing an exception" == nullptr );
    }
    catch ( const std::out_of_range & e )
    {
        assert ( e . what () == "invalid matrix size"s );
    }


    CDiagonalMatrix<CTestCnt>  m10 ( 12, 1 );
    assert ( CTestCnt::c_Inst == 12 );
    m10 . reshape ( 4 );
    assert ( CTestCnt::c_Inst == 72 );
    m10 . reshape ( 2 );
    assert ( CTestCnt::c_Inst == 34 );
    m10 . reshape ( 11 );
    assert ( CTestCnt::c_Inst == 142 );
    m10 . reshape ( 12 );
    assert ( CTestCnt::c_Inst == 144 );
    m10 . reshape ( 8 );
    assert ( CTestCnt::c_Inst == 124 );

    return EXIT_SUCCESS;
}
#endif /* __PROGTEST__ */
