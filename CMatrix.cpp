/**
 *@author Matouš Kovář <kovarm46@fit.cvut.cz>
 *@date 6/13/23
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

using namespace std;
#endif /* __PROGTEST__ */


/**
 * Poznamky
 *  Operator = udelat pres swapy, vracet referenci na this
 *  Pretizit operator na [] s referenci i bez
 *  Kdyz chci vnorovat, tak je potreba udelat i konstruktor na inicializaci nejake hodnoty, jinak me to nepusti
 * @tparam T_
 * @param lhs
 * @param rhs
 */

template<typename T_>
void Swap(T_ & lhs, T_ & rhs) {
    T_ tmp = lhs;
    lhs = rhs;
    rhs = tmp;
}

template<typename T_>
class CVector {
public:
    CVector ()
    {
        m_Capacity = m_Size = 5;
        m_Data = new T_ [m_Capacity];
    }
    CVector (size_t size)
    {
        m_Size = m_Capacity = size;
        m_Data = new T_ [size];
    }
    CVector (unsigned size, const T_ & initData)
    {
        m_Capacity = m_Size = size;
        m_Data = new T_ [size];
        for (size_t i = 0 ; i < m_Size ; i++ ) m_Data[i] = initData;
    }
    ~CVector() {delete [] m_Data; }

    CVector(const CVector<T_> & other )
    {
        m_Size = other.m_Size;
        m_Capacity = other.m_Capacity;
        m_Data = new T_ [m_Capacity];
        for ( size_t i = 0 ; i < m_Capacity ; i++)
            m_Data[i] = other.m_Data[i];
    }
    CVector & operator = (CVector<T_> other)
    {
        Swap(m_Size, other.m_Size);
        Swap (m_Capacity,other.m_Capacity);
        Swap(m_Data,other.m_Data);
        return *this;
    }
    T_ & operator[] (size_t index)
    {
        if(index >= m_Size)
            throw out_of_range("");
        return m_Data[index];
    }

    T_  operator[] (size_t index) const
    {
        if(index >= m_Size)
            throw out_of_range("");
        return m_Data[index];
    }

private:
    T_ * m_Data;
    size_t m_Size;
    size_t m_Capacity;
};


template<typename T_>
class CMatrix3 {
public:
    CMatrix3(unsigned n1, unsigned n2, unsigned n3)
    {
        m_Data = CVector(n1,CVector(n2,CVector<T_>(n3)));
    }
    CVector<CVector <T_>> & operator[] (size_t index)
    {
        return m_Data[index];
    }

    CVector<CVector <CVector<T_>>> m_Data;
};

#ifndef __PROGTEST__

int main(void) {
    CMatrix3<int> m(3, 4, 5);
    m[0][0][0] = 128;
    m[2][3][1] = 7;
    assert (m[0][0][0] == 128);
    assert (m[2][3][1] == 7);

    m[1][1][1] = m[0][0][0] + m[2][3][1];
    m[2][3][4] = 135;
    assert (m[1][1][1] == m[2][3][4]);

    CMatrix3<int> copy(m);
    try {
        copy[4][1][2];
        assert ("Missing exception for access i0 = 4, limit = 2" == nullptr);
    }
    catch (out_of_range & e) {}
    catch (...) { assert ("Incorrect exception thrown" == nullptr); }
    try {
        copy[1][4][2];
        assert ("Missing exception for access i1 = 4, limit = 3" == nullptr);
    }
    catch (out_of_range & e) {}
    catch (...) { assert ("Incorrect exception thrown" == nullptr); }
    try {
        copy[2][1][6];
        assert ("Missing exception for access i2 = 6, limit = 4" == nullptr);
    }
    catch (out_of_range & e) {}
    catch (...) { assert ("Incorrect exception thrown" == nullptr); }

    copy[2][3][1] = 42;
    assert (m[2][3][1] == 7);
    assert (copy[2][3][1] == 42);

    CMatrix3<int> n(2, 2, 2);
    n[0][1][0] = 56;
    n[1][0][1] = 42;
    m = n;
    n[0][1][0] = 0;
    assert (m[0][1][0] == 56);
    try {
        m[2][1][0];
        assert ("Missing exception for access i0 = 2, limit = 2" == nullptr);
    }
    catch (out_of_range & e) {}
    catch (...) { assert ("Incorrect exception thrown" == nullptr); }

    CMatrix3<string> s(2, 1, 1);
    s[0][0][0] = "Hello ";
    s[1][0][0] = "World!";
    assert ((s[0][0][0] + s[1][0][0]) == string("Hello World!"));

    return EXIT_SUCCESS;
}

#endif /* __PROGTEST__ */

