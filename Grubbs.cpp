
#include "Grubbs.h"


void GrubbsTest::_delete_item(vector<double>& data, uint32_t index) {
    if(data.size() && index < data.size()){
        auto it = data.begin();
        it += index;
        data.erase(it);
    }
}

uint32_t findIndexAtOrAfter(vector<double>& data, double value, uint32_t start)
{
    for(uint32_t i = start; i<data.size(); ++i)
    {
        if(data[i] == value) return i;
    }
    return data.size(); //trying to read from here will crash the program, I don't see any error handling in the original
}

vector<uint32_t> GrubbsTest::_get_indices(vector<double>& values) {
    vector<double> data (original_data);
    unordered_map<double, uint32_t> last_seen;
    vector<uint32_t> indices;
    for(auto it = values.begin(); it != values.end(); ++it)
    {
        double value = *it;
        uint32_t start = last_seen[value];
        uint32_t index = findIndexAtOrAfter(data, value, start);
        indices.push_back(index);
        last_seen[value] = index + 1;
    }
    return indices;
}



// Compute a significant value score
//        data: data set
//        alpha: significance level
//        return: G_test score
//maximum accurately handleable degrees of freedom is 100 (max amount of values
//is 102) anything more is treated as infinite

double GrubbsTest::_get_g_test(vector<double>& data, double alpha) {
    uint32_t n = data.size();
    uint32_t n_checked = n-3;
    if(n_checked>100) n_checked = 100;
    double t = t_dist_crit_vals[n_checked][get_t_significance_level_column_index(alpha)];
    return (((double)n-1) / sqrt((double)n)) * (sqrt( t*t / ((double)n-2 + t*t)));
}


// Perform one iteration of the Smirnov-Grubbs test.
int64_t GrubbsTest::_test_once(vector<double> data, double alpha) {
    Target target = get_target(data);
    double g = target.value / stdDev(data, true);  //Test is done on sample data
    double g_test = _get_g_test(data, alpha);
    if(g > g_test) return (int64_t) target.index;
    return -1;                                          
}


/**
 * 
 * @param alpha significance level
 * @param outliersRemoved Pointer to an array that is to be filled with the values and all outliers removed
 * if not used let it be a nullptr or just ignore it
 * @return a vector containing the removed values and their indices
 */
vector<Target> GrubbsTest::run(double alpha, vector<double>* outliersRemoved) {
    vector<double> data(original_data);
    vector<Target> outliers;
    Target t;
    while(true)
    {
        int64_t outlier_index = _test_once(data, alpha);
        if(outlier_index < 0) break;
        t.value = data[outlier_index];
        t.index = outlier_index;
        outliers.push_back(t);
        _delete_item(data, outlier_index);
    }
    
    if(outliersRemoved!= nullptr){
        outliersRemoved->assign(data.begin(), data.end());
    }
    
    return outliers;
}

void TwoSidedGrubbsTest::load_data(vector<double>& data) {
    original_data.assign(data.begin(), data.end());
}


Target TwoSidedGrubbsTest::get_target(vector<double>& data) {
    vector<double> relVals;
    Target t;
    fillRelativeDistances(data, relVals);
    t.index = findMaxElementIndex(relVals);
    t.value = relVals[t.index];
    return t;
}


int TwoSidedGrubbsTest::get_t_significance_level_column_index(double alpha) {
    double sig_array[] = {
        0.2 ,
        0.1 ,
        0.05 ,
        0.02 ,
        0.01 ,
        0.002
    };
    for(int i = 0; i<6; ++i)
    {
        if(sig_array[i] == alpha)
            return i;
    }
    return -1; //crash program if sign. level not one of the provided ones
}

OneSidedGrubbsTest::OneSidedGrubbsTest() : isMaxTest(true) {
}

void OneSidedGrubbsTest::load_data(vector<double>& data, bool isMaxTest) {
    original_data.assign(data.begin(), data.end());
    this->isMaxTest = isMaxTest;
}

Target OneSidedGrubbsTest::get_target(vector<double>& data) {
    Target t;
    t.index = get_index(data);
    t.value = data[t.index];
    t.value = abs(t.value - meanOfData(data));
    return t;
}

uint32_t OneSidedGrubbsTest::get_index(vector<double>& data) {
    if(isMaxTest) return findMaxElementIndex(data);
    return findMinElementIndex(data);
}

int OneSidedGrubbsTest::get_t_significance_level_column_index(double alpha) {
    double sig_array[] = {
        0.1 ,
        0.05 ,
        0.025 ,
        0.01 ,
        0.005 ,
        0.001
    };
    for(int i = 0; i<6; ++i)
    {
        if(sig_array[i] == alpha)
            return i;
    }
    return -1; //crash program if sign. level not one of the provided ones
}

