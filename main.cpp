#include <iostream>
#include <string>
#include <memory>
#include "json.h"

int main(){
    const std::string jsonString {"{ \"nome\":\"Crispim\",\"mochila2\":[12, [3]], \"mochila1\": {\"mochila3\": null}}"};

    Json pessoa = Json(jsonString);

    std::cout << pessoa <<  '\n';
    return 0;
}