#pragma once
#include <math.h>
#include <iostream>
#include "robustCryptomodule.hpp"

using namespace std;

/*
                NOT ROBUST RELAY

this is realization of generalized model of relay with two states.
*/
class notRobustRelay
{
private:
    size_t    maxStates;      //max number of states.
    size_t    state;          //current state; 
                                    //state є {0,1,2,...,2*maxStates-1}
                                    //{0,1,2,...,maxStates-1} - right states
                                    //{maxStates,...,2*maxStates-1} - error states
                                    //on every right state exists it`s error state
                                    //for every k rigth state exists k+maxState error state,k є {0,1,2,...,maxState-1}
    double  **transitMatrix;//transitMatrix[i][j] show the probability to change from i state to j state;
    void    initTransitMatrix(double coeficient,size_t error_range_modulus,size_t range_probability);
public:

    notRobustRelay(size_t maxStates_);
    notRobustRelay(const notRobustRelay& r);
    double** getTransitMatrix();
    double getStateProbability();
    size_t getState();
    void changeState(size_t newState);
    void printTransitMatrix();
    void testTransitMatrix();
    void clearTransitMatrix();
    double getTransitMatrixElement(size_t i,size_t j);
    ~notRobustRelay();
};

void notRobustRelay::initTransitMatrix(double coeficient=11.0,size_t error_range_modulus=100000,size_t range_probability=100000){
    //random filling
    srand(time(0));
    double x=(rand()%error_range_modulus)+error_range_modulus;
    x/=error_range_modulus*coeficient;

    double *temp=new double[maxStates];
    double sum;
    for(size_t i=0;i < maxStates;i++){
        sum=0.0;
        for(size_t j=0;j < maxStates;j++){
            //temp[j] є {range_probability,...,2*range_probability-1}
            temp[j]=(rand()%range_probability)+range_probability;
            sum+=temp[j];
        }
        for (size_t j = 0; j < maxStates; j++){
            temp[j]=temp[j]/sum;
            this->transitMatrix[i][j]=temp[j]-x;
        }
    }
    delete temp;
    for(size_t i=0;i < maxStates;i++){
        for(size_t j=maxStates;j<2*maxStates;j++){
            this->transitMatrix[i][j]=x;
        }
    }
    for(size_t i=maxStates;i < 2*maxStates;i++){
        for(size_t j=0;j < maxStates;j++){
            if(i-maxStates==j){
                this->transitMatrix[i][j]=this->transitMatrix[i-maxStates][j]+x;
            }
            else
            {
                this->transitMatrix[i][j]=this->transitMatrix[i-maxStates][j];
            }
        }
    }
    for(size_t i=maxStates;i < 2*maxStates;i++){
        for(size_t j=maxStates;j < 2*maxStates;j++){
            if(i==j){
                this->transitMatrix[i][j]=0.0;//error state cant transit to same error state
            }
            else
            {
                this->transitMatrix[i][j]=x;
            }
        }
    }
}

notRobustRelay::notRobustRelay(size_t maxStates)
{
    this->maxStates=maxStates;
    this->state=0;
    this->transitMatrix=new double*[2*maxStates];
    for(size_t i=0;i<2*maxStates;i++){
        this->transitMatrix[i]=new double [2*maxStates];
    }
    initTransitMatrix();
}

notRobustRelay::notRobustRelay(const notRobustRelay& r)
{
    this->maxStates=r.maxStates;
    this->state=r.state;
    this->transitMatrix=r.transitMatrix;
}

notRobustRelay::~notRobustRelay(){  
}

double** notRobustRelay::getTransitMatrix(){
    return transitMatrix;
}

double notRobustRelay::getStateProbability(){
    return transitMatrix[state][state];
}

void notRobustRelay::printTransitMatrix()
{
    for(size_t i=0;i<2*maxStates;i++){
        if(i==0){
            printf("%16s_%ld","state",i);
        }
        else
        {
            printf("%9s_%ld","state",i);
        }
    }
    printf("\n");
    for(size_t i=0;i<2*maxStates;i++){
        printf("%s_%ld |","state",i);
        printf("%9g",this->transitMatrix[i][0]);
        for(size_t j=1;j<2*maxStates;j++){
            printf("%11g",this->transitMatrix[i][j]);
        }   
        printf("\n");
    }
}

void notRobustRelay::testTransitMatrix()
{
    double sum;
    for(size_t i=0;i<2*maxStates;i++){
        sum=0.0;
        for(size_t j=0;j<2*maxStates;j++){
            sum+=transitMatrix[i][j];
        }
        cout<<"Row "<<i<<" sum = "<<sum<<endl;
    }
    
    for (size_t i = 0; i < maxStates; i++){
        sum=0.0;
        for (size_t j = 0; j < maxStates; j++){
            sum+=transitMatrix[i][j];
        }
        cout<<"Right state_"<<i<< " probability sum: "<<sum<<endl;
    }
    for (size_t i = 0; i < maxStates; i++){
        sum=0.0;
        for (size_t j = maxStates; j < 2*maxStates; j++){
            sum+=transitMatrix[i][j];
        }
        cout<<"Error state_"<<i<< " probability sum: "<<sum<<endl;
    }
    
}

double notRobustRelay::getTransitMatrixElement(size_t i,size_t j){
    return transitMatrix[i][j];
}

size_t notRobustRelay::getState(){
    return state;
} 

void notRobustRelay::clearTransitMatrix()
{
    for(size_t i=0;i<2*maxStates;i++){
        delete transitMatrix[i];
    }
    delete transitMatrix;
}

