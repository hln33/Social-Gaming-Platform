#include <gtest/gtest.h>
#include <string>
#include <string_view>
#include <memory>
#include <exception>
#include "ListObject.h"


// using RecursiveObject =  boost::recursive_variant_;
// using RecursiveList = std::vector<std::unique_ptr<RecursiveObject>>;
// using Expression = boost::make_recursive_variant<Object, RecursiveList>::type;



TEST(GeneralObject, firstTest){

 
    ObjectPtr name_pointer(std::make_unique<Name>(Name("a")));
   
    
    List l1;
    List l2;

    ObjectPtr list_pointer(std::move(std::make_unique<List>(l2)));
   
    l1.add(name_pointer);
    l1.add(list_pointer);

}