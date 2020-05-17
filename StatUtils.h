/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   StatUtils.h
 * Author: candle
 *
 * Created on May 9, 2020, 12:42 PM
 */

#include <vector>
#include <math.h>

using namespace std;

#ifndef STATUTILS_H
#define STATUTILS_H


double meanOfData(vector<double>& data);
void fillRelativeDistances(vector<double>& data, vector<double>& relValues);
double varianceOfData(vector<double>& data, bool isSample);
double stdDev(vector<double>& data, bool isSample);
uint32_t findMaxElementIndex(vector<double>& data);
uint32_t findMinElementIndex(vector<double>& data);

#endif /* STATUTILS_H */

