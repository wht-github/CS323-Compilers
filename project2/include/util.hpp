#include <iostream>
#include <string>
inline void logger(string s){std::cout<<s<<std::endl;}
inline void prterr(int id, int lineno, string message){
    std::cout<<"Error type "<<id<<" at Line " << lineno <<": "<<message<<std::endl;
}