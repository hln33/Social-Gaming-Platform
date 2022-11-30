#pragma once

#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <exception>

#include <iterator>
#include <any>
#include <optional>

#include <nlohmann/json.hpp>
#include <boost/variant.hpp>


#include "bliniAST.h"

// do we want to use this?? or boost??
using json = nlohmann::json;


using GameDataObject = boost::make_recursive_variant<
    int,
    bool,
    std::string,
    std::vector<boost::recursive_variant_>,
    std::unordered_map<std::string, boost::recursive_variant_>
>::type;


class are_strict_equals
    : public boost::static_visitor<bool>
{
public:

    template <typename T, typename U>
    bool operator()( const T &, const U & )
    {
        return false; // cannot compare different types
    }

    template <typename T>
    bool operator()( const T & lhs, const T & rhs )
    {
        return lhs == rhs;
    }

};

class are_strict_greater
    : public boost::static_visitor<bool>
{
public:

    template <typename T, typename U>
    bool operator()( const T &, const U & )
    {
        return false; // cannot compare different types
    }

    template <typename T>
    bool operator()( const T & lhs, const T & rhs )
    {
        return lhs > rhs;
    }

};


class are_strict_less
    : public boost::static_visitor<bool>
{
public:

    template <typename T, typename U>
    bool operator()( const T &, const U & )
    {
        return false; // cannot compare different types
    }

    template <typename T>
    bool operator()( const T & lhs, const T & rhs )
    {
        return lhs < rhs;
    }

};

class is_boolean_type
    : public boost::static_visitor<bool>
{
public:

    bool operator()(const bool & expr) 
    {
        return true;
    }

    template <typename T>
    bool operator()( const T & expr )
    {
        return false;
    }
};


class boolean_complement
    : public boost::static_visitor<bool>
{
public:

    bool operator()(const bool & expr) 
    {
        return !expr;
    }

    template <typename T>
    bool operator()( const T & expr )
    {
        return false;
    }
};

class access_property
    : public boost::static_visitor<GameDataObject>
{
public:

    access_property(std::string& property_name)
        : property{property_name}
    { }

    template <typename T>
    GameDataObject& operator()( const T & obj )
    {
        throw std::runtime_error("not a map type")
        return false;
    }

    const GameDataObject& operator()( const std::unordered_map<std::string, GameDataObject> & obj )
    {
        return obj.at(property);
    }

private:
    std::string property;
};

class access_index
    : public boost::static_visitor<GameDataObject>
{
public:

    access_index(int index)
        : idx{index}
    { }

    template <typename T>
    GameDataObject& operator()( const T & obj )
    {
        throw std::runtime_error("not a list type")
        return false;
    }

    const GameDataObject& operator()( const std::vector<GameDataObject> & obj )
    {
        return obj[idx];
    }

private:
    int idx;
};


// TODO: need game data store 
class GameDataStore {
public:
    GameDataObject access(std::string name) {
        return map[name];
    }

private:
    std::unordered_map<std::string, GameDataObject> map;
};


class MethodObject {
public:
    GameDataObject execute(std::vector<GameDataObject> args) {
        return executeImpl(args);
    }

private:
    virtual GameDataObject executeImpl(std::vector<GameDataObject> args) = 0;
};

class MethodMap {
public:

    void registerMethod(std::string& methodName, std::unique_ptr<MethodObject> method) {
        methods[methodName] = std::move(method);
    }

    MethodObject* get(std::string& name) {
        return methods[name].get();
    }

private:
    std::unordered_map<std::string, std::unique_ptr<MethodObject>> methods;
};



class ExpressionVisitor : public ASTVisitor {
public:
    void visit(NumberConstant& number) override {
        result = number.getValue();
    }
    void visit(EqExpression& expr) override {
        expr.getLeft().evaluate(*this);
        GameDataObject lhs = result;
        expr.getRight().evaluate(*this);
        GameDataObject rhs = result;

        result = boost::apply_visitor(are_strict_equals{}, lhs, rhs);
    }
    void visit(NeqExpression& expr) override {
        expr.getLeft().evaluate(*this);
        GameDataObject lhs = result;
        expr.getRight().evaluate(*this);
        GameDataObject rhs = result;

        result = !boost::apply_visitor(are_strict_equals{}, lhs, rhs);
    }
    void visit(NotExpression& expr) override {
        expr.getExpr().evaluate(*this);

        if (!boost::apply_visitor(is_boolean_type{}, result)) {
            // throw error
            throw std::runtime_error("expected a boolean");

        } 
        result = boost::apply_visitor(boolean_complement{}, result);
    }
    void visit(GtExpression& expr) override {
        expr.getLeft().evaluate(*this);
        GameDataObject lhs = result;
        expr.getRight().evaluate(*this);
        GameDataObject rhs = result;

        result = boost::apply_visitor(are_strict_greater{}, lhs, rhs);
    } 
    
    void visit(LtExpression& expr) override {
        expr.getLeft().evaluate(*this);
        GameDataObject lhs = result;
        expr.getRight().evaluate(*this);
        GameDataObject rhs = result;

        result = boost::apply_visitor(are_strict_less{}, lhs, rhs);
    }
    void visit(DotExpression& expr) override {
        expr.getLeft().evaluate(*this);
        GameDataObject lhs = result;
        expr.getProperty().evaluate(*this);
        std::string property = boost::get<std::string>(result);

        // -- See list attributes section in the project description --
        // if property == "elements":
        // Map map = Map<string, list<obj>>();
        // list<string> keys = Object.keys(lhs); -> gets the minimum set of keys
        // for (string key in keys) {
        //     map[key] = lhs.transform(x => x[key])) || error_type;
        // }
        // return map;

        result = boost::apply_visitor(access_property{property}, lhs);
    }
    void visit(DotProperty& expr) {

        result = expr.getPropertyName();

    }
    void visit(IndexExpression& expr) override {
        expr.getListExpr().evaluate(*this);
        GameDataObject list = result;
        expr.getIndexExpr().evaluate(*this);
        int idx = boost::get<int>(result);

        result = boost::apply_visitor(access_index{idx}, list);
    }
    void visit(SingleVariable& expr) override {
        std::string name = expr.getName();

        result = dataStore.access(name);
    }
    void visit(MethodArgs& margs) override {
        auto argList = std::vector<GameDataObject>{};

        if (!margs.empty()) {
            for (auto it = margs.begin(); it != margs.end(); it++) {
                it->get()->evaluate(*this);
                argList.push_back(result);
            }
        }
        
        result = argList;
    }
    void visit(MethodCall& expr) override {
        expr.getArgs().evaluate(*this);
        std::vector<GameDataObject> args = boost::get<std::vector<GameDataObject>>(result);
        
        // store the method names in a map and then just call execute
        // to get the result
        result = methodMap.get(expr.getName())->execute(args);
    }
private:
    GameDataStore dataStore;
    GameDataObject result;
    MethodMap methodMap;
};