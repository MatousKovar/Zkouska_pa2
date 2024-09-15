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

template<typename T_>
class CDiagonalMatrix {
public:
    // constructor ( n, k )

    //je potreba vektor nejak naplnit - nejdirve naplnime hlavni diagonalu - to jee vektor o velikosti n
    // hlavni diagonalni matice je jasna
    // vedlejsi jsou vektory vektoru, vektory vevnitr se zmensuji ve velikosti s tim jak se oddaluji od stredu
    CDiagonalMatrix(int n, int k)
            : n(n),
              k(k) {
        //tohle vytvori matici, vektor ktery bude mit n prvku
        for (size_t i = 0; i < n; i++)
            m_Diagonal.push_back({});
        //tohle bude vytvaret vektory
        for (size_t i = 1; i < k; i++) {
            int diagonalSize = n-i;
            vector<T_> leftDiagonal(diagonalSize);
            vector<T_> rightDiagonal (diagonalSize);
            m_LeftDiagonal.push_back(leftDiagonal);
            m_RightDiagonal.push_back(rightDiagonal);
        }
    }

    // destructor, copy cons, operator = (if needed)
    // operator () ( row, col )
    T_ & operator()(int row, int col) {
        if ( row > n || col > n)
            throw out_of_range("");
        //prvek je na diagonale
        if (row == col) {
            return m_Diagonal[row];
        }
            //prvek je na prave diagonale
        else if (row < col) {
            size_t indexOfVector = abs(row - col ) - 1;
            return m_RightDiagonal[indexOfVector][col - indexOfVector - 1];
        }
            //prvek je na leve diagonale
        else {
            size_t indexOfVector = abs(col - row) - 1;
            return m_LeftDiagonal[indexOfVector][col];
        }
    }

    // exists ( row, col ) const
bool exists(int row, int col) const
{
    // Check if the indices fall within the valid range
    if (row >= 0 && row < n && col >= 0 && col < n) {
        // Check if the element is on the main diagonal
        if (row == col)
            return true;
            // Check if the element is on the right diagonal
        else if (row < col) {
            size_t indexOfVector = col - row - 1;
            return indexOfVector < m_RightDiagonal.size() && row < m_RightDiagonal[indexOfVector].size();
        }
            // Check if the element is on the left diagonal
        else {
            size_t indexOfVector = row - col - 1;
            return indexOfVector < m_LeftDiagonal.size() && col < m_LeftDiagonal[indexOfVector].size();
        }
    }
    return false; // Indices are out of range
}
    }
    // reshape ( k )
    void reshape (int k)
    {

    }

private:
    int n;
    int k;
    vector<T_> m_Diagonal;
    vector<vector<T_>> m_LeftDiagonal;
    vector<vector<T_>> m_RightDiagonal;
};

#ifndef __PROGTEST__

class CTestCnt {
public:
    CTestCnt(int val = 0)
            : m_Val(val) {
        c_Inst++;
    }

    CTestCnt(const CTestCnt & src) noexcept
            : m_Val(src.m_Val) {
        c_Inst++;
    }

    CTestCnt(CTestCnt && src) noexcept
            : m_Val(src.m_Val) {
        c_Inst++;
    }

    ~CTestCnt(void) noexcept {
        c_Inst--;
    }

    CTestCnt & operator=(CTestCnt src) noexcept {
        swap(src.m_Val, m_Val);
        return *this;
    }

    inline static size_t c_Inst = 0;
private:
    int m_Val;
};

int main(void) {
    CDiagonalMatrix<int> m1(5, 4);

    for (int i = 0; i < 5; i++)
        m1(i, i) = 10 + i;
    for (int i = 0; i < 4; i++) {
        m1(i, i + 1) = 20 + i;
        m1(i + 1, i) = 30 + i;
    }
    for (int i = 0; i < 3; i++) {
        m1(i, i + 2) = 40 + i;
        m1(i + 2, i) = 50 + i;
    }


    for (int i = 0; i < 4; i++) {
        assert (m1(i, i + 1) == 20 + i);
        assert (m1(i + 1, i) == 30 + i);
    }

    m1(0, 3) = 60;
    m1(1, 4) = 61;
    m1(3, 0) = 70;
    m1(4, 1) = 71;

    CDiagonalMatrix<int> m2(m1);

    for (int i = 0; i < 5; i++)
        assert (m2(i, i) == 10 + i);
    for (int i = 0; i < 4; i++) {
        assert (m2(i, i + 1) == 20 + i);
        assert (m2(i + 1, i) == 30 + i);
    }

    //az sem jsem prosel, hura
    for (int i = 0; i < 3; i++) {
        assert (m2(i, i + 2) == 40 + i);
        assert (m2(i + 2, i) == 50 + i);
    }
    assert (m1(0, 3) == 60);
    assert (m2(0, 3) == 60);
    assert (m2(1, 4) == 61);
    assert (m2(3, 0) == 70);
    assert (m2(4, 1) == 71);


    m2(0, 0) = 100;
    assert (m1(0, 0) == 10);
    assert (m2(0, 0) == 100);


    assert (m1.exists(4, 1) && m1(4, 1) == 71);
    m1.reshape(2);
    assert (m1.exists(0, 1) && m1(0, 1) == 20);
    assert (!m1.exists(0, 2));
    try {
        m1(0, 0) = m1(0, 2);
        assert ("Missing an exception" == nullptr);
    }
    catch (const std::out_of_range & e) {
        assert (e.what() == "invalid index ( 0, 2 )"s);
    }


    m1.reshape(4);
    assert (m1.exists(0, 1) && m1(0, 1) == 20);
    assert (m1.exists(0, 2) && m1(0, 2) == 0);

    try {
        m1.reshape(6);
        assert ("Missing an exception" == nullptr);
    }
    catch (const std::out_of_range & e) {
        assert (e.what() == "invalid matrix size"s);
    }

    try {
        CDiagonalMatrix<int> m3(5, 6);
        assert ("Missing an exception" == nullptr);
    }
    catch (const std::out_of_range & e) {
        assert (e.what() == "invalid matrix size"s);
    }


    CDiagonalMatrix<CTestCnt> m10(12, 1);
    assert (CTestCnt::c_Inst == 12);
    m10.reshape(4);
    assert (CTestCnt::c_Inst == 72);
    m10.reshape(2);
    assert (CTestCnt::c_Inst == 34);
    m10.reshape(11);
    assert (CTestCnt::c_Inst == 142);
    m10.reshape(12);
    assert (CTestCnt::c_Inst == 144);
    m10.reshape(8);
    assert (CTestCnt::c_Inst == 124);

    return EXIT_SUCCESS;
}

#endif /* __PROGTEST__ */