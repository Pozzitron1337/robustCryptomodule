#pragma once
#include <iostream>
#include <fstream>
#include <cmath>
#include <string.h>
#include "robustCryptomodule.hpp"

using namespace std;

template <typename T, size_t row_size, size_t col_size>
class matrix{
    private:
       T M[row_size][col_size];
    public:
        matrix(T l=(T)0){  
            for (size_t i=0; i<row_size; i++){
                for(size_t j=0;j<col_size;j++){
                    if(i==j){
                        M[i][j]=l;
                    }else{
                        M[i][j]=(T)0;
                    }
                }
            }
        }
        matrix(T A[row_size]){
            for(size_t i=0;i<row_size;i++){
                M[i][0]=A[i];
            }
        }
        matrix(T A[row_size][col_size]){
            for(size_t i=0;i<row_size;i++){
                for(size_t j=0;j<col_size;j++){
                    M[i][j]=A[i][j];
                }
            }
        }
        
        matrix(T **A){
            for(size_t i=0;i<row_size;i++){
                for (size_t j = 0; j < col_size; j++){
                    M[i][j]=A[i][j];
                }
            }
        }
        
        matrix(matrix<T,row_size,col_size> &m){
            for(size_t i=0;i<row_size;i++){
                for (size_t j = 0; j < col_size; j++){
                    this->M[i][j]=m.M[i][j];
                }
            }
        }

        ~matrix(){}

        
        matrix<T,row_size,col_size> operator+(matrix<T,row_size,col_size>& m){
            matrix<T,row_size,col_size> result;
            for(size_t i=0;i<row_size;i++){
                for (size_t j=0;j<col_size;j++){
                    result.M[i][j]=this->M[i][j]+m.M[i][j];
                }
                
            }
            return result;
        }

        matrix<T,row_size,col_size> operator*(const T& l){
            matrix<T,row_size,col_size> result;
            for(int i=0;i<row_size;i++){
                for(int j=0;j<col_size;j++){
                    result.M[i][j]=this->M[i][j]*l;
                }
            }
            return result;
        } 


        template<size_t new_col_size>
        matrix<T,row_size,new_col_size> operator*(matrix<T,col_size,new_col_size>& m){
            matrix<T,row_size,new_col_size> result;
            T temp=(T)0;
            for(size_t i=0;i<row_size;i++){
                for(size_t j=0;j<new_col_size;j++){
                    for (size_t k = 0; k <col_size; k++){
                        temp+=this->M[i][k]*m.M[k][j];
                    }
                    result.M[i][j]=temp;
                    temp=(T)0;
                } 
            }
            return result;
        }

        T operator()(size_t i,size_t j){
            if(i<row_size&&j<col_size){
                return M[i][j];
            }else{
                return (T)0;
            }
        }
        T getElement(size_t i,size_t j){
            if(i<row_size&&j<col_size){
                return M[i][j];
            }else{
                return (T)0;
            }
        }
        void operator()(size_t i,size_t j,T element){
            if(i<row_size&&j<col_size){
                M[i][j]=element;
            }
        }

        matrix<T,col_size,row_size> transponce(){
            matrix<T,col_size,row_size> result;
            for(size_t i=0;i<col_size;i++){
                for (size_t j=0;j<row_size;j++){
                    result(i,j,this->M[j][i]);
                }
            }
            return result;
        }

        void print(){
            char *buf;
            string s;
            size_t maxTab_size_t=(size_t)1+(size_t)log10(row_size);
            string maxTab="%"+to_string(maxTab_size_t)+"d |";
            string init_table_indexes="%"+to_string(maxTab_size_t)+"s |";
            printf(init_table_indexes.c_str()," ");
            for(size_t i=0;i<col_size;i++){
                s=to_string(i);
                buf=new char[s.length()+1];
                strcpy(buf,s.c_str());
                printf("%6s",buf);
                delete buf;
            }
            printf("\n");
            for(size_t i=0; i<row_size; i++){
                printf(maxTab.c_str(),i);
                for(size_t j=0; j<col_size; j++){
                    s=to_string(this->M[i][j]);
                    buf=new char[s.length()+1];
                    strcpy(buf,s.c_str());
                    printf("%6s",buf);
                    delete buf;
                }
                printf("\n");
            }
        }

        //the sum of diagonal elements
        T trace(){
            T sum=(T)0;
            if(row_size==col_size){
                for(size_t i=0;i<row_size;i++){
                    sum+=M[i][i];
                }
            }
            return sum;
        }

        bool isSymmetric(){
            if(row_size!=col_size){
                return false;
            }
            for (size_t i = 0; i < row_size; i++){
                for (size_t j = 0; j < col_size; j++){
                    if(this->M[i][j]!=this->M[j][i]){
                        return false;   
                    }
                }
                
            }
            return true;
            
        }

        matrix<T,row_size,col_size> square(){
            if(row_size!=col_size){
                return *this;
            }
            matrix<T,row_size,col_size> result;
            T temp=(T)0;
            for(size_t i=0;i<row_size;i++){
                for(size_t j=0;j<col_size;j++){
                    for (size_t k = 0; k <col_size; k++){
                        temp+=this->M[i][k]*this->M[k][j];
                    }
                    result.M[i][j]=temp;
                    temp=(T)0;
                } 
            }
            return result;
        }

};

