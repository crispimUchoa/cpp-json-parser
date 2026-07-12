#ifndef JSON_H
#define JSON_H
#include <string>
#include <vector>
#include <memory>
#include <variant>
#include <iostream>
#include <unordered_map>
#include <functional>

class Json;
struct JsonValue;

using JsonObject = std::unordered_map<std::string, std::shared_ptr<JsonValue>>;

using JsonArray = std::vector<std::shared_ptr<JsonValue>>;

std::shared_ptr<JsonValue> JsAtt(JsonValue v);


struct JsonValue {
    std::variant<
        std::nullptr_t,
        bool,
        int, 
        double,
        std::string,
        JsonArray,
        JsonObject
    > value;

    JsonValue(const char* s): value( std::string(s) ) {};
    JsonValue(const std::string& s): value(s) {};
    JsonValue(int i): value(i) {};
    JsonValue(double d): value(d) {};
    JsonValue(bool b): value(b) {};
    
    JsonValue(const JsonArray& ja): value(ja) {};
    JsonValue(const JsonObject& jo): value(jo) {};

    JsonValue(std::nullptr_t): value(nullptr) {};
    JsonValue(const Json& js);

    JsonValue() : value(nullptr) {};
};

std::ostream& operator<<(std::ostream& out, const JsonValue& js);

class Json{

    public:
        JsonObject object;
        Json(const std::string& jsonString);
        Json();

        void attr(const std::string& key, const JsonValue& value);

        std::shared_ptr<JsonValue>& operator[](const std::string& key){
            return (this->object)[key];
        }

        
};

std::ostream& operator<<(std::ostream& out, const Json& js);


#endif