### Smirnov-Grubbs Test for C++

This is a rewrite for C++11 of the Smirnov-Grubbs test written in python by Masashi Shibata:
https://github.com/c-bata/outlier-utils

for critical values of the t-distribution the values provided by NIST have been used:
NIST/SEMATECH e-Handbook of Statistical Methods, https://www.itl.nist.gov/div898/handbook/eda/section3/eda3672.htm, 17. May 2020


## Usage:

Create an Object of either a one-sided or two-sided Grubbstest

`OneSidedGrubbsTest ot;`  
`TwoSidedGrubbsTest tt;`

Then load data into a vector and load the vector into the object:

`vector<double> data;`  
`// ... push values into vector`

`tt.load_data(data);`

For one-sided tests you have to specify if it is a Maximum test or a minimum test. Not specifying
it will assume it is a Maximum test:

`ot.load_data(data, false);`

If you want the data after the test with all the outliers removed (The Grubbs test will be run iteratively)
you have to allocate an empty vector and provide a pointer to it. Then you can run the test:

`vector<double> resultAfterOutliersRemoved;`  

`vector<Target> outliers = tt.run(TA_0_100, &resultAfterOutliersRemoved);  //same with one-sided tests`  

Valid Significance levels are defined in the header. Use the macros as providing any other significance level will crash the code, since the critical values are hardcoded.

`// TwoSided Significance Levels`
`#define TA_0_200 0.2`  
`#define TA_0_100 0.1`  
`#define TA_0_050 0.05`  
`#define TA_0_020 0.02`  
`#define TA_0_010 0.01`  
`#define TA_0_002 0.002`  

`// OneSided Significance Levels`  
`#define OA_0_100 0.1`  
`#define OA_0_050 0.05`  
`#define OA_0_025 0.025`  
`#define OA_0_010 0.01`  
`#define OA_0_005 0.005`  
`#define OA_0_001 0.001`  

The vector containing Target objects contains all the outliers found in the data. A Target structure contains the index where the value in the data is and the value itself:

`typedef struct Target{  
    uint32_t index;  
    double value;  
} Target;`

The provided data vector itself upon which the test is run is not modified.


## Example

An example program is provided for Unix

compile with

`g++ -o example -Wall *.cpp`

or 

`g++ -o example -Wall -O2 *.cpp`


