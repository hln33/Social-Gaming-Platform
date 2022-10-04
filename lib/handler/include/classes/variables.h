#include <map>
#include <any>
#include <string>

class Variables {
    public:
        std::map<std::string, std::any> variables;

        Variables(std::map<std::string, std::any> map);
};