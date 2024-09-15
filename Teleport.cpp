/**
 *@author Matouš Kovář <kovarm46@fit.cvut.cz>
 *@date 6/13/23
 *
 * https://www.fit-wiki.cz/%C5%A1kola/p%C5%99edm%C4%9Bty/bi-pa2/pa2_zkou%C5%A1ka_2023-05-25
 */
#ifndef __PROGTEST__
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cctype>
#include <cassert>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <stdexcept>
#include <string>
#include <list>
#include <vector>
#include <stack>
#include <queue>
#include <deque>
#include <map>
#include <unordered_map>
#include <set>
#include <unordered_set>
#include <algorithm>
#include <memory>
using namespace std;
#endif /* __PROGTEST__ */


//Jo tohle zvladnes, nejhorsi je rozmyslet jak ulozis ty data

class CTeleport
{

public:

    CTeleport & Add ( const string & from,
                      const string & to,
                      unsigned fromTime,
                      unsigned toTime )
    {
        m_Connections[from].insert({to,fromTime,toTime});
        return *this;
    }

    CTeleport & Optimize ( void )
    {
        return *this;
    }

    //Takze mame from, a chceme nejrychlejsi cestu z from do to
    //bfs potrebuje frontu - z ceho?
    //
    unsigned FindWay ( const string & from,
                       const string & to,
                       unsigned time )
    {
        unsigned result;
        //chci mit frontu mista a casu, kdy jsem tam byl
        //udrzovat to chci v mape, protoze muzu jednodusse kontrolovat jestli je cas mensi nebo vetsi
        queue<pair<string, int> > q;
        map < string , int > visited;

        q.push({from,time}); // prvni misto, kde jsem v tento cas

        while ( ! q.empty())
        {
            auto v = q.front();
            q.pop();
            //ted koukam na sousedy
            for ( const auto & a : m_Connections[v.first])
            {
                //cas a musi byt logicky vetsi, nez cas toho co jsem ted vybral z fronty
                if (a.m_From >= v.second &&  (visited.count(a.m_Name) == 0 || visited[a.m_Name] > a.m_To))
                {
                    q.push({a.m_Name,a.m_To});
                    visited[a.m_Name] = a.m_To;
                }
            }
        }
        if (visited.count(to) == 0 )
            throw invalid_argument("");
        return visited[to];
    }
private:
    struct Connection{
        string m_Name;
        unsigned m_From;
        unsigned m_To;
        Connection(const string & to, const unsigned from,const unsigned toTime) : m_Name(to), m_From(from), m_To(toTime) {}
        bool operator < (const Connection & other) const
        {
            return tie(m_Name, m_From,m_To) < tie( other.m_Name, other.m_From, other.m_To);
        }
    };
    //pro kazde mesto bude existovat set vsech moznyc spoju z nej
    map < string, set<Connection> > m_Connections;
};

#ifndef __PROGTEST__
int main ( void )
{
    CTeleport t;
    t . Add ( "Prague", "Vienna", 0, 7 )
            . Add ( "Vienna", "Berlin", 9, 260 )
            . Add ( "Vienna", "London", 8, 120 )
            . Add ( "Vienna", "Chicago", 4, 3 )
            . Add ( "Prague", "Vienna", 10, 10 ) . Optimize ( );

    assert ( t . FindWay ( "Prague", "Vienna", 0 ) == 7 );
    assert ( t . FindWay ( "Prague", "Vienna", 1 ) == 10 );
    assert ( t . FindWay ( "Prague", "London", 0 ) == 120 );
    assert ( t . FindWay ( "Vienna", "Chicago", 4 ) == 3 );

    try { t . FindWay ( "Prague", "London", 2 ); assert ( "Missing exception" == nullptr ); }
    catch ( const invalid_argument & e ) { }
    catch ( ... ) { assert ( "Invalid exception" == nullptr ); }
    try { t . FindWay ( "Prague", "Chicago", 0 ); assert ( "Missing exception" == nullptr ); }
    catch ( const invalid_argument & e ) { }
    catch ( ... ) { assert ( "Invalid exception" == nullptr ); }

    t . Add ( "Dallas", "Atlanta", 150, 30 )
            . Add ( "Berlin", "Helsinki", 1080, 2560 )
            . Add ( "Chicago", "Frankfurt", 50, 0 )
            . Add ( "Helsinki", "Vienna", 3200, 3 )
            . Add ( "Chicago", "London", 10, 12 )
            . Add ( "London", "Atlanta", 20, 40 )
            . Add ( "Vienna", "Atlanta", 10, 50 )
            . Add ( "Prague", "Vienna", 1, 6 )
            . Add ( "Berlin", "Helsinki", 265, 265 )
            . Add ( "Berlin", "London", 259, 0 ) . Optimize ( );

    assert ( t . FindWay ( "Prague", "Frankfurt", 0 ) == 0 );
    assert ( t . FindWay ( "Prague", "Atlanta", 0 ) == 40 );
    assert ( t . FindWay ( "Prague", "Atlanta", 10 ) == 50 );

    return EXIT_SUCCESS;
}
#endif /* __PROGTEST__ */

