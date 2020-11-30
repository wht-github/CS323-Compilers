struct A {
    struct B{
        int c;
    }b;
};
int main() {
    struct A a;
    a.b = {};
}