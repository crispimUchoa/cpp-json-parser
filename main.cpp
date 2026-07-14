#include <iostream>
#include <string>
#include <memory>
#include "json.h"

int main(){
    Json crispim = Json::loads("crispim.json");
    crispim.attr("mochila1", "nadica de nada");
    std::cout << crispim <<  '\n';
    return 0;
}