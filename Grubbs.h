
#include <vector>
#include <unordered_map>
#include <math.h>
#include "StatUtils.h"

using namespace std;

#ifndef GRUBBS_H
#define GRUBBS_H

#define DEFAULT_ALPHA 0.05

// TwoSided Significance Levels
#define TA_0_200 0.2
#define TA_0_100 0.1
#define TA_0_050 0.05
#define TA_0_020 0.02
#define TA_0_010 0.01
#define TA_0_002 0.002

// OneSided Significance Levels
#define OA_0_100 0.1
#define OA_0_050 0.05
#define OA_0_025 0.025
#define OA_0_010 0.01
#define OA_0_005 0.005
#define OA_0_001 0.001


typedef struct Target{
    uint32_t index;
    double value;
} Target;

class GrubbsTest
{
public:
    const static double t_dist_crit_vals[][6];
    vector<double> original_data;
    
    void _delete_item(vector<double>& data, uint32_t index);
    vector<uint32_t> _get_indices(vector<double>& values);
    double _get_g_test(vector<double>& data, double alpha);
    int64_t _test_once(vector<double> data, double alpha);
    
    vector<Target> run(double alpha = DEFAULT_ALPHA, vector<double>* outliersRemoved = nullptr);
    
    virtual Target get_target(vector<double>& data) = 0;
    virtual int get_t_significance_level_column_index(double alpha) = 0;
};

class TwoSidedGrubbsTest : public GrubbsTest
{
public:
    void load_data(vector<double>& data);
    Target get_target(vector<double>& data) override;
    int get_t_significance_level_column_index(double alpha) override;
};

class OneSidedGrubbsTest: public GrubbsTest
{
public:
    bool isMaxTest;
    OneSidedGrubbsTest();
    
    void load_data(vector<double>& data, bool isMaxTest);
    Target get_target(vector<double>& data) override;
    int get_t_significance_level_column_index(double alpha) override;
    uint32_t get_index(vector<double>& data);
};

#endif /* GRUBBS_H */

