#include "json.h"
#include <memory>
#include <cctype>

std::shared_ptr<JsonValue> JsAtt(JsonValue v) {
    return std::make_shared<JsonValue>(v);
};

std::ostream& operator<<(std::ostream& out, const JsonValue& js){
    std::visit([&out](auto&& arg) {
         using T = std::decay_t<decltype(arg)>;

        if constexpr (std::is_same_v<T, std::nullptr_t>){
            out << "null";
        } else if constexpr (std::is_same_v<T, JsonObject>) {
            out << "{" << '\n';
            for (const auto& [key, value] : arg){
                out << '\t' << '\"' << key << "\"" << ": " << *value << '\n';
            }
            out << "}";

        } else if constexpr(std::is_same_v<T, JsonArray>) {
            out << "[]";
        } else if constexpr(std::is_same_v<T, std::string>) {
            out << '\"' << arg << '\"';
        } else {
            std::cout << std::boolalpha;
            out << arg;

        };

    }, js.value);
    return out;
};



std::ostream& operator<<(std::ostream& out, const Json& js){
    out << JsonValue(js.object);
    return out;
};


Json::Json() {
};

JsonValue::JsonValue(const Json& js): value(js.object) {};

void Json::attr(const std::string& key, const JsonValue& value){
    this->object[key] = JsAtt(value);
};



Json::Json(const std::string& jsonString) {
    size_t cursor {0};
    char currentChar {jsonString.at(cursor)};


    auto incrementCursor = [&](){
        ++cursor;
        if (cursor< jsonString.length()){
            currentChar=jsonString.at(cursor);
        };
    };

    auto ignoreWS = [&](){
        if (std::isspace(currentChar)){
            incrementCursor();
        };
    };

    ignoreWS();
    if (currentChar != '{'){
        throw std::runtime_error("Json must start with \'{\'");
    };

    incrementCursor();



    while (currentChar != '}' && cursor < jsonString.length()){
        ignoreWS();
        if (currentChar != '\"'){
            throw std::runtime_error("Keys must be init with '\"");
        };
        incrementCursor();
        const size_t initKey {cursor};

        while(currentChar != '\"'){
            incrementCursor();
        };
        const size_t keySize {cursor - initKey}; 
        const std::string key {jsonString.substr(initKey, keySize)};


        auto parseStr = [&](){
            incrementCursor();
            const size_t initValue {cursor};

            while(currentChar != '\"'){
                incrementCursor();
            };

            const size_t valueSize {cursor - initValue}; 
            const JsonValue value {jsonString.substr(initValue, valueSize)};
            this->attr(key, value);
            incrementCursor();
        };

        auto parseNum = [&](){
            const size_t initValue {cursor};

            while (std::isdigit(currentChar)){
                incrementCursor();
            }

            const size_t valueSize {cursor - initValue }; 
            
            const JsonValue value {
                std::stoi(jsonString.substr(initValue, valueSize))
            };

            this->attr(key, value);

        };

        auto parseTrue = [&](){
            const size_t initValue {cursor};
            incrementCursor();
            incrementCursor();
            incrementCursor();
            incrementCursor();
            const size_t valueSize {cursor - initValue }; 
            if (jsonString.substr(initValue, valueSize) != "true") {
                throw std::runtime_error("JSON Inválid value.");
            };
            this->attr(key, true);
        };

        auto parseFalse = [&](){
            const size_t initValue {cursor};
            incrementCursor();
            incrementCursor();
            incrementCursor();
            incrementCursor();
            incrementCursor();
            const size_t valueSize {cursor - initValue }; 
            if (jsonString.substr(initValue, valueSize) != "false") {
                throw std::runtime_error("JSON Inválid value.");
            };
            this->attr(key, false);
        };

        auto parseNull = [&](){
            const size_t initValue {cursor};
            incrementCursor();
            incrementCursor();
            incrementCursor();
            incrementCursor();
            const size_t valueSize {cursor - initValue }; 
            if (jsonString.substr(initValue, valueSize) != "null") {
                throw std::runtime_error("JSON Inválid value.");
            };
            this->attr(key, nullptr);
        };

        incrementCursor();
        ignoreWS();

        if (currentChar != ':'){
            throw std::runtime_error("Expected :");
        };

        incrementCursor();
        ignoreWS();

        if (currentChar == '\"'){
           parseStr();
        } else if (std::isdigit(currentChar)) {
           parseNum();
        } else if (currentChar == 't') {
            parseTrue();
        } else if (currentChar == 'f') {
            parseFalse();
        } else if (currentChar == 'n') {
            parseNull();
        };

        ignoreWS();

        if (currentChar != ',' && currentChar != '}'){
            throw std::runtime_error("Expected '\"','\"'" );
        }
        ignoreWS();

        incrementCursor();
    };
};       
