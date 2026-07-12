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
            out << "[";
            const size_t len = arg.size();
            
            for (size_t i {0}; i < len; i ++){
                out << *arg.at(i);

                if (i < len - 1){
                    out << ", ";
                };
            };


            out << "]";
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
            incrementCursor();
            return value;
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

            return value;

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
            
            return true;
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
            return false;
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
            return nullptr;
        };

        std::function<JsonValue()> parseValue;
        parseValue = [&](){
            JsonValue value;
            if (currentChar == '\"'){
            value = parseStr();
            } else if (std::isdigit(currentChar)) {
            value = parseNum();
            } else if (currentChar == 't') {
                value = parseTrue();
            } else if (currentChar == 'f') {
                value = parseFalse();
            } else if (currentChar == 'n') {
                value = parseNull();
            } else if (currentChar == '['){
                value.value = JsonArray();
                incrementCursor();
                while (currentChar != ']' && cursor < jsonString.length()){
                    ignoreWS();
                    JsonValue newValue {parseValue()};
                    auto& arr = std::get<JsonArray>(value.value);
                    arr.push_back(JsAtt(newValue));

                    ignoreWS();
                    if (currentChar == ']') break;
                    
                    if (currentChar != ',') {
                        throw std::runtime_error("Expected \',\'");
                    }

                    incrementCursor();
                };
                incrementCursor();
            } else if (currentChar == '{'){
                size_t bracketsCounter = 0;
                
                Json newJson = Json(jsonString.substr(cursor, jsonString.length() - cursor - 1));
                
                do {
                    ignoreWS();
                    if (currentChar == '{') {
                        ++bracketsCounter;
                    } else if (currentChar == '}'){
                        --bracketsCounter;
                    };
                    incrementCursor();
                } while (bracketsCounter > 0);
                value.value = newJson.object;
            };

            return value;
        };

        incrementCursor();
        ignoreWS();

        if (currentChar != ':'){
            throw std::runtime_error("Expected :");
        };

        incrementCursor();
        ignoreWS();

        const JsonValue value = parseValue();
        this->attr(key, value);

        ignoreWS();

        if (currentChar == '}') break;

        if (currentChar != ','){
            std::cout << "Cursor: " << cursor << '\n';
            std::cout << "Current : " << currentChar << '\n';
            std::cout << "String : " << jsonString << "\n\n\n";
            std::cout << "Parser : " << jsonString.substr(cursor) << "\n\n\n";
            throw std::runtime_error("Expected ," );
        };
        ignoreWS();

        incrementCursor();
    };
};       
