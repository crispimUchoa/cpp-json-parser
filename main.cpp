#include <iostream>
#include <string>
#include <memory>
#include "json.h"

int main(){
    const std::string jsonString = R"(
    { 
        "nome": "Crispim", 
        "mochila2":[12, [3]],
        "mochila1": {
            "mochila3": null
            }
    }
    )";

    Json pessoa = Json(jsonString);
    pessoa.attr("nome", "paulo");
    std::string nome = pessoa.get<std::string>("nome");
    std::cout << nome <<  '\n';
    return 0;
}