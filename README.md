# JSON Parser (C++)

A simple JSON parser written in modern C++ as a learning project. It supports parsing JSON strings into native C++ data structures and serializing them back to valid JSON.

## Features

* Parse JSON objects and arrays
* Support for:

  * `null`
  * `bool`
  * `int`
  * `double`
  * `string`
  * nested arrays and objects
* Serialize parsed data back to JSON
* Read JSON from files

## Build

```bash
g++ main.cpp json.cpp -std=c++17 -o parser
```

## Example

```cpp
Json json = Json::fromFile("example.json");

std::cout << json << std::endl;
```

## Purpose

This project was developed to better understand recursive parsing, lexical analysis, and modern C++ features such as `std::variant`, `std::shared_ptr`, and STL containers.