//critical values , v = 1 has index 0 and so on
const double GrubbsTest::t_dist_crit_vals[][6] = 
/*     0.2     0.1     0.05     0.02     0.01     0.002     alpha for two sided test*/
/*     0.1     0.05    0.025    0.01     0.005    0.001     alpha for one sided test*/
/*    0.90     0.95    0.975    0.99     0.995    0.999    1-a/2 and 1-a for twosided or onesided significance levels respectively See above */
{
    {3.078,   6.314,  12.706,  31.821,  63.657,  318.313},  //v 1
    {1.886,   2.920,   4.303,   6.965,   9.925,  22.327},   //v 2
    {1.638,   2.353,   3.182,   4.541,   5.841,  10.215},   //v 3
    {1.533,   2.132,   2.776,   3.747,   4.604,   7.173},   //v 4
    {1.476,   2.015,   2.571,   3.365,   4.032,   5.893},   //v 5
    {1.440,   1.943,   2.447,   3.143,   3.707,   5.208},   //v 6
    {1.415,   1.895,   2.365,   2.998,   3.499,   4.782},   //v 7
    {1.397,   1.860,   2.306,   2.896,   3.355,   4.499},   //v 8
    {1.383,   1.833,   2.262,   2.821,   3.250,   4.296},   //v 9
    {1.372,   1.812,   2.228,   2.764,   3.169,   4.143},   //v 10
    {1.363,   1.796,   2.201,   2.718,   3.106,   4.024},   //v 11
    {1.356,   1.782,   2.179,   2.681,   3.055,   3.929},   //v 12
    {1.350,   1.771,   2.160,   2.650,   3.012,   3.852},   //v 13
    {1.345,   1.761,   2.145,   2.624,   2.977,   3.787},   //v 14
    {1.341,   1.753,   2.131,   2.602,   2.947,   3.733},   //v 15
    {1.337,   1.746,   2.120,   2.583,   2.921,   3.686},   //v 16
    {1.333,   1.740,   2.110,   2.567,   2.898,   3.646},   //v 17
    {1.330,   1.734,   2.101,   2.552,   2.878,   3.610},   //v 18
    {1.328,   1.729,   2.093,   2.539,   2.861,   3.579},   //v 19
    {1.325,   1.725,   2.086,   2.528,   2.845,   3.552},   //v 20
    {1.323,   1.721,   2.080,   2.518,   2.831,   3.527},   //v 21
    {1.321,   1.717,   2.074,   2.508,   2.819,   3.505},   //v 22
    {1.319,   1.714,   2.069,   2.500,   2.807,   3.485},   //v 23
    {1.318,   1.711,   2.064,   2.492,   2.797,   3.467},   //v 24
    {1.316,   1.708,   2.060,   2.485,   2.787,   3.450},   //v 25
    {1.315,   1.706,   2.056,   2.479,   2.779,   3.435},   //v 26
    {1.314,   1.703,   2.052,   2.473,   2.771,   3.421},   //v 27
    {1.313,   1.701,   2.048,   2.467,   2.763,   3.408},   //v 28
    {1.311,   1.699,   2.045,   2.462,   2.756,   3.396},   //v 29
    {1.310,   1.697,   2.042,   2.457,   2.750,   3.385},   //v 30
    {1.309,   1.696,   2.040,   2.453,   2.744,   3.375},   //v 31
    {1.309,   1.694,   2.037,   2.449,   2.738,   3.365},   //v 32
    {1.308,   1.692,   2.035,   2.445,   2.733,   3.356},   //v 33
    {1.307,   1.691,   2.032,   2.441,   2.728,   3.348},   //v 34
    {1.306,   1.690,   2.030,   2.438,   2.724,   3.340},   //v 35
    {1.306,   1.688,   2.028,   2.434,   2.719,   3.333},   //v 36
    {1.305,   1.687,   2.026,   2.431,   2.715,   3.326},   //v 37
    {1.304,   1.686,   2.024,   2.429,   2.712,   3.319},   //v 38
    {1.304,   1.685,   2.023,   2.426,   2.708,   3.313},   //v 39
    {1.303,   1.684,   2.021,   2.423,   2.704,   3.307},   //v 40
    {1.303,   1.683,   2.020,   2.421,   2.701,   3.301},   //v 41
    {1.302,   1.682,   2.018,   2.418,   2.698,   3.296},   //v 42
    {1.302,   1.681,   2.017,   2.416,   2.695,   3.291},   //v 43
    {1.301,   1.680,   2.015,   2.414,   2.692,   3.286},   //v 44
    {1.301,   1.679,   2.014,   2.412,   2.690,   3.281},   //v 45
    {1.300,   1.679,   2.013,   2.410,   2.687,   3.277},   //v 46
    {1.300,   1.678,   2.012,   2.408,   2.685,   3.273},   //v 47
    {1.299,   1.677,   2.011,   2.407,   2.682,   3.269},   //v 48
    {1.299,   1.677,   2.010,   2.405,   2.680,   3.265},   //v 49
    {1.299,   1.676,   2.009,   2.403,   2.678,   3.261},   //v 50
    {1.298,   1.675,   2.008,   2.402,   2.676,   3.258},   //v 51
    {1.298,   1.675,   2.007,   2.400,   2.674,   3.255},   //v 52
    {1.298,   1.674,   2.006,   2.399,   2.672,   3.251},   //v 53
    {1.297,   1.674,   2.005,   2.397,   2.670,   3.248},   //v 54
    {1.297,   1.673,   2.004,   2.396,   2.668,   3.245},   //v 55
    {1.297,   1.673,   2.003,   2.395,   2.667,   3.242},   //v 56
    {1.297,   1.672,   2.002,   2.394,   2.665,   3.239},   //v 57
    {1.296,   1.672,   2.002,   2.392,   2.663,   3.237},   //v 58
    {1.296,   1.671,   2.001,   2.391,   2.662,   3.234},   //v 59
    {1.296,   1.671,   2.000,   2.390,   2.660,   3.232},   //v 60
    {1.296,   1.670,   2.000,   2.389,   2.659,   3.229},   //v 61
    {1.295,   1.670,   1.999,   2.388,   2.657,   3.227},   //v 62
    {1.295,   1.669,   1.998,   2.387,   2.656,   3.225},   //v 63
    {1.295,   1.669,   1.998,   2.386,   2.655,   3.223},   //v 64
    {1.295,   1.669,   1.997,   2.385,   2.654,   3.220},   //v 65
    {1.295,   1.668,   1.997,   2.384,   2.652,   3.218},   //v 66
    {1.294,   1.668,   1.996,   2.383,   2.651,   3.216},   //v 67
    {1.294,   1.668,   1.995,   2.382,   2.650,   3.214},   //v 68
    {1.294,   1.667,   1.995,   2.382,   2.649,   3.213},   //v 69
    {1.294,   1.667,   1.994,   2.381,   2.648,   3.211},   //v 70
    {1.294,   1.667,   1.994,   2.380,   2.647,   3.209},   //v 71
    {1.293,   1.666,   1.993,   2.379,   2.646,   3.207},   //v 72
    {1.293,   1.666,   1.993,   2.379,   2.645,   3.206},   //v 73
    {1.293,   1.666,   1.993,   2.378,   2.644,   3.204},   //v 74
    {1.293,   1.665,   1.992,   2.377,   2.643,   3.202},   //v 75
    {1.293,   1.665,   1.992,   2.376,   2.642,   3.201},   //v 76
    {1.293,   1.665,   1.991,   2.376,   2.641,   3.199},   //v 77
    {1.292,   1.665,   1.991,   2.375,   2.640,   3.198},   //v 78
    {1.292,   1.664,   1.990,   2.374,   2.640,   3.197},   //v 79
    {1.292,   1.664,   1.990,   2.374,   2.639,   3.195},   //v 80
    {1.292,   1.664,   1.990,   2.373,   2.638,   3.194},   //v 81
    {1.292,   1.664,   1.989,   2.373,   2.637,   3.193},   //v 82
    {1.292,   1.663,   1.989,   2.372,   2.636,   3.191},   //v 83
    {1.292,   1.663,   1.989,   2.372,   2.636,   3.190},   //v 84
    {1.292,   1.663,   1.988,   2.371,   2.635,   3.189},   //v 85
    {1.291,   1.663,   1.988,   2.370,   2.634,   3.188},   //v 86
    {1.291,   1.663,   1.988,   2.370,   2.634,   3.187},   //v 87
    {1.291,   1.662,   1.987,   2.369,   2.633,   3.185},   //v 88
    {1.291,   1.662,   1.987,   2.369,   2.632,   3.184},   //v 89
    {1.291,   1.662,   1.987,   2.368,   2.632,   3.183},   //v 90
    {1.291,   1.662,   1.986,   2.368,   2.631,   3.182},   //v 91
    {1.291,   1.662,   1.986,   2.368,   2.630,   3.181},   //v 92
    {1.291,   1.661,   1.986,   2.367,   2.630,   3.180},   //v 93
    {1.291,   1.661,   1.986,   2.367,   2.629,   3.179},   //v 94
    {1.291,   1.661,   1.985,   2.366,   2.629,   3.178},   //v 95
    {1.290,   1.661,   1.985,   2.366,   2.628,   3.177},   //v 96
    {1.290,   1.661,   1.985,   2.365,   2.627,   3.176},   //v 97
    {1.290,   1.661,   1.984,   2.365,   2.627,   3.175},   //v 98
    {1.290,   1.660,   1.984,   2.365,   2.626,   3.175},   //v 99
    {1.290,   1.660,   1.984,   2.364,   2.626,   3.174},   //v 100
    {1.282,   1.645,   1.960,   2.326,   2.576,   3.090},   //v infinite
};