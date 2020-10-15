#include <iostream>
#include <algorithm>
#include <vector>
#include <map>
#include "robustCryptomodule.hpp"

using namespace std;

template<size_t maxRelays,size_t maxState>
class relayCircuit
{
private:
    size_t state;

    //the list of relays
    vector<notRobustRelay> relays;

    //matrix that shows wich element 
    matrix<int,maxRelays,maxRelays> adjacencyMatrix;
    
    //the list of pathes from begin to maxRelays-1
    vector<vector<size_t>> pathes;

public:
    
    relayCircuit(matrix<int,maxRelays,maxRelays> adjacencyMatrix,size_t init_state=0){
        this->state=init_state;
        this->adjacencyMatrix=adjacencyMatrix;
        notRobustRelay r{maxState};
        for(size_t i=0;i<maxRelays;i++){
            relays.push_back(notRobustRelay{r});
        }
        built_pathes();
    }
    relayCircuit(matrix<int,maxRelays,maxRelays> adjacencyMatrix,notRobustRelay r,size_t init_state=0){
        this->state = init_state;
        this->adjacencyMatrix=adjacencyMatrix;
        for(size_t i=0;i<maxRelays;i++){
            relays.push_back(notRobustRelay{r});
        }
        
        built_pathes();
    }
    ~relayCircuit(){
        relays[0].clearTransitMatrix();  
    }

    //counts number of pathes from begin to v,v=0,...,maxRelay-1
    size_t count(size_t v,bool *w,size_t *d){
        if(w[v]){
            return d[v];
        }else{
            size_t sum=0;
            w[v]=true;
            for(size_t i=0;i<maxRelays;i++){
                if(adjacencyMatrix(v,i)&&i<v){
                    sum+=count(i,w,d);
                }
            }
            d[v]=sum;
            return sum;
        }
    }

    //counts number of pathes from begin to maxRelay-1 
    size_t count_paths(){
        bool w[maxRelays];
        size_t d[maxRelays];//number of pathes from begin to v,vє{1,2,...,maxRelays-1}
        w[0]=true;
        d[0]=1;
        for(size_t i=1;i<maxRelays;i++){
            w[i]=false;
            d[i]=0;
        }
        auto res= count(maxRelays-1,w,d);
        return res;
    }

    void built(size_t v,vector<size_t> &temp_vector){
        if(v==0){
            //if the begin is reached
            vector<size_t> path(temp_vector);
            reverse(path.begin(),path.end());
            pathes.push_back(path);
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
    vector<vector<size_t>> built_pathes(){
        vector<size_t> temp_vector;
        temp_vector.push_back(maxRelays-1);
        built(maxRelays-1,temp_vector);
        return pathes;
    }

    void printPathes(){
        cout<<"Pathes:"<<endl;
        for(auto i:pathes){
            for(auto it=i.begin();it!=i.end();++it){
                cout<<*it<<" ";
            }
            cout<<endl;
        }
    }

    //generate increasing sequence in lexographic order
    vector<vector<size_t>> generateRelaysCombinations(size_t len){
        vector<vector<size_t>> result;
        if(len==0){
            return result;
        }
        vector<size_t> t;
        for(size_t i=1;i<=len;i++){
            t.push_back(i);
        }
        result.push_back(t);
        auto it=t.end()-1;//pointer to last element
        auto preMaxRelayNumber=maxRelays-2;//minus 0 and maxRelays-1
        auto endLoopStatement=preMaxRelayNumber-len+1;
        while ((*t.begin())!=endLoopStatement)
        {
            if(*it!=preMaxRelayNumber){
                *it=(*it)+1;
                result.push_back(t);
            }
            else
            {
                do{
                    it--;
                }
                while(*(it)+1==*(it+1));
                    
                *it=(*it)+1;
                for(auto j=it+1;j<t.end();j++){
                    *j=*(j-1)+1;
                }
                result.push_back(t);
                it=t.end()-1;
            }
        }
        return result;
    }

    vector<vector<size_t>> getValidPathes(size_t state){
        vector<vector<size_t>> result;
        bool isValid=true;
        for(auto i:pathes){
            for(auto j:i){
                if(relays[j].getState()!=state){
                    isValid=false;
                    break;
                }
            }
            if(isValid){
                vector<size_t> temp(i);
                temp.erase(temp.begin());
                temp.pop_back();
                result.push_back(temp);
            }
            isValid=true;
        }
        
        return result;
    }

    /*examples:
    * <1> is subvector of <1,2,4>
    * <2,3> is subvector of <1,2,3,5>
    * <2,3> is not subvector of <2,4,5>
    */
    bool isSubvector(vector<size_t> subvect,vector<size_t> vect){
        if(subvect.size()>vect.size()){
            return false;
        }
        auto it_vect=vect.begin();
        auto it_subvect=subvect.begin();
        if(subvect.size()==vect.size()){
            while (it_vect!=vect.end())
            {
                if(*it_vect!=*it_subvect){
                    return false;
                }
                it_vect++;
                it_subvect++;
            }
            return true;
        }
        while(*it_vect!=*it_subvect){ 
            it_vect++;
        }
        if(it_vect-1==vect.end()){
            return false;
        }
        else
        {
            while(it_subvect!=subvect.end()){
                if(*it_vect==*it_subvect){
                    it_vect++;
                    it_subvect++;
                }
                else
                {
                    return false;
                }
            }
            return true;
        }  
    }

    void showRelayParametrs(){
        cout<<"Relay properties:"<<endl;
        relays[0].printTransitMatrix();
        relays[0].testTransitMatrix();
        for(size_t i=0;i<maxRelays;i++){
            cout<<"State of relay_"<<i<<" = "<<relays[i].getState()<<endl;
        }
    }
    
    void calculate_h(){
        double h=0.0;
        double p;
        size_t A=0;
        for(size_t n=1;n<maxRelays-1;n++){
            auto allCombinationsOfRelays=generateRelaysCombinations(n);
            auto validPathes=getValidPathes(state);
            for(auto i:allCombinationsOfRelays){
                for(auto j:validPathes){
                    if(isSubvector(j,i)){
                        A++;
                        break;
                    }
                }    
            }
            p=relays[n].getStateProbability();
            h+=A*pow(p,n)*pow(1-p,maxRelays-2-n);
            cout<<"A_"<<n<<" = "<<A<<endl;
            cout<<"p = "<<p<<endl;
            cout<<"h = "<<h<<endl<<endl;
            A=0;
        }

        

    }
};



