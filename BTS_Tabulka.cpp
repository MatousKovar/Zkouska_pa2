/**
 *@author Matouš Kovář <kovarm46@fit.cvut.cz>
 *@date 6/13/23
 */

/**
 * Vytvořte tabulku (mapu) za použití binárních vyhledávacích stromů bez vyvažování, klíčem i hodnotou je string. Umístění prvku
 * ve stromu se odvozuje z lexikografického řazení klíčů (operator <). Konstruktor implementovaný od Ládi, kopírující konstruktor
 * ani operátor přiřazení není nutno implementovat. Tabulka umožňuje vkládat prvky (vaše implementace) a navíc uchovávat pořadí jejich vložení.
 * K tomu slouží ukazatel m_nextOrder v instanci CNode (tvoří spojový seznam) a ukazatele m_first, m_last v CTree.

 * Ukládaná data ve třídách:

 * CNode(string m_key, string m_val, CNode* m_L, CNode* m_R, CNode* m_nextOrder);

 * CTree(CNode* m_first, CNode* m_last, CNode* m_root);

 * Potřeba implementovat:
    destruktor
    bool insert(key, val)… vloží prvek na správné místo ve stromu, vrátí true v případě úspěchu (klíč ještě není ve stromu obsažen)
    bool isSet(key)… vrátí true pro obsažený klíč
    operator «… výpis párů (klíč, hodnota) podle pořadí vložení do tabulky

 */

// Vse ok, mem leaky nejsou, zkontrolovano s valgrindem

#include<iostream>
#include<string>
#include<cassert>
#include<sstream>
using namespace std;

class CTree{
public:
    CTree() = default;
    CTree(const CTree & src) = delete;
    CTree & operator = (const CTree & src) = delete;
    ~CTree(){
        if ( !m_Root)
            return;
        CNode * head = m_First;

        while ( head )
        {
            CNode * tmp = head->m_NextOrder;
            delete head;
            head = tmp;
        }
    }
    bool isSet(const string & key){
        if(! m_Root)
            return false;
        CNode ** head = &m_Root;
        while (*head)
        {
            if((*head)->m_Key == key)
                return true;
            else if((*head)->m_Key > key)
                head = &(*head)->m_L;
            else
                head = &(*head)->m_R;
        }
        return false;
    }
    bool insert(const string & key, const string & val){
        if ( !m_Root )
        {
            m_Root = new CNode(key,val);
            m_First = m_Last = m_Root;
            return true;
        }
        CNode ** head = &m_Root;
        while (*head)
        {
            if ( (*head)->m_Key == key)
                return false;
            else if ( (*head)->m_Key > key)
                head = &(*head)->m_L;
            else
                head = &(*head)->m_R;
        }

        *head = new CNode (key, val);
        m_Last ->m_NextOrder  = *head;
        m_Last = *head;
        return true;
    }
    friend ostream & operator << (ostream & os, const CTree & src){
        os << "{";
        CNode * head = src.m_Root;
        bool first = true;
        while ( head )
        {
            if (! first )
                os<< ", ";
            first = false;
            os << head->m_Key << " => " << head->m_Val;
            head = head->m_NextOrder;
        }
        os << "}";
        return os;
    }
protected:
    class CNode{
    public:
        CNode(const string & key, const string & val)
                :m_Key(key), m_Val(val) {}
        string m_Key, m_Val;
        CNode * m_L = nullptr, * m_R = nullptr;
        CNode * m_NextOrder = nullptr;
    };
    CNode * m_Root = nullptr;
    CNode * m_First = nullptr, * m_Last = nullptr;
    friend int main();
};

int main(void){
    CTree t;
    stringstream ss;
    ss << t;
    assert(ss.str() == "{}");
    ss.clear();
    ss.str("");
    assert(t.insert("PA1", "done"));
    assert(t.m_First == t.m_Last);
    assert(t.m_First->m_Key == "PA1");
    assert(!t.isSet("UOS"));
    assert(t.insert("PA2", "fail"));
    assert(t.insert("UOS", "funny"));

    cout << t << endl;
    ss << t;
    assert(ss.str() == "{PA1 => done, PA2 => fail, UOS => funny}");
    ss.clear();
    ss.str("");


    assert(t.m_Root->m_L== nullptr);
    assert(t.m_Last->m_Key == "UOS");
    assert(t.m_Root->m_R->m_Key == "PA2");
    assert(t.m_Root->m_R->m_L == nullptr);
    assert(t.m_Root->m_R->m_R->m_Key == "UOS");
    assert(t.m_First->m_NextOrder->m_NextOrder == t.m_Last);
    assert(t.isSet("PA2"));

    assert(t.insert("CAO", "lul"));
    assert(t.insert("LIN", "F"));
    assert(t.m_Root->m_L->m_Key == "CAO");
    assert(t.m_Root->m_L->m_L == nullptr);
    assert(t.m_Root->m_L->m_R->m_Key == "LIN");
    assert(t.m_Last == t.m_Root->m_L->m_R );
    assert(t.m_Root->m_L->m_R->m_L == nullptr);
    assert(t.m_First->m_NextOrder->m_NextOrder->m_NextOrder->m_NextOrder = t.m_Root->m_L->m_R);
    assert(t.isSet("LIN"));

    ss << t;
    assert(ss.str() == "{PA1 => done, PA2 => fail, UOS => funny, CAO => lul, LIN => F}");
    ss.clear();
    ss.str("");

    assert(t.insert("SAP", "shit"));
    assert(t.m_Root->m_R->m_R->m_L->m_Key == "SAP");
    assert(t.m_Last == t.m_Root->m_R->m_R->m_L);


    ss << t;
    assert(ss.str() == "{PA1 => done, PA2 => fail, UOS => funny, CAO => lul, LIN => F, SAP => shit}");
    ss.clear();
    ss.str("");

    assert(!t.isSet("PA3"));
    assert(t.isSet("LIN"));
    assert(t.isSet("SAP"));

    return 0;
}

