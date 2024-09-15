/**
 *@author Matouš Kovář <kovarm46@fit.cvut.cz>
 *@date 6/13/23
 * https://www.fit-wiki.cz/%C5%A1kola/p%C5%99edm%C4%9Bty/bi-pa2/pa2_zkou%C5%A1ka_2023-06-01
 */

#ifndef __PROGTEST__

#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cctype>
#include <string>
#include <iostream>
#include <stdexcept>
#include <list>
#include <vector>
#include <algorithm>

using namespace std;
#endif /* __PROGTEST__ */


/**
 * Poznamky
 * Delej si pomocny struktury, fakt jo prosim
 * Je zajimavy jak tady funguje ten operator ()
 *
 * @tparam T_
 */


template<typename T_>
class CSparseMatrix {
public:
    CSparseMatrix (unsigned rows, unsigned columns)
    {
        m_Rows = rows;
        m_Columns = columns;
    }

    bool contains (size_t x, size_t y)
    {
        auto iter = lower_bound(m_Data.begin(), m_Data.end(), Cords(x,y));
        if (iter == m_Data.end())
            return false;
        else
            return iter->m_X == x && iter->m_Y == y;
    }

    T_ & operator () (size_t rows, size_t columns)
    {
        if ( rows >= m_Rows || columns >= m_Columns )
            throw out_of_range("Index error");
        if (!contains(rows,columns))
            return m_Data.insert(lower_bound(m_Data.begin(), m_Data.end(),Cords(rows,columns)),Cords(rows,columns))->m_Val;
        else {
            auto iter = lower_bound(m_Data.begin(), m_Data.end(), Cords(rows, columns));
            return iter->m_Val;
        }
    }

    // destructor, operator =, copy constructor if needed
    // operator ( row, col )
    // contains ( row, col )
private:

    struct Cords {
        Cords(unsigned x, unsigned y, T_ val) : m_X(x), m_Y(y), m_Val(val) {}

        Cords(unsigned x, unsigned y) : m_X(x), m_Y(y){}

        size_t m_X;
        size_t m_Y;
        T_ m_Val;

        bool operator<(const Cords & other) const {
            if (m_X != other.m_X)
                return m_X < other.m_X;
            return m_Y < other.m_Y;
        }
    };

    vector<Cords> m_Data;
    unsigned m_Rows;
    unsigned m_Columns;
};

#ifndef __PROGTEST__

int main(void) {
    CSparseMatrix<int> m1(300, 700);

    assert (!m1.contains(0, 0));
    m1(0, 0) = 123;
    m1(1, 1) = 321;
    m1(2, 2) = m1(1, 1) + m1(0, 0);
    assert (m1.contains(0, 0) && m1(0, 0) == 123);
    assert (m1.contains(1, 1) && m1(1, 1) == 321);
    assert (m1.contains(2, 2) && m1(2, 2) == 444);

    for (int i = 0; i < 300; ++i)
        for (int j = 0; j < 300; ++j)
            m1(i, j) = i * j;

    for (int i = 0; i < 300; ++i)
        for (int j = 0; j < 300; ++j)
            assert (m1.contains(i, j) && m1(i, j) == i * j);

    try {
        m1(300, 300) = 666;
        assert (false);
    }
    catch (out_of_range & o) {
        assert (o.what() == "Index error"s);
    }
    catch (...) {
        assert (false);
    }

    try {
        m1(299, 300) = 322;
    }
    catch (...) {
        assert (false);
    }

    CSparseMatrix m2 = m1;

    for (int i = 0; i < 300; ++i)
        for (int j = 0; j < 300; ++j)
            assert (m2.contains(i, j) && m2(i, j) == i * j);
    m1(0, 0) = 15;

    assert (m2(0, 0) == 0);
    assert (m1(0, 0) == 15);

    CSparseMatrix<int> m3(1000, 1000);
    assert (!m3.contains(666, 666));
    m3(666, 666) = 666;
    assert (m3.contains(666, 666) && m3(666, 666) == 666);
    m3 = m1;
    assert (!m3.contains(666, 666));
    assert (m3.contains(0, 0) && m3(0, 0) == 15);

    for (int i = 1; i < 300; ++i)
        for (int j = 1; j < 300; ++j)
            assert (m3.contains(i, j) && m3(i, j) == i * j);

    CSparseMatrix<string> m4(1000000, 1000000);
    assert (!m4.contains(0, 0));
    assert (!m4.contains(999999, 999999));
    m4(0, 0) = "P";
    m4(10, 40) = "r";
    m4(55, 50000) = "o";
    m4(400000, 11) = "g";
    m4(999999, 1221) = "t";
    m4(121, 169) = "e";
    m4(66, 6) = "s";
    m4(6, 66) = "t";
    assert (m4.contains(0, 0) && m4(0, 0) == "P"s);
    assert (m4.contains(10, 40) && m4(10, 40) == "r"s);
    assert (m4.contains(55, 50000) && m4(55, 50000) == "o"s);
    assert (m4.contains(400000, 11) && m4(400000, 11) == "g"s);
    assert (m4.contains(999999, 1221) && m4(999999, 1221) == "t"s);
    assert (m4.contains(121, 169) && m4(121, 169) == "e"s);
    assert (m4.contains(66, 6) && m4(66, 6) == "s"s);
    assert (m4.contains(6, 66) && m4(6, 66) == "t"s);
    return EXIT_SUCCESS;
}

#endif /* __PROGTEST__ */

