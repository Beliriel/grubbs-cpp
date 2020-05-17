/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include <complex>

#include "StatUtils.h"


double meanOfData(vector<double>& data){
    double mean = 0;
    double n = (double) data.size();
    for(uint32_t i = 0; i<data.size(); ++i){
        mean += data[i]/n;
    }
    return mean;
}

void fillRelativeDistances(vector<double>& data, vector<double>& relValues){
    double mean = meanOfData(data);
    relValues.clear();
    for(uint32_t i = 0; i<data.size(); ++i){
        relValues.push_back(abs(data[i]-mean));
    }
}

double varianceOfData(vector<double>& data, bool isSample){
    double size = (double) data.size();
    double mean = meanOfData(data);
    double variance = 0;
    if(isSample) --size;
    for(uint32_t i = 0; i<data.size(); ++i){
        variance += pow(data[i]-mean, 2)/size;
    }
    return variance;
}

double stdDev(vector<double>& data, bool isSample){
    return sqrt(varianceOfData(data, isSample));
}

uint32_t findMaxElementIndex(vector<double>& data){
    double max = data[0];
    uint32_t index = 0;
    for(uint32_t i = 0; i<data.size(); ++i){
        if(data[i]>max){
            index = i;
            max = data[i];
        }
    }
    return index;
}

uint32_t findMinElementIndex(vector<double>& data){
    double max = data[0];
    uint32_t index = 0;
    for(uint32_t i = 0; i<data.size(); ++i){
        if(data[i]<max){
            index = i;
            max = data[i];
        }
    }
    return index;
}