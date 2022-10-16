#include "../include/handler.h"
#include <string>

// use this main function to test handler code for now. In the future we will probably use unit tests instead.
int main() {
    std::string config = "{\"configuration\":{\"name\":\"ZenGame\",\"playercount\":{\"min\":0,\"max\":0},\"audience\":false,\"setup\":{}},\"constants\":{},\"variables\":{},\"per-player\":{},\"per-audience\":{},\"rules\":{}}";
    recieveMessage(config);
    
}