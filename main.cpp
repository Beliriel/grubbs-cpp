/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.cpp
 * Author: candle
 *
 * Created on May 3, 2020, 3:56 PM
 */

#include <cstdlib>
#include <stdio.h>
#include "Grubbs.h"

using namespace std;




int main(int argc, char** argv) {

    vector<double> test;
    test.push_back(-1);
    test.push_back(8);
    test.push_back(9);
    test.push_back(10);
    test.push_back(9);
    test.push_back(20);
    for(auto it = test.begin(); it!=test.end(); ++it)
    {
        printf("%f\t", *it);
    }
    printf("\n");
    printf("mean: %f\n", meanOfData(test));
    printf("variance: %f\n", varianceOfData(test, false));
    printf("stdDev: %f\n", stdDev(test, false));
    
    TwoSidedGrubbsTest tt;
    tt.load_data(test);
    
    //OneSidedGrubbsTest ot;
    //ot.load_data(test, false);
    
    vector<double> resultAfterOutliersRemoved;
    tt.run(TA_0_100, &resultAfterOutliersRemoved);
    for(auto it = resultAfterOutliersRemoved.begin(); it!=resultAfterOutliersRemoved.end(); ++it)
    {
        printf("%f\t", *it);
    }
    printf("\n");
    return 0;
}

