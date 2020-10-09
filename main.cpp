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
    matrix<int,s,s> B(*A);
    relayCircuit<s> r{2,*A};
   
    cout<<r.count_paths()<<endl;
    //r.built_pathes();

    
    return 0;
}