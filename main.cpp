#include <iostream>
#include <string>
#include <memory>
#include "json.h"

int main(){
    const std::string jsonString {"{ \"nome\":\"Crispim\",\"jovem\":true,\"loiro\":null}"};

    Json pessoa = Json(jsonString);

    std::cout << pessoa <<  '\n';
    return 0;
}