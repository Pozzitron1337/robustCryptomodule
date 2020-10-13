#include "robustCryptomodule.hpp"
#include <vector>
using namespace std;

void test(){
    const size_t size=6;
    const size_t states=2;
    int a[size][size] = {   {0,1,1,1,0,0},
                            {1,0,0,0,1,0},
                            {1,0,0,1,1,0},
                            {1,0,1,0,1,1},
                            {0,1,1,1,0,1},
                            {0,0,0,1,1,0}};
    matrix<int,size,size> *A=new matrix<int,size,size>(a);
    relayCircuit<size,states> r{*A};
    r.calculate_h();
}
void two_relays_paralell(){
    const size_t first_and_last=2;
    const size_t size=2+first_and_last;
    const size_t states=2;
    int a[size][size]{  {0,1,1,0},
                        {1,0,0,1},
                        {1,0,0,1},
                        {0,1,1,0}};

    matrix<int,size,size> A(a);
    relayCircuit<size,states> r{A};
    r.showRelayParametrs();
    r.calculate_h();
   

}

void four_relays_paralell(){
    const size_t first_and_last=2;
    const size_t size=4+first_and_last;
    const size_t states=2;
    int a[size][size]{  {0,1,0,1,0,0},
                        {1,0,1,0,0,0},
                        {0,1,0,0,0,1},
                        {1,0,0,0,1,0},
                        {0,0,0,1,0,1},
                        {0,0,1,0,1,0}};

    matrix<int,size,size> A(a);
    relayCircuit<size,states> r{A};
    r.showRelayParametrs();
    r.calculate_h();
}

int main(int argc, char *argv[]){
    two_relays_paralell();
    four_relays_paralell();
    
    return 0;
}