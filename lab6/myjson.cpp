union JValue {
    JString String;
    JNumber Number;
    JBool Bool;
    JNull Null;
    JObject Object;
    JArray Array;
}
struct JString
{
    char * base;
    int size;
};
struct JBool{
    bool value;
}
struct JNumber{
    union {
        int val;
        float val;
    }
}
struct JElement{
    char * name;
    JValue value;
}
struct JObject{
    JElement * first;
    int size;
}
struct JArray{
    Jvalue * first;
    int size;
}
struct JNull{}