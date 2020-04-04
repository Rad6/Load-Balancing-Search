#include "presenter.h"


int main(int argc, char const *argv[]) 
{

    presenter p(atoi(argv[1]), argv[2], argv[3]);
    p.read_data(); 
    p.print_data();

    return 0;
}