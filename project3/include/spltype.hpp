#ifndef SPLTYPE_HPP
#define SPLTYPE_HPP
#include <iostream>
#include <optional>
#include <string>
#include <unordered_map>
#include <vector>
using std::optional;
using std::string;
using std::unordered_map;
using std::vector;
enum class Primitive { INT,
                       FLOAT,
                       CHAR,
                       TRUE,
                       FALSE };
enum class Category { PRIMITIVE,
                      ARRAY,
                      STRUCT,
                      FUNCTION,
                      TUPLE,
                      BOOLEAN,
                      UNCERTAIN,
                      NTUPLE };
enum class LRV { L,
                 R };
class Type {
   public:
    bool initialized = false;
    optional<string> name;     //for struct types
    optional<string> varname;  // for check
    optional<Primitive> primitive = {};
    optional<Type *> type_to_cal = {};
    Category category;
    LRV lrv = LRV::R;
    // virtual bool initialize_getter(){return initialized;}
    virtual Category cat_getter() { return category; }
    virtual void cat_setter(Category c) { this->category = c; }
};
class Array : public Type {
   public:
    optional<Primitive> primitive = {};
    Category category = Category::ARRAY;
    unsigned int size = 0;
    Type *type = nullptr;
    virtual Category cat_getter() { return category; }
    virtual void cat_setter(Category c) { this->category = c; }
};

class Struct : public Type {
   public:
    optional<Primitive> primitive = {};
    Category category = Category::STRUCT;
    unordered_map<string, Type *> fields;
    virtual Category cat_getter() { return category; }
    virtual void cat_setter(Category c) { this->category = c; }
};
class Tuple : public Type {
   public:
    vector<Type *> args;
    Category category = Category::TUPLE;
    virtual Category cat_getter() { return category; }
    virtual void cat_setter(Category c) { this->category = c; }
};
class NTuple : public Type {
   public:
    vector<std::pair<string, Type *> > nargs;
    Category category = Category::NTUPLE;
    virtual Category cat_getter() { return category; }
    virtual void cat_setter(Category c) { this->category = c; }
};
class Function : public Type {
   public:
    optional<Primitive> primitive = {};
    Category category = Category::FUNCTION;
    Type *return_type = nullptr;
    Tuple *funcargs = nullptr;
    virtual Category cat_getter() { return category; }
    virtual void cat_setter(Category c) { this->category = c; }
};

static bool type_equal(Type *a, Type *b) {
    if (a == nullptr && b == nullptr) return true;
    if (a == nullptr || b == nullptr) return false;
    if (a->cat_getter() == b->cat_getter()) {
        switch (a->cat_getter()) {
            case Category::PRIMITIVE: {
                if (a->primitive.has_value() && b->primitive.has_value()) {
                    return a->primitive.value() == b->primitive.value();
                } else {
                    return false;
                }
            }
            case Category::STRUCT: {
                return a->name.value() == b->name.value();
            }
            case Category::ARRAY: {
                auto arr_a = (Array *)a;
                auto arr_b = (Array *)b;
                if (arr_a->size != arr_b->size) {
                    return false;
                }
                return type_equal(arr_a->type, arr_b->type);
            }
            case Category::TUPLE: {
                auto tup_a = (Tuple *)a;
                auto tup_b = (Tuple *)b;
                if (tup_a->args.size() == tup_b->args.size()) {
                    int t = tup_a->args.size();
                    for (int i = 0; i < t; i++) {
                        if (!type_equal(tup_a->args[i], tup_b->args[i])) {
                            return false;
                        }
                    }
                    return true;
                } else {
                    return false;
                }
            }
            case Category::FUNCTION: {
                auto fun_a = (Function *)a;
                auto fun_b = (Function *)b;
                // overload is not good
                return type_equal(fun_a->return_type, fun_b->return_type) && type_equal(fun_a->funcargs, fun_b->funcargs);
            }
            default: {
                std::cerr << "Unknown ENUM in type checking" << std::endl;
                exit(1);
            }
        }
    } else {
        return false;
    }
}
static bool tuple_equal_to(Type *base, Type *other) {
    if (other->cat_getter() == Category::TUPLE) {
        auto tmp = (Tuple *)other;
        for (size_t i = 0; i < tmp->args.size(); ++i) {
            if (!tuple_equal_to(base, tmp->args[i])) {
                return false;
            }
        }
        return true;
    } else {
        return type_equal(base, other);
    }
}
#endif
