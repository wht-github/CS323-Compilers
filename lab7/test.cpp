#include "myjson.hpp"
#include <iostream>
using namespace std;
int main(int arg){
    JValue * x = new JValue();
    x->Number = JNumber{10};
    cout << x->Number.value << endl;
}