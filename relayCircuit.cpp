#include <iostream>
#include <vector>
#include "robustCryptomodule.hpp"

using namespace std;

template<size_t maxRelays>
class relayCircuit
{
private:
    vector<notRobustRelay> relays;
    size_t maxState;
    matrix<int,maxRelays,maxRelays> adjacencyMatrix; //
    size_t d[maxRelays];//number of pathes from begin to v,vє{1,2,...,maxRelays-1}
    vector<vector<size_t>> pathes;//the list of pathes from begin to maxRelays-1
public:
    relayCircuit(size_t maxState,matrix<int,maxRelays,maxRelays> adjacencyMatrix){
        this->maxState=maxState;
        this->adjacencyMatrix=adjacencyMatrix;
        notRobustRelay r{maxState};
        for(size_t i=0;i<maxRelays;i++){
            relays.push_back(notRobustRelay{r});
        }
        d[0]=1;
        for(size_t i=1;i<maxRelays;i++){
            d[i]=0;
        }
        
    }
    ~relayCircuit(){
        relays[0].clearTransitMatrix();
        
    }

    //counts number of pathes from begin to v,v=0,...,maxRelay-1
    size_t count(size_t v,bool *w){
        if(w[v]){
            return d[v];
        }else{
            size_t sum=0;
            w[v]=true;
            for(size_t i=0;i<maxRelays;i++){
                if(adjacencyMatrix.getElement(v,i)==1&&i<v){
                    sum+=count(i,w);
                }
            }
            d[v]=sum;
            return sum;
        }
    }

    //counts number of pathes from begin to maxRelay-1 
    size_t count_paths(){
        bool w[maxRelays];
        w[0]=true;
        for(size_t i=1;i<maxRelays;i++){
            w[i]=false;
        }
        auto res= count(maxRelays-1,w);
        //cout<<res<<endl;
        return res;
    }

    void built(size_t v,vector<size_t> &temp_vector){
        if(v==0){
            //if the begin is reached
            pathes.push_back(vector<size_t>(temp_vector));
        }else{
            for(size_t i=0;i<maxRelays;i++){
                if(adjacencyMatrix.getElement(v,i)==1&&i<v){
                    temp_vector.push_back(i);
                    built(i,temp_vector);
                    temp_vector.pop_back();
                }
            }
        }

    }
    //built the pathes form begin to maxRelays-1
    void built_pathes(){
        vector<size_t> temp_vector;
        temp_vector.push_back(maxRelays-1);
        built(maxRelays-1,temp_vector);
        for(auto i:pathes){
            for(auto j:i){
                printf("%3d",j);
            }
            printf("\n");
        }
    }


};



