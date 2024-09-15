#include<iostream>
#include<cassert>
#include<vector>
#include<set>
#include<map>
#include<string>
#include<unordered_map>
#include<memory>
#include<unordered_set>
#include<queue>
#include<list>
#include<sstream>
#include<cstring>

using namespace std;

class CPkg{
public:
    //TODO constructor
    CPkg () =default;
    CPkg(const string & name) : m_Name(name) {}
    //TODO addDep(string)
    CPkg & addDep (string dep)
    {
        m_Deps.insert(dep);
        return *this;
    }
    bool operator < (const CPkg & other) const
    {
        return m_Name < other.m_Name;
    }
    set <string> GetDeps () const
    {
        return m_Deps;
    }

private:
    string m_Name;
    set<string> m_Deps;
};
class CPkgSys{
public:
    CPkgSys & addPkg (const CPkg & pkg)
    {
        m_Available.insert(pkg);
        return *this;
    }
    //TODO addPkg
    //TODO install(list<string> )

    set<string> install ( list<string> packages)
    {
        set<string> res;
        queue<string> q;
        set<string> visited;

        for (const auto & a : packages) {
            if (m_Available.find(a) == m_Available.end()) {
                //todo chyba
            }
            if(m_Installed.find(a) == m_Installed.end())
                res.insert(a);
            m_Installed.insert(* m_Available.find(a));
            q.push(a);
        }

        while(!q.empty())
        {
            auto current = q.front();
            q.pop();
            visited.insert(current);
            for (const auto & a : m_Available.find(CPkg(current))->GetDeps())
            {
                if (m_Available.find(a) == m_Available.end())
                    throw invalid_argument("");
                if(visited.find(a) == visited.end()){
                    q.push(a);
                    if (m_Installed.find(a) == m_Installed.end())
                    {
                        m_Installed.insert(*m_Available.find(a));
                        res.insert(a);
                        visited.insert(a);
                    }
                }
            }
        }
        return res;


    }
    //TODO operator <<
    friend ostream & operator << (ostream & os , const CPkgSys & a)
    {
        bool first = true;
        for (const auto & c : a.m_Installed)
        {
            for (const auto & b : c.GetDeps() )
            {
                if ( ! first)
                    os << ", ";
                os << b;
            }
        }
        return os;
    }
private:
    set<CPkg> m_Available;
    set<CPkg> m_Installed;
};

int main(void){
    CPkgSys s;
    stringstream ss;
    s.addPkg(CPkg("ssh").addDep("sudo").addDep("apt"))
            .addPkg(CPkg("sudo").addDep("git").addDep("c++"));
    s.addPkg(CPkg("apt"))
            .addPkg(CPkg("c++").addDep("c").addDep("asm").addDep("fortran"));
    s.addPkg(CPkg("git"))
            .addPkg(CPkg("c").addDep("kekw"))
            .addPkg(CPkg("kekw"))
            .addPkg(CPkg("asm"))
            .addPkg(CPkg("fortran"));
    s.addPkg(CPkg("python").addDep("bash").addDep("sadge"))
            .addPkg(CPkg("karel").addDep("python"))
            .addPkg(CPkg("bash").addDep("sadge"))
            .addPkg(CPkg("sadge"))
            .addPkg(CPkg("cython").addDep("dev"));
    s.addPkg(CPkg("perl"));

    ss << s;
    assert(ss.str() == "");
    ss.clear();
    ss.str("");

    set<string> t1 = s.install(list<string> {"sudo"});
    assert(t1 == (set<string> {"asm", "c", "c++", "fortran", "git", "kekw", "sudo"}));
    set<string> t2 = s.install(list<string> {"ssh", "c++"});
    assert(t2 == (set<string> {"apt", "ssh"}));

//    ss << s;
//    assert(ss.str() == "apt, asm, c, c++, fortran, git, kekw, ssh, sudo");
//    ss.clear();
//    ss.str("");

    try{
        set<string> e = s.install(list<string> {"karel", "cython"});
        assert("Sem ses nemel dostat debilku" == nullptr);
    }
    catch(const invalid_argument & e){
        assert(strcmp("Package not found.", e.what()) == 0);
    }
    set<string> t3 = s.install(list<string> {"karel", "fortran", "git"});
    assert(t3 == (set<string> {"bash", "karel", "python", "sadge"}));

    s.addPkg(CPkg("java").addDep("utils"))
            .addPkg(CPkg("utils").addDep("VB"))
            .addPkg(CPkg("VB").addDep("java"));

    set<string> t4 = s.install(list<string> {"java", "perl"});
    assert(t4 == (set<string> {"VB", "java", "perl", "utils"}));

    ss << s;
    assert(ss.str() == "VB, apt, asm, bash, c, c++, fortran, git, java, karel, kekw, perl, python, sadge, ssh, sudo, utils");
    ss.clear();
    ss.str("");

    CPkgSys k;
    k.addPkg(CPkg("ssh").addDep("sudo").addDep("apt"))
            .addPkg(CPkg("sudo").addDep("git"));
    k.addPkg(CPkg("apt"));
    k.addPkg(CPkg("git"))
            .addPkg(CPkg("c").addDep("kekw"))
            .addPkg(CPkg("kekw"))
            .addPkg(CPkg("asm"))
            .addPkg(CPkg("fortran"));
    k.addPkg(CPkg("python").addDep("bash").addDep("sadge"))
            .addPkg(CPkg("karel").addDep("python"))
            .addPkg(CPkg("bash").addDep("sadge"))
            .addPkg(CPkg("sadge"));
    k.addPkg(CPkg("perl").addDep("no"));

    set<string> t5 = k.install(list<string> {"asm"});
    assert(t5 == (set<string> {"asm"}));
    set<string> t6 = k.install(list<string> {"python", "ssh"});
    assert(t6 == (set<string> {"apt", "bash", "git", "python", "sadge", "ssh", "sudo"}));

    try{
        set<string> t7 = k.install(list<string> {"perl", "c"});
        assert("Sem ses nemel dostat debilku" == nullptr);
    }
    catch(const invalid_argument & e){
        assert(strcmp("Package not found.", e.what()) == 0);
    }
    set<string> t8 = k.install(list<string> {"c", "ssh", "karel"});

    assert(t8 == (set<string> {"c", "karel", "kekw"}));

    ss << k;
    assert(ss.str() == "apt, asm, bash, c, git, karel, kekw, python, sadge, ssh, sudo");
    ss.clear();
    ss.str("");

}

