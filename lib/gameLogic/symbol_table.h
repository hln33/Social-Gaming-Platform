#pragma once

#include <unordered_map>
#include <list>
#include <utility>

class VarInfo {
public:
    std::string_view ident;
    int type;
    /**
     * 
     * Types:
     * - int
     * - string
     * - bool
     * - list (maybe??)
     * 
     * so far...
     * 
    */
    int value;
};


using Scope = std::unordered_map<std::string_view, VarInfo>;
using ScopeStack = std::list<Scope>;

class ScopeTable {
private:
    ScopeStack symtbl;

public:
    ~ScopeTable();

    Scope::iterator findSymbol(std::string_view);

    void pushScope();

    void popScope();

    bool insertSymbol(VarInfo&);

    bool not_in_current_scope(std::string_view);
};


class SymbolTable {
private:

public:
    ScopeTable temp;
    ScopeTable hidden;
    ScopeTable skct;
    ScopeTable funcs;
};
