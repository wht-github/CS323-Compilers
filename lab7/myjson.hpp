#ifndef MYJSON_HPP
#define MYJSON_HPP
#include <string>
#include <vector>
#include <unordered_map>

using std::string, std::vector, std::unordered_map;
enum class Category
{
    OBJECT,
    ARRAY,
    STR,
    NUM,
    BOOLEAN,
    NIL
};
union Val
{
    ObjectMember *members;
    ArrayValue *values;
    string *s;
    double number;
    bool boolean;
};
class JsonObject
{
    Category category;
    Val val;
};

class ObjectMember
{
    vector<pair<string *, ObjectMember*> >values;
};

class ArrayValue
{
    vector<JsonObject*> values;

};

#endif