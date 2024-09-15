/**
 *@author Matouš Kovář <kovarm46@fit.cvut.cz>
 *@date 6/14/23
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

class CTeleport
{
    // todo

public:

    // ctor
    // dtor

    CTeleport & Add ( const string & from,
                      const string & to,
                      unsigned fromTime,
                      unsigned toTime )
    {
        m_Connections[from].insert(CConnection({to,fromTime,toTime}));
        return *this;
    }

    CTeleport & Optimize ( void )
    {
        return *this;
    }

    unsigned FindWay ( const string & from,
                       const string & to,
                       unsigned time )
    {
        queue<pair <string, unsigned> > q;
        map<string, unsigned> visited; // kde jsme byli a v jakej cas

        q.push({from, time});

        while(!q.empty())
        {
            auto current = q.front();
            q.pop();

            for (const auto & a : m_Connections[current.first])
            {
                if (a.m_FromTime >= current.second && (visited.find(a.m_To) == visited.end() || visited[a.m_To] > a.m_ToTime) )
                {
                    q.push({a.m_To,a.m_ToTime});
                    visited[a.m_To] = a.m_ToTime;
                }
            }
        }
        if ( visited.find(to) == visited.end())
            throw invalid_argument("");
        return visited[to];
    }
private:
    struct CConnection{
        string m_To;
        unsigned m_FromTime;
        unsigned m_ToTime;
        bool operator < (const CConnection & other) const
        {
            return tie(m_To,m_FromTime,m_ToTime) < tie(other.m_To,other.m_FromTime,other.m_ToTime);
        }
    };
    map<string, set <CConnection>  > m_Connections;
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

