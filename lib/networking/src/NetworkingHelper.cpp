#include "NetworkingHelper.h"

#include <boost/random/random_device.hpp>
#include <boost/random/uniform_int_distribution.hpp>


std::string randomCode(){
  std::string chars(
        "abcdefghijklmnopqrstuvwxyz"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "1234567890");


    boost::random::random_device rng;
    boost::random::uniform_int_distribution<> index_dist(0, chars.size() - 1);
    std::string code = "";
    for(int i = 0; i < 6; ++i) {
        code.append(std::string(1,chars[index_dist(rng)]));
    }

    return code;
}