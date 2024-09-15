/**
 *@author Matouš Kovář <kovarm46@fit.cvut.cz>
 *@date 6/12/23
 */

/**
 * Implementujte metodu insert (vkládání), erase (mazání prvků
 * ze stromu a zároveň je potřeba uchovat správné pořádí podle
 * data vkládání) a vlastní destruktor, ostatní metody jsou
 * naimplentované. Kopírující konstruktor i operátor přiřazení
 * zakázán. Není povolené si přidat žádnou členskou proměnnou,
 * ale lze naimplementovat vlastní pomocné metody.
 *
 * Za správnost šablony neručím :-).
 */

#include <string>
#include <cassert>

using namespace std;

class CTree {
public:
    CTree() = default;

    ~CTree() {
        //todo
    }

    CTree(const CTree & src) = delete;

    bool operator=(const CTree & other) = delete;

    bool isSet(string key) {
        CNode *n = m_Root;
        while (n) {
            if (key == n->m_Key)
                return true;
            else if (key > n->m_Key)
                n = n->m_R;
            else
                n = n->m_L;
        }
        return false;
    }

    //Root se nemeni!
    bool insert(const string & key) {
        if (!m_First) {
            m_First = new CNode(key);
            m_Last = m_Root = m_First;
            return true;
        }

        CNode **head = &m_Root;
        while (*head) {
            //already exists
            if (key == (*head)->m_Key)
                return false;
            else if (key < (*head)->m_Key)
                //chceme menit pointer takze ne *head = (*head)->m_L!!!!
                head = &(*head)->m_L;
            else
                head = &(*head)->m_R;
        }
        //Zde uz menime hodnotu - pouzijeme *
        *head = new CNode(key);

        m_Last->m_NextOrder = *head;
        (*head)->m_PrevOrder = m_Last;
        m_Last = *head;
        return true;
    }


    bool erase(const string & key) {
        if (!m_First)
            return false;
        CNode ** head= &m_Root;
        while (*head)
        {
            if ((*head)->m_Key == key)
            {
                CNode * toDelete = *head;
                CNode::SwapOrdering(*head);
                if(m_First == *head) m_First = m_First->m_NextOrder;
                if(m_Last == *head) m_Last = m_Last->m_PrevOrder;

                if ((*head)->m_L && (*head)->m_R)
                {
                    head = &(*head)->m_R;
                    while((*head)->m_L)
                        head = &(*head)->m_L;
                    toDelete -> m_Key = (*head)->m_Key;
                    toDelete = *head;
                }
                if ( toDelete -> m_L )
                {
                    *head = toDelete->m_L;
                }
                else
                {
                    *head = toDelete->m_R;
                }
                toDelete ->m_L = toDelete->m_R = nullptr;
                delete toDelete;
                return true;
            }
            else if ( (*head)->m_Key > key)
                head = &(*head)->m_L;
            else
                head = &(*head)->m_R;
        }
        return false;
    }

protected:

    class CNode {
    public:
        CNode(string key) : m_Key(key) {}

        ~CNode() {delete m_L; delete m_R;}
        string m_Key;
        CNode *m_L = nullptr;
        CNode *m_R = nullptr;
        CNode *m_PrevOrder = nullptr;
        CNode *m_NextOrder = nullptr;

        static void SwapOrdering(CNode *head) {
            if (head->m_PrevOrder) {
                head->m_PrevOrder->m_NextOrder = head->m_NextOrder;
            }
            if (head->m_NextOrder) {
                head->m_NextOrder->m_PrevOrder = head->m_PrevOrder;
            }
        }
    };

    CNode *m_Root = nullptr;
    CNode *m_First = nullptr;
    CNode *m_Last = nullptr;
};

class CTester : public CTree {
public:
    CTester() = default;

