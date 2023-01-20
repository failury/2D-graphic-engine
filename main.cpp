#include "app.hpp"
#include <iostream>
#include <stdexcept>
#include <cstdlib>
int main(){
    engine::app a{};
    try{
        a.run();
    } catch (const std::exception &e){
        std::cerr << e.what() << '\n';
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}