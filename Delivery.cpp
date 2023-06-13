/**
 *@author Matouš Kovář <kovarm46@fit.cvut.cz>
 *@date 6/12/23
 */
/**
 *
Úkolem je vytvořit třídu CDelivery, která má analyzovat pokrytí měst doručovacími sklady.
Je zadána mapa s městy a cestami mezi nimi. Každé město je jednoznačně identifikováno svým jménem a cesty mezi nimi
 jsou vždy chápány jako obousměrné (cesta A - B je přístupná ve směru A → B i B → A).

Metoda třídy poté dostane množinu měst z předem zadané mapy, které reprezentují sklady. Jako výsledek tato metoda vrátí
vector<int>, kde se vzrůstajícím indexem je vždy určen počet měst, které se nachází maximálně v této vzdálenosti od nejbližšího skladu.
Vzdálenost je rovna počtu měst, které je potřeba projít od skladu k danému městu. Na indexu 0 tedy bude
počet odpovídat vždy počtu skladů, na indexu 1 budou navíc města vzdáleny 1 od skladu a tak dále. Výsledek končí,
pokud už by dalším zvětšením vzdálenosti nepřibylo žádné další město (byla tedy dosažena všechna města, ke kterým existuje
aspoň nějaká cesta). Podrobný popis výsledku viz první test v šabloně.
Třída CDelivery má následující rozhraní:

konstruktor

        inicializuje prázdnou třídu (neexistují zatím žádné cesty).

destruktor

        Uvolní paměť.

addConn(string a, string b)

Přidá obousměrnou trasu mezi A a B. Pokud již trasa existuje, neudělá metoda nic. Navrací takovou hodnotu, aby šla dále řetězit za sebou (fluent interface, tj. trasa.addConnection(„x“, „y“).addConnection(„a“, „b“);

vector<int> findCoverage(set<string> depots)

Metoda funguje viz zadání výše a testy v šabloně.
Vzdálenost se vždy bere co nejkratší, ale můžeme začít v jakémkoliv skladě.
Pokud je v množině skladů zadáno neexistující město (takové, z/do kterého nevedou žádné cesty), vyvolá metoda výjimku typu invalid_argument (na textu výjimky nezáleží).
 */



#include <sstream>
#include <string>
#include <list>
#include <vector>
#include <stack>
#include <queue>
#include <deque>
#include <iomanip>
#include <iostream>
#include <map>
#include <set>
#include <algorithm>
#include <memory>
#include <cassert>
using namespace std;

class CDelivery
{
public:
    // ctor
    // dtor
    CDelivery & addConn ( const string & a, const string & b );
    vector<int> findCoverage ( const set<string> & depots ) const;

private:
    map <string , set<string> > m_Stops;
};

CDelivery & CDelivery::addConn ( const string & a, const string & b )
{
    m_Stops[a].insert(b);
    m_Stops[b].insert(a);
    return *this;
}

vector<int> CDelivery::findCoverage ( const set<string> & depots ) const
{
    vector<int> result;
    queue<string> toVisit;
    set<string> visited;
    //represents only number of nodes in current layer
    size_t nodesInLayer = depots.size();

    //nejdrive pridame prvni uzliky
    for( const auto & a : depots)
    {
        if(m_Stops.find(a) == m_Stops.end()) throw std::invalid_argument("");
        toVisit.push(a);
    }

    while (toVisit.size() != 0)
    {
        //zpracovat jednu vrstvu
        set<string> neighbors;
        for (size_t i = 0 ; i < nodesInLayer ; i++)
        {
            visited.insert(toVisit.front());
            for(const auto & a : m_Stops.find(toVisit.front())->second)
                    neighbors.insert(a);
            toVisit.pop();
        }
        //naplnime unikatni sousedy, kteri jeste nebyli navstiveni.
        for (const auto & a : neighbors)
            if(visited.find(a) == visited.end())
                toVisit.push(a);
        if (result.size() == 0 ) result.push_back(nodesInLayer);
        else
            result.push_back(nodesInLayer + result[result.size() - 1]);
        nodesInLayer = toVisit.size();
    }
    return result;
}

int main ()
{
    CDelivery t0;
    vector<int> r;
    t0
            .addConn("Austin", "Berlin")
            .addConn("Chicago", "Berlin")
            .addConn("Chicago", "Dallas")
            .addConn("Dallas", "Essen")
            .addConn("Essen", "Austin")
            .addConn("Frankfurt", "Essen")
            .addConn("Gyor", "Frankfurt")
            .addConn("Helsinki", "Istanbul")
            .addConn("Istanbul", "Jakarta");

    r = t0.findCoverage(set<string>{"Berlin"});
    for (const auto & a : r)
        cout << a << endl;
    assert ( r == (vector<int>{1, 3, 5, 6, 7}) );
    /* result:
     * [0]: Berlin = 1
     * [1]: Austin, Berlin, Chicago = 3
     * [2]: Austin, Berlin, Chicago, Dallas, Essen = 5
     * [3]: Austin, Berlin, Chicago, Dallas, Essen, Frankfurt = 6
     * [4]: Austin, Berlin, Chicago, Dallas, Essen, Frankfurt, Gyor = 7
     */

    r = t0.findCoverage(set<string>{"Berlin", "Essen"});
    assert ( r == (vector<int>{2, 6, 7}) );

    r = t0.findCoverage(set<string>{"Helsinki"});
    assert ( r == (vector<int>{1, 2, 3}) );

    r = t0.findCoverage(set<string>{"Istanbul"});
    assert ( r == (vector<int>{1, 3}) );

    r = t0.findCoverage(set<string>{"Austin", "Jakarta"});
    assert ( r == (vector<int>{2, 5, 9, 10}) );

    r = t0.findCoverage(set<string>{"Chicago", "Gyor", "Helsinki", "Jakarta"});
    assert ( r == (vector<int>{4, 8, 10}) );

    try
    {
        r = t0.findCoverage(set<string>{"Incorrect city"});
        assert ( "No invalid_argument exception caught!" == nullptr );
    }
    catch ( const invalid_argument & e ) {}

    return EXIT_SUCCESS;
}

