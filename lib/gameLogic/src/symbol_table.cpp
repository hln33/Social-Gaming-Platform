
#include <unordered_map>
#include <list>
#include <utility>

#include "../includes/symbol_table.h"

/**
 * 
 * variable scope class for json interpreter
 * 
 * TODO:
 * should we use pointers? yes or no?
 * for exmple should it be this:
 * using Scope = std::unordered_map<std::string_view, VarInfo>;
 * 
 * OR this:
 * using Scope = std::unordered_map<std::string_view, VarInfo *>;
 *                                                          --^--
 * 
 * 
*/


using Scope = std::unordered_map<std::string_view, VarInfo>;
using ScopeStack = std::list<Scope>;


ScopeTable::~ScopeTable() {
    while (!this->symtbl.empty()) { 
        this->popScope(); 
    }
}

Scope::iterator ScopeTable::findSymbol(std::string_view ident) {
    for (Scope& s : this->symtbl) {
        auto x = s.find(ident);
        if (x != s.end()) {
            return x;
        }
    }
    return this->symtbl.back().end();
}

void ScopeTable::pushScope() { 
    this->symtbl.push_front(Scope());
}

void ScopeTable::popScope() {
    if (!this->symtbl.empty()) {
        this->symtbl.front().clear();
        this->symtbl.pop_front();
    }
}

bool ScopeTable::insertSymbol(VarInfo& desc) {
    if (this->symtbl.empty()) {
        this->pushScope();
    }
    this->symtbl.front().insert({ desc.ident, desc }).second;
}

bool ScopeTable::not_in_current_scope(std::string_view ident) {
    if (this->symtbl.empty()) { return true; }
    return this->symtbl.front().find(ident) == this->symtbl.front().end();
}