    void test() {
        CTester t0;
        assert(t0.insert("PA1") == true);
        assert(t0.m_First->m_Key == "PA1" && t0.m_First->m_NextOrder == nullptr);
        assert(t0.isSet("PA1") == true);
        assert(t0.insert("UOS") == true);
        assert(t0.insert("PA2") == true);
        assert(t0.isSet("PA2") == true);
        assert(t0.isSet("PA3") == false);

        assert(t0.insert("PA2") == false);
        assert(t0.insert("CAO") == true);
        assert(t0.insert("LIN") == true);
        assert(t0.insert("AAG") == true);
        assert(t0.insert("AG1") == true);
        assert(t0.insert("ZDM") == true);

        assert(t0.m_First->m_Key == "PA1"
               && t0.m_First->m_NextOrder->m_Key == "UOS"
               && t0.m_First->m_NextOrder->m_NextOrder->m_Key == "PA2"
               && t0.m_First->m_NextOrder->m_NextOrder->m_NextOrder->m_Key == "CAO"
               && t0.m_First->m_NextOrder->m_NextOrder->m_NextOrder->m_NextOrder->m_Key == "LIN"
               && t0.m_First->m_NextOrder->m_NextOrder->m_NextOrder->m_NextOrder->m_NextOrder->m_Key == "AAG"
               &&
               t0.m_First->m_NextOrder->m_NextOrder->m_NextOrder->m_NextOrder->m_NextOrder->m_NextOrder->m_Key == "AG1"
               &&
               t0.m_First->m_NextOrder->m_NextOrder->m_NextOrder->m_NextOrder->m_NextOrder->m_NextOrder->m_NextOrder->m_Key ==
               "ZDM");

        assert(t0.m_Last->m_Key == "ZDM"
               && t0.m_Last->m_PrevOrder->m_Key == "AG1"
               && t0.m_Last->m_PrevOrder->m_PrevOrder->m_Key == "AAG"
               && t0.m_Last->m_PrevOrder->m_PrevOrder->m_PrevOrder->m_Key == "LIN"
               && t0.m_Last->m_PrevOrder->m_PrevOrder->m_PrevOrder->m_PrevOrder->m_Key == "CAO"
               && t0.m_Last->m_PrevOrder->m_PrevOrder->m_PrevOrder->m_PrevOrder->m_PrevOrder->m_Key == "PA2"
               &&
               t0.m_Last->m_PrevOrder->m_PrevOrder->m_PrevOrder->m_PrevOrder->m_PrevOrder->m_PrevOrder->m_Key == "UOS"
               &&
               t0.m_Last->m_PrevOrder->m_PrevOrder->m_PrevOrder->m_PrevOrder->m_PrevOrder->m_PrevOrder->m_PrevOrder->m_Key ==
               "PA1");

        assert(t0.erase("") == false);

        assert(t0.erase("ZDM") == true);
        assert(t0.m_First->m_Key == "PA1"
               && t0.m_First->m_NextOrder->m_Key == "UOS"
               && t0.m_First->m_NextOrder->m_NextOrder->m_Key == "PA2"
               && t0.m_First->m_NextOrder->m_NextOrder->m_NextOrder->m_Key == "CAO"
               && t0.m_First->m_NextOrder->m_NextOrder->m_NextOrder->m_NextOrder->m_Key == "LIN"
               && t0.m_First->m_NextOrder->m_NextOrder->m_NextOrder->m_NextOrder->m_NextOrder->m_Key == "AAG"
               && t0.m_First->m_NextOrder->m_NextOrder->m_NextOrder->m_NextOrder->m_NextOrder->m_NextOrder->m_Key ==
                  "AG1");
        assert(t0.m_Last->m_Key == "AG1"
               && t0.m_Last->m_PrevOrder->m_Key == "AAG"
               && t0.m_Last->m_PrevOrder->m_PrevOrder->m_Key == "LIN"
               && t0.m_Last->m_PrevOrder->m_PrevOrder->m_PrevOrder->m_Key == "CAO"
               && t0.m_Last->m_PrevOrder->m_PrevOrder->m_PrevOrder->m_PrevOrder->m_Key == "PA2"
               && t0.m_Last->m_PrevOrder->m_PrevOrder->m_PrevOrder->m_PrevOrder->m_PrevOrder->m_Key == "UOS"
               &&
               t0.m_Last->m_PrevOrder->m_PrevOrder->m_PrevOrder->m_PrevOrder->m_PrevOrder->m_PrevOrder->m_Key == "PA1");
        assert(t0.isSet("ZDM") == false);

        assert(t0.erase("AAG") == true);
        assert(t0.m_First->m_Key == "PA1"
               && t0.m_First->m_NextOrder->m_Key == "UOS"
               && t0.m_First->m_NextOrder->m_NextOrder->m_Key == "PA2"
               && t0.m_First->m_NextOrder->m_NextOrder->m_NextOrder->m_Key == "CAO"
               && t0.m_First->m_NextOrder->m_NextOrder->m_NextOrder->m_NextOrder->m_Key == "LIN"
               && t0.m_First->m_NextOrder->m_NextOrder->m_NextOrder->m_NextOrder->m_NextOrder->m_Key == "AG1");
        assert(t0.m_Last->m_Key == "AG1"
               && t0.m_Last->m_PrevOrder->m_Key == "LIN"
               && t0.m_Last->m_PrevOrder->m_PrevOrder->m_Key == "CAO"
               && t0.m_Last->m_PrevOrder->m_PrevOrder->m_PrevOrder->m_Key == "PA2"
               && t0.m_Last->m_PrevOrder->m_PrevOrder->m_PrevOrder->m_PrevOrder->m_Key == "UOS"
               && t0.m_Last->m_PrevOrder->m_PrevOrder->m_PrevOrder->m_PrevOrder->m_PrevOrder->m_Key == "PA1");
        assert(t0.isSet("AAG") == false);

        assert(t0.erase("CAO") == true);
        assert(t0.m_First->m_Key == "PA1"
               && t0.m_First->m_NextOrder->m_Key == "UOS"
               && t0.m_First->m_NextOrder->m_NextOrder->m_Key == "PA2"
               && t0.m_First->m_NextOrder->m_NextOrder->m_NextOrder->m_Key == "LIN"
               && t0.m_First->m_NextOrder->m_NextOrder->m_NextOrder->m_NextOrder->m_Key == "AG1");
        assert(t0.m_Last->m_Key == "AG1"
               && t0.m_Last->m_PrevOrder->m_Key == "LIN"
               && t0.m_Last->m_PrevOrder->m_PrevOrder->m_Key == "PA2"
               && t0.m_Last->m_PrevOrder->m_PrevOrder->m_PrevOrder->m_Key == "UOS"
               && t0.m_Last->m_PrevOrder->m_PrevOrder->m_PrevOrder->m_PrevOrder->m_Key == "PA1");
        assert(t0.isSet("CAO") == false);

        assert(t0.erase("UOS") == true);
        assert(t0.m_First->m_Key == "PA1"
               && t0.m_First->m_NextOrder->m_Key == "PA2"
               && t0.m_First->m_NextOrder->m_NextOrder->m_Key == "LIN"
               && t0.m_First->m_NextOrder->m_NextOrder->m_NextOrder->m_Key == "AG1");
        assert(t0.m_Last->m_Key == "AG1"
               && t0.m_Last->m_PrevOrder->m_Key == "LIN"
               && t0.m_Last->m_PrevOrder->m_PrevOrder->m_Key == "PA2"
               && t0.m_Last->m_PrevOrder->m_PrevOrder->m_PrevOrder->m_Key == "PA1");
        assert(t0.isSet("UOS") == false);

        assert(t0.erase("UOS") == false);
        assert(t0.m_First->m_Key == "PA1"
               && t0.m_First->m_NextOrder->m_Key == "PA2"
               && t0.m_First->m_NextOrder->m_NextOrder->m_Key == "LIN"
               && t0.m_First->m_NextOrder->m_NextOrder->m_NextOrder->m_Key == "AG1");
        assert(t0.m_Last->m_Key == "AG1"
               && t0.m_Last->m_PrevOrder->m_Key == "LIN"
               && t0.m_Last->m_PrevOrder->m_PrevOrder->m_Key == "PA2"
               && t0.m_Last->m_PrevOrder->m_PrevOrder->m_PrevOrder->m_Key == "PA1");
        assert(t0.isSet("UOS") == false);

        assert(t0.erase("LIN") == true);
        assert(t0.m_First->m_Key == "PA1"
               && t0.m_First->m_NextOrder->m_Key == "PA2"
               && t0.m_First->m_NextOrder->m_NextOrder->m_Key == "AG1");
        assert(t0.m_Last->m_Key == "AG1"
               && t0.m_Last->m_PrevOrder->m_Key == "PA2"
               && t0.m_Last->m_PrevOrder->m_PrevOrder->m_Key == "PA1");
        assert(t0.isSet("LIN") == false);

        assert(t0.erase("PA1") == true);
        assert(t0.m_First->m_Key == "PA2"
               && t0.m_First->m_NextOrder->m_Key == "AG1");
        assert(t0.m_Last->m_Key == "AG1"
               && t0.m_Last->m_PrevOrder->m_Key == "PA2");
        assert(t0.isSet("PA1") == false);

        assert(t0.erase("PA2") == true);
        assert(t0.m_First->m_Key == "AG1");
        assert(t0.m_Last->m_Key == "AG1");
        assert(t0.isSet("PA2") == false);

        assert(t0.erase("AG1") == true);
        assert(t0.m_First == t0.m_Last);
        assert(t0.isSet("AG1") == false);
    }
};

int main() {
    CTester t;
    t.test();

    return EXIT_SUCCESS;
}

