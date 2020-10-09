#include "robustCryptomodule.hpp"

using namespace std;



int main(int argc, char *argv[]){

    
    const size_t s=6;
   
    int a[s][s] = { {0,1,1,1,0,0},
                    {1,0,0,0,1,0},
                    {1,0,0,1,1,0},
                    {1,0,1,0,1,1},
                    {0,1,1,1,0,1},
                    {0,0,0,1,1,0}};
    matrix<int,s,s> *A=new matrix<int,s,s>(a);
    
    relayCircuit<s> r{2,*A};
   
    r.count_paths();
    r.built_pathes();

    
    return 0;
}