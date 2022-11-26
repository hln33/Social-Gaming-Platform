#pragma once

#include "boost/variant.hpp"
#include "string.h"
#include <memory>
#include<iostream>


class Object{
    public:
        virtual void executeImpl() = 0;
        auto clone() const { return std::unique_ptr<Object>(clone_impl()); }

        Object(){}
        virtual Object* clone_impl() const = 0;
        virtual ~Object() = default;
};


struct ObjectPtr
{
    std::unique_ptr<Object> ptr; 
    ObjectPtr(ObjectPtr const& other) : ptr(other.ptr->clone()) {
        std::cout << "object Ptr clone()" << "\n";
    }
    ObjectPtr(std::unique_ptr<Object> other) : ptr(std::move(other)) {
        std::cout << "moved ownership" << "\n";
    }
};


class Name : public Object {
    public:
        Name(std::string name) : name(std::move(name)){}
        Name(const Name&  other){
            this->name = other.name;
        }
        virtual Name* clone_impl() const override { return new Name(*this); };          
        void executeImpl() override{}
    private:
        std::string name;
};




// recursive variant definition, it can store a single object pointer or list of object pointer

class List : public Object {
    public:
        using RecursiveObject =  boost::recursive_variant_;
        using RecursiveList = std::vector<RecursiveObject>;
        using Expression = boost::make_recursive_variant<ObjectPtr, RecursiveList>::type;

        List(){

        }
        List(const List& other){
            std::cout << "TODO: might need to implement list copy" << "\n";
        }

        void add(ObjectPtr objectPtr) {
            list.push_back(objectPtr);
        }
        virtual List* clone_impl() const override { return new List(*this); };         
        void executeImpl() override{}
    private:
        std::vector<Expression> list;
};


class BinaryOperator {
    public: 
        std::string type;
        std::unique_ptr<Object> left;
        std::unique_ptr<Object> right;
};

