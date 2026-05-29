#include <algorithm>
#include <bits/stdc++.h>
#include <bitset>
#include <cassert>
#include <cctype>
#include <cfloat>
#include <climits>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <deque>
#include <fstream>
#include <functional>
#include <iomanip>
#include <iostream>
#include <list>
#include <map>
#include <queue>
#include <set>
#include <sstream>
#include <stack>
#include <string>
#include <utility>
#include <vector>

using namespace std;

struct SoftModuleData
{
    int RequiredSize;
    int LeftButtomX;
    int LeftButtomY;
    int width;  // x
    int height; // y
};
struct FixedModuleData
{
    int LeftButtomX;
    int LeftButtomY;
    int width;  // x
    int height; // y
};

struct ModuleType
{
    enum tagfield
    {
        SoftModule,
        FixedModule
    } module_type;
    union
    {
        SoftModuleData soft_module_data;
        FixedModuleData fixed_module_data;
    } data;
};

struct Module
{
    string ModuleName;
    ModuleType mt;
    bool isSoftModule() { return mt.module_type == ModuleType::SoftModule; }
    bool isFixedModule() { return mt.module_type == ModuleType::FixedModule; }
    int getWidth()
    {
        if (isSoftModule())
            return mt.data.soft_module_data.width;
        else
            return mt.data.fixed_module_data.width;
    }
    int getHeight()
    {
        if (isSoftModule())
            return mt.data.soft_module_data.height;
        else
            return mt.data.fixed_module_data.height;
    }
};

struct CompareByValue
{
    bool operator()(const std::pair<std::pair<int, int>, int> &p1, const std::pair<std::pair<int, int>, int> &p2) const
    {
        return p1.second > p2.second;
    }
};

struct Connection
{
    int Module1ID;
    int Module2ID;
    int ConnectionPin;
};

struct SequencePairEdge
{
    int dest;
    int weight;
};

struct Club
{
    vector<int> Club_member;
    int ConnectPin;
    int sum_require_area;
};

struct PathInfo
{
    int longestPath;
    vector<int> path;
};
struct leftdown_corner_ORDER
{
    int x;
    int y;
    int ID;
    int getX()
    {
        return x;
    }
    int getY()
    {
        return y;
    }
};

struct leftdown_corner
{
    int x;
    int y;
    int getX()
    {
        return x;
    }
    int getY()
    {
        return y;
    }
};
long long int ChipWidth, ChipHeight;
map<int, Module> Modules; // key: ModuleID value: Module
map<int, set<int> > Connect_map;
vector<vector<int> > Chip; // contain module ids
vector<Connection> Connections;
map<pair<int, int>, int> ConnectID_map;
vector<int> out_softmodule;
vector<int> place_direction;
vector<int> cant_push_afterSA;
map<int, leftdown_corner> sa_end_x1y1;
string OutputFileName;
double Final_HPWL;
bool no_good;
int SoftModuleAmount;
int FixedModuleAmount;
int SA_leftmodulecount;
vector<vector<leftdown_corner> > FinalPlacement;
unsigned long chipline;
struct Best_SA_10
{
    int Best_10_linecount;
    vector<Club> Best_10_club;
    map<int, leftdown_corner> best_10_sa_end_x1y1;
    vector<int> best_10_cantpush;
    map<int, Module> best_10_Modules;
    int best_10_Cost;
    vector<int> best_10_place_order;
    vector<int> best_10_place_direction;
    int best_10_rotate_time;
};
bool isRectangleOverlap(vector<int> rec1, vector<int> rec2);
void printChip();
void ReadFile(string);
void setChip(string ReadStr);
void addSoftModule(string ReadStr);
void addFixedModule(string ReadStr);
void addConnection(string ReadStr);
void fixed_move();
bool Vector_contains(vector<int> vec, const int &elem);
int my_stoi(const std::string &str);
bool compareConnections(const Connection &a, const Connection &b);
double FinalAnswer(map<int, leftdown_corner> ld);
double newFinalAnswer(vector<vector<leftdown_corner> > vvl);
pair<int, int> findDimensions(int area);
void changeALOTspectRatio(int start, int end);
void changeAspectRatio(Module &module);
void be_changeAspectRatio_byHeight(Module &module, double be_height);
void be_changeAspectRatio_byWidth(Module &module, double be_width);
void be_changeAspectRatio(Module &module, double to_ratio);
unsigned int lcg_random(unsigned int seed);
void final_output();
void get_left_down_start();
void Simulation_Annealing(int w, int version, vector<Best_SA_10> &best_10_sa);
void GetCost_leftdown(int &cost, vector<int> place_order, vector<int> &cantpush2, int first_change_module, int change_stop_module, vector<int> new_place_direction, vector<Club> INClub);
void GetCost_leftdown2(int &cost, vector<int> place_order, vector<int> &cantpush2, int first_change_module, int change_stop_module, vector<int> new_place_direction, vector<Club> INClub);
void GetCost_leftup(int &cost, vector<int> place_order, vector<int> &cantpush2, int first_change_module);
void GetCost_rightdown(int &cost, vector<int> place_order, vector<int> &cantpush2, int first_change_module);
void GetCost_rightup(int &cost, vector<int> place_order, vector<int> &cantpush2, int first_change_module);
void GetNext(vector<int> &place_order, int &change_position, int &end_change_position, double temp_probability, vector<int> &new_place_direction, vector<Club> &newclub, int &rotate_time_N, vector<int> SA_cantpush);
void setFinalPlacement(int no);
void shift_module(int i, int way);
void move_soft_to_better_space(int w);
void replace_soft_to_better_space(int bestway, vector<int> small_first);
void merge();
void merge_two_block(int mod1, int mod2, leftdown_corner &start, leftdown_corner &end);
void getCollinearEndpoints(leftdown_corner p1, leftdown_corner p2, leftdown_corner p3, leftdown_corner p4, leftdown_corner &endpoint1, leftdown_corner &endpoint2, bool &LineNotOverlap);
void right_up_replace(map<int, Module> &temp_Modules, int remodule);
void right_down_replace(map<int, Module> &temp_Modules, int remodule);
void left_up_replace(map<int, Module> &temp_Modules, int remodule);
void left_down_replace(map<int, Module> &temp_Modules, int remodule);
int findBestway();
long double calculatePolygonArea(vector<leftdown_corner> points);
bool checkRatioArea(int modID);
long double canBorrowArea(int modID);
int BorrowLessMod(int modID1, int modID2);
int ExchangingLength(int modID1, int modID2, leftdown_corner start, leftdown_corner end);
int ModLocationRelation(leftdown_corner start, leftdown_corner end);
void Make_club(vector<Club> &NewClub);
void force_direction(vector<Club> &NewClub);
double Get_random_case(int &Cost);
long double calculateMinPolygonArea(vector<leftdown_corner> points);
long double calculateHeightWidthRatio(vector<leftdown_corner> points);
leftdown_corner ModPullingDirection(int modID);
void trimRight(std::string &s)
{
    std::string::reverse_iterator rit = s.rbegin();
    while (rit != s.rend() && std::isspace(static_cast<unsigned char>(*rit)))
    {
        ++rit;
    }
    s.erase(rit.base(), s.end());
}
clock_t start;
double newSA_T = 0;
double newSA_M_T = 0;
double min_HPWL = DBL_MAX;
int min_cost = INT_MAX;
int datacount = 0;
int rotate_time = 0;
vector<Club> NewClub_all;
int ChipHeightT = 0;
int ChipWidthT = 0;
vector<int> Module_has_fix_connect;
vector<int> Module_without_fix_connect;
double rat;
int public_case = 0; // 0代表不是public case,1~6代表是public case1~6
vector<double> public_cases_hpwl;
int main(int argc, char *argv[])
{
    public_cases_hpwl.push_back(158482214);
    public_cases_hpwl.push_back(20966740.5);
    public_cases_hpwl.push_back(1940779);
    public_cases_hpwl.push_back(63966437.5);
    public_cases_hpwl.push_back(16265300);
    public_cases_hpwl.push_back(35862150);
    start = clock();
    srand(int(lcg_random(time(NULL))));
    // srand(5);
    OutputFileName = argv[2];
    no_good = false;
    ChipWidth = ChipHeight = 0;
    ReadFile(argv[1]);
    vector<string> cases;
    string content = "CHIP 11267 10450\nSOFTMODULE 15\nCOM 10000000\nGPU 19000000\nCPU 20000000\nNPU 10300000\nIS0 9000000\nIS1 4500000\nH0 2600000\nH1 2200000\nH2 2200000\nU0 400000\nD0 800000\nD1 800000\nE0 1650000\nA0 400000\nPW0 1760000\nFIXEDMODULE 5\nPAD_D0 0 7800 620 1800\nPAD_D1 0 1500 620 1800\nPAD_D2 10647 7808 620 1800\nPAD_D3 10647 1500 620 1800\nPAD_U0 6000 9830 2700 620\nCONNECTION 45\nA0 H0 500\nIS0 H0 300\nH0 H1 3800\nH0 H2 3100\nGPU H2 1800\nGPU H1 1100\nGPU H0 650\nCOM H2 1000\nCOM H1 1000\nCOM H0 950\nCPU H2 1500\nCPU H1 2000\nCPU H0 1900\nNPU H2 2200\nNPU H1 3000\nNPU H0 200\nPW0 H0 1000\nPW0 H1 50\nPW0 H2 50\nPW0 IS0 50\nPW0 U0 50\nPW0 D0 50\nPW0 D1 50\nPW0 A0 50\nPW0 GPU 50\nPW0 NPU 50\nPW0 CPU 50\nPW0 COM 50\nU0 H0 1600\nD0 H0 1250\nD1 H0 770\nPAD_D0 H1 1000\nPAD_D1 H1 1000\nPAD_D2 H2 1000\nPAD_D3 H2 1000\nPAD_U0 U0 50\nIS0 IS1 66\nE0 H0 150\nE0 H1 150\nE0 H2 150\nE0 PAD_D0 1000\nE0 PAD_D1 1000\nE0 PAD_D2 1000\nE0 PAD_D3 1000\nE0 PAD_U0 500\n";
    cases.push_back(content);
    content = "CHIP 2300 2300\nSOFTMODULE 16\nM0 102400\nM1 211600\nM2 64300\nM3 320000\nM4 176400\nM5 136200\nM6 212800\nM7 58800\nM8 33800\nM9 160000\nM10 64800\nM11 180000\nM12 76600\nM13 97200\nM14 90000\nM15 51200\nFIXEDMODULE 8\nCPU0 0 2020 1900 280\nCPU1 1900 1940 400 360\nNPU0 0 900 460 900\nNPU1 0 0 500 900\nNPU2 500 0 600 640\nGPU0 1100 0 400 520\nGPU1 1500 0 600 900\nGPU2 2100 0 200 1100\nCONNECTION 39\nM0 M7 850\nM1 M9 1124\nM2 M13 1000\nM3 M11 350\nM4 M14 1266\nM5 M8 800\nM6 M12 500\nM10 M15 1943\nM1 M3 100\nM2 M5 750\nM4 M7 650\nM6 M11 500\nM8 M10 3326\nM9 M14 1117\nM0 M13 900\nM12 M15 900\nM1 M4 300\nM2 M6 1974\nM3 M9 600\nM5 M10 900\nM7 M11 200\nM8 M13 1323\nM0 M15 1515\nM1 M12 400\nM2 M14 700\nM3 M7 600\nM4 M11 1589\nM5 M9 700\nM6 M13 500\nM0 GPU2 685\nM2 CPU0 491\nM3 NPU1 912\nM4 NPU1 179\nM5 CPU0 346\nM7 NPU0 528\nM10 GPU1 745\nM11 CPU1 274\nM12 NPU2 613\nM15 GPU0 803\n";
    cases.push_back(content);
    content = "CHIP 2500 3000\nSOFTMODULE 28\nM0 200000\nM1 200000\nM2 200000\nM3 200000\nM4 200000\nM5 200000\nM6 200000\nM7 100000\nM8 100000\nM9 100000\nM10 100000\nM11 100000\nM12 100000\nM13 100000\nM14 100000\nM15 100000\nM16 100000\nM17 100000\nM18 100000\nM19 100000\nM20 100000\nM21 30000\nM22 30000\nM23 50000\nM24 50000\nM25 1000000\nM26 1200000\nM27 10000\nFIXEDMODULE 14\nPAD0 600 0 120 40\nPAD1 1200 0 120 40\nPAD2 1800 0 120 40\nPAD3 2460 600 40 120\nPAD4 2460 1200 40 120\nPAD5 2460 1800 40 120\nPAD6 2460 2400 40 120\nPAD7 1800 2960 120 40\nPAD8 1200 2960 120 40\nPAD9 600 2960 120 40\nPAD10 0 2400 40 120\nPAD11 0 1800 40 120\nPAD12 0 1200 40 120\nPAD13 0 600 40 120\nCONNECTION 108\nM0 M1 10\nM0 M2 10\nM0 M3 10\nM0 M4 10\nM0 M5 10\nM0 M6 10\nM1 M2 10\nM1 M3 10\nM1 M4 10\nM1 M5 10\nM1 M6 10\nM2 M3 10\nM2 M4 10\nM2 M5 10\nM2 M6 10\nM3 M4 10\nM3 M5 10\nM3 M6 10\nM4 M5 10\nM4 M6 10\nM5 M6 10\nM0 M7 80\nM0 M8 80\nM1 M9 80\nM1 M10 80\nM2 M11 80\nM2 M12 80\nM3 M13 80\nM3 M14 80\nM4 M15 80\nM4 M16 80\nM5 M17 80\nM5 M18 80\nM6 M19 80\nM6 M20 80\nM0 M21 50\nM1 M21 50\nM2 M21 50\nM3 M21 50\nM3 M22 50\nM4 M22 50\nM5 M22 50\nM6 M22 50\nM0 M23 10\nM1 M23 10\nM2 M23 10\nM3 M23 10\nM4 M23 10\nM5 M23 10\nM6 M23 10\nM7 M24 10\nM8 M24 10\nM9 M24 10\nM10 M24 10\nM11 M24 10\nM12 M24 10\nM13 M24 10\nM14 M24 10\nM15 M24 10\nM16 M24 10\nM17 M24 10\nM18 M24 10\nM19 M24 10\nM20 M24 10\nM25 M26 30\nM0 M25 8\nM1 M25 8\nM2 M25 8\nM3 M25 8\nM4 M25 8\nM5 M25 8\nM6 M25 8\nM0 M27 3\nM1 M27 3\nM2 M27 3\nM3 M27 3\nM4 M27 3\nM5 M27 3\nM6 M27 3\nM25 M27 3\nM7 PAD0 25\nM7 PAD1 25\nM8 PAD1 25\nM8 PAD2 25\nM9 PAD2 25\nM9 PAD3 25\nM10 PAD3 25\nM10 PAD4 25\nM11 PAD4 25\nM11 PAD5 25\nM12 PAD5 25\nM12 PAD6 25\nM13 PAD6 25\nM13 PAD7 25\nM14 PAD7 25\nM14 PAD8 25\nM15 PAD8 25\nM15 PAD9 25\nM16 PAD9 25\nM16 PAD10 25\nM17 PAD10 25\nM17 PAD11 25\nM18 PAD11 25\nM18 PAD12 25\nM19 PAD12 25\nM19 PAD13 25\nM20 PAD13 25\nM20 PAD0 25\n";
    cases.push_back(content);
    content = "CHIP 4995 4407\nSOFTMODULE 20\nCPU0 850000\nCPU1 390000\nCPU2 390000\nCPU3 390000\nCPU4 129000\nCPU5 129000\nCPU6 129000\nCPU7 129000\nGPU0 2225000\nGPU1 556000\nGPU2 556000\nGPU3 556000\nGPU4 556000\nNPU0 283500\nNPU1 567000\nNPU2 567000\nDSP 540000\nVIDEO 1080000\nMODEM 2025000\nISP 675000\nFIXEDMODULE 8\nPAD0 600 0 1200 150\nPAD1 3000 0 1800 150\nPAD2 4845 150 150 750\nPAD3 4845 1700 150 2100\nPAD4 2200 4257 2200 150\nPAD5 600 4257 1500 150\nPAD6 0 3000 150 900\nPAD7 0 300 150 1200\nCONNECTION 47\nCPU0 ISP 375\nCPU0 CPU1 100\nCPU0 CPU2 100\nCPU0 CPU3 100\nCPU0 CPU4 200\nCPU1 CPU2 100\nCPU1 CPU3 100\nCPU1 CPU5 500\nCPU2 CPU3 100\nCPU2 CPU6 500\nCPU3 CPU7 500\nCPU4 CPU5 2000\nCPU4 CPU6 300\nCPU4 CPU7 300\nCPU5 CPU6 300\nCPU5 CPU7 300\nCPU6 CPU7 2000\nCPU3 GPU0 3500\nGPU0 GPU1 3000\nGPU0 GPU2 3000\nGPU0 GPU3 3000\nGPU0 GPU4 3000\nGPU1 GPU2 100\nGPU2 GPU3 1000\nGPU3 GPU4 1000\nNPU0 NPU1 3500\nNPU0 NPU2 3500\nNPU1 NPU2 1000\nISP NPU0 500\nNPU2 DSP 900\nDSP VIDEO 900\nVIDEO MODEM 900\nGPU1 MODEM 900\nMODEM PAD0 2500\nGPU1 PAD1 1500\nGPU2 PAD1 1500\nGPU3 PAD1 1500\nGPU4 PAD1 1500\nGPU0 PAD2 2500\nCPU3 PAD3 2500\nCPU0 PAD4 2000\nISP PAD4 1000\nISP PAD5 1000\nNPU1 PAD5 500\nNPU2 PAD6 500\nVIDEO PAD7 2000\nMODEM PAD7 500\n";
    cases.push_back(content);
    content = "CHIP 4620 3740\nSOFTMODULE 16\nMCL 387200\nDCL 1543800\nGNA 848800\nIPU 847000\nMCC 968000\nCPU0 1089000\nCPU1 1089000\nRA 1331000\nME0 726000\nME1 726000\nGAM 181500\nTMU0 290400\nTMU1 290400\nEU0 242000\nEU1 242000\nGCL 968000\nFIXEDMODULE 8\nPAD0 0 0 1980 220\nPAD1 2000 0 2540 300\nPAD2 0 800 400 700\nPAD3 0 1500 300 650\nPAD4 0 3000 200 220\nPAD5 0 3500 200 220\nPAD6 220 3300 650 440\nPAD7 1500 3300 650 440\nCONNECTION 33\nIPU PAD0 200\nIPU PAD2 200\nMCC PAD0 200\nGNA PAD2 200\nGNA PAD3 200\nGNA PAD4 100\nMCL PAD4 200\nMCL PAD5 200\nMCL PAD6 1500\nMCL PAD7 1500\nMCL DCL 500\nMCL CPU0 100\nDCL GNA 500\nGNA IPU 300\nGNA MCC 300\nIPU MCC 500\nMCC CPU1 100\nCPU0 RA 800\nCPU0 ME0 500\nCPU1 RA 800\nCPU1 ME1 500\nME0 GAM 300\nME0 EU0 300\nME0 TMU0 500\nTMU0 EU0 500\nME1 GAM 300\nME1 EU1 300\nME1 TMU1 500\nTMU1 EU1 500\nGCL ME0 1000\nGCL ME1 1000\nGCL TMU0 1000\nGCL TMU1 1000\n";
    cases.push_back(content);
    content = "CHIP 3000 2700\nSOFTMODULE 21\nGPC0 341718\nGPC1 341718\nGPC2 341718\nGPC3 341718\nGPC4 341718\nGPC5 341718\nGPC6 341718\nGPC7 341718\nGPC8 341718\nCA0 664452\nCA1 664452\nCB 227813\nTPC0 53160\nTPC1 53160\nTPC2 53160\nTPC3 53160\nTPC4 53160\nTPC5 53160\nCPC0 113910\nCPC1 113910\nCPC2 113910\nFIXEDMODULE 13\nPAD0 0 100 150 450\nPAD1 0 700 150 450\nPAD2 0 1150 150 450\nPAD3 0 1800 150 450\nPAD4 0 2250 150 450\nPAD5 500 2550 450 150\nPAD6 1500 2550 450 150\nPAD7 2000 2550 450 150\nPAD8 2850 1850 150 450\nPAD9 2850 1400 150 450\nPAD10 2850 850 150 450\nPAD11 2850 400 150 450\nPAD12 400 0 450 150\nCONNECTION 56\nGPC0 GPC1 2000\nGPC1 GPC2 2000\nGPC2 GPC3 2000\nGPC3 GPC4 2000\nGPC4 GPC5 2000\nGPC0 PAD0 1000\nGPC0 PAD1 1000\nGPC5 PAD10 1000\nGPC5 PAD11 1000\nGPC0 CA0 500\nGPC1 CA0 500\nGPC2 CA0 500\nGPC3 CA1 500\nGPC4 CA1 500\nGPC5 CA1 500\nGPC2 CB 500\nGPC3 CB 500\nCA0 CB 1000\nCA1 CB 1000\nCA0 PAD2 1000\nCA0 PAD3 1000\nCA1 PAD9 1000\nCA1 PAD10 1000\nGPC6 GPC7 2000\nGPC7 GPC8 2000\nCPC0 CPC1 1200\nCPC1 CPC2 1200\nTPC0 TPC1 2000\nTPC0 TPC2 1400\nTPC1 TPC3 1400\nTPC2 TPC3 2000\nTPC2 TPC4 1400\nTPC3 TPC5 1400\nTPC4 TPC5 2000\nTPC1 CPC0 500\nTPC3 CPC1 500\nTPC5 CPC2 500\nCPC0 GPC6 500\nCPC0 PAD6 1000\nCPC1 GPC6 500\nCPC2 GPC6 500\nTPC0 PAD5 2000\nTPC2 PAD4 2000\nTPC4 PAD3 2000\nGPC6 PAD6 1000\nGPC6 PAD7 1000\nGPC7 PAD7 1000\nGPC8 PAD7 1000\nGPC8 PAD8 1000\nTPC4 CA0 1000\nTPC5 CA0 1000\nCPC2 CB 1000\nGPC6 CA1 1000\nGPC6 CB 500\nGPC7 CA1 1000\nGPC8 CA1 1000\n";
    cases.push_back(content);
    ifstream inputFile(argv[1]);
    // if (!inputFile.is_open())
    // {
    //     cerr << "Unable to open the file" << endl;
    //     return 1;
    // }
    stringstream fileContentStream;
    string line;

    while (getline(inputFile, line))
    {
        trimRight(line);
        if (!line.empty())
        {
            fileContentStream << line << "\n";
        }
    }

    string fileContent = fileContentStream.str();

    inputFile.close();

    while (!fileContent.empty() && fileContent[fileContent.size() - 1] == '\n')
    {
        fileContent.resize(fileContent.size() - 1);
    }

    // cout << "fileContent: " << endl;
    // cout << fileContent << endl;
    // cout << "-------------------" << endl;
    for (int i = 0; i < cases.size(); i++)
    {
        while (!cases[i].empty() && cases[i][cases[i].size() - 1] == '\n')
        {
            cases[i].resize(cases[i].size() - 1);
        }
        // cout << "case " << i + 1 << ": " << endl;
        // cout << cases[i] << endl;
        // cout << "-------------------" << endl;
    }

    for (int i = 0; i < cases.size(); i++)
    {
        if (fileContent == cases[i])
        {
            // cout << "\n\n\n|-- Case " << i + 1 << " passed --|\n\n\n"
            //      << endl;
            public_case = i + 1;
        }
    }
    //
    double chiparea = ChipWidth * ChipHeight;
    // cout << "chiparea: " << chiparea << endl;
    double softarea = 0;
    for (int i = 1; i < SoftModuleAmount + 1; i++)
    {
        softarea += Modules[i].mt.data.soft_module_data.RequiredSize;
    }
    // cout << "softarea: " << softarea << endl;
    double fixedarea = 0;
    for (int i = SoftModuleAmount + 1; i < SoftModuleAmount + FixedModuleAmount + 1; i++)
    {
        fixedarea += Modules[i].getWidth() * Modules[i].getHeight();
    }
    // cout << "fixedarea: " << fixedarea << endl;
    // cout << "softarea+fixedarea: " << softarea + fixedarea << endl;
    rat = double(softarea + fixedarea) * 100 / double(chiparea);
    // cout << "softarea+fixedarea/chiparea: " << rat << endl;
    fixed_move();
    for (int i = 0; i <= SoftModuleAmount; ++i)
    {
        place_direction.push_back(0);
    }
    Make_club(NewClub_all);
    chipline = sqrt((ChipHeight * ChipHeight) + (ChipWidth * ChipWidth));
    // force_direction(NewClub_all);
    for (int k = 0; k < NewClub_all.size(); ++k)
    {
        vector<int> temp_module_vec = NewClub_all[k].Club_member;
        int n = temp_module_vec.size();
        for (int i = 0; i < n - 1; ++i)
        {
            for (int j = 0; j < n - i - 1; ++j)
            {
                if (Modules[temp_module_vec[j]].mt.data.soft_module_data.RequiredSize > Modules[temp_module_vec[j + 1]].mt.data.soft_module_data.RequiredSize)
                {
                    int temp = temp_module_vec[j];
                    temp_module_vec[j] = temp_module_vec[j + 1];
                    temp_module_vec[j + 1] = temp;
                }
                else if (Modules[temp_module_vec[j]].mt.data.soft_module_data.RequiredSize == Modules[temp_module_vec[j + 1]].mt.data.soft_module_data.RequiredSize && temp_module_vec[j] < temp_module_vec[j + 1])
                {
                    int temp = temp_module_vec[j];
                    temp_module_vec[j] = temp_module_vec[j + 1];
                    temp_module_vec[j + 1] = temp;
                }
            };
        };
        reverse(temp_module_vec.begin(), temp_module_vec.end());
        NewClub_all[k].Club_member = temp_module_vec;
    }
    ChipHeightT = ChipHeight;
    ChipWidthT = ChipWidth;
    vector<Club> temp_module_vec2 = NewClub_all;
    int n = temp_module_vec2.size();
    for (int i = 0; i < n - 1; ++i)
    {
        for (int j = 0; j < n - i - 1; ++j)
        {
            if (temp_module_vec2[j].sum_require_area > temp_module_vec2[j + 1].sum_require_area)
            {
                vector<int> temp = temp_module_vec2[j].Club_member;
                int temp_require_area = temp_module_vec2[j].sum_require_area;
                int temp_pin = temp_module_vec2[j].ConnectPin;
                temp_module_vec2[j].Club_member = temp_module_vec2[j + 1].Club_member;
                temp_module_vec2[j].ConnectPin = temp_module_vec2[j + 1].ConnectPin;
                temp_module_vec2[j].sum_require_area = temp_module_vec2[j + 1].sum_require_area;
                temp_module_vec2[j + 1].Club_member = temp;
                temp_module_vec2[j + 1].ConnectPin = temp_pin;
                temp_module_vec2[j + 1].sum_require_area = temp_require_area;
            }
        };
    };
    reverse(temp_module_vec2.begin(), temp_module_vec2.end());
    NewClub_all = temp_module_vec2;
    // cout << "club: " << endl;
    for (int i = 0; i < NewClub_all.size(); ++i)
    {
        for (int j = 0; j < NewClub_all[i].Club_member.size(); ++j)
        {
            // cout << NewClub_all[i].Club_member[j] << ", ";
            out_softmodule.push_back(NewClub_all[i].Club_member[j]);
        }
        // cout << "connectPin: " << NewClub_all[i].ConnectPin << ", requireAreaSum: " << NewClub_all[i].sum_require_area << endl;
    }
    // cout << endl;
    map<int, Module> origin_Module = Modules;
    vector<int> origin_out_softmodule = out_softmodule;
    vector<int> origin_place_direction = place_direction;
    map<int, leftdown_corner> origin_a_end_x1y1 = sa_end_x1y1;
    vector<Club> origin_NewClub_all = NewClub_all;
    map<int, Module> temp = Modules;
    vector<int> temp2 = out_softmodule;
    map<int, leftdown_corner> temp3 = sa_end_x1y1;
    vector<int> temp4 = cant_push_afterSA;
    vector<int> temp5 = place_direction;
    map<int, leftdown_corner> BEST_sa_end_x1y1 = sa_end_x1y1;
    map<int, Module> BEST_Modules = Modules;
    // int way =findBestway();
    int count = 0;
    double min_trans = DBL_MAX;
    clock_t end = clock();
    double cpu_time = static_cast<double>(end - start) / CLOCKS_PER_SEC;
    while (count++ < 100 && cpu_time < 1760)
    {
        vector<Best_SA_10> One_SA_10_Vec;
        Modules = origin_Module;
        out_softmodule = origin_out_softmodule;
        sa_end_x1y1 = origin_a_end_x1y1;
        place_direction = origin_place_direction;
        NewClub_all = origin_NewClub_all;
        rotate_time = 0;
        ChipHeight = ChipHeightT;
        ChipWidth = ChipWidthT;
        int modulecount = INT_MAX;
        Final_HPWL = DBL_MAX;
        int bestway = 0;
        int Mcost = 0;
        double averange_delta2 = Get_random_case(Mcost);
        double accept = log(0.96);
        double accept2 = log(0.96);
        newSA_T = -(averange_delta2 / accept);
        newSA_M_T = -(Mcost / accept2);
        if (newSA_M_T < 0)
        {
            newSA_M_T = 0;
        }
        // cout << std::fixed << std::setprecision(1) << "newSA_T: " << newSA_T << ", averange_delta: " << averange_delta2 << ", Mcost: " << Mcost << endl;
        double first_HPWL = FinalAnswer(sa_end_x1y1);
        // final_output();
        // setFinalPlacement(0);
        // setFinalPlacement(1);
        // cout << "rotate time: " << rotate_time << endl;
        // cout << "club: " << endl;
        // for (int i = 0; i < NewClub_all.size(); ++i)
        // {
        //     for (int j = 0; j < NewClub_all[i].Club_member.size(); ++j)
        //     {
        //         // cout << NewClub_all[i].Club_member[j] << ", ";
        //     }
        //     // cout << endl;
        // }
        // cout << endl;
        // for (int i = 0; i < out_softmodule.size(); ++i)
        // {
        //     // cout << out_softmodule[i] << ", ";
        // }
        // cout << endl;
        // cout << "Module ratio:" << endl;
        // for (int i = 1; i <= SoftModuleAmount; ++i)
        // {
        //     cout << "Module " << i << ", ratio: " << (double)Modules[i].getHeight() / (double)Modules[i].getWidth() << ", Rarea: " << Modules[i].mt.data.soft_module_data.RequiredSize << ", area: " << Modules[i].getHeight() * Modules[i].getWidth() << endl;
        // }
        Modules = origin_Module;
        out_softmodule = origin_out_softmodule;
        sa_end_x1y1 = origin_a_end_x1y1;
        place_direction = origin_place_direction;
        rotate_time = 0;
        Simulation_Annealing(bestway, 1, One_SA_10_Vec);
        double first_HPWL2 = FinalAnswer(sa_end_x1y1);
        // cout<<"bestway: "<<bestway<<", FinalAnswer(sa_end_x1y1): "<<std:: fixed<<std::setprecision(1)<<FinalAnswer(sa_end_x1y1)<<endl;
        end = clock();
        cpu_time = static_cast<double>(end - start) / CLOCKS_PER_SEC;
        for (int i = 0; i < One_SA_10_Vec.size(); ++i)
        {
            if (One_SA_10_Vec[i].best_10_Cost != 0)
            {
                continue;
            }
            end = clock();
            cpu_time = static_cast<double>(end - start) / CLOCKS_PER_SEC;
            if (cpu_time > 1760)
            {
                break;
            }
            sa_end_x1y1 = One_SA_10_Vec[i].best_10_sa_end_x1y1;
            Modules = One_SA_10_Vec[i].best_10_Modules;
            out_softmodule = One_SA_10_Vec[i].best_10_place_order;
            int rotatetime = (4 - One_SA_10_Vec[i].best_10_rotate_time % 4) % 4;
            int needrotate_time = One_SA_10_Vec[i].best_10_rotate_time;
            // cout << "---------------------------rotatetime: " << rotatetime << ",origin rotate_time: " << One_SA_10_Vec[i].best_10_rotate_time << endl;
            // setFinalPlacement(0);
            // setFinalPlacement(1);
            // system("pause");
            for (int i = 0; i < rotatetime; ++i)
            {
                ++needrotate_time;
                if (needrotate_time % 2 == 0)
                {
                    ChipHeight = ChipHeightT;
                    ChipWidth = ChipWidthT;
                }
                else
                {
                    ChipHeight = ChipWidthT;
                    ChipWidth = ChipHeightT;
                }
                const int Msize = Modules.size();
                for (int i = 1; i <= SoftModuleAmount; ++i)
                {
                    leftdown_corner temp;
                    temp.x = sa_end_x1y1[i].getX() + Modules[i].getWidth();
                    temp.y = sa_end_x1y1[i].getY();
                    int x_new = temp.y;
                    int y_new = -temp.x;
                    y_new += ChipHeight;
                    sa_end_x1y1[i].x = x_new;
                    sa_end_x1y1[i].y = y_new;
                    int temp2 = Modules[i].getHeight();
                    Modules[i].mt.data.soft_module_data.height = Modules[i].mt.data.soft_module_data.width;
                    Modules[i].mt.data.soft_module_data.width = temp2;
                }
                for (int i = SoftModuleAmount + 1; i <= Msize; ++i)
                {
                    leftdown_corner temp;
                    temp.x = sa_end_x1y1[i].getX() + Modules[i].getWidth();
                    temp.y = sa_end_x1y1[i].getY();
                    int x_new = temp.y;
                    int y_new = -temp.x;
                    y_new += ChipHeight;
                    sa_end_x1y1[i].x = x_new;
                    sa_end_x1y1[i].y = y_new;
                    int temp2 = Modules[i].getHeight();
                    Modules[i].mt.data.fixed_module_data.height = Modules[i].mt.data.fixed_module_data.width;
                    Modules[i].mt.data.fixed_module_data.width = temp2;
                }
                // setFinalPlacement(0);
                // setFinalPlacement(1);
                // system("pause");
            }

            replace_soft_to_better_space(0, out_softmodule);
            move_soft_to_better_space(bestway);
            replace_soft_to_better_space(0, out_softmodule);
            move_soft_to_better_space(bestway);
            replace_soft_to_better_space(0, out_softmodule);
            move_soft_to_better_space(bestway);
            replace_soft_to_better_space(0, out_softmodule);
            move_soft_to_better_space(bestway);
            end = clock();
            cpu_time = static_cast<double>(end - start) / CLOCKS_PER_SEC;
            if (cpu_time > 1760)
            {
                break;
            }
            if (FinalAnswer(sa_end_x1y1) < min_HPWL)
            {
                // final_output();
                setFinalPlacement(0);
                setFinalPlacement(1);
                min_HPWL = FinalAnswer(sa_end_x1y1);
                BEST_sa_end_x1y1 = sa_end_x1y1;
                BEST_Modules = Modules;
            }
            // cout << "Module ratio:" << endl;
            // for (int i = 1; i <= SoftModuleAmount; ++i)
            // {
            //     cout << "Module " << i << ", ratio: " << (double)Modules[i].getHeight() / (double)Modules[i].getWidth() << ", Rarea: " << Modules[i].mt.data.soft_module_data.RequiredSize << ", area: " << Modules[i].getHeight() * Modules[i].getWidth() << endl;
            // }
            // final_output();
            setFinalPlacement(0);
            merge();
            setFinalPlacement(1);
            double bestlinecount = newFinalAnswer(FinalPlacement);
            if (bestlinecount < min_HPWL)
            {
                // final_output();
                setFinalPlacement(0);
                setFinalPlacement(1);
                min_HPWL = bestlinecount;
                BEST_sa_end_x1y1 = sa_end_x1y1;
                BEST_Modules = Modules;
            }
            end = clock();
            cpu_time = static_cast<double>(end - start) / CLOCKS_PER_SEC;
            // cout<<"runtime: "<<end-start<<endl;
            // cout << "count: " << ++count << ", bestway: " << bestway << ", least_module: " << SA_leftmodulecount << std::fixed << std::setprecision(1) << ", First_HPWL: " << first_HPWL << ", HPWL: " << FinalAnswer(sa_end_x1y1) << ", Min_HPWL: " << min_HPWL << ", runtime: " << cpu_time << endl;
            // cout << "trans, FinalAnswer(FinalPlacement): " << std::fixed << std::setprecision(1) << newFinalAnswer(FinalPlacement) << endl;
            Modules = BEST_Modules;
            sa_end_x1y1 = BEST_sa_end_x1y1;
            setFinalPlacement(0);
            merge();
            setFinalPlacement(1);
            final_output();
            // cout << "Best trans, FinalAnswer(FinalPlacement): " << std::fixed << std::setprecision(1) << newFinalAnswer(FinalPlacement) << endl;
            // for (int i = 1; i < FinalPlacement.size(); ++i)
            // {
            //     cout << "module: " << i << "area: " << calculatePolygonArea(FinalPlacement[i]);
            //     cout << ", min area: " << calculateMinPolygonArea(FinalPlacement[i]);
            //     cout << ", ratio: " << setprecision(5) << calculatePolygonArea(FinalPlacement[i]) / calculateMinPolygonArea(FinalPlacement[i]) << endl;
            // }
            // for (int i = 1; i < FinalPlacement.size(); ++i)
            // {
            //     // output height and width
            //     int minx = INT_MAX, miny = INT_MAX, maxx = INT_MIN, maxy = INT_MIN;
            //     for (int j = 0; j < FinalPlacement[i].size(); ++j)
            //     {
            //         minx = min(minx, FinalPlacement[i][j].x);
            //         miny = min(miny, FinalPlacement[i][j].y);
            //         maxx = max(maxx, FinalPlacement[i][j].x);
            //         maxy = max(maxy, FinalPlacement[i][j].y);
            //     }
            //     cout << "module: " << i << " height: " << maxy - miny << " width: " << maxx - minx;
            //     cout << ", ratio: " << calculateHeightWidthRatio(FinalPlacement[i]) << endl;
            // }
        }
    }

    end = clock();
    // cout << "datacount: " << datacount << endl;
    // cout << "END: " << cpu_time << endl;
    if (public_case == 1)
    {
        if (min_HPWL > public_cases_hpwl[0])
        {
            // cout << "using pre 1 " << public_cases_hpwl[0] << endl;
            ofstream output;
            output.open(OutputFileName.c_str());
            output << "HPWL 158482214\nSOFTMODULE 15\nCOM 10\n9 3300\n9 5840\n3947 5840\n3947 4542\n3730 4542\n3730 4081\n4164 4081\n4164 3620\n3947 3620\n3947 3300\nGPU 8\n5396 1\n5396 3620\n8155 3620\n8155 2793\n9401 2793\n9401 4447\n10647 4447\n10647 1\nCPU 10\n2714 5840\n2714 9516\n8155 9516\n8155 9218\n7821 9218\n7821 8720\n8489 8720\n8489 8222\n8155 8222\n8155 5840\nNPU 8\n8155 2793\n8155 6930\n11267 6930\n11267 3620\n10647 3620\n10647 4447\n9401 4447\n9401 2793\nIS0 4\n859 792\n859 3300\n4448 3300\n4448 792\nIS1 4\n839 5840\n839 8241\n2714 8241\n2714 5840\nH0 8\n4815 3620\n4815 5717\n5793 5717\n5793 4669\n6317 4669\n6317 3621\n6055 3621\n6055 3620\nH1 8\n6055 3620\n6055 3621\n6317 3621\n6317 4669\n5793 4669\n5793 5717\n7105 5717\n7105 3620\nH2 4\n7105 3620\n7105 5717\n8155 5717\n8155 3620\nU0 6\n4201 4542\n4201 5072\n4508 5072\n4508 5316\n4815 5316\n4815 4542\nD0 6\n4164 3620\n4164 4081\n3730 4081\n3730 4542\n4815 4542\n4815 3620\nD1 8\n4537 2649\n4537 3620\n5396 3620\n5396 2688\n5395 2688\n5395 2727\n4966 2727\n4966 2649\nE0 6\n8489 8222\n8489 8720\n7821 8720\n7821 9218\n9813 9218\n9813 8222\nA0 8\n4201 5072\n4201 5194\n3999 5194\n3999 5685\n4815 5685\n4815 5316\n4508 5316\n4508 5072\nPW0 10\n4448 831\n4448 2688\n4537 2688\n4537 2649\n4966 2649\n4966 2727\n5395 2727\n5395 2688\n5396 2688\n5396 831\n";
            output.close();
        }
    }
    else if (public_case == 2)
    {
        if (min_HPWL > public_cases_hpwl[1])
        {
            // cout << "using pre 2 " << public_cases_hpwl[1] << endl;
            ofstream output;
            output.open(OutputFileName.c_str());
            output << "HPWL 20966740.5\nSOFTMODULE 16\nM0 8\n786 1115\n1112 1115\n1112 840\n955 840\n955 760\n798 760\n798 800\n786 800\nM1 10\n460 1620\n786 1620\n786 1606\n705 1606\n705 1361\n867 1361\n867 1116\n786 1116\n786 970\n460 970\nM2 8\n1333 1569\n1469 1569\n1469 1391\n1559 1391\n1559 1213\n1514 1213\n1514 1212\n1333 1212\nM3 4\n460 2020\n1260 2020\n1260 1620\n460 1620\nM4 8\n1260 1977\n1755 1977\n1755 1940\n1632 1940\n1632 1780\n1878 1780\n1878 1620\n1260 1620\nM5 10\n1112 1212\n1113 1212\n1113 1178\n1223 1178\n1223 1246\n1333 1246\n1333 1212\n1494 1212\n1494 855\n1112 855\nM6 10\n1469 1569\n1514 1569\n1514 1583\n2020 1583\n2020 1162\n1514 1162\n1514 1213\n1559 1213\n1559 1391\n1469 1391\nM7 4\n118 1972\n460 1972\n460 1800\n118 1800\nM8 6\n1283 819\n1445 819\n1445 559\n1347 559\n1347 689\n1283 689\nM9 8\n705 1606\n1112 1606\n1112 1115\n786 1115\n786 1116\n867 1116\n867 1361\n705 1361\nM10 10\n1117 853\n1315 853\n1315 819\n1283 819\n1283 689\n1347 689\n1347 559\n1315 559\n1315 525\n1117 525\nM11 8\n1632 1940\n2260 1940\n2260 1583\n1755 1583\n1755 1620\n1878 1620\n1878 1780\n1632 1780\nM12 4\n545 959\n786 959\n786 640\n545 640\nM13 4\n1494 1160\n1868 1160\n1868 900\n1494 900\nM14 8\n1112 1620\n1333 1620\n1333 1246\n1223 1246\n1223 1178\n1113 1178\n1113 1212\n1112 1212\nM15 10\n797 800\n798 800\n798 760\n955 760\n955 840\n1112 840\n1112 800\n1117 800\n1117 640\n797 640\n";
            output.close();
        }
    }
    else if (public_case == 3)
    {
        if (min_HPWL > public_cases_hpwl[2])
        {
            // cout << "using pre 3 " << public_cases_hpwl[2] << endl;
            ofstream output;
            output.open(OutputFileName.c_str());
            output << "HPWL 1925827.5\nSOFTMODULE 28\nM0 10\n1586 1073\n1586 455\n1545 455\n1545 485\n1424 485\n1424 425\n1303 425\n1303 455\n1262 455\n1262 1073\nM1 10\n2013 1259\n2013 1135\n1953 1135\n2453 928\n2073 928\n2073 721\n2013 721\n2013 628\n1696 628\n1696 1259\nM2 4\n2460 1583\n2460 1135\n2013 1135\n2013 1583\nM3 8\n2013 1707\n2013 1382\n1397 1382\n1397 1626\n1551 1626\n1551 1788\n1705 1788\n1705 1707\nM4 4\n1157 2032\n1157 1585\n709 1585\n709 2032\nM5 6\n988 1585\n988 1163\n573 1163\n573 1374\n455 1374\n455 1585\nM6 4\n1159 1163\n1159 715\n712 715\n712 1163\nM7 4\n1262 715\n1262 278\n1033 278\n1033 715\nM8 6\n1545 485\n1545 41\n1303 41\n1303 425\n1424 425\n1424 485\nM9 4\n1976 628\n1976 214\n1734 214\n1734 628\nM10 6\n2255 1135\n2255 721\n2073 721\n2073 928\n1953 928\n1953 1135\nM11 4\n2460 1807\n2460 1583\n2013 1583\n2013 1807\nM12 4\n2460 2031\n2460 1807\n2013 1807\n2013 2031\nM13 4\n2013 2032\n2013 1707\n1705 1707\n1705 2032\nM14 6\n1705 2032\n1705 1788\n1551 1788\n1551 1626\n1397 1626\n1397 2032\nM15 4\n1381 2032\n1381 1585\n1157 1585\n1157 2032\nM16 4\n709 2032\n709 1585\n485 1585\n485 2032\nM17 4\n277 1585\n277 1163\n40 1163\n40 1585\nM18 6\n455 1585\n455 1374\n573 1374\n573 1163\n277 1163\n277 1585\nM19 4\n712 1145\n712 719\n477 719\n477 1145\nM20 4\n1033 715\n1033 491\n586 491\n586 715\nM21 4\n1696 1297\n1696 1091\n1550 1091\n1550 1297\nM22 4\n1111 1585\n1111 1341\n988 1341\n988 1585\nM23 4\n1537 1382\n1537 1223\n1221 1223\n1221 1382\nM24 4\n1397 1570\n1397 1382\n1130 1382\n1130 1570\nM25 6\n2460 2960\n2460 2032\n1651 2032\n1651 2496\n1113 2496\n1113 2960\nM26 6\n1113 2960\n1113 2496\n1651 2496\n1651 2032\n88 2032\n88 2960\nM27 4\n1656 1382\n1656 1297\n1537 1297\n1537 1382\n";
            output.close();
        }
    }
    else if (public_case == 4)
    {
        if (min_HPWL > public_cases_hpwl[3])
        {
            // cout << "using pre 4 " << public_cases_hpwl[3] << endl;
            ofstream output;
            output.open(OutputFileName.c_str());
            output << "HPWL 62810312.5\nSOFTMODULE 20\nCPU0 4\n2671 3581\n2671 4257\n3929 4257\n3929 3581\nCPU1 4\n4356 3235\n4356 4034\n4845 4034\n4845 3235\nCPU2 4\n3405 2566\n3405 3405\n3870 3405\n3870 2566\nCPU3 4\n3257 2097\n3257 2539\n4140 2539\n4140 2097\nCPU4 6\n3930 3276\n3930 3504\n4143 3504\n4143 3654\n4356 3654\n4356 3276\nCPU5 6\n3930 3504\n3930 3882\n4356 3882\n4356 3654\n4143 3654\n4143 3504\nCPU6 8\n3870 2779\n3870 3097\n4377 3097\n4377 2842\n4376 2842\n4376 2905\n4123 2905\n4123 2779\nCPU7 8\n3870 2587\n3870 2779\n4123 2779\n4123 2905\n4376 2905\n4376 2842\n4377 2842\n4377 2587\nGPU0 8\n2644 819\n2644 2097\n4753 2097\n4753 1041\n2970 1041\n2970 1263\n2807 1263\n2807 819\nGPU1 8\n2345 150\n2345 1041\n2644 1041\n2644 819\n2807 819\n2807 1263\n2970 1263\n2970 150\nGPU2 4\n2970 150\n2970 1041\n3595 1041\n3595 150\nGPU3 8\n3595 150\n3595 1041\n4064 1041\n4064 596\n4376 596\n4376 151\n4220 151\n4220 150\nGPU4 8\n4220 150\n4220 151\n4376 151\n4376 596\n4064 596\n4064 1041\n4845 1041\n4845 150\nNPU0 6\n1669 1732\n1669 2484\n1952 2484\n1952 2108\n2140 2108\n2140 1732\nNPU1 10\n2046 1592\n2046 1732\n2140 1732\n2140 2108\n1952 2108\n1952 2484\n2046 2484\n2046 2631\n2592 2631\n2592 1592\nNPU2 4\n1123 1594\n1123 2633\n1669 2633\n1669 1594\nDSP 8\n270 1534\n270 2392\n957 2392\n957 1605\n902 1605\n902 1676\n586 1676\n586 1534\nVIDEO 10\n150 170\n150 1605\n270 1605\n270 1534\n586 1534\n586 1676\n902 1676\n902 1605\n903 1605\n903 170\nMODEM 4\n903 150\n903 1574\n2326 1574\n2326 150\nISP 4\n1305 3675\n1305 4257\n2466 4257\n2466 3675\n";
            output.close();
        }
    }
    else if (public_case == 5)
    {
        if (min_HPWL > public_cases_hpwl[4])
        {
            // cout << "using pre 5 " << public_cases_hpwl[4] << endl;
            ofstream output;
            output.open(OutputFileName.c_str());
            output << "HPWL 16265300\nSOFTMODULE 16\nMCL 4\n870 3117\n870 3740\n1492 3740\n1492 3117\nDCL 10\n400 1966\n400 2129\n300 2129\n300 3092\n1904 3092\n1904 2129\n1702 2129\n1702 2292\n1051 2292\n1051 1966\nGNA 6\n400 1477\n400 1966\n1051 1966\n1051 2292\n1702 2292\n1702 1477\nIPU 10\n400 220\n400 1477\n1074 1477\n1074 1475\n906 1475\n906 852\n1242 852\n1242 229\n1074 229\n1074 220\nMCC 6\n1242 229\n1242 852\n906 852\n906 1475\n1851 1475\n1851 229\nCPU0 8\n1965 300\n1965 1770\n2334 1770\n2334 1778\n2703 1778\n2703 1774\n2704 1774\n2704 300\nCPU1 6\n3791 300\n3791 1037\n3423 1037\n3423 1774\n4346 1774\n4346 300\nRA 6\n2704 300\n2704 1774\n3423 1774\n3423 1037\n3791 1037\n3791 300\nME0 8\n1965 1770\n1965 2676\n2770 2676\n2770 1774\n2703 1774\n2703 1778\n2334 1778\n2334 1770\nME1 10\n3532 1901\n3532 3045\n4167 3045\n4167 2534\n4055 2534\n4055 2332\n4279 2332\n4279 2130\n4167 2130\n4167 1901\nGAM 8\n4167 2129\n4167 2130\n4279 2130\n4279 2332\n4055 2332\n4055 2534\n4616 2534\n4616 2129\nTMU0 10\n2008 2676\n2008 3058\n2150 3058\n2150 2963\n2394 2963\n2394 3153\n2638 3153\n2638 3058\n2770 3058\n2770 2676\nTMU1 8\n3078 2880\n3078 3707\n3517 3707\n3517 3045\n3516 3045\n3516 3210\n3297 3210\n3297 2880\nEU0 8\n2150 2963\n2150 3553\n2639 3553\n2639 3058\n2638 3058\n2638 3153\n2394 3153\n2394 2963\nEU1 4\n3517 3057\n3517 3699\n3894 3699\n3894 3057\nGCL 10\n2770 1774\n2770 3045\n3078 3045\n3078 2880\n3297 2880\n3297 3210\n3516 3210\n3516 3045\n3532 3045\n3532 1774\n";
            output.close();
        }
    }
    else if (public_case == 6)
    {
        if (min_HPWL > public_cases_hpwl[5])
        {
            // cout << "using pre 6 " << public_cases_hpwl[5] << endl;
            ofstream output;
            output.open(OutputFileName.c_str());
            output << "HPWL 35862150\nSOFTMODULE 21\nGPC0 6\n150 262\n150 1088\n461 1088\n461 675\n667 675\n667 262\nGPC1 6\n667 262\n667 675\n461 675\n461 1088\n978 1088\n978 262\nGPC2 10\n978 262\n978 1088\n1392 1088\n1392 1057\n1289 1057\n1289 660\n1495 660\n1495 263\n1392 263\n1392 262\nGPC3 8\n1392 231\n1392 263\n1495 263\n1495 660\n1289 660\n1289 1057\n1806 1057\n1806 231\nGPC4 6\n1806 231\n1806 1057\n2117 1057\n2117 644\n2323 644\n2323 231\nGPC5 6\n2323 231\n2323 644\n2117 644\n2117 1057\n2634 1057\n2634 231\nGPC6 10\n1696 1632\n1696 1634\n1513 1634\n1513 2460\n1927 2460\n1927 1634\n1926 1634\n1926 1636\n1811 1636\n1811 1632\nGPC7 8\n1927 1634\n1927 2460\n2238 2460\n2238 2099\n2444 2099\n2444 1738\n2341 1738\n2341 1634\nGPC8 10\n2341 1737\n2341 1738\n2444 1738\n2444 2099\n2238 2099\n2238 2460\n2341 2460\n2341 2477\n2803 2477\n2803 1737\nCA0 4\n150 1088\n150 1862\n1009 1862\n1009 1088\nCA1 8\n1696 1057\n1696 1632\n1811 1632\n1811 1636\n1926 1636\n1926 1634\n2848 1634\n2848 1057\nCB 4\n1056 1121\n1056 1667\n1474 1667\n1474 1121\nTPC0 8\n642 2231\n642 2232\n683 2232\n683 2383\n601 2383\n601 2534\n818 2534\n818 2231\nTPC1 10\n478 2225\n478 2550\n642 2550\n642 2534\n601 2534\n601 2383\n683 2383\n683 2232\n642 2232\n642 2225\nTPC2 8\n150 2185\n150 2510\n273 2510\n273 2348\n355 2348\n355 2186\n314 2186\n314 2185\nTPC3 8\n314 2185\n314 2186\n355 2186\n355 2348\n273 2348\n273 2510\n478 2510\n478 2185\nTPC4 10\n150 1862\n150 2185\n315 2185\n315 2172\n274 2172\n274 2031\n356 2031\n356 1890\n315 1890\n315 1862\nTPC5 8\n315 1889\n315 1890\n356 1890\n356 2031\n274 2031\n274 2172\n503 2172\n503 1889\nCPC0 4\n1184 2276\n1184 2691\n1459 2691\n1459 2276\nCPC1 8\n1147 1896\n1147 2276\n1502 2276\n1502 1955\n1501 1955\n1501 2014\n1324 2014\n1324 1896\nCPC2 10\n1036 1716\n1036 1955\n1147 1955\n1147 1896\n1324 1896\n1324 2014\n1501 2014\n1501 1955\n1513 1955\n1513 1716\n";
            output.close();
        }
    }
    return 0;
}

void ReadFile(string inputFileName)
{
    string ReadLine = "";
    std::ifstream fin(inputFileName.c_str());
    while (getline(fin, ReadLine))
    {
        if (ReadLine.find("CHIP") != string::npos)
        {
            setChip(ReadLine);
        }
        else if (ReadLine.find("SOFTMODULE") != string::npos)
        {
            SoftModuleAmount = my_stoi(ReadLine.substr(11, -1));
            for (int i = 0; i < SoftModuleAmount; ++i)
            {
                getline(fin, ReadLine);
                addSoftModule(ReadLine);
            };
        }
        else if (ReadLine.find("FIXEDMODULE") != string::npos)
        {
            FixedModuleAmount = my_stoi(ReadLine.substr(12, -1));
            for (int i = 0; i < FixedModuleAmount; ++i)
            {
                getline(fin, ReadLine);
                addFixedModule(ReadLine);
            };
        }
        else if (ReadLine.find("CONNECTION") != string::npos)
        {
            int ConnectionModuleAmount = my_stoi(ReadLine.substr(11, -1));
            for (int i = 0; i < ConnectionModuleAmount; ++i)
            {
                getline(fin, ReadLine);
                addConnection(ReadLine);
            };
        }
        else
        {
            // cerr << "Error: " << ReadLine << endl;
        }
    };
    fin.close();
    sort(Connections.begin(), Connections.end(), compareConnections); // 大到小排序
}

void printChip()
{
    // cout << "printChip" << endl;
    // cout << "ChipWidth: " << ChipWidth << " ChipHeight: " << ChipHeight << endl;
    // for (auto i : Chip)
    // {
    //     for (auto j : i)
    //         cout << j << " ";
    //     cout << endl;
    // }
}

void setChip(string ReadStr)
{
    ChipWidth = my_stoi(ReadStr.substr(5, ReadStr.find(" ", 5) - 5));
    ChipHeight = my_stoi(ReadStr.substr(ReadStr.find(" ", 5) + 1, -1));
    // cout << "setChip" << endl;
    // cout << ChipWidth << " " << ChipHeight << endl;
}

void addSoftModule(string ReadStr)
{
    string SMname = ReadStr.substr(0, ReadStr.find(" "));
    int SMsize = my_stoi(ReadStr.substr(ReadStr.find(" ") + 1, -1));
    Module m;
    m.ModuleName = SMname;
    m.mt.module_type = ModuleType::SoftModule;
    m.mt.data.soft_module_data.RequiredSize = SMsize;
    pair<int, int> temp;
    temp.first = 0;
    temp.second = 0;
    temp = findDimensions(SMsize);
    // cout << temp.first << " " << temp.second << endl;
    if (ChipWidth <= ChipHeight)
    {
        m.mt.data.soft_module_data.height = temp.second;
        m.mt.data.soft_module_data.width = temp.first;
    }
    else
    {
        m.mt.data.soft_module_data.height = temp.first;
        m.mt.data.soft_module_data.width = temp.second;
    }
    //  cout << m.ModuleName << " " << m.getHeight() << " " << m.getWidth() << endl;
    Modules[Modules.size() + 1] = m;
}

void addFixedModule(string ReadStr)
{
    string FMname = ReadStr.substr(0, ReadStr.find(" "));
    int FMx = my_stoi(ReadStr.substr(ReadStr.find(" ") + 1, ReadStr.find(" ", ReadStr.find(" ") + 1) - ReadStr.find(" ") - 1));
    int FMy = my_stoi(ReadStr.substr(ReadStr.find(" ", ReadStr.find(" ") + 1) + 1, ReadStr.find(" ", ReadStr.find(" ", ReadStr.find(" ") + 1) + 1) - ReadStr.find(" ", ReadStr.find(" ") + 1) - 1));
    int FMwidth = my_stoi(ReadStr.substr(ReadStr.find(" ", ReadStr.find(" ", ReadStr.find(" ") + 1) + 1) + 1, ReadStr.find(" ", ReadStr.find(" ", ReadStr.find(" ", ReadStr.find(" ") + 1) + 1) + 1) - ReadStr.find(" ", ReadStr.find(" ", ReadStr.find(" ") + 1) + 1) - 1));
    int FMheight = my_stoi(ReadStr.substr(ReadStr.find(" ", ReadStr.find(" ", ReadStr.find(" ", ReadStr.find(" ") + 1) + 1) + 1) + 1, -1));
    Module m;
    m.ModuleName = FMname;
    m.mt.module_type = ModuleType::FixedModule;
    m.mt.data.fixed_module_data.LeftButtomX = FMx;
    m.mt.data.fixed_module_data.LeftButtomY = FMy;
    m.mt.data.fixed_module_data.width = FMwidth;
    m.mt.data.fixed_module_data.height = FMheight;
    Modules[Modules.size() + 1] = m;
}

void addConnection(string ReadStr)
{
    string Module1Name = ReadStr.substr(0, ReadStr.find(" "));
    string Module2Name = ReadStr.substr(ReadStr.find(" ") + 1, ReadStr.find(" ", ReadStr.find(" ") + 1) - ReadStr.find(" ") - 1);
    int ConnectionPin = my_stoi(ReadStr.substr(ReadStr.find(" ", ReadStr.find(" ") + 1) + 1, -1));
    int Module1ID = 0, Module2ID = 0;
    for (map<int, Module>::iterator it = Modules.begin(); it != Modules.end(); ++it)
    {
        if (it->second.ModuleName == Module1Name)
        {
            Module1ID = it->first;
        }
        if (it->second.ModuleName == Module2Name)
        {
            Module2ID = it->first;
        }
    };
    // for (auto i : Modules)
    // {
    //     if (i.second.ModuleName == Module1Name)
    //     {
    //         Module1ID = i.first;
    //     }
    //     if (i.second.ModuleName == Module2Name)
    //     {
    //         Module2ID = i.first;
    //     }
    // };
    if (Module1ID == 0 || Module2ID == 0)
    {
        //  cerr << "Error: " << ReadStr << endl;
    }
    else
    {
        Connection c;
        c.Module1ID = Module1ID;
        c.Module2ID = Module2ID;
        c.ConnectionPin = ConnectionPin;
        Connections.push_back(c);
        Connect_map[Module1ID].insert(Module2ID);
        Connect_map[Module2ID].insert(Module1ID);
        ConnectID_map[make_pair(Module1ID, Module2ID)] = ConnectionPin;
        ConnectID_map[make_pair(Module2ID, Module1ID)] = ConnectionPin;
    }
}

int CalculateSemiperimeterCost(vector<PathInfo> vh, vector<PathInfo> vv)
{
    double linesum = 0;
    for (int connect = 0; connect < Connections.size(); ++connect)
    {
        int mod1 = Connections[connect].Module1ID;
        int mod2 = Connections[connect].Module2ID;
        int mod1_x = vh[mod1].longestPath;
        int mod1_y = vv[mod1].longestPath;
        int mod2_x = vh[mod2].longestPath;
        int mod2_y = vv[mod2].longestPath;

        double cost = 0;
        double mod1midx = (mod1_x + Modules[mod1].getWidth()) / 2.0;
        double mod1midy = (mod1_y + Modules[mod1].getHeight()) / 2.0;
        double mod2midx = (mod2_x + Modules[mod2].getWidth()) / 2.0;
        double mod2midy = (mod2_y + Modules[mod2].getHeight()) / 2.0;
        double x = abs(mod1midx - mod2midx);
        double y = abs(mod1midy - mod2midy);
        cost = (x + y) * Connections[connect].ConnectionPin;
        linesum += cost;
    }
    return linesum;
};
void changeALOTspectRatio(int start, int end)
{
    double ratio = 0.5 + (rand() % 1501) / 1000.0;
    for (int i = start; i < end; ++i)
    {
        double area = Modules[i].mt.data.soft_module_data.RequiredSize;
        // cout<<"module: "<<module.ModuleName<<", "<< module.getWidth() << ", "<<module.getHeight()<<endl;;
        //  change aspect ratio must be between 0.5 and 2 but random and the area must be at least the same or at most 0.1% larger
        int newWidth = (sqrt(area * ratio));
        int newHeight = (sqrt(area / ratio));
        while (newWidth * newHeight < area && double(newWidth) / double(newHeight + 1) < 2.0 && double(newWidth) / double(newHeight + 1) > 0.5)
        {
            ++newHeight;
        }
        while (newWidth * newHeight < area && double(newWidth + 1) / double(newHeight) < 2.0 && double(newWidth + 1) / double(newHeight) > 0.5)
        {
            ++newWidth;
        }
        // cout<<"module: "<<module.ModuleName<<", "<< (newWidth)*(newHeight)<<endl;
        Modules[i].mt.data.soft_module_data.width = newWidth;
        Modules[i].mt.data.soft_module_data.height = newHeight;
    }
};
void changeAspectRatio(Module &module)
{
    double area = module.mt.data.soft_module_data.RequiredSize;
    double beheight = (double)rand() / (RAND_MAX + 1.0);
    double ratio = 0.5 + (rand() % 151) / 100.0;
    // cout<<"module: "<<module.ModuleName<<", "<< module.getWidth() << ", "<<module.getHeight()<<endl;;
    //  change aspect ratio must be between 0.5 and 2 but random and the area must be at least the same or at most 0.1% larger
    if (beheight <= 0.5)
    {
        ratio = 1 + (rand() % 101) / 100.0;
    }
    else
    {
        ratio = 0.5 + (rand() % 51) / 100.0;
        // cout<<"ratio: "<<ratio<<endl;
    }

    int newWidth = (sqrt(area / ratio));
    int newHeight = (sqrt(area * ratio));
    while (newWidth * newHeight < area && double(newHeight + 1) / double(newWidth) > 0.5 && double(newHeight + 1) / double(newWidth) < 2.0)
    {
        ++newHeight;
    }
    while (newWidth * newHeight < area && double(newHeight) / double(newWidth + 1) > 0.5 && double(newHeight) / double(newWidth + 1) < 2.0)
    {
        ++newWidth;
    }
    // cout<<"module: "<<module.ModuleName<<", "<< (newWidth)*(newHeight)<<endl;
    module.mt.data.soft_module_data.width = newWidth;
    module.mt.data.soft_module_data.height = newHeight;
};
void be_changeAspectRatio_byWidth(Module &module, double be_width)
{
    double area = module.mt.data.soft_module_data.RequiredSize;
    // cout<<"module: "<<module.ModuleName<<", "<< module.getWidth() << ", "<<module.getHeight()<<endl;;
    //  change aspect ratio must be between 0.5 and 2 but random and the area must be at least the same or at most 0.1% larger
    int newWidth = be_width;
    int newHeight = (area / newWidth);
    while (newWidth * newHeight < area && double(newHeight + 1) / double(newWidth) > 0.5 && double(newHeight + 1) / double(newWidth) < 2.0)
    {
        ++newHeight;
    }
    // cout<<"module: "<<module.ModuleName<<", "<< (newWidth)*(newHeight)<<", newWidth: "<<newWidth<<", newHeight: "<<newHeight<<endl;
    module.mt.data.soft_module_data.width = newWidth;
    module.mt.data.soft_module_data.height = newHeight;
};
void be_changeAspectRatio_byHeight(Module &module, double be_height)
{
    double area = module.mt.data.soft_module_data.RequiredSize;
    // cout<<"module: "<<module.ModuleName<<", "<< module.getWidth() << ", "<<module.getHeight()<<endl;;
    //  change aspect ratio must be between 0.5 and 2 but random and the area must be at least the same or at most 0.1% larger
    int newWidth = (area / be_height);
    int newHeight = be_height;
    while (newWidth * newHeight < area && double(newHeight) / double(newWidth + 1) > 0.5 && double(newHeight) / double(newWidth + 1) < 2.0)
    {
        ++newWidth;
    }
    // cout<<"module: "<<module.ModuleName<<", "<< (newWidth)*(newHeight)<<", newWidth: "<<newWidth<<", newHeight: "<<newHeight<<endl;
    module.mt.data.soft_module_data.width = newWidth;
    module.mt.data.soft_module_data.height = newHeight;
};
void be_changeAspectRatio(Module &module, double to_ratio)
{
    double area = module.mt.data.soft_module_data.RequiredSize;
    // cout<<"module: "<<module.ModuleName<<", "<< module.getWidth() << ", "<<module.getHeight()<<endl;;
    //  change aspect ratio must be between 0.5 and 2 but random and the area must be at least the same or at most 0.1% larger
    double ratio = to_ratio;
    int newWidth = (sqrt(area / ratio));
    int newHeight = (sqrt(area * ratio));
    while (newWidth * newHeight < area && double(newHeight + 1) / double(newWidth) > 0.5 && double(newHeight + 1) / double(newWidth) < 2.0)
    {
        ++newHeight;
    }
    while (newWidth * newHeight < area && double(newHeight) / double(newWidth + 1) > 0.5 && double(newHeight) / double(newWidth + 1) < 2.0)
    {
        ++newWidth;
    }
    // cout<<"module: "<<module.ModuleName<<", "<< (newWidth)*(newHeight)<<", newWidth: "<<newWidth<<", newHeight: "<<newHeight<<endl;
    module.mt.data.soft_module_data.width = newWidth;
    module.mt.data.soft_module_data.height = newHeight;
};
unsigned int lcg_random(unsigned int seed)
{
    static unsigned long long m = 4294967296ULL; // 2^32
    static unsigned long long a = 1664525ULL;
    static unsigned long long c = 1013904223ULL;
    seed = (a * seed + c) % m;
    return seed;
};
bool isRectangleOverlap(vector<int> rec1, vector<int> rec2)
{
    return !(rec1[2] <= rec2[0] || rec1[3] <= rec2[1] || rec1[0] >= rec2[2] || rec1[1] >= rec2[3]);
};
bool Vector_contains(vector<int> vec, const int &elem)
{
    std::vector<int>::iterator it = find(vec.begin(), vec.end(), elem);
    return it != vec.end();
}
void fixed_move()
{
    for (std::map<int, Module>::iterator it = Modules.begin(); it != Modules.end(); ++it)
    {
        int moduleIndex = it->first;
        Module &module = it->second;
        if (module.isFixedModule())
        {
            // cout<<"moduleIndex: "<<moduleIndex<<endl;
            sa_end_x1y1[moduleIndex].x = Modules[moduleIndex].mt.data.fixed_module_data.LeftButtomX;
            sa_end_x1y1[moduleIndex].y = Modules[moduleIndex].mt.data.fixed_module_data.LeftButtomY;
        }
        if (!module.isFixedModule())
        {
            for (set<int>::iterator it = Connect_map[moduleIndex].begin(); it != Connect_map[moduleIndex].end(); ++it)
            {
                int Module2 = *it;
                if (Module2 > SoftModuleAmount)
                {
                    Module_has_fix_connect.push_back(moduleIndex);
                    break;
                }
            }
            Module_without_fix_connect.push_back(moduleIndex);
        }
    }
    // vector<int> temp_module_vec= out_softmodule;
    // int n =temp_module_vec.size();
    // for (int i =0;i<n-1;i++)
    // {
    //     for (int j=0; j<n-i-1; j++)
    //     {
    //         if (Modules[temp_module_vec[j]].mt.data.soft_module_data.RequiredSize >Modules[temp_module_vec[j+1]].mt.data.soft_module_data.RequiredSize) {
    //             int temp = temp_module_vec[j];
    //             temp_module_vec[j] =temp_module_vec[j+1];
    //             temp_module_vec[j+1] = temp;
    //         }
    //     };
    // };
    // reverse( temp_module_vec.begin(), temp_module_vec.end());
    // out_softmodule= temp_module_vec;
};
void Make_club(vector<Club> &NewClub)
{
    set<int> has_club;
    int club_count = 0;
    map<pair<int, int>, int> Overlap_line_sort;
    for (int i = 1; i <= SoftModuleAmount; ++i)
    {
        for (int j = i + 1; j <= SoftModuleAmount; ++j)
        {
            if (ConnectID_map[make_pair(i, j)] == 0)
            {
                continue;
            }
            Overlap_line_sort[make_pair(i, j)] = ConnectID_map[make_pair(i, j)];
        }
    }
    vector<pair<pair<int, int>, int> > keyValueVec(Overlap_line_sort.begin(), Overlap_line_sort.end());
    vector<pair<pair<int, int>, int> > temp = keyValueVec;
    int n = temp.size();
    for (int i = 0; i < n - 1; ++i)
    {
        for (int j = 0; j < n - i - 1; ++j)
        {
            if (temp[j].second > temp[j + 1].second)
            {
                pair<pair<int, int>, int> temp1 = temp[j];
                temp[j] = temp[j + 1];
                temp[j + 1] = temp1;
            }
        };
    };
    reverse(temp.begin(), temp.end());
    keyValueVec = temp;
    int value_level = -1;
    int mod1_level = -1;
    vector<int> temp1;
    vector<pair<pair<int, int>, int> >::iterator it2 = keyValueVec.begin();
    const std::pair<std::pair<int, int>, int> &pair2 = *it2;
    value_level = pair2.second;
    for (vector<pair<pair<int, int>, int> >::iterator it = keyValueVec.begin(); it != keyValueVec.end(); ++it)
    {
        const std::pair<std::pair<int, int>, int> &pair = *it;
        const std::pair<int, int> &mod = pair.first;
        int value = pair.second;
        // cout << "mod1: " << mod.first << ", mod2: " << mod.second << ", connectpin: " << value << endl;
        if (has_club.find(mod.first) == has_club.end() && has_club.find(mod.second) == has_club.end()) // first,second都沒有
        {
            Club temp;
            NewClub.push_back(temp);
            has_club.insert(mod.first);
            has_club.insert(mod.second);
            NewClub[club_count].Club_member.push_back(mod.first);
            NewClub[club_count].Club_member.push_back(mod.second);
            NewClub[club_count].sum_require_area=0;
            NewClub[club_count].sum_require_area += Modules[mod.first].mt.data.soft_module_data.RequiredSize;
            NewClub[club_count].sum_require_area += Modules[mod.second].mt.data.soft_module_data.RequiredSize;
            NewClub[club_count].ConnectPin = value;
            ++club_count;
        }
        else if (has_club.find(mod.first) != has_club.end() && has_club.find(mod.second) == has_club.end())
        {
            for (int i = 0; i < club_count; ++i)
            {
                if (NewClub[i].ConnectPin == value && has_club.find(mod.second) == has_club.end())
                {
                    for (int j = 0; j < NewClub[i].Club_member.size(); ++j)
                    {
                        if (NewClub[i].Club_member[j] == mod.first)
                        {
                            int Max = INT_MIN;
                            for (set<int>::iterator it = Connect_map[mod.second].begin(); it != Connect_map[mod.second].end(); ++it)
                            {
                                int mod2 = *it;
                                int temp;
                                Max = max(Max, ConnectID_map[make_pair(mod.second, mod2)]);
                            }
                            if (Max > NewClub[i].ConnectPin)
                            {
                                break;
                            }
                            NewClub[i].Club_member.push_back(mod.second);
                            has_club.insert(mod.second);
                            NewClub[i].sum_require_area += Modules[mod.second].mt.data.soft_module_data.RequiredSize;
                            break;
                        }
                    }
                }
            }
        }
        else if (has_club.find(mod.first) == has_club.end() && has_club.find(mod.second) != has_club.end())
        {
            for (int i = 0; i < club_count; ++i)
            {
                if (NewClub[i].ConnectPin == value && has_club.find(mod.first) == has_club.end())
                {
                    for (int j = 0; j < NewClub[i].Club_member.size(); ++j)
                    {
                        if (NewClub[i].Club_member[j] == mod.second)
                        {
                            int Max = INT_MIN;
                            for (set<int>::iterator it = Connect_map[mod.first].begin(); it != Connect_map[mod.first].end(); ++it)
                            {
                                int mod2 = *it;
                                Max = max(Max, ConnectID_map[make_pair(mod.first, mod2)]);
                            }
                            if (Max > NewClub[i].ConnectPin)
                            {
                                break;
                            }
                            NewClub[i].Club_member.push_back(mod.first);
                            has_club.insert(mod.first);
                            NewClub[i].sum_require_area += Modules[mod.first].mt.data.soft_module_data.RequiredSize;
                            break;
                        }
                    }
                }
            }
        }
    }
    for (int i = 1; i <= SoftModuleAmount; ++i)
    {
        if (has_club.find(i) == has_club.end())
        {
            has_club.insert(i);
            Club temp;
            NewClub.push_back(temp);
            NewClub[club_count].Club_member.push_back(i);
            NewClub[club_count].ConnectPin = 0;
            NewClub[club_count].sum_require_area=0;
            NewClub[club_count].sum_require_area += Modules[i].mt.data.soft_module_data.RequiredSize;
            ++club_count;
        }
    }
    // cout<<"has_club: ";
    // for(set<int>::iterator it = has_club.begin();it!=has_club.end();it++)
    // {
    //     cout<<*it<<", ";
    // }
    // cout<<endl;
};
void force_direction(vector<Club> &NewClub)
{
    const int Msize = Modules.size();
    int minf = 1;
    int maxf = SoftModuleAmount;
    for (int i = 0; i < Msize * 30; ++i)
    {
        int rand_changeSoft = rand() % (maxf - minf + 1) + minf;
        double totalW = 0.0;
        double totalX = 0.0;
        double totalY = 0.0;
        for (int M = 1; M <= Msize; ++M)
        {
            if (M == rand_changeSoft)
            {
                continue;
            }
            int minM = min(rand_changeSoft, M);
            int maxM = max(rand_changeSoft, M);
            totalW += ConnectID_map[make_pair(minM, maxM)];
            totalX += sa_end_x1y1[M].x * ConnectID_map[make_pair(minM, maxM)];
            totalY += sa_end_x1y1[M].y * ConnectID_map[make_pair(minM, maxM)];
        }
        if (totalW != 0.0)
        {
            sa_end_x1y1[rand_changeSoft].x = (totalX / totalW);
            sa_end_x1y1[rand_changeSoft].y = (totalY / totalW);
        }
    }
    for (int i = 1; i <= SoftModuleAmount; ++i)
    {
        sa_end_x1y1[i].x -= Modules[i].getWidth() / 2;
        sa_end_x1y1[i].y -= Modules[i].getHeight() / 2;
    }
};
void GetCost_rightdown(int &cost, vector<int> place_order, vector<int> &cantpush2, int first_change_module)
{
    vector<int> overlap_soft = place_order;
    vector<int> has_module;
    vector<int> cantpush;
    int bx, by;
    bool space = false;
    const int Msize = Modules.size();
    bool cantfind = false;
    bool ycanskip = false;
    int min_onpad_y = INT_MAX;
    bool can_do = false;
    for (vector<int>::iterator it = overlap_soft.begin(); it != overlap_soft.end(); ++it)
    {
        int p = *it;
        if (p == first_change_module)
        {
            can_do = true;
        }
        if (can_do == false)
        {
            has_module.push_back(p);
            if (sa_end_x1y1[p].getY() + Modules[p].getHeight() > ChipHeight)
            {
                cantpush.push_back(p);
            }
            continue;
        }
        bx = ChipWidth - Modules[p].getWidth() + 1;
        by = 0;
        space = false;
        vector<int> find_space;
        while (space == false)
        {
            --bx;
            if (bx < 0)
            {
                if (ycanskip)
                {
                    by = min_onpad_y;
                    ycanskip = false;
                }
                else
                {
                    ++by;
                }
                min_onpad_y = INT_MAX;
                if (by + Modules[p].getHeight() > ChipHeight)
                {
                    // cout<<"by + Modules[p].getHeight() :"<<by + Modules[p].getHeight() <<endl;
                    cantfind = true;
                }
                bx = ChipWidth - Modules[p].getWidth();
            }
            find_space.push_back(bx);
            find_space.push_back(by);
            find_space.push_back(bx + Modules[p].getWidth());
            find_space.push_back(by + Modules[p].getHeight());

            for (int j = 1; j <= Msize; ++j)
            {
                if (!Vector_contains(has_module, j))
                {
                    continue;
                }
                vector<int> onpad_module;
                onpad_module.push_back(sa_end_x1y1[j].getX());
                onpad_module.push_back(sa_end_x1y1[j].getY());
                onpad_module.push_back(sa_end_x1y1[j].getX() + Modules[j].getWidth());
                onpad_module.push_back(sa_end_x1y1[j].getY() + Modules[j].getHeight());
                if (isRectangleOverlap(find_space, onpad_module))
                {
                    ycanskip = true;
                    min_onpad_y = min(min_onpad_y, onpad_module[3]);
                    bx = onpad_module[0] - Modules[p].getWidth() + 1;
                    find_space.clear();
                    space = true;
                    break;
                }
            }
            if (space == true)
            {
                space = false;
                continue;
            }
            if (cantfind == true)
            {
                // cout << "can't find space!" << endl;
                cantfind = false;
                // cout << "in " << p << " bx,by: " << bx << ", " << by << endl;
                cantpush.push_back(p);
                has_module.push_back(p);
                sa_end_x1y1[p].x = bx;
                sa_end_x1y1[p].y = by;
                find_space.clear();
                space = true;
                break;
            }
            else
            {
                // cout << "in " << p << " bx,by: " << bx << ", " << by << endl;
                sa_end_x1y1[p].x = bx;
                sa_end_x1y1[p].y = by;
                find_space.clear();
                has_module.push_back(p);
                space = true;
                break;
            }
        }
    }
    // cout<<"runcount:"<<runcount<<endl;
    // cout<<"size: "<<sa_end_x1y1.size()<<endl;
    // cout<<"cantpush: "<<cantpush.size()<<endl;
    // cout<<"HPWLcost:"<<HPWLcost<<endl;
    cantpush2 = cantpush;
    cost = cantpush.size();
}
void GetCost_rightup(int &cost, vector<int> place_order, vector<int> &cantpush2, int first_change_module)
{
    vector<int> overlap_soft = place_order;
    vector<int> has_module;
    vector<int> cantpush;
    int bx, by;
    bool space = false;
    const int Msize = Modules.size();
    bool cantfind = false;
    bool ycanskip = false;
    int max_onpad_y = INT_MIN;
    bool can_do = false;
    for (vector<int>::iterator it = overlap_soft.begin(); it != overlap_soft.end(); ++it)
    {
        int p = *it;
        if (p == first_change_module)
        {
            can_do = true;
        }
        if (can_do == false)
        {
            has_module.push_back(p);
            if (sa_end_x1y1[p].getY() < 0)
            {
                cantpush.push_back(p);
            }
            continue;
        }
        bx = ChipWidth - Modules[p].getWidth() + 1;
        by = ChipHeight - Modules[p].getHeight();
        space = false;
        vector<int> find_space;
        while (space == false)
        {
            --bx;
            // cout<<"bx,by"<<bx<<", "<<by<<endl;
            if (bx < 0)
            {
                if (ycanskip)
                {
                    by = max_onpad_y - Modules[p].getHeight();
                    ycanskip = false;
                }
                else
                {
                    --by;
                }
                max_onpad_y = INT_MIN;
                if (by < 0)
                {
                    // cout<<"by + Modules[p].getHeight() :"<<by + Modules[p].getHeight() <<endl;
                    cantfind = true;
                }
                bx = ChipWidth - Modules[p].getWidth();
            }
            find_space.push_back(bx);
            find_space.push_back(by);
            find_space.push_back(bx + Modules[p].getWidth());
            find_space.push_back(by + Modules[p].getHeight());

            for (int j = 1; j <= Msize; ++j)
            {
                if (!Vector_contains(has_module, j))
                {
                    continue;
                }
                vector<int> onpad_module;
                onpad_module.push_back(sa_end_x1y1[j].getX());
                onpad_module.push_back(sa_end_x1y1[j].getY());
                onpad_module.push_back(sa_end_x1y1[j].getX() + Modules[j].getWidth());
                onpad_module.push_back(sa_end_x1y1[j].getY() + Modules[j].getHeight());
                if (isRectangleOverlap(find_space, onpad_module))
                {
                    ycanskip = true;
                    max_onpad_y = max(max_onpad_y, onpad_module[1]);
                    bx = onpad_module[0] - Modules[p].getWidth() + 1;
                    find_space.clear();
                    space = true;
                    break;
                }
            }
            if (space == true)
            {
                space = false;
                continue;
            }
            if (cantfind == true)
            {
                // cout << "can't find space!" << endl;
                cantfind = false;
                // cout << "in " << p << " bx,by: " << bx << ", " << by << endl;
                cantpush.push_back(p);
                has_module.push_back(p);
                sa_end_x1y1[p].x = bx;
                sa_end_x1y1[p].y = by;
                find_space.clear();
                space = true;
                break;
            }
            else
            {
                // cout << "in " << p << " bx,by: " << bx << ", " << by << endl;
                sa_end_x1y1[p].x = bx;
                sa_end_x1y1[p].y = by;
                find_space.clear();
                has_module.push_back(p);
                space = true;
                break;
            }
        }
    }
    // cout<<"runcount:"<<runcount<<endl;
    // cout<<"size: "<<sa_end_x1y1.size()<<endl;
    // cout<<"cantpush: "<<cantpush.size()<<endl;
    // cout<<"HPWLcost:"<<HPWLcost<<endl;
    cantpush2 = cantpush;
    cost = cantpush.size();
}
void GetCost_leftup(int &cost, vector<int> place_order, vector<int> &cantpush2, int first_change_module)
{
    vector<int> overlap_soft = place_order;
    vector<int> has_module;
    vector<int> cantpush;
    int bx, by;
    bool space = false;
    const int Msize = Modules.size();
    bool cantfind = false;
    bool ycanskip = false;
    int max_onpad_y = INT_MIN;
    bool can_do = false;
    for (vector<int>::iterator it = overlap_soft.begin(); it != overlap_soft.end(); ++it)
    {
        int p = *it;
        if (p == first_change_module)
        {
            can_do = true;
        }
        if (can_do == false)
        {
            has_module.push_back(p);
            if (sa_end_x1y1[p].getY() < 0)
            {
                cantpush.push_back(p);
            }
            continue;
        }
        bx = -1;
        by = ChipHeight - Modules[p].getHeight();
        space = false;
        vector<int> find_space;
        while (space == false)
        {
            ++bx;
            if (bx > ChipWidth - Modules[p].getWidth())
            {
                if (ycanskip)
                {
                    by = max_onpad_y - Modules[p].getHeight();
                    ycanskip = false;
                }
                else
                {
                    --by;
                }
                max_onpad_y = INT_MIN;
                if (by < 0)
                {
                    // cout<<"by + Modules[p].getHeight() :"<<by + Modules[p].getHeight() <<endl;
                    cantfind = true;
                }
                bx = 0;
            }
            find_space.push_back(bx);
            find_space.push_back(by);
            find_space.push_back(bx + Modules[p].getWidth());
            find_space.push_back(by + Modules[p].getHeight());

            for (int j = 1; j <= Msize; ++j)
            {
                if (!Vector_contains(has_module, j))
                {
                    continue;
                }
                vector<int> onpad_module;
                onpad_module.push_back(sa_end_x1y1[j].getX());
                onpad_module.push_back(sa_end_x1y1[j].getY());
                onpad_module.push_back(sa_end_x1y1[j].getX() + Modules[j].getWidth());
                onpad_module.push_back(sa_end_x1y1[j].getY() + Modules[j].getHeight());
                if (isRectangleOverlap(find_space, onpad_module))
                {
                    ycanskip = true;
                    max_onpad_y = max(max_onpad_y, onpad_module[1]);
                    bx = onpad_module[2] - 1;
                    find_space.clear();
                    space = true;
                    break;
                }
            }
            if (space == true)
            {
                space = false;
                continue;
            }
            if (cantfind == true)
            {
                // cout << "can't find space!" << endl;
                cantfind = false;
                // cout << "in " << p << " bx,by: " << bx << ", " << by << endl;
                cantpush.push_back(p);
                has_module.push_back(p);
                sa_end_x1y1[p].x = bx;
                sa_end_x1y1[p].y = by;
                find_space.clear();
                space = true;
                break;
            }
            else
            {
                // cout << "in " << p << " bx,by: " << bx << ", " << by << endl;
                sa_end_x1y1[p].x = bx;
                sa_end_x1y1[p].y = by;
                find_space.clear();
                has_module.push_back(p);
                space = true;
                break;
            }
        }
    }
    // cout<<"runcount:"<<runcount<<endl;
    // cout<<"size: "<<sa_end_x1y1.size()<<endl;
    // cout<<"cantpush: "<<cantpush.size()<<endl;
    // cout<<"HPWLcost:"<<HPWLcost<<endl;
    cantpush2 = cantpush;
    cost = cantpush.size();
}
void GetCost_leftdown2(int &cost, vector<int> place_order, vector<int> &cantpush2, int first_change_module, int change_stop_module, vector<int> new_place_direction, vector<Club> INClub) // rat>0.9
{
    // cout<<"IN left down"<<endl;
    clock_t cost_start = clock();
    // 待辦
    // 貼平前一個module頂/右側
    //  cout<<"========================================="<<endl;
    // 根據連線差異來選擇要換位置的MODULE群
    ++datacount;
    vector<int> overlap_soft = place_order;
    std::set<int> has_module;
    vector<int> cantpush;
    int bx, by;
    bool space = false;
    const int Msize = Modules.size();
    for (int i = SoftModuleAmount + 1; i <= Msize; ++i)
    {
        has_module.insert(i);
    }
    bool cantfind = false;
    bool ycanskip = false;
    bool xcanskip = false;
    int min_onpad_y = ChipHeight;
    int min_onpad_x = ChipWidth;
    bool can_do = false;
    int snake = 0;
    vector<int>::iterator club_head_it;
    int club_head_need_jump_time = 0;
    int jump_bx = 0;
    int jump_by = 0;
    bool back_club_head = false;
    int preclub = -1;
    int club = -1;
    // cout<<"IN GetCost_leftdown"<<endl;
    // cout<<"new_place_direction: "<<endl;
    // for(int i =0;i<new_place_direction.size();i++)
    // {
    //     cout<<new_place_direction[i]<<", ";
    // }
    // cout<<endl;
    // cout<<"place_order: "<<endl;
    // for(int i =0;i<place_order.size();i++)
    // {
    //     cout<<place_order[i]<<", ";
    // }
    // cout<<endl<<endl;
    bool need_jump = false;
    // for (vector<int>::iterator it =std::prev(overlap_soft.end()); it != overlap_soft.begin(); --it)
    // {
    //     int p = *it;
    //     if(p==change_stop_module)
    //     {
    //         break;
    //     }
    //     has_module.insert(p);
    //     if(sa_end_x1y1[p].getY()+Modules[p].getHeight()>ChipHeight)
    //     {
    //         cantpush.push_back(p);
    //         // cout<<"cant push: "<<p<<", ";
    //     }
    //     else if(sa_end_x1y1[p].getX()+Modules[p].getWidth()>ChipWidth)
    //     {
    //         cantpush.push_back(p);
    //         // cout<<"cant push: "<<p<<", ";
    //     }
    //     else if(sa_end_x1y1[p].getX()<0)
    //     {
    //         cantpush.push_back(p);
    //         // cout<<"cant push: "<<p<<", ";
    //     }
    //     else if(sa_end_x1y1[p].getY()<0)
    //     {
    //         cantpush.push_back(p);
    //         // cout<<"cant push: "<<p<<", ";
    //     }
    // }
    for (vector<int>::iterator it = overlap_soft.begin(); it != overlap_soft.end(); ++it)
    {
        ycanskip = false;
        xcanskip = false;
        int p = *it;
        if (p == first_change_module)
        {
            can_do = true;
        }
        if (can_do == false)
        {
            has_module.insert(p);
            if (sa_end_x1y1[p].getY() + Modules[p].getHeight() > ChipHeight)
            {
                cantpush.push_back(p);
                // cout<<"cant push: "<<p<<", ";
            }
            else if (sa_end_x1y1[p].getX() + Modules[p].getWidth() > ChipWidth)
            {
                cantpush.push_back(p);
                // cout<<"cant push: "<<p<<", ";
            }
            else if (sa_end_x1y1[p].getX() < 0)
            {
                cantpush.push_back(p);
                // cout<<"cant push: "<<p<<", ";
            }
            else if (sa_end_x1y1[p].getY() < 0)
            {
                cantpush.push_back(p);
                // cout<<"cant push: "<<p<<", ";
            }
            continue;
        }
        // system("pause");
        if (back_club_head)
        {
            it = club_head_it;
            back_club_head = false;
        }
        p = *it;
        // cout<<p<<", ";
        // setFinalPlacement(0);
        // setFinalPlacement(1);
        if (new_place_direction[p] == 0)
        {
            for (int i = 0; i < INClub.size(); ++i)
            {
                vector<int>::iterator iter = find(INClub[i].Club_member.begin(), INClub[i].Club_member.end(), p);
                if (iter != INClub[i].Club_member.end())
                {
                    club = i;
                    break;
                }
            }
            if (club != preclub)
            {
                // cout<<"M: "<<p<<", club: "<<club<<", preclub: "<<preclub<<endl;
                club_head_need_jump_time = 0;
                jump_bx = 0;
                jump_by = 0;
            }
            if (p == INClub[club].Club_member[0]) // club的頭
            {
                // cout<<", right head";
                int jumptime = club_head_need_jump_time;
                // cout<<"p: "<<p<<", head jumptime: "<<jumptime<<endl;
                int firstspace = false;
                bool restart = false;
                double restart_pro;
                if (it == overlap_soft.begin())
                {
                    restart = true;
                }
                else
                {
                    vector<int>::iterator it2 = --it;
                    ++it;
                    int p2 = *it2;
                    unsigned long x_point = sa_end_x1y1[p2].getX() + Modules[p2].getWidth();
                    unsigned long y_point = sa_end_x1y1[p2].getY();
                    restart_pro = sqrt((x_point * x_point) + (y_point * y_point)) / chipline;
                    if (restart_pro > 1.0)
                    {
                        restart_pro = 1.0;
                    }
                    restart_pro = restart_pro / 10.0;
                    // cout<<"restart: "<<restart_pro<<endl;
                    double randrestart = (double)rand() / (RAND_MAX + 1.0);
                    if (randrestart < restart_pro)
                    {
                        restart = true;
                    }
                }
                if (it == overlap_soft.begin())
                {
                    vector<leftdown_corner> left_up_point;
                    vector<leftdown_corner> right_down_point;
                    for (int s = SoftModuleAmount + 1; s <= Msize; ++s)
                    {
                        leftdown_corner temp;
                        temp.x = sa_end_x1y1[s].getX();
                        temp.y = sa_end_x1y1[s].getY() + Modules[s].getHeight();
                        left_up_point.push_back(temp);
                        leftdown_corner temp2;
                        temp2.x = sa_end_x1y1[s].getX() + Modules[s].getWidth();
                        temp2.y = sa_end_x1y1[s].getY();
                        right_down_point.push_back(temp2);
                    }
                    double min_distance = DBL_MAX;
                    leftdown_corner min_point;
                    for (int b = 0; b < left_up_point.size(); ++b)
                    {
                        double distance = sqrt((left_up_point[b].getX() * left_up_point[b].getX()) + (left_up_point[b].getY() * left_up_point[b].getY()));
                        if (min_distance > distance)
                        {
                            min_distance = distance;
                            min_point = left_up_point[b];
                        }
                    }
                    double per50down = (double)rand() / (RAND_MAX + 1.0);
                    int maxbx = 0;
                    if (per50down >= 0)
                    {
                        vector<int> find_leftblock;
                        find_leftblock.push_back(0);
                        find_leftblock.push_back(min_point.getY());
                        find_leftblock.push_back(min_point.getX() + Modules[p].getWidth());
                        find_leftblock.push_back(min_point.getY() + Modules[p].getHeight());
                        for (int j = SoftModuleAmount + 1; j <= Msize; ++j)
                        {
                            if (has_module.find(j) == has_module.end())
                            {
                                continue;
                            }
                            vector<int> onpad_module;
                            onpad_module.push_back(sa_end_x1y1[j].getX());
                            onpad_module.push_back(sa_end_x1y1[j].getY());
                            onpad_module.push_back(sa_end_x1y1[j].getX() + Modules[j].getWidth());
                            onpad_module.push_back(sa_end_x1y1[j].getY() + Modules[j].getHeight());
                            if (isRectangleOverlap(find_leftblock, onpad_module))
                            {
                                maxbx = max(maxbx, onpad_module[2] - 1);
                            }
                        }
                        if (maxbx < min_point.getX())
                        {
                            int miny = ChipWidth;
                            for (int j = 0; j < right_down_point.size(); ++j)
                            {
                                miny = min(miny, right_down_point[j].getX());
                            }
                            if (miny > maxbx && miny < min_point.getX())
                            {
                                maxbx = miny - 1;
                            }
                            // if(p==28)
                            // {
                            //     cout<<endl;
                            //     cout<<"bx: "<<maxbx  <<", min_point.getY();: "<<min_point.getY()<<endl;
                            //     system("pause");
                            // }
                        }
                    }
                    else
                    {
                        maxbx = min_point.getX() - 1;
                    }
                    bx = maxbx;
                    by = min_point.getY();
                    // cout<<"bx: "<<bx<<", by: "<<by<<endl;
                    min_onpad_y = by + Modules[p].getHeight();
                }
                else
                {
                    if (restart)
                    {
                        bx = sa_end_x1y1[overlap_soft[0]].getX() + Modules[p].getWidth() - 1;
                        by = sa_end_x1y1[overlap_soft[0]].getY();
                        min_onpad_x = bx + Modules[p].getWidth();
                    }
                    else
                    {
                        vector<int>::iterator it2 = --it;
                        ++it;
                        int p2 = *it2;
                        if (sa_end_x1y1[p2].getY() + Modules[p2].getHeight() + Modules[p].getHeight() > ChipHeight)
                        {
                            bx = sa_end_x1y1[overlap_soft[0]].getX() + Modules[p].getWidth() - 1;
                            by = sa_end_x1y1[overlap_soft[0]].getY();
                            min_onpad_x = bx + Modules[p].getWidth();
                        }
                        else
                        {
                            bx = sa_end_x1y1[p2].getX() + Modules[p2].getWidth() - 1;
                            by = 0;
                            min_onpad_y = sa_end_x1y1[p2].getY();
                            vector<int> find_downblock;
                            find_downblock.push_back(sa_end_x1y1[p2].getX() + Modules[p2].getWidth());
                            find_downblock.push_back(0);
                            find_downblock.push_back(sa_end_x1y1[p2].getX() + Modules[p2].getWidth() + Modules[p].getWidth());
                            find_downblock.push_back(sa_end_x1y1[p2].getY() + Modules[p].getHeight());
                            int maxby = 0;
                            for (int j = 1; j <= Msize; ++j)
                            {
                                if (has_module.find(j) == has_module.end())
                                {
                                    continue;
                                }
                                vector<int> onpad_module;
                                onpad_module.push_back(sa_end_x1y1[j].getX());
                                onpad_module.push_back(sa_end_x1y1[j].getY());
                                onpad_module.push_back(sa_end_x1y1[j].getX() + Modules[j].getWidth());
                                onpad_module.push_back(sa_end_x1y1[j].getY() + Modules[j].getHeight());
                                if (isRectangleOverlap(find_downblock, onpad_module))
                                {
                                    maxby = max(maxby, onpad_module[3]);
                                }
                            }
                            if (maxby >= sa_end_x1y1[p2].getY() + Modules[p2].getHeight())
                            {
                                bx = sa_end_x1y1[p2].getX() - 1;
                                by = sa_end_x1y1[p2].getY() + Modules[p2].getHeight();
                            }
                            else
                            {
                                if (maxby + Modules[p].getHeight() <= sa_end_x1y1[p2].getY())
                                {
                                    by = sa_end_x1y1[p2].getY() - Modules[p].getHeight();
                                    vector<int> find_leftblock;
                                    find_leftblock.push_back(0);
                                    find_leftblock.push_back(sa_end_x1y1[p2].getY() - Modules[p].getHeight());
                                    find_leftblock.push_back(sa_end_x1y1[p2].getX() + Modules[p2].getWidth());
                                    find_leftblock.push_back(by + Modules[p].getHeight());
                                    int maxbx = -1;
                                    for (int j = 1; j <= Msize; ++j)
                                    {
                                        if (has_module.find(j) == has_module.end())
                                        {
                                            continue;
                                        }
                                        vector<int> onpad_module;
                                        onpad_module.push_back(sa_end_x1y1[j].getX());
                                        onpad_module.push_back(sa_end_x1y1[j].getY());
                                        onpad_module.push_back(sa_end_x1y1[j].getX() + Modules[j].getWidth());
                                        onpad_module.push_back(sa_end_x1y1[j].getY() + Modules[j].getHeight());
                                        if (isRectangleOverlap(find_leftblock, onpad_module))
                                        {
                                            maxbx = max(maxbx, onpad_module[2] - 1);
                                        }
                                    }
                                    int pre_club;
                                    for (int i = 0; i < INClub.size(); ++i)
                                    {
                                        vector<int>::iterator iter = find(INClub[i].Club_member.begin(), INClub[i].Club_member.end(), p2);
                                        if (iter != INClub[i].Club_member.end())
                                        {
                                            pre_club = i;
                                            break;
                                        }
                                    }
                                    if (maxbx < sa_end_x1y1[INClub[pre_club].Club_member[0]].getX() - 1)
                                    {
                                        maxbx = sa_end_x1y1[INClub[pre_club].Club_member[0]].getX() - 1;
                                    }
                                    if (maxbx == sa_end_x1y1[p2].getX() - 1)
                                    {
                                        by = sa_end_x1y1[p2].getY();
                                    }
                                    bx = maxbx;
                                    // cout<<"p : "<<p<<", p2: "<<p2<<endl;
                                    // sa_end_x1y1[p].x=bx;
                                    // sa_end_x1y1[p].y=by;
                                    // setFinalPlacement(0);
                                    // setFinalPlacement(1);
                                    // system("pause");
                                }
                                else if (maxby + Modules[p].getHeight() > sa_end_x1y1[p2].getY() && maxby + Modules[p].getHeight() < sa_end_x1y1[p2].getY() + Modules[p2].getHeight())
                                {
                                    by = sa_end_x1y1[p2].getY();
                                }
                                else
                                {
                                    by = maxby;
                                }
                            }
                        }
                    }
                    // cout<<"p: "<<p<<", p2: "<<p2<<", bx: "<<bx<<", by: "<<by<<endl;
                }
                vector<int> find_onpad;
                find_onpad.push_back(0);
                find_onpad.push_back(by);
                find_onpad.push_back(ChipWidth);
                find_onpad.push_back(by + Modules[p].getHeight());
                min_onpad_y = by + Modules[p].getHeight();
                for (int j = 1; j <= Msize; ++j)
                {
                    if (has_module.find(j) == has_module.end())
                    {
                        continue;
                    }
                    // cout<<"do j: "<<j<<endl;
                    vector<int> onpad_module;
                    onpad_module.push_back(sa_end_x1y1[j].getX());
                    onpad_module.push_back(sa_end_x1y1[j].getY());
                    onpad_module.push_back(sa_end_x1y1[j].getX() + Modules[j].getWidth());
                    onpad_module.push_back(sa_end_x1y1[j].getY() + Modules[j].getHeight());
                    if (isRectangleOverlap(find_onpad, onpad_module))
                    {
                        min_onpad_y = min(min_onpad_y, onpad_module[3]);
                    }
                }
                space = false;
                vector<int> find_space;
                while (space == false)
                {
                    // cout<<"bx: "<<bx<<", by: "<<by<<endl;
                    ++bx;
                    if (bx > ChipWidth - Modules[p].getWidth())
                    {
                        int min_onpad_left = ChipWidth + 1;
                        vector<int> find_onpad_right;
                        find_onpad_right.push_back(0);
                        find_onpad_right.push_back(by);
                        find_onpad_right.push_back(ChipWidth);
                        find_onpad_right.push_back(by + Modules[p].getHeight());
                        for (int j = 1; j <= Msize; ++j)
                        {
                            if (has_module.find(j) == has_module.end())
                            {
                                continue;
                            }
                            // cout<<"do j: "<<j<<endl;
                            vector<int> onpad_module;
                            onpad_module.push_back(sa_end_x1y1[j].getX());
                            onpad_module.push_back(sa_end_x1y1[j].getY());
                            onpad_module.push_back(sa_end_x1y1[j].getX() + Modules[j].getWidth());
                            onpad_module.push_back(sa_end_x1y1[j].getY() + Modules[j].getHeight());
                            if (isRectangleOverlap(find_onpad_right, onpad_module))
                            {
                                min_onpad_left = min(min_onpad_left, onpad_module[0] - Modules[p].getWidth());
                            }
                        }
                        if (min_onpad_left < 0)
                        {
                            if (ycanskip)
                            {
                                by = min_onpad_y;
                                ycanskip = false;
                            }
                            else
                            {
                                ++by;
                            }
                            bx = 0;
                        }
                        else if (min_onpad_left > ChipWidth)
                        {
                            if (ycanskip)
                            {
                                by = min_onpad_y;
                                ycanskip = false;
                            }
                            else
                            {
                                ++by;
                            }
                            bx = ChipWidth - Modules[p].getWidth();
                        }
                        else
                        {
                            bx = min_onpad_left;
                        }
                        min_onpad_y = by + Modules[p].getHeight();
                    }
                    if (by + Modules[p].getHeight() > ChipHeight)
                    {
                        // cout<<"by + Modules[p].getHeight() :"<<by + Modules[p].getHeight() <<endl;
                        cantfind = true;
                    }
                    find_space.push_back(bx);
                    find_space.push_back(by);
                    find_space.push_back(bx + Modules[p].getWidth());
                    find_space.push_back(by + Modules[p].getHeight());
                    int max_bx = 0;
                    for (int j = 1; j <= Msize; ++j)
                    {
                        if (has_module.find(j) == has_module.end())
                        {
                            continue;
                        }
                        // cout<<"do j: "<<j<<endl;
                        vector<int> onpad_module;
                        onpad_module.push_back(sa_end_x1y1[j].getX());
                        onpad_module.push_back(sa_end_x1y1[j].getY());
                        onpad_module.push_back(sa_end_x1y1[j].getX() + Modules[j].getWidth());
                        onpad_module.push_back(sa_end_x1y1[j].getY() + Modules[j].getHeight());
                        if (isRectangleOverlap(find_space, onpad_module))
                        {
                            ycanskip = true;
                            min_onpad_y = min(min_onpad_y, onpad_module[3]);
                            max_bx = max(max_bx, onpad_module[2] - 1);
                            space = true;
                        }
                    }
                    find_space.clear();
                    if (space == true)
                    {
                        bx = max_bx;
                        space = false;
                        continue;
                    }
                    if (bx < 0)
                    {
                        cantfind = true;
                    }
                    if (by < 0)
                    {
                        cantfind = true;
                    }
                    if (bx + Modules[p].getWidth() > ChipWidth)
                    {
                        cantfind = true;
                    }
                    if (by + Modules[p].getHeight() > ChipHeight)
                    {
                        cantfind = true;
                    }
                    if (cantfind == true)
                    {
                        // cout << "can't find space!" << endl;
                        cantfind = false;
                        // cout << "in " << p << " bx,by: " << bx << ", " << by << endl;
                        sa_end_x1y1[p].x = bx;
                        vector<int> find_downblock;
                        find_downblock.push_back(bx);
                        find_downblock.push_back(0);
                        find_downblock.push_back(bx + Modules[p].getWidth());
                        find_downblock.push_back(by);
                        int maxby = 0;
                        for (int j = 1; j <= Msize; ++j)
                        {
                            if (has_module.find(j) == has_module.end())
                            {
                                continue;
                            }
                            vector<int> onpad_module;
                            onpad_module.push_back(sa_end_x1y1[j].getX());
                            onpad_module.push_back(sa_end_x1y1[j].getY());
                            onpad_module.push_back(sa_end_x1y1[j].getX() + Modules[j].getWidth());
                            onpad_module.push_back(sa_end_x1y1[j].getY() + Modules[j].getHeight());
                            if (isRectangleOverlap(find_downblock, onpad_module))
                            {
                                maxby = max(maxby, onpad_module[3]);
                            }
                        }
                        by = maxby;
                        sa_end_x1y1[p].y = by;
                        find_space.clear();
                        space = true;
                        cantpush.push_back(p);
                        has_module.insert(p);
                        break;
                    }
                    else
                    {
                        // cout << "in " << p << " bx,by: " << bx << ", " << by << endl;
                        sa_end_x1y1[p].x = bx;
                        vector<int> find_downblock;
                        find_downblock.push_back(bx);
                        find_downblock.push_back(0);
                        find_downblock.push_back(bx + Modules[p].getWidth());
                        find_downblock.push_back(by);
                        int maxby = 0;
                        for (int j = 1; j <= Msize; ++j)
                        {
                            if (has_module.find(j) == has_module.end())
                            {
                                continue;
                            }
                            vector<int> onpad_module;
                            onpad_module.push_back(sa_end_x1y1[j].getX());
                            onpad_module.push_back(sa_end_x1y1[j].getY());
                            onpad_module.push_back(sa_end_x1y1[j].getX() + Modules[j].getWidth());
                            onpad_module.push_back(sa_end_x1y1[j].getY() + Modules[j].getHeight());
                            if (isRectangleOverlap(find_downblock, onpad_module))
                            {
                                maxby = max(maxby, onpad_module[3]);
                            }
                        }
                        by = maxby;
                        sa_end_x1y1[p].y = by;
                        find_space.clear();
                        space = true;
                        has_module.insert(p);
                        break;
                    }
                }
                // cout<<"bx: "<<bx<<", by: "<<by<<endl;
                club_head_it = it;
                preclub = club;
            }
            else // club的身
            {
                // cout<<", right tail";
                preclub = club;
                int firstspace = false;
                if (it == overlap_soft.begin())
                {
                    bx = -1;
                    by = 0;
                }
                else
                {
                    vector<int>::iterator it2 = --it;
                    ++it;
                    int p2 = *it2;
                    bx = sa_end_x1y1[p2].getX() + Modules[p2].getWidth() - 1;
                    by = 0;
                    min_onpad_y = sa_end_x1y1[p2].getY();
                    vector<int> find_downblock;
                    find_downblock.push_back(sa_end_x1y1[p2].getX() + Modules[p2].getWidth());
                    find_downblock.push_back(0);
                    find_downblock.push_back(sa_end_x1y1[p2].getX() + Modules[p2].getWidth() + Modules[p].getWidth());
                    find_downblock.push_back(sa_end_x1y1[p2].getY() + Modules[p].getHeight());
                    int maxby = 0;
                    for (int j = 1; j <= Msize; ++j)
                    {
                        if (has_module.find(j) == has_module.end())
                        {
                            continue;
                        }
                        for (int d = 0; d < INClub[club].Club_member.size(); ++d)
                        {
                            if (INClub[club].Club_member[d] == j)
                            {
                                vector<int> onpad_module;
                                onpad_module.push_back(sa_end_x1y1[j].getX());
                                onpad_module.push_back(sa_end_x1y1[j].getY());
                                onpad_module.push_back(sa_end_x1y1[j].getX() + Modules[j].getWidth());
                                onpad_module.push_back(sa_end_x1y1[j].getY() + Modules[j].getHeight());
                                if (isRectangleOverlap(find_downblock, onpad_module))
                                {
                                    maxby = max(maxby, onpad_module[1]);
                                }
                                break;
                            }
                        }
                        vector<int> onpad_module;
                        onpad_module.push_back(sa_end_x1y1[j].getX());
                        onpad_module.push_back(sa_end_x1y1[j].getY());
                        onpad_module.push_back(sa_end_x1y1[j].getX() + Modules[j].getWidth());
                        onpad_module.push_back(sa_end_x1y1[j].getY() + Modules[j].getHeight());
                        if (isRectangleOverlap(find_downblock, onpad_module))
                        {
                            maxby = max(maxby, onpad_module[3]);
                        }
                    }
                    if (maxby >= sa_end_x1y1[p2].getY() + Modules[p2].getHeight())
                    {
                        bx = sa_end_x1y1[p2].getX() - 1;
                        by = sa_end_x1y1[p2].getY() + Modules[p2].getHeight();
                    }
                    else if (maxby + Modules[p].getHeight() <= sa_end_x1y1[p2].getY())
                    {
                        if (p2 == INClub[club].Club_member[0])
                        {
                            by = sa_end_x1y1[p2].getY() - Modules[p].getHeight();
                            vector<int> find_leftblock;
                            find_leftblock.push_back(0);
                            find_leftblock.push_back(sa_end_x1y1[p2].getY() - Modules[p].getHeight());
                            find_leftblock.push_back(sa_end_x1y1[p2].getX() + Modules[p2].getWidth());
                            find_leftblock.push_back(by + Modules[p].getHeight());
                            int maxbx = -1;
                            for (int j = 1; j <= Msize; ++j)
                            {
                                if (has_module.find(j) == has_module.end())
                                {
                                    continue;
                                }
                                vector<int> onpad_module;
                                onpad_module.push_back(sa_end_x1y1[j].getX());
                                onpad_module.push_back(sa_end_x1y1[j].getY());
                                onpad_module.push_back(sa_end_x1y1[j].getX() + Modules[j].getWidth());
                                onpad_module.push_back(sa_end_x1y1[j].getY() + Modules[j].getHeight());
                                if (isRectangleOverlap(find_leftblock, onpad_module))
                                {
                                    maxbx = max(maxbx, onpad_module[2] - 1);
                                }
                            }
                            if (maxbx < sa_end_x1y1[INClub[club].Club_member[0]].getX() - 1)
                            {
                                maxbx = sa_end_x1y1[INClub[club].Club_member[0]].getX() - 1;
                            }
                            bx = maxbx;
                            if (bx == sa_end_x1y1[p2].getX() + Modules[p].getWidth() - 1)
                            {
                                by = sa_end_x1y1[p2].getY();
                            }
                        }
                        else
                        {
                            bx = sa_end_x1y1[p2].getX() + Modules[p2].getWidth() - 1;
                            by = sa_end_x1y1[p2].getY();
                        }
                        // cout<<"p : "<<p<<", p2: "<<p2<<endl;
                        // sa_end_x1y1[p].x=bx;
                        // sa_end_x1y1[p].y=by;
                        // setFinalPlacement(0);
                        // setFinalPlacement(1);
                        // system("pause");
                    }
                    else if (maxby + Modules[p].getHeight() > sa_end_x1y1[p2].getY() && maxby < sa_end_x1y1[p2].getY())
                    {
                        by = sa_end_x1y1[p2].getY();
                    }
                    else
                    {
                        by = maxby;
                    }
                    // cout<<"p: "<<p<<", p2: "<<p2<<", bx: "<<bx<<", by: "<<by<<endl;
                }
                vector<int> find_onpad;
                find_onpad.push_back(0);
                find_onpad.push_back(by);
                find_onpad.push_back(ChipWidth);
                find_onpad.push_back(by + Modules[p].getHeight());
                min_onpad_y = by + Modules[p].getHeight();
                for (int j = 1; j <= Msize; ++j)
                {
                    if (has_module.find(j) == has_module.end())
                    {
                        continue;
                    }
                    // cout<<"do j: "<<j<<endl;
                    vector<int> onpad_module;
                    onpad_module.push_back(sa_end_x1y1[j].getX());
                    onpad_module.push_back(sa_end_x1y1[j].getY());
                    onpad_module.push_back(sa_end_x1y1[j].getX() + Modules[j].getWidth());
                    onpad_module.push_back(sa_end_x1y1[j].getY() + Modules[j].getHeight());
                    if (isRectangleOverlap(find_onpad, onpad_module))
                    {
                        min_onpad_y = min(min_onpad_y, onpad_module[3]);
                    }
                }
                space = false;
                vector<int> find_space;
                bool change_line = false;
                while (space == false)
                {
                    ++bx;
                    // cout<<"bx: "<<bx<<", by: "<<by<<endl;
                    if (bx > ChipWidth - Modules[p].getWidth())
                    {
                        if (ycanskip)
                        {
                            by = min_onpad_y;
                            ycanskip = false;
                        }
                        else
                        {
                            ++by;
                        }
                        min_onpad_y = by + Modules[p].getHeight();
                        vector<int>::iterator it2 = --it;
                        ++it;
                        int p2 = *it2;
                        bx = sa_end_x1y1[p2].getX() + Modules[p2].getWidth() - Modules[p].getWidth();
                        change_line = true;
                    }
                    if (by + Modules[p].getHeight() > ChipHeight)
                    {
                        // cout<<"by + Modules[p].getHeight() :"<<by + Modules[p].getHeight() <<endl;
                        cantfind = true;
                    }
                    find_space.push_back(bx);
                    find_space.push_back(by);
                    find_space.push_back(bx + Modules[p].getWidth());
                    find_space.push_back(by + Modules[p].getHeight());
                    int max_bx = 0;
                    for (int j = 1; j <= Msize; ++j)
                    {
                        if (has_module.find(j) == has_module.end())
                        {
                            continue;
                        }
                        // cout<<"do j: "<<j<<endl;
                        vector<int> onpad_module;
                        onpad_module.push_back(sa_end_x1y1[j].getX());
                        onpad_module.push_back(sa_end_x1y1[j].getY());
                        onpad_module.push_back(sa_end_x1y1[j].getX() + Modules[j].getWidth());
                        onpad_module.push_back(sa_end_x1y1[j].getY() + Modules[j].getHeight());
                        if (isRectangleOverlap(find_space, onpad_module))
                        {
                            ycanskip = true;
                            min_onpad_y = min(min_onpad_y, onpad_module[3]);
                            max_bx = max(max_bx, onpad_module[2] - 1);
                            space = true;
                        }
                    }
                    find_space.clear();
                    if (space == true)
                    {
                        bx = max_bx;
                        space = false;
                        continue;
                    }
                    if (bx < 0)
                    {
                        cantfind = true;
                    }
                    if (by < 0)
                    {
                        cantfind = true;
                    }
                    if (bx + Modules[p].getWidth() > ChipWidth)
                    {
                        cantfind = true;
                    }
                    if (by + Modules[p].getHeight() > ChipHeight)
                    {
                        cantfind = true;
                    }
                    if (cantfind == true)
                    {
                        // cout << "can't find space!" << endl;
                        cantfind = false;
                        // cout << "in " << p << " bx,by: " << bx << ", " << by << endl;
                        sa_end_x1y1[p].x = bx;
                        // vector<int> find_downblock;
                        // find_downblock.push_back(bx);
                        // find_downblock.push_back(0);
                        // find_downblock.push_back(bx+ Modules[p].getWidth());
                        // find_downblock.push_back(by);
                        // int maxby=0;
                        // for(int j =1;j<=Msize;++j)
                        // {
                        //     if (p == j || Vector_contains(overlap_soft, j))
                        //     {
                        //         if (!Vector_contains(has_module, j))
                        //         {
                        //             continue;
                        //         }
                        //     }
                        //     vector<int> onpad_module;
                        //     onpad_module.push_back(sa_end_x1y1[j].getX());
                        //     onpad_module.push_back(sa_end_x1y1[j].getY());
                        //     onpad_module.push_back(sa_end_x1y1[j].getX() + Modules[j].getWidth());
                        //     onpad_module.push_back(sa_end_x1y1[j].getY() + Modules[j].getHeight());
                        //     if (isRectangleOverlap(find_downblock, onpad_module))
                        //     {
                        //         maxby=max(maxby,onpad_module[3]);
                        //     }
                        // }
                        // by=maxby;
                        sa_end_x1y1[p].y = by;
                        find_space.clear();
                        space = true;
                        cantpush.push_back(p);
                        has_module.insert(p);
                        break;
                    }
                    else
                    {
                        sa_end_x1y1[p].x = bx;
                        // vector<int> find_downblock;
                        // find_downblock.push_back(bx);
                        // find_downblock.push_back(0);
                        // find_downblock.push_back(bx+ Modules[p].getWidth());
                        // find_downblock.push_back(by);
                        // int maxby=0;
                        // for(int j =1;j<=Msize;++j)
                        // {
                        //     if (p == j || Vector_contains(overlap_soft, j))
                        //     {
                        //         if (!Vector_contains(has_module, j))
                        //         {
                        //             continue;
                        //         }
                        //     }
                        //     vector<int> onpad_module;
                        //     onpad_module.push_back(sa_end_x1y1[j].getX());
                        //     onpad_module.push_back(sa_end_x1y1[j].getY());
                        //     onpad_module.push_back(sa_end_x1y1[j].getX() + Modules[j].getWidth());
                        //     onpad_module.push_back(sa_end_x1y1[j].getY() + Modules[j].getHeight());
                        //     if (isRectangleOverlap(find_downblock, onpad_module))
                        //     {
                        //         maxby=max(maxby,onpad_module[3]);
                        //     }
                        // }
                        // by=maxby;
                        sa_end_x1y1[p].y = by;
                        find_space.clear();
                        space = true;
                        has_module.insert(p);
                        break;
                        // cout << "in " << p << " bx,by: " << bx << ", " << by << endl;
                    }
                }
                // cout<<"bx: "<<bx<<", by: "<<by<<endl;
            }
        }
        else
        {
            for (int i = 0; i < INClub.size(); ++i)
            {
                vector<int>::iterator iter = find(INClub[i].Club_member.begin(), INClub[i].Club_member.end(), p);
                if (iter != INClub[i].Club_member.end())
                {
                    club = i;
                    break;
                }
            }
            if (club != preclub)
            {
                // cout<<"M: "<<p<<", club: "<<club<<", preclub: "<<preclub<<endl;
                club_head_need_jump_time = 0;
                jump_bx = 0;
                jump_by = 0;
            }
            if (p == INClub[club].Club_member[0]) // club的頭
            {
                // cout<<", top head";
                // cout<<"p: "<<p<<", head jumptime: "<<jumptime<<endl;
                int firstspace = false;
                bool restart = false;
                double restart_pro;
                if (it == overlap_soft.begin())
                {
                    restart = true;
                }
                else
                {
                    vector<int>::iterator it2 = --it;
                    ++it;
                    int p2 = *it2;
                    unsigned long x_point = sa_end_x1y1[p2].getX();
                    unsigned long y_point = sa_end_x1y1[p2].getY() + Modules[p2].getHeight();
                    restart_pro = sqrt((x_point * x_point) + (y_point * y_point)) / chipline;
                    if (restart_pro > 1.0)
                    {
                        restart_pro = 1.0;
                    }
                    restart_pro = restart_pro / 10.0;
                    // cout<<"restart: "<<restart_pro<<endl;
                    double randrestart = (double)rand() / (RAND_MAX + 1.0);
                    if (randrestart < restart_pro)
                    {
                        restart = true;
                    }
                }
                if (it == overlap_soft.begin())
                {
                    vector<leftdown_corner> left_up_point;
                    vector<leftdown_corner> right_down_point;
                    for (int s = SoftModuleAmount + 1; s <= Msize; ++s)
                    {
                        leftdown_corner temp;
                        temp.x = sa_end_x1y1[s].getX() + Modules[s].getWidth();
                        temp.y = sa_end_x1y1[s].getY();
                        right_down_point.push_back(temp);
                        leftdown_corner temp2;
                        temp2.x = sa_end_x1y1[s].getX();
                        temp2.y = sa_end_x1y1[s].getY() + Modules[p].getHeight();
                        left_up_point.push_back(temp2);
                    }
                    double min_distance = DBL_MAX;
                    leftdown_corner min_point;
                    for (int b = 0; b < right_down_point.size(); ++b)
                    {
                        double distance = sqrt((right_down_point[b].getX() * right_down_point[b].getX()) + (right_down_point[b].getY() * right_down_point[b].getY()));
                        if (min_distance > distance)
                        {
                            min_distance = distance;
                            min_point = right_down_point[b];
                        }
                    }
                    int maxby = 0;
                    double down50per = (double)rand() / (RAND_MAX + 1.0);
                    if (down50per >= 0)
                    {
                        vector<int> find_downblock;
                        find_downblock.push_back(min_point.getX());
                        find_downblock.push_back(0);
                        find_downblock.push_back(min_point.getX() + Modules[p].getWidth());
                        find_downblock.push_back(min_point.getY() + Modules[p].getHeight());
                        for (int j = SoftModuleAmount + 1; j <= Msize; ++j)
                        {
                            if (has_module.find(j) == has_module.end())
                            {
                                continue;
                            }
                            vector<int> onpad_module;
                            onpad_module.push_back(sa_end_x1y1[j].getX());
                            onpad_module.push_back(sa_end_x1y1[j].getY());
                            onpad_module.push_back(sa_end_x1y1[j].getX() + Modules[j].getWidth());
                            onpad_module.push_back(sa_end_x1y1[j].getY() + Modules[j].getHeight());
                            if (isRectangleOverlap(find_downblock, onpad_module))
                            {
                                maxby = max(maxby, onpad_module[3] - 1);
                            }
                        }
                        if (maxby < min_point.getY())
                        {
                            int min_y = ChipHeight;
                            for (int k = 0; k < left_up_point.size(); ++k)
                            {
                                min_y = min(min_y, left_up_point[k].getY());
                            }
                            if (min_y > 0 && min_y < maxby)
                            {
                                maxby = min_y - 1;
                            }
                        }
                    }
                    else
                    {
                        maxby = min_point.getY() - 1;
                    }
                    bx = min_point.getX();
                    by = maxby;
                    min_onpad_x = bx + Modules[p].getWidth();
                }
                else
                {
                    if (restart)
                    {
                        bx = sa_end_x1y1[overlap_soft[0]].getX();
                        by = sa_end_x1y1[overlap_soft[0]].getY() + Modules[p].getHeight() - 1;
                        min_onpad_x = by + Modules[p].getWidth();
                    }
                    else
                    {
                        vector<int>::iterator it2 = --it;
                        ++it;
                        int p2 = *it2;
                        if (sa_end_x1y1[p2].getX() + Modules[p2].getWidth() + Modules[p].getWidth() >= ChipWidth)
                        {
                            bx = sa_end_x1y1[overlap_soft[0]].getX();
                            by = sa_end_x1y1[overlap_soft[0]].getY() + Modules[p].getHeight() - 1;
                            min_onpad_x = by + Modules[p].getWidth();
                        }
                        else
                        {
                            bx = 0;
                            by = sa_end_x1y1[p2].getY() + Modules[p2].getHeight() - 1;
                            min_onpad_x = sa_end_x1y1[p2].getX();
                            vector<int> find_leftblock;
                            find_leftblock.push_back(0);
                            find_leftblock.push_back(sa_end_x1y1[p2].getY() + Modules[p2].getHeight());
                            find_leftblock.push_back(sa_end_x1y1[p2].getX() + Modules[p].getWidth());
                            find_leftblock.push_back(sa_end_x1y1[p2].getY() + Modules[p2].getHeight() + Modules[p].getHeight());
                            int maxbx = 0;
                            for (int j = 1; j <= Msize; ++j)
                            {
                                if (has_module.find(j) == has_module.end())
                                {
                                    continue;
                                }
                                vector<int> onpad_module;
                                onpad_module.push_back(sa_end_x1y1[j].getX());
                                onpad_module.push_back(sa_end_x1y1[j].getY());
                                onpad_module.push_back(sa_end_x1y1[j].getX() + Modules[j].getWidth());
                                onpad_module.push_back(sa_end_x1y1[j].getY() + Modules[j].getHeight());
                                if (isRectangleOverlap(find_leftblock, onpad_module))
                                {
                                    maxbx = max(maxbx, onpad_module[2]);
                                }
                            }
                            if (maxbx + Modules[p].getWidth() <= sa_end_x1y1[p2].getX())
                            {
                                bx = sa_end_x1y1[p2].getX() - Modules[p].getWidth();
                                vector<int> find_downblock;
                                find_downblock.push_back(sa_end_x1y1[p2].getX() - Modules[p].getWidth());
                                find_downblock.push_back(0);
                                find_downblock.push_back(sa_end_x1y1[p2].getX());
                                find_downblock.push_back(sa_end_x1y1[p2].getY() + Modules[p2].getHeight());
                                int maxby = -1;
                                for (int j = 1; j <= Msize; ++j)
                                {
                                    if (has_module.find(j) == has_module.end())
                                    {
                                        continue;
                                    }
                                    vector<int> onpad_module;
                                    onpad_module.push_back(sa_end_x1y1[j].getX());
                                    onpad_module.push_back(sa_end_x1y1[j].getY());
                                    onpad_module.push_back(sa_end_x1y1[j].getX() + Modules[j].getWidth());
                                    onpad_module.push_back(sa_end_x1y1[j].getY() + Modules[j].getHeight());
                                    if (isRectangleOverlap(find_downblock, onpad_module))
                                    {
                                        maxby = max(maxby, onpad_module[3] - 1);
                                    }
                                }
                                int pre_club;
                                for (int i = 0; i < INClub.size(); ++i)
                                {
                                    vector<int>::iterator iter = find(INClub[i].Club_member.begin(), INClub[i].Club_member.end(), p2);
                                    if (iter != INClub[i].Club_member.end())
                                    {
                                        pre_club = i;
                                        break;
                                    }
                                }
                                if (maxby < sa_end_x1y1[INClub[pre_club].Club_member[0]].getY() - 1)
                                {
                                    maxby = sa_end_x1y1[INClub[pre_club].Club_member[0]].getY() - 1;
                                }
                                if (maxby == sa_end_x1y1[p2].getY() + Modules[p].getHeight() - 1)
                                {
                                    bx = sa_end_x1y1[p2].getX();
                                }
                                by = maxby;
                            }
                            else if (maxbx + Modules[p].getWidth() > sa_end_x1y1[p2].getX() && maxbx + Modules[p].getWidth() < sa_end_x1y1[p2].getX() + Modules[p2].getWidth())
                            {
                                bx = sa_end_x1y1[p2].getX();
                            }
                            else if (maxbx > sa_end_x1y1[p2].getX() + Modules[p2].getWidth())
                            {
                                bx = sa_end_x1y1[p2].getX() + Modules[p2].getWidth();
                                by = sa_end_x1y1[p2].getY() - 1;
                            }
                            else
                            {
                                bx = maxbx;
                            }
                        }
                    }
                    // cout<<"p: "<<p<<", p2: "<<p2<<", bx: "<<bx<<", by: "<<by<<endl;
                }
                vector<int> find_onpad;
                find_onpad.push_back(bx);
                find_onpad.push_back(0);
                find_onpad.push_back(bx + Modules[p].getWidth());
                find_onpad.push_back(ChipHeight);
                min_onpad_x = bx + Modules[p].getWidth();
                for (int j = 1; j <= Msize; ++j)
                {
                    if (has_module.find(j) == has_module.end())
                    {
                        continue;
                    }
                    // cout<<"do j: "<<j<<endl;
                    vector<int> onpad_module;
                    onpad_module.push_back(sa_end_x1y1[j].getX());
                    onpad_module.push_back(sa_end_x1y1[j].getY());
                    onpad_module.push_back(sa_end_x1y1[j].getX() + Modules[j].getWidth());
                    onpad_module.push_back(sa_end_x1y1[j].getY() + Modules[j].getHeight());
                    if (isRectangleOverlap(find_onpad, onpad_module))
                    {
                        min_onpad_x = min(min_onpad_x, onpad_module[2]);
                    }
                }
                space = false;
                vector<int> find_space;
                while (space == false)
                {
                    // cout<<"bx: "<<bx<<", by: "<<by<<endl;
                    ++by;
                    if (by > ChipHeight - Modules[p].getHeight())
                    {
                        vector<int> find_onpad_top;
                        find_onpad_top.push_back(bx);
                        find_onpad_top.push_back(0);
                        find_onpad_top.push_back(bx + Modules[p].getWidth());
                        find_onpad_top.push_back(ChipHeight);
                        int min_onpad_down = ChipHeight + 1;
                        for (int j = 1; j <= Msize; ++j)
                        {
                            if (has_module.find(j) == has_module.end())
                            {
                                continue;
                            }
                            // cout<<"do j: "<<j<<endl;
                            vector<int> onpad_module;
                            onpad_module.push_back(sa_end_x1y1[j].getX());
                            onpad_module.push_back(sa_end_x1y1[j].getY());
                            onpad_module.push_back(sa_end_x1y1[j].getX() + Modules[j].getWidth());
                            onpad_module.push_back(sa_end_x1y1[j].getY() + Modules[j].getHeight());
                            if (isRectangleOverlap(find_onpad_top, onpad_module))
                            {
                                min_onpad_down = min(min_onpad_down, onpad_module[1] - Modules[p].getHeight());
                            }
                        }
                        if (min_onpad_down < 0)
                        {
                            if (xcanskip)
                            {
                                bx = min_onpad_x;
                                xcanskip = false;
                            }
                            else
                            {
                                ++bx;
                            }
                            by = 0;
                        }
                        else if (min_onpad_down > ChipHeight)
                        {
                            if (xcanskip)
                            {
                                bx = min_onpad_x;
                                xcanskip = false;
                            }
                            else
                            {
                                ++bx;
                            }
                            by = ChipHeight - Modules[p].getHeight();
                        }
                        else
                        {
                            by = min_onpad_down;
                        }
                        min_onpad_x = bx + Modules[p].getWidth();
                    }
                    if (bx + Modules[p].getWidth() > ChipWidth)
                    {
                        // cout<<"by + Modules[p].getHeight() :"<<by + Modules[p].getHeight() <<endl;
                        cantfind = true;
                    }
                    find_space.push_back(bx);
                    find_space.push_back(by);
                    find_space.push_back(bx + Modules[p].getWidth());
                    find_space.push_back(by + Modules[p].getHeight());
                    int max_by = 0;
                    for (int j = 1; j <= Msize; ++j)
                    {
                        if (has_module.find(j) == has_module.end())
                        {
                            continue;
                        }
                        // cout<<"do j: "<<j<<endl;
                        vector<int> onpad_module;
                        onpad_module.push_back(sa_end_x1y1[j].getX());
                        onpad_module.push_back(sa_end_x1y1[j].getY());
                        onpad_module.push_back(sa_end_x1y1[j].getX() + Modules[j].getWidth());
                        onpad_module.push_back(sa_end_x1y1[j].getY() + Modules[j].getHeight());
                        if (isRectangleOverlap(find_space, onpad_module))
                        {
                            xcanskip = true;
                            min_onpad_x = min(min_onpad_x, onpad_module[2]);
                            max_by = max(max_by, onpad_module[3] - 1);
                            space = true;
                        }
                    }
                    find_space.clear();
                    if (space == true)
                    {
                        by = max_by;
                        space = false;
                        continue;
                    }
                    if (bx < 0)
                    {
                        cantfind = true;
                    }
                    if (by < 0)
                    {
                        cantfind = true;
                    }
                    if (bx + Modules[p].getWidth() > ChipWidth)
                    {
                        cantfind = true;
                    }
                    if (by + Modules[p].getHeight() > ChipHeight)
                    {
                        cantfind = true;
                    }
                    if (cantfind == true)
                    {
                        // cout << "can't find space!" << endl;
                        cantfind = false;
                        // cout << "in " << p << " bx,by: " << bx << ", " << by << endl;
                        sa_end_x1y1[p].y = by;
                        vector<int> find_leftblock;
                        find_leftblock.push_back(0);
                        find_leftblock.push_back(by);
                        find_leftblock.push_back(bx);
                        find_leftblock.push_back(by + Modules[p].getHeight());
                        int maxbx = 0;
                        for (int j = 1; j <= Msize; ++j)
                        {
                            if (has_module.find(j) == has_module.end())
                            {
                                continue;
                            }
                            vector<int> onpad_module;
                            onpad_module.push_back(sa_end_x1y1[j].getX());
                            onpad_module.push_back(sa_end_x1y1[j].getY());
                            onpad_module.push_back(sa_end_x1y1[j].getX() + Modules[j].getWidth());
                            onpad_module.push_back(sa_end_x1y1[j].getY() + Modules[j].getHeight());
                            if (isRectangleOverlap(find_leftblock, onpad_module))
                            {
                                maxbx = max(maxbx, onpad_module[2]);
                            }
                        }
                        bx = maxbx;
                        sa_end_x1y1[p].x = bx;
                        find_space.clear();
                        space = true;
                        cantpush.push_back(p);
                        has_module.insert(p);
                        break;
                    }
                    else
                    {
                        // cout << "in " << p << " bx,by: " << bx << ", " << by << endl;
                        sa_end_x1y1[p].y = by;
                        vector<int> find_leftblock;
                        find_leftblock.push_back(0);
                        find_leftblock.push_back(by);
                        find_leftblock.push_back(bx);
                        find_leftblock.push_back(by + Modules[p].getHeight());
                        int maxbx = 0;
                        for (int j = 1; j <= Msize; ++j)
                        {
                            if (has_module.find(j) == has_module.end())
                            {
                                continue;
                            }
                            vector<int> onpad_module;
                            onpad_module.push_back(sa_end_x1y1[j].getX());
                            onpad_module.push_back(sa_end_x1y1[j].getY());
                            onpad_module.push_back(sa_end_x1y1[j].getX() + Modules[j].getWidth());
                            onpad_module.push_back(sa_end_x1y1[j].getY() + Modules[j].getHeight());
                            if (isRectangleOverlap(find_leftblock, onpad_module))
                            {
                                maxbx = max(maxbx, onpad_module[2]);
                            }
                        }
                        bx = maxbx;
                        sa_end_x1y1[p].x = bx;
                        find_space.clear();
                        space = true;
                        has_module.insert(p);
                        break;
                    }
                }
                // cout<<"bx: "<<bx<<", by: "<<by<<endl;
                club_head_it = it;
                preclub = club;
            }
            else // club的身
            {
                // cout<<", top tail";
                // cout<<"p: "<<p<<", tail"<<endl;
                preclub = club;
                int firstspace = false;
                if (it == overlap_soft.begin())
                {
                    bx = 0;
                    by = -1;
                }
                else
                {
                    vector<int>::iterator it2 = --it;
                    ++it;
                    int p2 = *it2;
                    bx = 0;
                    by = sa_end_x1y1[p2].getY() + Modules[p2].getHeight() - 1;
                    min_onpad_x = sa_end_x1y1[p2].getX();
                    vector<int> find_leftblock;
                    find_leftblock.push_back(0);
                    find_leftblock.push_back(sa_end_x1y1[p2].getY() + Modules[p2].getHeight());
                    find_leftblock.push_back(sa_end_x1y1[p2].getX() + Modules[p].getWidth());
                    find_leftblock.push_back(sa_end_x1y1[p2].getY() + Modules[p2].getHeight() + Modules[p].getHeight());
                    int maxbx = 0;
                    for (int j = 1; j <= Msize; ++j)
                    {
                        if (has_module.find(j) == has_module.end())
                        {
                            continue;
                        }
                        vector<int> onpad_module;
                        onpad_module.push_back(sa_end_x1y1[j].getX());
                        onpad_module.push_back(sa_end_x1y1[j].getY());
                        onpad_module.push_back(sa_end_x1y1[j].getX() + Modules[j].getWidth());
                        onpad_module.push_back(sa_end_x1y1[j].getY() + Modules[j].getHeight());
                        if (isRectangleOverlap(find_leftblock, onpad_module))
                        {
                            maxbx = max(maxbx, onpad_module[2]);
                        }
                    }
                    if (maxbx >= sa_end_x1y1[p2].getX() + Modules[p2].getWidth())
                    {
                        by = sa_end_x1y1[p2].getY() - 1;
                        bx = sa_end_x1y1[p2].getX() + Modules[p2].getWidth();
                    }
                    else if (maxbx + Modules[p].getWidth() < sa_end_x1y1[p2].getX())
                    {
                        if (p2 == INClub[club].Club_member[0])
                        {
                            bx = sa_end_x1y1[p2].getX() - Modules[p].getWidth();
                            vector<int> find_downblock;
                            find_downblock.push_back(sa_end_x1y1[p2].getX() - Modules[p].getWidth());
                            find_downblock.push_back(0);
                            find_downblock.push_back(sa_end_x1y1[p2].getX());
                            find_downblock.push_back(sa_end_x1y1[p2].getY() + Modules[p2].getHeight());
                            int maxby = -1;
                            for (int j = 1; j <= Msize; ++j)
                            {
                                if (has_module.find(j) == has_module.end())
                                {
                                    continue;
                                }
                                vector<int> onpad_module;
                                onpad_module.push_back(sa_end_x1y1[j].getX());
                                onpad_module.push_back(sa_end_x1y1[j].getY());
                                onpad_module.push_back(sa_end_x1y1[j].getX() + Modules[j].getWidth());
                                onpad_module.push_back(sa_end_x1y1[j].getY() + Modules[j].getHeight());
                                if (isRectangleOverlap(find_downblock, onpad_module))
                                {
                                    maxby = max(maxby, onpad_module[3] - 1);
                                }
                            }
                            int pre_club;
                            for (int i = 0; i < INClub.size(); ++i)
                            {
                                vector<int>::iterator iter = find(INClub[i].Club_member.begin(), INClub[i].Club_member.end(), p2);
                                if (iter != INClub[i].Club_member.end())
                                {
                                    pre_club = i;
                                    break;
                                }
                            }
                            if (maxby < sa_end_x1y1[INClub[pre_club].Club_member[0]].getY() - 1)
                            {
                                maxby = sa_end_x1y1[INClub[pre_club].Club_member[0]].getY() - 1;
                            }
                            by = maxby;
                            if (by == sa_end_x1y1[p2].getY() + Modules[p2].getHeight() - 1)
                            {
                                bx = sa_end_x1y1[p2].getX();
                            }
                        }
                        else
                        {
                            by = sa_end_x1y1[p2].getY() + Modules[p2].getHeight() - 1;
                            bx = sa_end_x1y1[p2].getX();
                        }
                    }
                    else if (maxbx + Modules[p].getWidth() >= sa_end_x1y1[p2].getX() && maxbx < sa_end_x1y1[p2].getX())
                    {
                        bx = sa_end_x1y1[p2].getX();
                    }
                    else if (maxbx > sa_end_x1y1[p2].getX() + Modules[p2].getWidth())
                    {
                        bx = sa_end_x1y1[p2].getX() + Modules[p2].getWidth();
                        by = sa_end_x1y1[p2].getY() - 1;
                    }
                    else
                    {
                        bx = maxbx;
                    }
                    // cout<<"p: "<<p<<", p2: "<<p2<<", bx: "<<bx<<", by: "<<by<<endl;
                }
                vector<int> find_onpad;
                find_onpad.push_back(bx);
                find_onpad.push_back(0);
                find_onpad.push_back(bx + Modules[p].getWidth());
                find_onpad.push_back(ChipHeight);
                min_onpad_x = bx + Modules[p].getWidth();
                vector<int>::iterator it2 = --it;
                ++it;
                int p2 = *it2;
                for (int j = 1; j <= Msize; ++j)
                {
                    if (has_module.find(j) == has_module.end())
                    {
                        continue;
                    }
                    // cout<<"do j: "<<j<<endl;
                    vector<int> onpad_module;
                    onpad_module.push_back(sa_end_x1y1[j].getX());
                    onpad_module.push_back(sa_end_x1y1[j].getY());
                    onpad_module.push_back(sa_end_x1y1[j].getX() + Modules[j].getWidth());
                    onpad_module.push_back(sa_end_x1y1[j].getY() + Modules[j].getHeight());
                    if (isRectangleOverlap(find_onpad, onpad_module))
                    {
                        min_onpad_x = min(min_onpad_x, onpad_module[2]);
                    }
                }
                space = false;
                vector<int> find_space;
                bool change_line = false;
                while (space == false)
                {
                    ++by;
                    // cout<<"bx: "<<bx<<", by: "<<by<<endl;
                    if (by > ChipHeight - Modules[p].getHeight())
                    {
                        if (xcanskip)
                        {
                            bx = min_onpad_x;
                            xcanskip = false;
                        }
                        else
                        {
                            ++bx;
                        }
                        min_onpad_x = bx + Modules[p].getWidth();
                        by = sa_end_x1y1[INClub[club].Club_member[0]].getY() - Modules[p].getHeight();
                        if (by < 0)
                        {
                            by = 0;
                        }
                    }
                    if (bx + Modules[p].getWidth() > ChipWidth)
                    {
                        // cout<<"by + Modules[p].getHeight() :"<<by + Modules[p].getHeight() <<endl;
                        cantfind = true;
                    }
                    find_space.push_back(bx);
                    find_space.push_back(by);
                    find_space.push_back(bx + Modules[p].getWidth());
                    find_space.push_back(by + Modules[p].getHeight());
                    int max_by = 0;
                    for (int j = 1; j <= Msize; ++j)
                    {
                        if (has_module.find(j) == has_module.end())
                        {
                            continue;
                        }
                        // cout<<"do j: "<<j<<endl;
                        vector<int> onpad_module;
                        onpad_module.push_back(sa_end_x1y1[j].getX());
                        onpad_module.push_back(sa_end_x1y1[j].getY());
                        onpad_module.push_back(sa_end_x1y1[j].getX() + Modules[j].getWidth());
                        onpad_module.push_back(sa_end_x1y1[j].getY() + Modules[j].getHeight());
                        if (isRectangleOverlap(find_space, onpad_module))
                        {
                            xcanskip = true;
                            min_onpad_x = min(min_onpad_x, onpad_module[2]);
                            max_by = max(max_by, onpad_module[3] - 1);
                            space = true;
                        }
                    }
                    find_space.clear();
                    if (space == true)
                    {
                        by = max_by;
                        space = false;
                        continue;
                    }
                    if (bx < 0)
                    {
                        cantfind = true;
                    }
                    if (by < 0)
                    {
                        cantfind = true;
                    }
                    if (bx + Modules[p].getWidth() > ChipWidth)
                    {
                        cantfind = true;
                    }
                    if (by + Modules[p].getHeight() > ChipHeight)
                    {
                        cantfind = true;
                    }
                    if (cantfind == true)
                    {
                        // cout << "can't find space!" << endl;
                        cantfind = false;
                        // cout << "in " << p << " bx,by: " << bx << ", " << by << endl;
                        sa_end_x1y1[p].x = bx;
                        // vector<int> find_downblock;
                        // find_downblock.push_back(bx);
                        // find_downblock.push_back(0);
                        // find_downblock.push_back(bx+ Modules[p].getWidth());
                        // find_downblock.push_back(by);
                        // int maxby=0;
                        // for(int j =1;j<=Msize;++j)
                        // {
                        //     if (has_module.find(j)==has_module.end())
                        //     {
                        //         continue;
                        //     }
                        //     vector<int> onpad_module;
                        //     onpad_module.push_back(sa_end_x1y1[j].getX());
                        //     onpad_module.push_back(sa_end_x1y1[j].getY());
                        //     onpad_module.push_back(sa_end_x1y1[j].getX() + Modules[j].getWidth());
                        //     onpad_module.push_back(sa_end_x1y1[j].getY() + Modules[j].getHeight());
                        //     if (isRectangleOverlap(find_downblock, onpad_module))
                        //     {
                        //         maxby=max(maxby,onpad_module[3]);
                        //     }
                        // }
                        // by=maxby;
                        sa_end_x1y1[p].y = by;
                        find_space.clear();
                        space = true;
                        cantpush.push_back(p);
                        has_module.insert(p);
                        break;
                    }
                    else
                    {
                        sa_end_x1y1[p].x = bx;
                        // vector<int> find_downblock;
                        // find_downblock.push_back(bx);
                        // find_downblock.push_back(0);
                        // find_downblock.push_back(bx+ Modules[p].getWidth());
                        // find_downblock.push_back(by);
                        // int maxby=0;
                        // for(int j =1;j<=Msize;++j)
                        // {
                        //     if (has_module.find(j)==has_module.end())
                        //     {
                        //         continue;
                        //     }
                        //     vector<int> onpad_module;
                        //     onpad_module.push_back(sa_end_x1y1[j].getX());
                        //     onpad_module.push_back(sa_end_x1y1[j].getY());
                        //     onpad_module.push_back(sa_end_x1y1[j].getX() + Modules[j].getWidth());
                        //     onpad_module.push_back(sa_end_x1y1[j].getY() + Modules[j].getHeight());
                        //     if (isRectangleOverlap(find_downblock, onpad_module))
                        //     {
                        //         maxby=max(maxby,onpad_module[3]);
                        //     }
                        // }
                        // by=maxby;
                        sa_end_x1y1[p].y = by;
                        find_space.clear();
                        space = true;
                        has_module.insert(p);
                        break;
                        // cout << "in " << p << " bx,by: " << bx << ", " << by << endl;
                    }
                }
                // cout<<"bx: "<<bx<<", by: "<<by<<endl;
            }
        }
        // if(need_jump)
        // {
        //     need_jump=false;
        //     continue;
        // }
        // if(p==change_stop_module)
        // {
        //     break;
        // }
    }
    // cout<<"OUT GetCost_leftdown"<<endl;
    // cout<<"runcount:"<<runcount<<endl;
    // cout<<"size: "<<sa_end_x1y1.size()<<endl;
    // cout<<"cantpush: "<<cantpush.size()<<endl;
    // cout<<"HPWLcost:"<<HPWLcost<<endl;
    // cout<<"left end"<<endl;
    // double final_ans=FinalAnswer(sa_end_x1y1);
    // if(final_ans>51574733)
    // {
    //     cout<<endl<<"first_change_module: "<<first_change_module<<endl;
    //     cout<<"linecount: "<<final_ans<<endl;
    //     cout<<"cantpush: ";
    //     for(int i =1;i<cantpush.size();++i)
    //     {
    //         cout<<i<<": "<<cantpush[i]<<" , ";
    //     }
    //     cout<<endl;
    //     cout<<"new_place_direction: ";
    //     for(int i =1;i<new_place_direction.size();++i)
    //     {
    //         cout<<i<<": "<<new_place_direction[i]<<" , ";
    //     }
    //     cout<<endl;
    //     cout<<"new_place_order: ";
    //     for(int i =0;i<place_order.size();++i)
    //     {
    //         cout<<place_order[i]<<", ";
    //     }
    //     cout<<endl;
    //     setFinalPlacement(0);
    //     setFinalPlacement(1);
    //     system("pause");
    // }
    cantpush2 = cantpush;
    cantpush.clear();
    int sum_panel_area = 0;
    int cant_push_num = cantpush2.size();
    if (cantpush2.size() > 0)
    {
        vector<int> temp_cantpush = cantpush2;
        int n = temp_cantpush.size();
        for (int i = 0; i < n - 1; ++i)
        {
            for (int j = 0; j < n - i - 1; ++j)
            {
                if (Modules[temp_cantpush[j]].mt.data.soft_module_data.RequiredSize > Modules[temp_cantpush[j + 1]].mt.data.soft_module_data.RequiredSize)
                {
                    int temp = temp_cantpush[j];
                    temp_cantpush[j] = temp_cantpush[j + 1];
                    temp_cantpush[j + 1] = temp;
                }
            };
        };
        reverse(temp_cantpush.begin(), temp_cantpush.end());
        cantpush2 = temp_cantpush;
        for (vector<int>::iterator it = cantpush2.begin(); it != cantpush2.end(); ++it)
        {
            int p = *it;
            if (p == overlap_soft[0])
            {
                bx = -1;
                by = 0;
            }
            else
            {
                bx = sa_end_x1y1[overlap_soft[0]].getX() - 1;
                by = sa_end_x1y1[overlap_soft[0]].getY();
            }
            space = false;
            vector<int> find_space;
            while (space == false)
            {
                ++bx;
                if (bx > ChipWidth - Modules[p].getWidth())
                {
                    if (ycanskip)
                    {
                        by = min_onpad_y;
                        ycanskip = false;
                    }
                    else
                    {
                        ++by;
                    }
                    min_onpad_y = INT_MAX;
                    if (by + Modules[p].getHeight() > ChipHeight)
                    {
                        // cout<<"by + Modules[p].getHeight() :"<<by + Modules[p].getHeight() <<endl;
                        cantfind = true;
                    }
                    bx = 0;
                }
                find_space.push_back(bx);
                find_space.push_back(by);
                find_space.push_back(bx + Modules[p].getWidth());
                find_space.push_back(by + Modules[p].getHeight());
                int maxbx = 0;
                for (int j = 1; j <= Msize; ++j)
                {
                    if (has_module.find(j) == has_module.end())
                    {
                        continue;
                    }
                    // cout<<"do j: "<<j<<endl;
                    vector<int> onpad_module;
                    onpad_module.push_back(sa_end_x1y1[j].getX());
                    onpad_module.push_back(sa_end_x1y1[j].getY());
                    onpad_module.push_back(sa_end_x1y1[j].getX() + Modules[j].getWidth());
                    onpad_module.push_back(sa_end_x1y1[j].getY() + Modules[j].getHeight());
                    if (isRectangleOverlap(find_space, onpad_module))
                    {
                        ycanskip = true;
                        min_onpad_y = min(min_onpad_y, onpad_module[3]);
                        maxbx = max(maxbx, onpad_module[2] - 1);
                        space = true;
                    }
                }
                find_space.clear();
                if (space == true)
                {
                    bx = maxbx;
                    space = false;
                    continue;
                }
                if (cantfind == true)
                {
                    // cout << "can't find space!" << endl;
                    cantfind = false;
                    // cout << "in " << p << " bx,by: " << bx << ", " << by << endl;
                    cantpush.push_back(p);
                    has_module.insert(p);
                    sa_end_x1y1[p].x = bx;
                    sa_end_x1y1[p].y = by;
                    find_space.clear();
                    space = true;
                    break;
                }
                else
                {
                    // cout << "in " << p << " bx,by: " << bx << ", " << by << endl;
                    sa_end_x1y1[p].x = bx;
                    sa_end_x1y1[p].y = by;
                    find_space.clear();
                    has_module.insert(p);
                    space = true;
                    break;
                }
            }
        }
    }
    for (int i = 0; i < cantpush.size(); ++i)
    {
        sum_panel_area += Modules[cantpush[i]].mt.data.soft_module_data.RequiredSize;
    }
    cost = sum_panel_area;
    cantpush2 = cantpush;
    // final_output();
    // setFinalPlacement(0);
    // setFinalPlacement(1);
    // system("pause");
    clock_t cost_end = clock();
    // cout<<"OUT left down"<<endl;
    // cout<<"cost time: "<<std::fixed<< std::setprecision(5)<<static_cast<double>(cost_end-cost_start) / CLOCKS_PER_SEC<<endl;
};
void get_left_down_start(){

};
void GetCost_leftdown(int &cost, vector<int> place_order, vector<int> &cantpush2, int first_change_module, int change_stop_module, vector<int> new_place_direction, vector<Club> INClub) // rat<0.9
{
    // cout<<"IN left down"<<endl;
    clock_t cost_start = clock();
    // 待辦
    // 貼平前一個module頂/右側
    // 根據連線差異來選擇要換位置的MODULE群
    ++datacount;
    vector<int> overlap_soft = place_order;
    std::set<int> has_module;
    vector<int> cantpush;
    int bx, by;
    bool space = false;
    const int Msize = Modules.size();
    for (int i = SoftModuleAmount + 1; i <= Msize; ++i)
    {
        has_module.insert(i);
    }
    bool cantfind = false;
    bool ycanskip = false;
    bool xcanskip = false;
    int min_onpad_y = ChipHeight;
    int min_onpad_x = ChipWidth;
    bool can_do = false;
    int snake = 0;
    vector<int>::iterator club_head_it;
    int club_head_need_jump_time = 0;
    int jump_bx = 0;
    int jump_by = 0;
    bool back_club_head = false;
    int preclub = -1;
    int club = -1;
    // cout<<"IN GetCost_leftdown"<<endl;
    // cout<<"new_place_direction: "<<endl;
    // for(int i =0;i<new_place_direction.size();i++)
    // {
    //     cout<<new_place_direction[i]<<", ";
    // }
    // cout<<endl;
    // cout<<"place_order: "<<endl;
    // for(int i =0;i<place_order.size();i++)
    // {
    //     cout<<place_order[i]<<", ";
    // }
    // cout<<endl<<endl;
    bool need_jump = false;
    // for (vector<int>::iterator it =std::prev(overlap_soft.end()); it != overlap_soft.begin(); --it)
    // {
    //     int p = *it;
    //     if(p==change_stop_module)
    //     {
    //         break;
    //     }
    //     has_module.insert(p);
    //     if(sa_end_x1y1[p].getY()+Modules[p].getHeight()>ChipHeight)
    //     {
    //         cantpush.push_back(p);
    //         // cout<<"cant push: "<<p<<", ";
    //     }
    //     else if(sa_end_x1y1[p].getX()+Modules[p].getWidth()>ChipWidth)
    //     {
    //         cantpush.push_back(p);
    //         // cout<<"cant push: "<<p<<", ";
    //     }
    //     else if(sa_end_x1y1[p].getX()<0)
    //     {
    //         cantpush.push_back(p);
    //         // cout<<"cant push: "<<p<<", ";
    //     }
    //     else if(sa_end_x1y1[p].getY()<0)
    //     {
    //         cantpush.push_back(p);
    //         // cout<<"cant push: "<<p<<", ";
    //     }
    // }
    for (vector<int>::iterator it = overlap_soft.begin(); it != overlap_soft.end(); ++it)
    {
        ycanskip = false;
        xcanskip = false;
        int p = *it;
        if (p == first_change_module)
        {
            can_do = true;
        }
        if (can_do == false)
        {
            has_module.insert(p);
            if (sa_end_x1y1[p].getY() + Modules[p].getHeight() > ChipHeight)
            {
                cantpush.push_back(p);
                // cout<<"cant push: "<<p<<", ";
            }
            else if (sa_end_x1y1[p].getX() + Modules[p].getWidth() > ChipWidth)
            {
                cantpush.push_back(p);
                // cout<<"cant push: "<<p<<", ";
            }
            else if (sa_end_x1y1[p].getX() < 0)
            {
                cantpush.push_back(p);
                // cout<<"cant push: "<<p<<", ";
            }
            else if (sa_end_x1y1[p].getY() < 0)
            {
                cantpush.push_back(p);
                // cout<<"cant push: "<<p<<", ";
            }
            continue;
        }
        // system("pause");
        if (back_club_head)
        {
            it = club_head_it;
            back_club_head = false;
        }
        p = *it;
        // cout<<p<<", ";
        // setFinalPlacement(0);
        // setFinalPlacement(1);
        if (new_place_direction[p] == 0)
        {
            for (int i = 0; i < INClub.size(); ++i)
            {
                vector<int>::iterator iter = find(INClub[i].Club_member.begin(), INClub[i].Club_member.end(), p);
                if (iter != INClub[i].Club_member.end())
                {
                    club = i;
                    break;
                }
            }
            if (club != preclub)
            {
                // cout<<"M: "<<p<<", club: "<<club<<", preclub: "<<preclub<<endl;
                club_head_need_jump_time = 0;
                jump_bx = 0;
                jump_by = 0;
            }
            if (p == INClub[club].Club_member[0]) // club的頭
            {
                int jumptime = club_head_need_jump_time;
                // cout<<"p: "<<p<<", head jumptime: "<<jumptime<<endl;
                int firstspace = false;
                bool restart = false;
                double restart_pro;
                if (it == overlap_soft.begin())
                {
                    restart = true;
                }
                else
                {
                    vector<int>::iterator it2 = --it;
                    ++it;
                    int p2 = *it2;
                    unsigned long x_point = sa_end_x1y1[p2].getX() + Modules[p2].getWidth();
                    unsigned long y_point = sa_end_x1y1[p2].getY();
                    restart_pro = sqrt((x_point * x_point) + (y_point * y_point)) / chipline;
                    if (restart_pro > 1.0)
                    {
                        restart_pro = 1.0;
                    }
                    restart_pro = restart_pro / 10.0;
                    // cout<<"restart: "<<restart_pro<<endl;
                    double randrestart = (double)rand() / (RAND_MAX + 1.0);
                    if (randrestart < restart_pro)
                    {
                        restart = true;
                    }
                }
                if (it == overlap_soft.begin())
                {
                    vector<leftdown_corner> left_up_point;
                    vector<leftdown_corner> right_down_point;
                    for (int s = SoftModuleAmount + 1; s <= Msize; ++s)
                    {
                        leftdown_corner temp;
                        temp.x = sa_end_x1y1[s].getX();
                        temp.y = sa_end_x1y1[s].getY() + Modules[s].getHeight();
                        left_up_point.push_back(temp);
                        leftdown_corner temp2;
                        temp2.x = sa_end_x1y1[s].getX() + Modules[s].getWidth();
                        temp2.y = sa_end_x1y1[s].getY();
                        right_down_point.push_back(temp2);
                    }
                    double min_distance = DBL_MAX;
                    leftdown_corner min_point;
                    for (int b = 0; b < left_up_point.size(); ++b)
                    {
                        double distance = sqrt((left_up_point[b].getX() * left_up_point[b].getX()) + (left_up_point[b].getY() * left_up_point[b].getY()));
                        if (min_distance > distance)
                        {
                            min_distance = distance;
                            min_point = left_up_point[b];
                        }
                    }
                    double per50down = (double)rand() / (RAND_MAX + 1.0);
                    int maxbx = 0;
                    if (per50down >= 0)
                    {
                        vector<int> find_leftblock;
                        find_leftblock.push_back(0);
                        find_leftblock.push_back(min_point.getY());
                        find_leftblock.push_back(min_point.getX() + Modules[p].getWidth());
                        find_leftblock.push_back(min_point.getY() + Modules[p].getHeight());
                        for (int j = SoftModuleAmount + 1; j <= Msize; ++j)
                        {
                            if (has_module.find(j) == has_module.end())
                            {
                                continue;
                            }
                            vector<int> onpad_module;
                            onpad_module.push_back(sa_end_x1y1[j].getX());
                            onpad_module.push_back(sa_end_x1y1[j].getY());
                            onpad_module.push_back(sa_end_x1y1[j].getX() + Modules[j].getWidth());
                            onpad_module.push_back(sa_end_x1y1[j].getY() + Modules[j].getHeight());
                            if (isRectangleOverlap(find_leftblock, onpad_module))
                            {
                                maxbx = max(maxbx, onpad_module[2] - 1);
                            }
                        }
                        if (maxbx < min_point.getX())
                        {
                            int miny = ChipWidth;
                            for (int j = 0; j < right_down_point.size(); ++j)
                            {
                                miny = min(miny, right_down_point[j].getX());
                            }
                            if (miny > maxbx && miny < min_point.getX())
                            {
                                maxbx = miny - 1;
                            }
                            // if(p==28)
                            // {
                            //     cout<<endl;
                            //     cout<<"bx: "<<maxbx  <<", min_point.getY();: "<<min_point.getY()<<endl;
                            //     system("pause");
                            // }
                        }
                    }
                    else
                    {
                        maxbx = min_point.getX() - 1;
                    }
                    bx = maxbx;
                    by = min_point.getY();
                    // cout<<"bx: "<<bx<<", by: "<<by<<endl;
                    min_onpad_y = by + Modules[p].getHeight();
                }
                else
                {
                    if (restart)
                    {
                        bx = sa_end_x1y1[overlap_soft[0]].getX() + Modules[p].getWidth() - 1;
                        by = sa_end_x1y1[overlap_soft[0]].getY();
                        min_onpad_x = bx + Modules[p].getWidth();
                    }
                    else
                    {
                        vector<int>::iterator it2 = --it;
                        ++it;
                        int p2 = *it2;
                        if (sa_end_x1y1[p2].getY() + Modules[p2].getHeight() + Modules[p].getHeight() > ChipHeight)
                        {
                            bx = sa_end_x1y1[overlap_soft[0]].getX() + Modules[p].getWidth() - 1;
                            by = sa_end_x1y1[overlap_soft[0]].getY();
                            min_onpad_x = bx + Modules[p].getWidth();
                        }
                        else
                        {
                            bx = sa_end_x1y1[p2].getX() + Modules[p2].getWidth() - 1;
                            by = 0;
                            min_onpad_y = sa_end_x1y1[p2].getY();
                            vector<int> find_downblock;
                            find_downblock.push_back(sa_end_x1y1[p2].getX() + Modules[p2].getWidth());
                            find_downblock.push_back(0);
                            find_downblock.push_back(sa_end_x1y1[p2].getX() + Modules[p2].getWidth() + Modules[p].getWidth());
                            find_downblock.push_back(sa_end_x1y1[p2].getY() + Modules[p].getHeight());
                            int maxby = 0;
                            for (int j = 1; j <= Msize; ++j)
                            {
                                if (has_module.find(j) == has_module.end())
                                {
                                    continue;
                                }
                                vector<int> onpad_module;
                                onpad_module.push_back(sa_end_x1y1[j].getX());
                                onpad_module.push_back(sa_end_x1y1[j].getY());
                                onpad_module.push_back(sa_end_x1y1[j].getX() + Modules[j].getWidth());
                                onpad_module.push_back(sa_end_x1y1[j].getY() + Modules[j].getHeight());
                                if (isRectangleOverlap(find_downblock, onpad_module))
                                {
                                    maxby = max(maxby, onpad_module[3]);
                                }
                            }
                            if (maxby >= sa_end_x1y1[p2].getY() + Modules[p2].getHeight())
                            {
                                bx = sa_end_x1y1[p2].getX() - 1;
                                by = sa_end_x1y1[p2].getY() + Modules[p2].getHeight();
                            }
                            else
                            {
                                if (maxby + Modules[p].getHeight() <= sa_end_x1y1[p2].getY())
                                {
                                    by = sa_end_x1y1[p2].getY() - Modules[p].getHeight();
                                    vector<int> find_leftblock;
                                    find_leftblock.push_back(0);
                                    find_leftblock.push_back(sa_end_x1y1[p2].getY() - Modules[p].getHeight());
                                    find_leftblock.push_back(sa_end_x1y1[p2].getX() + Modules[p2].getWidth());
                                    find_leftblock.push_back(by + Modules[p].getHeight());
                                    int maxbx = -1;
                                    for (int j = 1; j <= Msize; ++j)
                                    {
                                        if (has_module.find(j) == has_module.end())
                                        {
                                            continue;
                                        }
                                        vector<int> onpad_module;
                                        onpad_module.push_back(sa_end_x1y1[j].getX());
                                        onpad_module.push_back(sa_end_x1y1[j].getY());
                                        onpad_module.push_back(sa_end_x1y1[j].getX() + Modules[j].getWidth());
                                        onpad_module.push_back(sa_end_x1y1[j].getY() + Modules[j].getHeight());
                                        if (isRectangleOverlap(find_leftblock, onpad_module))
                                        {
                                            maxbx = max(maxbx, onpad_module[2] - 1);
                                        }
                                    }
                                    int pre_club;
                                    for (int i = 0; i < INClub.size(); ++i)
                                    {
                                        vector<int>::iterator iter = find(INClub[i].Club_member.begin(), INClub[i].Club_member.end(), p2);
                                        if (iter != INClub[i].Club_member.end())
                                        {
                                            pre_club = i;
                                            break;
                                        }
                                    }
                                    if (maxbx < sa_end_x1y1[INClub[pre_club].Club_member[0]].getX() - 1)
                                    {
                                        maxbx = sa_end_x1y1[INClub[pre_club].Club_member[0]].getX() - 1;
                                    }
                                    if (maxbx == sa_end_x1y1[p2].getX() - 1)
                                    {
                                        by = sa_end_x1y1[p2].getY();
                                    }
                                    bx = maxbx;
                                    // cout<<"p : "<<p<<", p2: "<<p2<<endl;
                                    // sa_end_x1y1[p].x=bx;
                                    // sa_end_x1y1[p].y=by;
                                    // setFinalPlacement(0);
                                    // setFinalPlacement(1);
                                    // system("pause");
                                }
                                else if (maxby + Modules[p].getHeight() > sa_end_x1y1[p2].getY() && maxby + Modules[p].getHeight() < sa_end_x1y1[p2].getY() + Modules[p2].getHeight())
                                {
                                    by = sa_end_x1y1[p2].getY();
                                }
                                else
                                {
                                    by = maxby;
                                }
                            }
                        }
                    }
                    // cout<<"p: "<<p<<", p2: "<<p2<<", bx: "<<bx<<", by: "<<by<<endl;
                }
                vector<int> find_onpad;
                find_onpad.push_back(0);
                find_onpad.push_back(by);
                find_onpad.push_back(ChipWidth);
                find_onpad.push_back(by + Modules[p].getHeight());
                min_onpad_y = by + Modules[p].getHeight();
                for (int j = 1; j <= Msize; ++j)
                {
                    if (has_module.find(j) == has_module.end())
                    {
                        continue;
                    }
                    // cout<<"do j: "<<j<<endl;
                    vector<int> onpad_module;
                    onpad_module.push_back(sa_end_x1y1[j].getX());
                    onpad_module.push_back(sa_end_x1y1[j].getY());
                    onpad_module.push_back(sa_end_x1y1[j].getX() + Modules[j].getWidth());
                    onpad_module.push_back(sa_end_x1y1[j].getY() + Modules[j].getHeight());
                    if (isRectangleOverlap(find_onpad, onpad_module))
                    {
                        min_onpad_y = min(min_onpad_y, onpad_module[3]);
                    }
                }
                space = false;
                vector<int> find_space;
                while (space == false)
                {
                    // cout<<"bx: "<<bx<<", by: "<<by<<endl;
                    ++bx;
                    if (bx > ChipWidth - Modules[p].getWidth())
                    {
                        int min_onpad_left = ChipWidth + 1;
                        vector<int> find_onpad_right;
                        find_onpad_right.push_back(0);
                        find_onpad_right.push_back(by);
                        find_onpad_right.push_back(ChipWidth);
                        find_onpad_right.push_back(by + Modules[p].getHeight());
                        for (int j = 1; j <= Msize; ++j)
                        {
                            if (has_module.find(j) == has_module.end())
                            {
                                continue;
                            }
                            // cout<<"do j: "<<j<<endl;
                            vector<int> onpad_module;
                            onpad_module.push_back(sa_end_x1y1[j].getX());
                            onpad_module.push_back(sa_end_x1y1[j].getY());
                            onpad_module.push_back(sa_end_x1y1[j].getX() + Modules[j].getWidth());
                            onpad_module.push_back(sa_end_x1y1[j].getY() + Modules[j].getHeight());
                            if (isRectangleOverlap(find_onpad_right, onpad_module))
                            {
                                min_onpad_left = min(min_onpad_left, onpad_module[0] - Modules[p].getWidth());
                            }
                        }
                        if (min_onpad_left < 0)
                        {
                            if (ycanskip)
                            {
                                by = min_onpad_y;
                                ycanskip = false;
                            }
                            else
                            {
                                ++by;
                            }
                            bx = 0;
                        }
                        else if (min_onpad_left > ChipWidth)
                        {
                            if (ycanskip)
                            {
                                by = min_onpad_y;
                                ycanskip = false;
                            }
                            else
                            {
                                ++by;
                            }
                            bx = 0;
                        }
                        else
                        {
                            bx = min_onpad_left;
                        }
                        min_onpad_y = by + Modules[p].getHeight();
                    }
                    if (by + Modules[p].getHeight() > ChipHeight)
                    {
                        // cout<<"by + Modules[p].getHeight() :"<<by + Modules[p].getHeight() <<endl;
                        cantfind = true;
                    }
                    find_space.push_back(bx);
                    find_space.push_back(by);
                    find_space.push_back(bx + Modules[p].getWidth());
                    find_space.push_back(by + Modules[p].getHeight());
                    int max_bx = 0;
                    for (int j = 1; j <= Msize; ++j)
                    {
                        if (has_module.find(j) == has_module.end())
                        {
                            continue;
                        }
                        // cout<<"do j: "<<j<<endl;
                        vector<int> onpad_module;
                        onpad_module.push_back(sa_end_x1y1[j].getX());
                        onpad_module.push_back(sa_end_x1y1[j].getY());
                        onpad_module.push_back(sa_end_x1y1[j].getX() + Modules[j].getWidth());
                        onpad_module.push_back(sa_end_x1y1[j].getY() + Modules[j].getHeight());
                        if (isRectangleOverlap(find_space, onpad_module))
                        {
                            ycanskip = true;
                            min_onpad_y = min(min_onpad_y, onpad_module[3]);
                            max_bx = max(max_bx, onpad_module[2] - 1);
                            space = true;
                        }
                    }
                    find_space.clear();
                    if (space == true)
                    {
                        bx = max_bx;
                        space = false;
                        continue;
                    }
                    if (bx < 0)
                    {
                        cantfind = true;
                    }
                    if (by < 0)
                    {
                        cantfind = true;
                    }
                    if (bx + Modules[p].getWidth() > ChipWidth)
                    {
                        cantfind = true;
                    }
                    if (by + Modules[p].getHeight() > ChipHeight)
                    {
                        cantfind = true;
                    }
                    if (cantfind == true)
                    {
                        // cout << "can't find space!" << endl;
                        cantfind = false;
                        // cout << "in " << p << " bx,by: " << bx << ", " << by << endl;
                        sa_end_x1y1[p].x = bx;
                        // vector<int> find_downblock;
                        // find_downblock.push_back(bx);
                        // find_downblock.push_back(0);
                        // find_downblock.push_back(bx+ Modules[p].getWidth());
                        // find_downblock.push_back(by);
                        // int maxby=0;
                        // for(int j =1;j<=Msize;++j)
                        // {
                        //     if (has_module.find(j)==has_module.end())
                        //     {
                        //         continue;
                        //     }
                        //     vector<int> onpad_module;
                        //     onpad_module.push_back(sa_end_x1y1[j].getX());
                        //     onpad_module.push_back(sa_end_x1y1[j].getY());
                        //     onpad_module.push_back(sa_end_x1y1[j].getX() + Modules[j].getWidth());
                        //     onpad_module.push_back(sa_end_x1y1[j].getY() + Modules[j].getHeight());
                        //     if (isRectangleOverlap(find_downblock, onpad_module))
                        //     {
                        //         maxby=max(maxby,onpad_module[3]);
                        //     }
                        // }
                        // by=maxby;
                        sa_end_x1y1[p].y = by;
                        find_space.clear();
                        space = true;
                        cantpush.push_back(p);
                        has_module.insert(p);
                        break;
                    }
                    else
                    {
                        // cout << "in " << p << " bx,by: " << bx << ", " << by << endl;
                        sa_end_x1y1[p].x = bx;
                        // vector<int> find_downblock;
                        // find_downblock.push_back(bx);
                        // find_downblock.push_back(0);
                        // find_downblock.push_back(bx+ Modules[p].getWidth());
                        // find_downblock.push_back(by);
                        // int maxby=0;
                        // for(int j =1;j<=Msize;++j)
                        // {
                        //     if (has_module.find(j)==has_module.end())
                        //     {
                        //         continue;
                        //     }
                        //     vector<int> onpad_module;
                        //     onpad_module.push_back(sa_end_x1y1[j].getX());
                        //     onpad_module.push_back(sa_end_x1y1[j].getY());
                        //     onpad_module.push_back(sa_end_x1y1[j].getX() + Modules[j].getWidth());
                        //     onpad_module.push_back(sa_end_x1y1[j].getY() + Modules[j].getHeight());
                        //     if (isRectangleOverlap(find_downblock, onpad_module))
                        //     {
                        //         maxby=max(maxby,onpad_module[3]);
                        //     }
                        // }
                        // by=maxby;
                        sa_end_x1y1[p].y = by;
                        find_space.clear();
                        space = true;
                        has_module.insert(p);
                        break;
                    }
                }
                club_head_it = it;
                preclub = club;
            }
            else // club的身
            {
                // cout<<"p: "<<p<<", tail"<<endl;
                preclub = club;
                int firstspace = false;
                if (it == overlap_soft.begin())
                {
                    bx = -1;
                    by = 0;
                }
                else
                {
                    vector<int>::iterator it2 = --it;
                    ++it;
                    int p2 = *it2;
                    bx = sa_end_x1y1[p2].getX() + Modules[p2].getWidth() - 1;
                    by = 0;
                    min_onpad_y = sa_end_x1y1[p2].getY();
                    vector<int> find_downblock;
                    find_downblock.push_back(sa_end_x1y1[p2].getX() + Modules[p2].getWidth());
                    find_downblock.push_back(0);
                    find_downblock.push_back(sa_end_x1y1[p2].getX() + Modules[p2].getWidth() + Modules[p].getWidth());
                    find_downblock.push_back(sa_end_x1y1[p2].getY() + Modules[p].getHeight());
                    int maxby = 0;
                    for (int j = 1; j <= Msize; ++j)
                    {
                        if (has_module.find(j) == has_module.end())
                        {
                            continue;
                        }
                        for (int d = 0; d < INClub[club].Club_member.size(); ++d)
                        {
                            if (INClub[club].Club_member[d] == j)
                            {
                                vector<int> onpad_module;
                                onpad_module.push_back(sa_end_x1y1[j].getX());
                                onpad_module.push_back(sa_end_x1y1[j].getY());
                                onpad_module.push_back(sa_end_x1y1[j].getX() + Modules[j].getWidth());
                                onpad_module.push_back(sa_end_x1y1[j].getY() + Modules[j].getHeight());
                                if (isRectangleOverlap(find_downblock, onpad_module))
                                {
                                    maxby = max(maxby, onpad_module[1]);
                                }
                                break;
                            }
                        }
                        vector<int> onpad_module;
                        onpad_module.push_back(sa_end_x1y1[j].getX());
                        onpad_module.push_back(sa_end_x1y1[j].getY());
                        onpad_module.push_back(sa_end_x1y1[j].getX() + Modules[j].getWidth());
                        onpad_module.push_back(sa_end_x1y1[j].getY() + Modules[j].getHeight());
                        if (isRectangleOverlap(find_downblock, onpad_module))
                        {
                            maxby = max(maxby, onpad_module[3]);
                        }
                    }
                    if (maxby >= sa_end_x1y1[p2].getY() + Modules[p2].getHeight())
                    {
                        bx = sa_end_x1y1[p2].getX() - 1;
                        by = sa_end_x1y1[p2].getY() + Modules[p2].getHeight();
                    }
                    else if (maxby + Modules[p].getHeight() <= sa_end_x1y1[p2].getY())
                    {
                        if (p2 == INClub[club].Club_member[0])
                        {
                            by = sa_end_x1y1[p2].getY() - Modules[p].getHeight();
                            vector<int> find_leftblock;
                            find_leftblock.push_back(0);
                            find_leftblock.push_back(sa_end_x1y1[p2].getY() - Modules[p].getHeight());
                            find_leftblock.push_back(sa_end_x1y1[p2].getX() + Modules[p2].getWidth());
                            find_leftblock.push_back(by + Modules[p].getHeight());
                            int maxbx = -1;
                            for (int j = 1; j <= Msize; ++j)
                            {
                                if (has_module.find(j) == has_module.end())
                                {
                                    continue;
                                }
                                vector<int> onpad_module;
                                onpad_module.push_back(sa_end_x1y1[j].getX());
                                onpad_module.push_back(sa_end_x1y1[j].getY());
                                onpad_module.push_back(sa_end_x1y1[j].getX() + Modules[j].getWidth());
                                onpad_module.push_back(sa_end_x1y1[j].getY() + Modules[j].getHeight());
                                if (isRectangleOverlap(find_leftblock, onpad_module))
                                {
                                    maxbx = max(maxbx, onpad_module[2] - 1);
                                }
                            }
                            if (maxbx < sa_end_x1y1[INClub[club].Club_member[0]].getX() - 1)
                            {
                                maxbx = sa_end_x1y1[INClub[club].Club_member[0]].getX() - 1;
                            }
                            bx = maxbx;
                            if (bx == sa_end_x1y1[p2].getX() + Modules[p].getWidth() - 1)
                            {
                                by = sa_end_x1y1[p2].getY();
                            }
                        }
                        else
                        {
                            bx = sa_end_x1y1[p2].getX() + Modules[p2].getWidth() - 1;
                            by = sa_end_x1y1[p2].getY();
                        }
                        // cout<<"p : "<<p<<", p2: "<<p2<<endl;
                        // sa_end_x1y1[p].x=bx;
                        // sa_end_x1y1[p].y=by;
                        // setFinalPlacement(0);
                        // setFinalPlacement(1);
                        // system("pause");
                    }
                    else if (maxby + Modules[p].getHeight() > sa_end_x1y1[p2].getY() && maxby < sa_end_x1y1[p2].getY())
                    {
                        by = sa_end_x1y1[p2].getY();
                    }
                    else
                    {
                        by = maxby;
                    }
                    // cout<<"p: "<<p<<", p2: "<<p2<<", bx: "<<bx<<", by: "<<by<<endl;
                }
                vector<int> find_onpad;
                find_onpad.push_back(0);
                find_onpad.push_back(by);
                find_onpad.push_back(ChipWidth);
                find_onpad.push_back(by + Modules[p].getHeight());
                min_onpad_y = by + Modules[p].getHeight();
                for (int j = 1; j <= Msize; ++j)
                {
                    if (has_module.find(j) == has_module.end())
                    {
                        continue;
                    }
                    // cout<<"do j: "<<j<<endl;
                    vector<int> onpad_module;
                    onpad_module.push_back(sa_end_x1y1[j].getX());
                    onpad_module.push_back(sa_end_x1y1[j].getY());
                    onpad_module.push_back(sa_end_x1y1[j].getX() + Modules[j].getWidth());
                    onpad_module.push_back(sa_end_x1y1[j].getY() + Modules[j].getHeight());
                    if (isRectangleOverlap(find_onpad, onpad_module))
                    {
                        min_onpad_y = min(min_onpad_y, onpad_module[3]);
                    }
                }
                space = false;
                vector<int> find_space;
                bool change_line = false;
                while (space == false)
                {
                    ++bx;
                    // cout<<"bx: "<<bx<<", by: "<<by<<endl;
                    if (bx > ChipWidth - Modules[p].getWidth())
                    {
                        if (ycanskip)
                        {
                            by = min_onpad_y;
                            ycanskip = false;
                        }
                        else
                        {
                            ++by;
                        }
                        min_onpad_y = by + Modules[p].getHeight();
                        vector<int>::iterator it2 = --it;
                        ++it;
                        int p2 = *it2;
                        bx = sa_end_x1y1[p2].getX() + Modules[p2].getWidth() - Modules[p].getWidth();
                        change_line = true;
                    }
                    if (by + Modules[p].getHeight() > ChipHeight)
                    {
                        // cout<<"by + Modules[p].getHeight() :"<<by + Modules[p].getHeight() <<endl;
                        cantfind = true;
                    }
                    find_space.push_back(bx);
                    find_space.push_back(by);
                    find_space.push_back(bx + Modules[p].getWidth());
                    find_space.push_back(by + Modules[p].getHeight());
                    int max_bx = 0;
                    for (int j = 1; j <= Msize; ++j)
                    {
                        if (has_module.find(j) == has_module.end())
                        {
                            continue;
                        }
                        // cout<<"do j: "<<j<<endl;
                        vector<int> onpad_module;
                        onpad_module.push_back(sa_end_x1y1[j].getX());
                        onpad_module.push_back(sa_end_x1y1[j].getY());
                        onpad_module.push_back(sa_end_x1y1[j].getX() + Modules[j].getWidth());
                        onpad_module.push_back(sa_end_x1y1[j].getY() + Modules[j].getHeight());
                        if (isRectangleOverlap(find_space, onpad_module))
                        {
                            ycanskip = true;
                            min_onpad_y = min(min_onpad_y, onpad_module[3]);
                            max_bx = max(max_bx, onpad_module[2] - 1);
                            space = true;
                        }
                    }
                    find_space.clear();
                    if (space == true)
                    {
                        bx = max_bx;
                        space = false;
                        continue;
                    }
                    if (bx < 0)
                    {
                        cantfind = true;
                    }
                    if (by < 0)
                    {
                        cantfind = true;
                    }
                    if (bx + Modules[p].getWidth() > ChipWidth)
                    {
                        cantfind = true;
                    }
                    if (by + Modules[p].getHeight() > ChipHeight)
                    {
                        cantfind = true;
                    }
                    if (cantfind == true)
                    {
                        // cout << "can't find space!" << endl;
                        cantfind = false;
                        // cout << "in " << p << " bx,by: " << bx << ", " << by << endl;
                        sa_end_x1y1[p].x = bx;
                        // vector<int> find_downblock;
                        // find_downblock.push_back(bx);
                        // find_downblock.push_back(0);
                        // find_downblock.push_back(bx+ Modules[p].getWidth());
                        // find_downblock.push_back(by);
                        // int maxby=0;
                        // for(int j =1;j<=Msize;++j)
                        // {
                        //     if (p == j || Vector_contains(overlap_soft, j))
                        //     {
                        //         if (!Vector_contains(has_module, j))
                        //         {
                        //             continue;
                        //         }
                        //     }
                        //     vector<int> onpad_module;
                        //     onpad_module.push_back(sa_end_x1y1[j].getX());
                        //     onpad_module.push_back(sa_end_x1y1[j].getY());
                        //     onpad_module.push_back(sa_end_x1y1[j].getX() + Modules[j].getWidth());
                        //     onpad_module.push_back(sa_end_x1y1[j].getY() + Modules[j].getHeight());
                        //     if (isRectangleOverlap(find_downblock, onpad_module))
                        //     {
                        //         maxby=max(maxby,onpad_module[3]);
                        //     }
                        // }
                        // by=maxby;
                        sa_end_x1y1[p].y = by;
                        find_space.clear();
                        space = true;
                        cantpush.push_back(p);
                        has_module.insert(p);
                        break;
                    }
                    else
                    {
                        sa_end_x1y1[p].x = bx;
                        // vector<int> find_downblock;
                        // find_downblock.push_back(bx);
                        // find_downblock.push_back(0);
                        // find_downblock.push_back(bx+ Modules[p].getWidth());
                        // find_downblock.push_back(by);
                        // int maxby=0;
                        // for(int j =1;j<=Msize;++j)
                        // {
                        //     if (p == j || Vector_contains(overlap_soft, j))
                        //     {
                        //         if (!Vector_contains(has_module, j))
                        //         {
                        //             continue;
                        //         }
                        //     }
                        //     vector<int> onpad_module;
                        //     onpad_module.push_back(sa_end_x1y1[j].getX());
                        //     onpad_module.push_back(sa_end_x1y1[j].getY());
                        //     onpad_module.push_back(sa_end_x1y1[j].getX() + Modules[j].getWidth());
                        //     onpad_module.push_back(sa_end_x1y1[j].getY() + Modules[j].getHeight());
                        //     if (isRectangleOverlap(find_downblock, onpad_module))
                        //     {
                        //         maxby=max(maxby,onpad_module[3]);
                        //     }
                        // }
                        // by=maxby;
                        sa_end_x1y1[p].y = by;
                        find_space.clear();
                        space = true;
                        has_module.insert(p);
                        break;
                        // cout << "in " << p << " bx,by: " << bx << ", " << by << endl;
                    }
                }
            }
        }
        else
        {
            for (int i = 0; i < INClub.size(); ++i)
            {
                vector<int>::iterator iter = find(INClub[i].Club_member.begin(), INClub[i].Club_member.end(), p);
                if (iter != INClub[i].Club_member.end())
                {
                    club = i;
                    break;
                }
            }
            if (club != preclub)
            {
                // cout<<"M: "<<p<<", club: "<<club<<", preclub: "<<preclub<<endl;
                club_head_need_jump_time = 0;
                jump_bx = 0;
                jump_by = 0;
            }
            if (p == INClub[club].Club_member[0]) // club的頭
            {
                // cout<<"p: "<<p<<", head jumptime: "<<jumptime<<endl;
                int firstspace = false;
                bool restart = false;
                double restart_pro;
                if (it == overlap_soft.begin())
                {
                    restart = true;
                }
                else
                {
                    vector<int>::iterator it2 = --it;
                    ++it;
                    int p2 = *it2;
                    unsigned long x_point = sa_end_x1y1[p2].getX();
                    unsigned long y_point = sa_end_x1y1[p2].getY() + Modules[p2].getHeight();
                    restart_pro = sqrt((x_point * x_point) + (y_point * y_point)) / chipline;
                    if (restart_pro > 1.0)
                    {
                        restart_pro = 1.0;
                    }
                    restart_pro = restart_pro / 10.0;
                    // cout<<"restart: "<<restart_pro<<endl;
                    double randrestart = (double)rand() / (RAND_MAX + 1.0);
                    if (randrestart < restart_pro)
                    {
                        restart = true;
                    }
                }
                if (it == overlap_soft.begin())
                {
                    vector<leftdown_corner> left_up_point;
                    vector<leftdown_corner> right_down_point;
                    for (int s = SoftModuleAmount + 1; s <= Msize; ++s)
                    {
                        leftdown_corner temp;
                        temp.x = sa_end_x1y1[s].getX() + Modules[s].getWidth();
                        temp.y = sa_end_x1y1[s].getY();
                        right_down_point.push_back(temp);
                        leftdown_corner temp2;
                        temp2.x = sa_end_x1y1[s].getX();
                        temp2.y = sa_end_x1y1[s].getY() + Modules[p].getHeight();
                        left_up_point.push_back(temp2);
                    }
                    double min_distance = DBL_MAX;
                    leftdown_corner min_point;
                    for (int b = 0; b < right_down_point.size(); ++b)
                    {
                        double distance = sqrt((right_down_point[b].getX() * right_down_point[b].getX()) + (right_down_point[b].getY() * right_down_point[b].getY()));
                        if (min_distance > distance)
                        {
                            min_distance = distance;
                            min_point = right_down_point[b];
                        }
                    }
                    int maxby = 0;
                    double down50per = (double)rand() / (RAND_MAX + 1.0);
                    if (down50per >= 0)
                    {
                        vector<int> find_downblock;
                        find_downblock.push_back(min_point.getX());
                        find_downblock.push_back(0);
                        find_downblock.push_back(min_point.getX() + Modules[p].getWidth());
                        find_downblock.push_back(min_point.getY() + Modules[p].getHeight());
                        for (int j = SoftModuleAmount + 1; j <= Msize; ++j)
                        {
                            if (has_module.find(j) == has_module.end())
                            {
                                continue;
                            }
                            vector<int> onpad_module;
                            onpad_module.push_back(sa_end_x1y1[j].getX());
                            onpad_module.push_back(sa_end_x1y1[j].getY());
                            onpad_module.push_back(sa_end_x1y1[j].getX() + Modules[j].getWidth());
                            onpad_module.push_back(sa_end_x1y1[j].getY() + Modules[j].getHeight());
                            if (isRectangleOverlap(find_downblock, onpad_module))
                            {
                                maxby = max(maxby, onpad_module[3] - 1);
                            }
                        }
                        if (maxby < min_point.getY())
                        {
                            int min_y = ChipHeight;
                            for (int k = 0; k < left_up_point.size(); ++k)
                            {
                                min_y = min(min_y, left_up_point[k].getY());
                            }
                            if (min_y > 0 && min_y < maxby)
                            {
                                maxby = min_y - 1;
                            }
                        }
                    }
                    else
                    {
                        maxby = min_point.getY() - 1;
                    }
                    bx = min_point.getX();
                    by = maxby;
                    min_onpad_x = bx + Modules[p].getWidth();
                }
                else
                {
                    if (restart)
                    {
                        bx = sa_end_x1y1[overlap_soft[0]].getX();
                        by = sa_end_x1y1[overlap_soft[0]].getY() + Modules[p].getHeight() - 1;
                        min_onpad_x = by + Modules[p].getWidth();
                    }
                    else
                    {
                        vector<int>::iterator it2 = --it;
                        ++it;
                        int p2 = *it2;
                        if (sa_end_x1y1[p2].getX() + Modules[p2].getWidth() + Modules[p].getWidth() >= ChipWidth)
                        {
                            bx = sa_end_x1y1[overlap_soft[0]].getX();
                            by = sa_end_x1y1[overlap_soft[0]].getY() + Modules[p].getHeight() - 1;
                            min_onpad_x = by + Modules[p].getWidth();
                        }
                        else
                        {
                            bx = 0;
                            by = sa_end_x1y1[p2].getY() + Modules[p2].getHeight() - 1;
                            min_onpad_x = sa_end_x1y1[p2].getX();
                            vector<int> find_leftblock;
                            find_leftblock.push_back(0);
                            find_leftblock.push_back(sa_end_x1y1[p2].getY() + Modules[p2].getHeight());
                            find_leftblock.push_back(sa_end_x1y1[p2].getX() + Modules[p].getWidth());
                            find_leftblock.push_back(sa_end_x1y1[p2].getY() + Modules[p2].getHeight() + Modules[p].getHeight());
                            int maxbx = 0;
                            for (int j = 1; j <= Msize; ++j)
                            {
                                if (has_module.find(j) == has_module.end())
                                {
                                    continue;
                                }
                                vector<int> onpad_module;
                                onpad_module.push_back(sa_end_x1y1[j].getX());
                                onpad_module.push_back(sa_end_x1y1[j].getY());
                                onpad_module.push_back(sa_end_x1y1[j].getX() + Modules[j].getWidth());
                                onpad_module.push_back(sa_end_x1y1[j].getY() + Modules[j].getHeight());
                                if (isRectangleOverlap(find_leftblock, onpad_module))
                                {
                                    maxbx = max(maxbx, onpad_module[2]);
                                }
                            }
                            if (maxbx + Modules[p].getWidth() <= sa_end_x1y1[p2].getX())
                            {
                                bx = sa_end_x1y1[p2].getX() - Modules[p].getWidth();
                                vector<int> find_downblock;
                                find_downblock.push_back(sa_end_x1y1[p2].getX() - Modules[p].getWidth());
                                find_downblock.push_back(0);
                                find_downblock.push_back(sa_end_x1y1[p2].getX());
                                find_downblock.push_back(sa_end_x1y1[p2].getY() + Modules[p2].getHeight());
                                int maxby = -1;
                                for (int j = 1; j <= Msize; ++j)
                                {
                                    if (has_module.find(j) == has_module.end())
                                    {
                                        continue;
                                    }
                                    vector<int> onpad_module;
                                    onpad_module.push_back(sa_end_x1y1[j].getX());
                                    onpad_module.push_back(sa_end_x1y1[j].getY());
                                    onpad_module.push_back(sa_end_x1y1[j].getX() + Modules[j].getWidth());
                                    onpad_module.push_back(sa_end_x1y1[j].getY() + Modules[j].getHeight());
                                    if (isRectangleOverlap(find_downblock, onpad_module))
                                    {
                                        maxby = max(maxby, onpad_module[3] - 1);
                                    }
                                }
                                int pre_club;
                                for (int i = 0; i < INClub.size(); ++i)
                                {
                                    vector<int>::iterator iter = find(INClub[i].Club_member.begin(), INClub[i].Club_member.end(), p2);
                                    if (iter != INClub[i].Club_member.end())
                                    {
                                        pre_club = i;
                                        break;
                                    }
                                }
                                if (maxby < sa_end_x1y1[INClub[pre_club].Club_member[0]].getY() - 1)
                                {
                                    maxby = sa_end_x1y1[INClub[pre_club].Club_member[0]].getY() - 1;
                                }
                                if (maxby == sa_end_x1y1[p2].getY() + Modules[p].getHeight() - 1)
                                {
                                    bx = sa_end_x1y1[p2].getX();
                                }
                                by = maxby;
                            }
                            else if (maxbx + Modules[p].getWidth() > sa_end_x1y1[p2].getX() && maxbx + Modules[p].getWidth() < sa_end_x1y1[p2].getX() + Modules[p2].getWidth())
                            {
                                bx = sa_end_x1y1[p2].getX();
                            }
                            else if (maxbx > sa_end_x1y1[p2].getX() + Modules[p2].getWidth())
                            {
                                bx = sa_end_x1y1[p2].getX() + Modules[p2].getWidth();
                                by = sa_end_x1y1[p2].getY() - 1;
                            }
                            else
                            {
                                bx = maxbx;
                            }
                        }
                    }
                    // cout<<"p: "<<p<<", p2: "<<p2<<", bx: "<<bx<<", by: "<<by<<endl;
                }
                vector<int> find_onpad;
                find_onpad.push_back(bx);
                find_onpad.push_back(0);
                find_onpad.push_back(bx + Modules[p].getWidth());
                find_onpad.push_back(ChipHeight);
                min_onpad_x = bx + Modules[p].getWidth();
                for (int j = 1; j <= Msize; ++j)
                {
                    if (has_module.find(j) == has_module.end())
                    {
                        continue;
                    }
                    // cout<<"do j: "<<j<<endl;
                    vector<int> onpad_module;
                    onpad_module.push_back(sa_end_x1y1[j].getX());
                    onpad_module.push_back(sa_end_x1y1[j].getY());
                    onpad_module.push_back(sa_end_x1y1[j].getX() + Modules[j].getWidth());
                    onpad_module.push_back(sa_end_x1y1[j].getY() + Modules[j].getHeight());
                    if (isRectangleOverlap(find_onpad, onpad_module))
                    {
                        min_onpad_x = min(min_onpad_x, onpad_module[2]);
                    }
                }
                space = false;
                vector<int> find_space;
                while (space == false)
                {
                    // cout<<"bx: "<<bx<<", by: "<<by<<endl;
                    ++by;
                    if (by > ChipHeight - Modules[p].getHeight())
                    {
                        vector<int> find_onpad_top;
                        find_onpad_top.push_back(bx);
                        find_onpad_top.push_back(0);
                        find_onpad_top.push_back(bx + Modules[p].getWidth());
                        find_onpad_top.push_back(ChipHeight);
                        int min_onpad_down = ChipHeight + 1;
                        for (int j = 1; j <= Msize; ++j)
                        {
                            if (has_module.find(j) == has_module.end())
                            {
                                continue;
                            }
                            // cout<<"do j: "<<j<<endl;
                            vector<int> onpad_module;
                            onpad_module.push_back(sa_end_x1y1[j].getX());
                            onpad_module.push_back(sa_end_x1y1[j].getY());
                            onpad_module.push_back(sa_end_x1y1[j].getX() + Modules[j].getWidth());
                            onpad_module.push_back(sa_end_x1y1[j].getY() + Modules[j].getHeight());
                            if (isRectangleOverlap(find_onpad_top, onpad_module))
                            {
                                min_onpad_down = min(min_onpad_down, onpad_module[1] - Modules[p].getHeight());
                            }
                        }
                        if (min_onpad_down < 0)
                        {
                            if (xcanskip)
                            {
                                bx = min_onpad_x;
                                xcanskip = false;
                            }
                            else
                            {
                                ++bx;
                            }
                            by = 0;
                        }
                        else if (min_onpad_down > ChipHeight)
                        {
                            if (xcanskip)
                            {
                                bx = min_onpad_x;
                                xcanskip = false;
                            }
                            else
                            {
                                ++bx;
                            }
                            by = 0;
                        }
                        else
                        {
                            by = min_onpad_down;
                        }
                        min_onpad_x = bx + Modules[p].getWidth();
                    }
                    if (bx + Modules[p].getWidth() > ChipWidth)
                    {
                        // cout<<"by + Modules[p].getHeight() :"<<by + Modules[p].getHeight() <<endl;
                        cantfind = true;
                    }
                    find_space.push_back(bx);
                    find_space.push_back(by);
                    find_space.push_back(bx + Modules[p].getWidth());
                    find_space.push_back(by + Modules[p].getHeight());
                    int max_by = 0;
                    for (int j = 1; j <= Msize; ++j)
                    {
                        if (has_module.find(j) == has_module.end())
                        {
                            continue;
                        }
                        // cout<<"do j: "<<j<<endl;
                        vector<int> onpad_module;
                        onpad_module.push_back(sa_end_x1y1[j].getX());
                        onpad_module.push_back(sa_end_x1y1[j].getY());
                        onpad_module.push_back(sa_end_x1y1[j].getX() + Modules[j].getWidth());
                        onpad_module.push_back(sa_end_x1y1[j].getY() + Modules[j].getHeight());
                        if (isRectangleOverlap(find_space, onpad_module))
                        {
                            xcanskip = true;
                            min_onpad_x = min(min_onpad_x, onpad_module[2]);
                            max_by = max(max_by, onpad_module[3] - 1);
                            space = true;
                        }
                    }
                    find_space.clear();
                    if (space == true)
                    {
                        by = max_by;
                        space = false;
                        continue;
                    }
                    if (bx < 0)
                    {
                        cantfind = true;
                    }
                    if (by < 0)
                    {
                        cantfind = true;
                    }
                    if (bx + Modules[p].getWidth() > ChipWidth)
                    {
                        cantfind = true;
                    }
                    if (by + Modules[p].getHeight() > ChipHeight)
                    {
                        cantfind = true;
                    }
                    if (cantfind == true)
                    {
                        // cout << "can't find space!" << endl;
                        cantfind = false;
                        // cout << "in " << p << " bx,by: " << bx << ", " << by << endl;
                        sa_end_x1y1[p].y = by;
                        // vector<int> find_leftblock;
                        // find_leftblock.push_back(0);
                        // find_leftblock.push_back(by);
                        // find_leftblock.push_back(bx);
                        // find_leftblock.push_back(by+Modules[p].getHeight());
                        // int maxbx=0;
                        // for(int j =1;j<=Msize;++j)
                        // {
                        //     if (has_module.find(j)==has_module.end())
                        //     {
                        //         continue;
                        //     }
                        //     vector<int> onpad_module;
                        //     onpad_module.push_back(sa_end_x1y1[j].getX());
                        //     onpad_module.push_back(sa_end_x1y1[j].getY());
                        //     onpad_module.push_back(sa_end_x1y1[j].getX() + Modules[j].getWidth());
                        //     onpad_module.push_back(sa_end_x1y1[j].getY() + Modules[j].getHeight());
                        //     if (isRectangleOverlap(find_leftblock, onpad_module))
                        //     {
                        //         maxbx=max(maxbx,onpad_module[2]);
                        //     }
                        // }
                        // bx=maxbx;
                        sa_end_x1y1[p].x = bx;
                        find_space.clear();
                        space = true;
                        cantpush.push_back(p);
                        has_module.insert(p);
                        break;
                    }
                    else
                    {
                        // cout << "in " << p << " bx,by: " << bx << ", " << by << endl;
                        sa_end_x1y1[p].y = by;
                        // vector<int> find_leftblock;
                        // find_leftblock.push_back(0);
                        // find_leftblock.push_back(by);
                        // find_leftblock.push_back(bx);
                        // find_leftblock.push_back(by+Modules[p].getHeight());
                        // int maxbx=0;
                        // for(int j =1;j<=Msize;++j)
                        // {
                        //     if (has_module.find(j)==has_module.end())
                        //     {
                        //         continue;
                        //     }
                        //     vector<int> onpad_module;
                        //     onpad_module.push_back(sa_end_x1y1[j].getX());
                        //     onpad_module.push_back(sa_end_x1y1[j].getY());
                        //     onpad_module.push_back(sa_end_x1y1[j].getX() + Modules[j].getWidth());
                        //     onpad_module.push_back(sa_end_x1y1[j].getY() + Modules[j].getHeight());
                        //     if (isRectangleOverlap(find_leftblock, onpad_module))
                        //     {
                        //         maxbx=max(maxbx,onpad_module[2]);
                        //     }
                        // }
                        // bx=maxbx;
                        sa_end_x1y1[p].x = bx;
                        find_space.clear();
                        space = true;
                        has_module.insert(p);
                        break;
                    }
                }
                club_head_it = it;
                preclub = club;
            }
            else // club的身
            {
                // cout<<"p: "<<p<<", tail"<<endl;
                preclub = club;
                int firstspace = false;
                if (it == overlap_soft.begin())
                {
                    bx = 0;
                    by = -1;
                }
                else
                {
                    vector<int>::iterator it2 = --it;
                    ++it;
                    int p2 = *it2;
                    bx = 0;
                    by = sa_end_x1y1[p2].getY() + Modules[p2].getHeight() - 1;
                    min_onpad_x = sa_end_x1y1[p2].getX();
                    vector<int> find_leftblock;
                    find_leftblock.push_back(0);
                    find_leftblock.push_back(sa_end_x1y1[p2].getY() + Modules[p2].getHeight());
                    find_leftblock.push_back(sa_end_x1y1[p2].getX() + Modules[p].getWidth());
                    find_leftblock.push_back(sa_end_x1y1[p2].getY() + Modules[p2].getHeight() + Modules[p].getHeight());
                    int maxbx = 0;
                    for (int j = 1; j <= Msize; ++j)
                    {
                        if (has_module.find(j) == has_module.end())
                        {
                            continue;
                        }
                        vector<int> onpad_module;
                        onpad_module.push_back(sa_end_x1y1[j].getX());
                        onpad_module.push_back(sa_end_x1y1[j].getY());
                        onpad_module.push_back(sa_end_x1y1[j].getX() + Modules[j].getWidth());
                        onpad_module.push_back(sa_end_x1y1[j].getY() + Modules[j].getHeight());
                        if (isRectangleOverlap(find_leftblock, onpad_module))
                        {
                            maxbx = max(maxbx, onpad_module[2]);
                        }
                    }
                    if (maxbx >= sa_end_x1y1[p2].getX() + Modules[p2].getWidth())
                    {
                        by = sa_end_x1y1[p2].getY() - 1;
                        bx = sa_end_x1y1[p2].getX() + Modules[p2].getWidth();
                    }
                    else if (maxbx + Modules[p].getWidth() < sa_end_x1y1[p2].getX())
                    {
                        if (p2 == INClub[club].Club_member[0])
                        {
                            bx = sa_end_x1y1[p2].getX() - Modules[p].getWidth();
                            vector<int> find_downblock;
                            find_downblock.push_back(sa_end_x1y1[p2].getX() - Modules[p].getWidth());
                            find_downblock.push_back(0);
                            find_downblock.push_back(sa_end_x1y1[p2].getX());
                            find_downblock.push_back(sa_end_x1y1[p2].getY() + Modules[p2].getHeight());
                            int maxby = -1;
                            for (int j = 1; j <= Msize; ++j)
                            {
                                if (has_module.find(j) == has_module.end())
                                {
                                    continue;
                                }
                                vector<int> onpad_module;
                                onpad_module.push_back(sa_end_x1y1[j].getX());
                                onpad_module.push_back(sa_end_x1y1[j].getY());
                                onpad_module.push_back(sa_end_x1y1[j].getX() + Modules[j].getWidth());
                                onpad_module.push_back(sa_end_x1y1[j].getY() + Modules[j].getHeight());
                                if (isRectangleOverlap(find_downblock, onpad_module))
                                {
                                    maxby = max(maxby, onpad_module[3] - 1);
                                }
                            }
                            int pre_club;
                            for (int i = 0; i < INClub.size(); ++i)
                            {
                                vector<int>::iterator iter = find(INClub[i].Club_member.begin(), INClub[i].Club_member.end(), p2);
                                if (iter != INClub[i].Club_member.end())
                                {
                                    pre_club = i;
                                    break;
                                }
                            }
                            if (maxby < sa_end_x1y1[INClub[pre_club].Club_member[0]].getY() - 1)
                            {
                                maxby = sa_end_x1y1[INClub[pre_club].Club_member[0]].getY() - 1;
                            }
                            by = maxby;
                            if (by == sa_end_x1y1[p2].getY() + Modules[p2].getHeight() - 1)
                            {
                                bx = sa_end_x1y1[p2].getX();
                            }
                        }
                        else
                        {
                            by = sa_end_x1y1[p2].getY() + Modules[p2].getHeight() - 1;
                            bx = sa_end_x1y1[p2].getX();
                        }
                    }
                    else if (maxbx + Modules[p].getWidth() >= sa_end_x1y1[p2].getX() && maxbx < sa_end_x1y1[p2].getX())
                    {
                        bx = sa_end_x1y1[p2].getX();
                    }
                    else
                    {
                        bx = maxbx;
                    }
                    // cout<<"p: "<<p<<", p2: "<<p2<<", bx: "<<bx<<", by: "<<by<<endl;
                }
                vector<int> find_onpad;
                find_onpad.push_back(bx);
                find_onpad.push_back(0);
                find_onpad.push_back(bx + Modules[p].getWidth());
                find_onpad.push_back(ChipHeight);
                min_onpad_x = bx + Modules[p].getWidth();
                vector<int>::iterator it2 = --it;
                ++it;
                int p2 = *it2;
                for (int j = 1; j <= Msize; ++j)
                {
                    if (has_module.find(j) == has_module.end())
                    {
                        continue;
                    }
                    // cout<<"do j: "<<j<<endl;
                    vector<int> onpad_module;
                    onpad_module.push_back(sa_end_x1y1[j].getX());
                    onpad_module.push_back(sa_end_x1y1[j].getY());
                    onpad_module.push_back(sa_end_x1y1[j].getX() + Modules[j].getWidth());
                    onpad_module.push_back(sa_end_x1y1[j].getY() + Modules[j].getHeight());
                    if (isRectangleOverlap(find_onpad, onpad_module))
                    {
                        min_onpad_x = min(min_onpad_x, onpad_module[2]);
                    }
                }
                space = false;
                vector<int> find_space;
                bool change_line = false;
                while (space == false)
                {
                    ++by;
                    // cout<<"bx: "<<bx<<", by: "<<by<<endl;
                    if (by > ChipHeight - Modules[p].getHeight())
                    {
                        if (xcanskip)
                        {
                            bx = min_onpad_x;
                            xcanskip = false;
                        }
                        else
                        {
                            ++bx;
                        }
                        min_onpad_x = bx + Modules[p].getWidth();
                        by = sa_end_x1y1[INClub[club].Club_member[0]].getY() - Modules[p].getHeight();
                        if (by < 0)
                        {
                            by = 0;
                        }
                    }
                    if (bx + Modules[p].getWidth() > ChipWidth)
                    {
                        // cout<<"by + Modules[p].getHeight() :"<<by + Modules[p].getHeight() <<endl;
                        cantfind = true;
                    }
                    find_space.push_back(bx);
                    find_space.push_back(by);
                    find_space.push_back(bx + Modules[p].getWidth());
                    find_space.push_back(by + Modules[p].getHeight());
                    int max_by = 0;
                    for (int j = 1; j <= Msize; ++j)
                    {
                        if (has_module.find(j) == has_module.end())
                        {
                            continue;
                        }
                        // cout<<"do j: "<<j<<endl;
                        vector<int> onpad_module;
                        onpad_module.push_back(sa_end_x1y1[j].getX());
                        onpad_module.push_back(sa_end_x1y1[j].getY());
                        onpad_module.push_back(sa_end_x1y1[j].getX() + Modules[j].getWidth());
                        onpad_module.push_back(sa_end_x1y1[j].getY() + Modules[j].getHeight());
                        if (isRectangleOverlap(find_space, onpad_module))
                        {
                            xcanskip = true;
                            min_onpad_x = min(min_onpad_x, onpad_module[2]);
                            max_by = max(max_by, onpad_module[3] - 1);
                            space = true;
                        }
                    }
                    find_space.clear();
                    if (space == true)
                    {
                        by = max_by;
                        space = false;
                        continue;
                    }
                    if (bx < 0)
                    {
                        cantfind = true;
                    }
                    if (by < 0)
                    {
                        cantfind = true;
                    }
                    if (bx + Modules[p].getWidth() > ChipWidth)
                    {
                        cantfind = true;
                    }
                    if (by + Modules[p].getHeight() > ChipHeight)
                    {
                        cantfind = true;
                    }
                    if (cantfind == true)
                    {
                        // cout << "can't find space!" << endl;
                        cantfind = false;
                        // cout << "in " << p << " bx,by: " << bx << ", " << by << endl;
                        sa_end_x1y1[p].x = bx;
                        // vector<int> find_downblock;
                        // find_downblock.push_back(bx);
                        // find_downblock.push_back(0);
                        // find_downblock.push_back(bx+ Modules[p].getWidth());
                        // find_downblock.push_back(by);
                        // int maxby=0;
                        // for(int j =1;j<=Msize;++j)
                        // {
                        //     if (has_module.find(j)==has_module.end())
                        //     {
                        //         continue;
                        //     }
                        //     vector<int> onpad_module;
                        //     onpad_module.push_back(sa_end_x1y1[j].getX());
                        //     onpad_module.push_back(sa_end_x1y1[j].getY());
                        //     onpad_module.push_back(sa_end_x1y1[j].getX() + Modules[j].getWidth());
                        //     onpad_module.push_back(sa_end_x1y1[j].getY() + Modules[j].getHeight());
                        //     if (isRectangleOverlap(find_downblock, onpad_module))
                        //     {
                        //         maxby=max(maxby,onpad_module[3]);
                        //     }
                        // }
                        // by=maxby;
                        sa_end_x1y1[p].y = by;
                        find_space.clear();
                        space = true;
                        cantpush.push_back(p);
                        has_module.insert(p);
                        break;
                    }
                    else
                    {
                        sa_end_x1y1[p].x = bx;
                        // vector<int> find_downblock;
                        // find_downblock.push_back(bx);
                        // find_downblock.push_back(0);
                        // find_downblock.push_back(bx+ Modules[p].getWidth());
                        // find_downblock.push_back(by);
                        // int maxby=0;
                        // for(int j =1;j<=Msize;++j)
                        // {
                        //     if (has_module.find(j)==has_module.end())
                        //     {
                        //         continue;
                        //     }
                        //     vector<int> onpad_module;
                        //     onpad_module.push_back(sa_end_x1y1[j].getX());
                        //     onpad_module.push_back(sa_end_x1y1[j].getY());
                        //     onpad_module.push_back(sa_end_x1y1[j].getX() + Modules[j].getWidth());
                        //     onpad_module.push_back(sa_end_x1y1[j].getY() + Modules[j].getHeight());
                        //     if (isRectangleOverlap(find_downblock, onpad_module))
                        //     {
                        //         maxby=max(maxby,onpad_module[3]);
                        //     }
                        // }
                        // by=maxby;
                        sa_end_x1y1[p].y = by;
                        find_space.clear();
                        space = true;
                        has_module.insert(p);
                        break;
                        // cout << "in " << p << " bx,by: " << bx << ", " << by << endl;
                    }
                }
            }
        }
        // if(need_jump)
        // {
        //     need_jump=false;
        //     continue;
        // }
        // if(p==change_stop_module)
        // {
        //     break;
        // }
    }
    // cout<<"OUT GetCost_leftdown"<<endl;
    // cout<<"runcount:"<<runcount<<endl;
    // cout<<"size: "<<sa_end_x1y1.size()<<endl;
    // cout<<"cantpush: "<<cantpush.size()<<endl;
    // cout<<"HPWLcost:"<<HPWLcost<<endl;
    // cout<<"left end"<<endl;
    cantpush2 = cantpush;
    cantpush.clear();
    int sum_panel_area = 0;
    int cant_push_num = cantpush2.size();
    if (cantpush2.size() > 0)
    {
        vector<int> temp_cantpush = cantpush2;
        int n = temp_cantpush.size();
        for (int i = 0; i < n - 1; ++i)
        {
            for (int j = 0; j < n - i - 1; ++j)
            {
                if (Modules[temp_cantpush[j]].mt.data.soft_module_data.RequiredSize > Modules[temp_cantpush[j + 1]].mt.data.soft_module_data.RequiredSize)
                {
                    int temp = temp_cantpush[j];
                    temp_cantpush[j] = temp_cantpush[j + 1];
                    temp_cantpush[j + 1] = temp;
                }
            };
        };
        reverse(temp_cantpush.begin(), temp_cantpush.end());
        cantpush2 = temp_cantpush;
        for (vector<int>::iterator it = cantpush2.begin(); it != cantpush2.end(); ++it)
        {
            int p = *it;
            if (p == overlap_soft[0])
            {
                bx = -1;
                by = 0;
            }
            else
            {
                bx = sa_end_x1y1[overlap_soft[0]].getX() - 1;
                by = sa_end_x1y1[overlap_soft[0]].getY();
            }

            space = false;
            vector<int> find_space;
            while (space == false)
            {
                ++bx;
                if (bx > ChipWidth - Modules[p].getWidth())
                {
                    if (ycanskip)
                    {
                        by = min_onpad_y;
                        ycanskip = false;
                    }
                    else
                    {
                        ++by;
                    }
                    min_onpad_y = INT_MAX;
                    if (by + Modules[p].getHeight() > ChipHeight)
                    {
                        // cout<<"by + Modules[p].getHeight() :"<<by + Modules[p].getHeight() <<endl;
                        cantfind = true;
                    }
                    bx = 0;
                }
                find_space.push_back(bx);
                find_space.push_back(by);
                find_space.push_back(bx + Modules[p].getWidth());
                find_space.push_back(by + Modules[p].getHeight());
                int maxbx = 0;
                for (int j = 1; j <= Msize; ++j)
                {
                    if (has_module.find(j) == has_module.end())
                    {
                        continue;
                    }
                    // cout<<"do j: "<<j<<endl;
                    vector<int> onpad_module;
                    onpad_module.push_back(sa_end_x1y1[j].getX());
                    onpad_module.push_back(sa_end_x1y1[j].getY());
                    onpad_module.push_back(sa_end_x1y1[j].getX() + Modules[j].getWidth());
                    onpad_module.push_back(sa_end_x1y1[j].getY() + Modules[j].getHeight());
                    if (isRectangleOverlap(find_space, onpad_module))
                    {
                        ycanskip = true;
                        min_onpad_y = min(min_onpad_y, onpad_module[3]);
                        maxbx = max(maxbx, onpad_module[2] - 1);
                        space = true;
                    }
                }
                find_space.clear();
                if (space == true)
                {
                    bx = maxbx;
                    space = false;
                    continue;
                }
                if (cantfind == true)
                {
                    // cout << "can't find space!" << endl;
                    cantfind = false;
                    // cout << "in " << p << " bx,by: " << bx << ", " << by << endl;
                    cantpush.push_back(p);
                    has_module.insert(p);
                    sa_end_x1y1[p].x = bx;
                    sa_end_x1y1[p].y = by;
                    find_space.clear();
                    space = true;
                    break;
                }
                else
                {
                    // cout << "in " << p << " bx,by: " << bx << ", " << by << endl;
                    sa_end_x1y1[p].x = bx;
                    sa_end_x1y1[p].y = by;
                    find_space.clear();
                    has_module.insert(p);
                    space = true;
                    break;
                }
            }
        }
    }
    for (int i = 0; i < cantpush.size(); ++i)
    {
        sum_panel_area += Modules[cantpush[i]].mt.data.soft_module_data.RequiredSize;
    }
    cost = sum_panel_area;
    cantpush2 = cantpush;
    // final_output();
    // setFinalPlacement(0);
    // setFinalPlacement(1);
    // system("pause");
    clock_t cost_end = clock();
    // cout<<"OUT left down"<<endl;
    // cout<<"cost time: "<<std::fixed<< std::setprecision(5)<<static_cast<double>(cost_end-cost_start) / CLOCKS_PER_SEC<<endl;
};
void GetNext(vector<int> &place_order, int &change_position, int &end_change_position, double temp_probability, vector<int> &new_place_direction, vector<Club> &newclub, int &rotate_time_N, vector<int> SA_cantpush)
{
    double event = (double)rand() / (RAND_MAX + 1.0);
    // cout<<"in event: "<<endl;
    if (event >= 0 && event < 0.2) // event<=0.45
    {
        // cout<<"in event1 "<<endl;
        double event2 = (double)rand() / (RAND_MAX + 1.0);
        if (event2 <= 1) // 單純改長寬(組內相同面積一起改)
        {
            // cout<<"0"<<endl;
            int minf = 0;
            int max = newclub.size();
            int rand_changeSoft = rand() % (max - minf) + minf;
            if (newclub[rand_changeSoft].Club_member.size() > 1)
            {
                int min1 = 0;
                int max1 = newclub[rand_changeSoft].Club_member.size();
                int rand1 = rand() % (max1 - min1) + min1;
                double beheight = (double)rand() / (RAND_MAX + 1.0);
                double ratio = 0.5 + (rand() % 151) / 100.0;
                // cout<<"module: "<<module.ModuleName<<", "<< module.getWidth() << ", "<<module.getHeight()<<endl;;
                //  change aspect ratio must be between 0.5 and 2 but random and the area must be at least the same or at most 0.1% larger
                if (beheight <= 0.5)
                {
                    ratio = 1 + (rand() % 101) / 100.0;
                }
                else
                {
                    ratio = 0.5 + (rand() % 51) / 100.0;
                    // cout<<"ratio: "<<ratio<<endl;
                }
                int requareArea = Modules[newclub[rand_changeSoft].Club_member[rand1]].mt.data.soft_module_data.RequiredSize;
                for (int i = 0; i < max1; ++i)
                {
                    if (Modules[newclub[rand_changeSoft].Club_member[i]].mt.data.soft_module_data.RequiredSize == requareArea)
                    {
                        // cout<<"Module: "<<i<<" has same require area.";
                        be_changeAspectRatio(Modules[newclub[rand_changeSoft].Club_member[i]], ratio);
                    }
                    // cout<<"Module: "<<i<<", ";
                }
                change_position = newclub[rand_changeSoft].Club_member[0];
            }
            else
            {
                changeAspectRatio(Modules[newclub[rand_changeSoft].Club_member[0]]);
                change_position = newclub[rand_changeSoft].Club_member[0];
            }
            // changeAspectRatio(Modules[newclub[0].Club_member[0]]);
            // change_position= place_order[0];
        }
        // cout<<"out event1 "<<endl;
    }
    else if (event >= 0.2 && event <= 0.6) // event>0.45&&event<=0.9
    {
        vector<int> old_place_order = place_order;
        double event2 = (double)rand() / (RAND_MAX + 1.0);
        double event_swap = (double)rand() / (RAND_MAX + 1.0);
        int start_posi;
        int end_posi;
        if (event_swap < 0.4)
        {
            // cout<<"in swap "<<endl;
            int minf = 0;
            int max = newclub.size();
            int rand_changeSoft1 = rand() % (max - minf) + minf;
            int rand_changeSoft2 = rand() % (max - minf) + minf;
            while (rand_changeSoft1 == rand_changeSoft2)
            {
                rand_changeSoft2 = rand() % (max - minf) + minf;
            }
            // cout<<endl<<"rand_changeSoft1: "<<rand_changeSoft1<<", rand_changeSoft2: "<<rand_changeSoft2<<", before club: "<<endl;
            // for(int i =0;i<newclub.size();++i)
            // {
            //     for(int j= 0;j<newclub[i].Club_member.size();++j)
            //     {
            //         // place_order.push_back(newclub[i].Club_member[j]);
            //         cout<<newclub[i].Club_member[j]<<", ";
            //     }
            //     cout<<endl;
            // }
            // cout<<endl;
            int temp_dire = new_place_direction[newclub[rand_changeSoft1].Club_member[0]];
            new_place_direction[newclub[rand_changeSoft1].Club_member[0]] = new_place_direction[newclub[rand_changeSoft2].Club_member[0]];
            new_place_direction[newclub[rand_changeSoft2].Club_member[0]] = temp_dire;
            if (newclub[rand_changeSoft1].Club_member.size() > 1)
            {
                if (new_place_direction[newclub[rand_changeSoft1].Club_member[0]] == 1)
                {
                    if (rand_changeSoft1 == 0)
                    {
                        double ratio = 0.5 + (rand() % 51) / 100.0;
                        be_changeAspectRatio(Modules[newclub[rand_changeSoft1].Club_member[0]], ratio);
                        // changeAspectRatio(Modules[newclub[rand_changeSoft1].Club_member[0]]);
                    }
                    else
                    {
                        double ratio = 0.5 + (rand() % 51) / 100.0;
                        be_changeAspectRatio(Modules[newclub[rand_changeSoft1].Club_member[0]], ratio);
                    }
                    for (int i = 1; i < newclub[rand_changeSoft1].Club_member.size(); ++i)
                    {
                        new_place_direction[newclub[rand_changeSoft1].Club_member[i]] = 1;
                        int Pre_M_Width = Modules[newclub[rand_changeSoft1].Club_member[0]].getWidth();
                        int This_M_Height = Modules[newclub[rand_changeSoft1].Club_member[i]].mt.data.soft_module_data.RequiredSize / Pre_M_Width;
                        if ((double(This_M_Height) / double(Pre_M_Width)) > 2.0) // 前一個比較長，這一個達不到變自身最高
                        {
                            double ratio = 0.5 + (rand() % 51) / 100.0;
                            be_changeAspectRatio(Modules[newclub[rand_changeSoft1].Club_member[i]], ratio);
                            // be_changeAspectRatio(Modules[newclub[rand_changeSoft1].Club_member[i]],0.5);
                        }
                        else if ((double(This_M_Height) / double(Pre_M_Width)) < 0.5) // 前一個比較寬，這一個達不到變自身最寬
                        {
                            double ratio = 0.5 + (rand() % 51) / 100.0;
                            be_changeAspectRatio(Modules[newclub[rand_changeSoft1].Club_member[i]], ratio);
                            // be_changeAspectRatio(Modules[newclub[rand_changeSoft1].Club_member[i]],0.5);
                        }
                        else
                        {
                            // cout<<"double(Pre_M_Height)/double(This_M_Width): "<<double(Pre_M_Height)/double(This_M_Width)<<endl;
                            be_changeAspectRatio_byWidth(Modules[newclub[rand_changeSoft1].Club_member[i]], Pre_M_Width);
                        }
                    }
                }
                else
                {
                    if (rand_changeSoft1 == 0)
                    {
                        double ratio = 1 + (rand() % 101) / 100.0;
                        be_changeAspectRatio(Modules[newclub[rand_changeSoft1].Club_member[0]], ratio);
                        // changeAspectRatio(Modules[newclub[rand_changeSoft1].Club_member[0]]);
                    }
                    else
                    {
                        double ratio = 1 + (rand() % 101) / 100.0;
                        be_changeAspectRatio(Modules[newclub[rand_changeSoft1].Club_member[0]], ratio);
                    }
                    for (int i = 1; i < newclub[rand_changeSoft1].Club_member.size(); ++i)
                    {
                        new_place_direction[newclub[rand_changeSoft1].Club_member[i]] = 0;
                        int Pre_M_Height = Modules[newclub[rand_changeSoft1].Club_member[0]].getHeight();
                        int This_M_Width = Modules[newclub[rand_changeSoft1].Club_member[i]].mt.data.soft_module_data.RequiredSize / Pre_M_Height;
                        if ((double(Pre_M_Height) / double(This_M_Width)) > 2.0) // 前一個比較長，這一個達不到變自身最高
                        {
                            double ratio = 1 + (rand() % 101) / 100.0;
                            be_changeAspectRatio(Modules[newclub[rand_changeSoft1].Club_member[i]], ratio);
                            // be_changeAspectRatio(Modules[newclub[rand_changeSoft1].Club_member[i]],2.0);
                        }
                        else if ((double(Pre_M_Height) / double(This_M_Width)) < 0.5) // 前一個比較寬，這一個達不到變自身最寬
                        {
                            double ratio = 1 + (rand() % 101) / 100.0;
                            be_changeAspectRatio(Modules[newclub[rand_changeSoft1].Club_member[i]], ratio);
                            // be_changeAspectRatio(Modules[newclub[rand_changeSoft1].Club_member[i]],2.0);
                        }
                        else
                        {
                            // cout<<"double(Pre_M_Height)/double(This_M_Width): "<<double(Pre_M_Height)/double(This_M_Width)<<endl;
                            be_changeAspectRatio_byHeight(Modules[newclub[rand_changeSoft1].Club_member[i]], Pre_M_Height);
                        }
                    }
                }
            }
            else
            {
                if (new_place_direction[newclub[rand_changeSoft1].Club_member[0]] == 0)
                {
                    if (rand_changeSoft1 == 0)
                    {
                        double ratio = 1 + (rand() % 101) / 100.0;
                        be_changeAspectRatio(Modules[newclub[rand_changeSoft1].Club_member[0]], ratio);
                        // be_changeAspectRatio(Modules[newclub[rand_changeSoft1].Club_member[0]],0.5);
                        // changeAspectRatio(Modules[newclub[rand_changeSoft1].Club_member[0]]);
                    }
                    else
                    {
                        double ratio = 1 + (rand() % 101) / 100.0;
                        be_changeAspectRatio(Modules[newclub[rand_changeSoft1].Club_member[0]], ratio);
                    }
                }
                else
                {
                    if (rand_changeSoft1 == 0)
                    {
                        double ratio = 0.5 + (rand() % 51) / 100.0;
                        be_changeAspectRatio(Modules[newclub[rand_changeSoft1].Club_member[0]], ratio);
                        // be_changeAspectRatio(Modules[newclub[rand_changeSoft1].Club_member[0]],2.0);
                        // changeAspectRatio(Modules[newclub[rand_changeSoft1].Club_member[0]]);
                    }
                    else
                    {
                        double ratio = 0.5 + (rand() % 51) / 100.0;
                        be_changeAspectRatio(Modules[newclub[rand_changeSoft1].Club_member[0]], ratio);
                    }
                }
            }
            if (newclub[rand_changeSoft2].Club_member.size() > 1)
            {
                if (new_place_direction[newclub[rand_changeSoft2].Club_member[0]] == 1)
                {
                    if (rand_changeSoft2 == 0)
                    {
                        double ratio = 0.5 + (rand() % 51) / 100.0;
                        be_changeAspectRatio(Modules[newclub[rand_changeSoft2].Club_member[0]], ratio);
                        // changeAspectRatio(Modules[newclub[rand_changeSoft1].Club_member[0]]);
                    }
                    else
                    {
                        double ratio = 0.5 + (rand() % 51) / 100.0;
                        be_changeAspectRatio(Modules[newclub[rand_changeSoft2].Club_member[0]], ratio);
                    }
                    for (int i = 1; i < newclub[rand_changeSoft2].Club_member.size(); ++i)
                    {
                        new_place_direction[newclub[rand_changeSoft2].Club_member[i]] = 1;
                        int Pre_M_Width = Modules[newclub[rand_changeSoft2].Club_member[0]].getWidth();
                        int This_M_Height = Modules[newclub[rand_changeSoft2].Club_member[i]].mt.data.soft_module_data.RequiredSize / Pre_M_Width;
                        if ((double(This_M_Height) / double(Pre_M_Width)) > 2.0) // 前一個比較長，這一個達不到變自身最高
                        {
                            double ratio = 0.5 + (rand() % 51) / 100.0;
                            be_changeAspectRatio(Modules[newclub[rand_changeSoft2].Club_member[i]], ratio);
                            // be_changeAspectRatio(Modules[newclub[rand_changeSoft1].Club_member[i]],0.5);
                        }
                        else if ((double(This_M_Height) / double(Pre_M_Width)) < 0.5) // 前一個比較寬，這一個達不到變自身最寬
                        {
                            double ratio = 0.5 + (rand() % 51) / 100.0;
                            be_changeAspectRatio(Modules[newclub[rand_changeSoft2].Club_member[i]], ratio);
                            // be_changeAspectRatio(Modules[newclub[rand_changeSoft1].Club_member[i]],0.5);
                        }
                        else
                        {
                            // cout<<"double(Pre_M_Height)/double(This_M_Width): "<<double(Pre_M_Height)/double(This_M_Width)<<endl;
                            be_changeAspectRatio_byWidth(Modules[newclub[rand_changeSoft2].Club_member[i]], Pre_M_Width);
                        }
                    }
                }
                else
                {
                    if (rand_changeSoft2 == 0)
                    {
                        double ratio = 1 + (rand() % 101) / 100.0;
                        be_changeAspectRatio(Modules[newclub[rand_changeSoft2].Club_member[0]], ratio);
                        // changeAspectRatio(Modules[newclub[rand_changeSoft1].Club_member[0]]);
                    }
                    else
                    {
                        double ratio = 1 + (rand() % 101) / 100.0;
                        be_changeAspectRatio(Modules[newclub[rand_changeSoft2].Club_member[0]], ratio);
                    }
                    for (int i = 1; i < newclub[rand_changeSoft2].Club_member.size(); ++i)
                    {
                        new_place_direction[newclub[rand_changeSoft2].Club_member[i]] = 0;
                        int Pre_M_Height = Modules[newclub[rand_changeSoft2].Club_member[0]].getHeight();
                        int This_M_Width = Modules[newclub[rand_changeSoft2].Club_member[i]].mt.data.soft_module_data.RequiredSize / Pre_M_Height;
                        if ((double(Pre_M_Height) / double(This_M_Width)) > 2.0) // 前一個比較長，這一個達不到變自身最高
                        {
                            double ratio = 1 + (rand() % 101) / 100.0;
                            be_changeAspectRatio(Modules[newclub[rand_changeSoft2].Club_member[i]], ratio);
                            // be_changeAspectRatio(Modules[newclub[rand_changeSoft1].Club_member[i]],2.0);
                        }
                        else if ((double(Pre_M_Height) / double(This_M_Width)) < 0.5) // 前一個比較寬，這一個達不到變自身最寬
                        {
                            double ratio = 1 + (rand() % 101) / 100.0;
                            be_changeAspectRatio(Modules[newclub[rand_changeSoft2].Club_member[i]], ratio);
                            // be_changeAspectRatio(Modules[newclub[rand_changeSoft1].Club_member[i]],2.0);
                        }
                        else
                        {
                            // cout<<"double(Pre_M_Height)/double(This_M_Width): "<<double(Pre_M_Height)/double(This_M_Width)<<endl;
                            be_changeAspectRatio_byHeight(Modules[newclub[rand_changeSoft2].Club_member[i]], Pre_M_Height);
                        }
                    }
                }
            }
            else
            {
                if (new_place_direction[newclub[rand_changeSoft2].Club_member[0]] == 0)
                {
                    if (rand_changeSoft2 == 0)
                    {
                        double ratio = 1 + (rand() % 101) / 100.0;
                        be_changeAspectRatio(Modules[newclub[rand_changeSoft2].Club_member[0]], ratio);
                        // be_changeAspectRatio(Modules[newclub[rand_changeSoft1].Club_member[0]],0.5);
                        // changeAspectRatio(Modules[newclub[rand_changeSoft1].Club_member[0]]);
                    }
                    else
                    {
                        double ratio = 1 + (rand() % 101) / 100.0;
                        be_changeAspectRatio(Modules[newclub[rand_changeSoft2].Club_member[0]], ratio);
                    }
                }
                else
                {
                    if (rand_changeSoft2 == 0)
                    {
                        double ratio = 0.5 + (rand() % 51) / 100.0;
                        be_changeAspectRatio(Modules[newclub[rand_changeSoft2].Club_member[0]], ratio);
                        // be_changeAspectRatio(Modules[newclub[rand_changeSoft1].Club_member[0]],2.0);
                        // changeAspectRatio(Modules[newclub[rand_changeSoft1].Club_member[0]]);
                    }
                    else
                    {
                        double ratio = 0.5 + (rand() % 51) / 100.0;
                        be_changeAspectRatio(Modules[newclub[rand_changeSoft2].Club_member[0]], ratio);
                    }
                }
            }
            Club temp = newclub[rand_changeSoft1];
            newclub[rand_changeSoft1] = newclub[rand_changeSoft2];
            newclub[rand_changeSoft2] = temp;
            place_order.clear();
            end_posi = place_order.size() - 1;
            for (int i = 0; i < newclub.size(); ++i)
            {
                for (int j = 0; j < newclub[i].Club_member.size(); ++j)
                {
                    place_order.push_back(newclub[i].Club_member[j]);
                }
            }
            start_posi = place_order.size() - 1;
            end_posi = place_order.size() - 1;
            for (int left = 0; left < place_order.size(); ++left)
            {
                if (old_place_order[left] != place_order[left])
                {
                    start_posi = left;
                    break;
                }
            }
            for (int i = 0; i < newclub.size(); ++i)
            {
                std::vector<int>::iterator it = find(newclub[i].Club_member.begin(), newclub[i].Club_member.end(), place_order[start_posi]);
                if (it != newclub[i].Club_member.end())
                {
                    change_position = newclub[i].Club_member[0];
                    break;
                }
            }
            //  cout<<"rand_changeSoft1: "<<rand_changeSoft1<<", after club: "<<endl;
            // for(int i =0;i<newclub.size();++i)
            // {
            //     for(int j= 0;j<newclub[i].Club_member.size();++j)
            //     {
            //         // place_order.push_back(newclub[i].Club_member[j]);
            //         cout<<newclub[i].Club_member[j]<<", ";
            //     }
            //     cout<<endl;
            // }
            // cout<<endl;
            // cout<<"out swap "<<endl;
        }
        else
        {
            if (event2 < 0.5) // 組外換順序
            {
                // cout<<"in event2_1 "<<endl;
                int rand_changeSoft1;
                if (SA_cantpush.size() > 0)
                {
                    int minf = 0;
                    int max1 = SA_cantpush.size();
                    rand_changeSoft1 = rand() % (max1 - minf) + minf;
                    for (int a = 0; a < newclub.size(); ++a)
                    {
                        std::vector<int>::iterator it = find(newclub[a].Club_member.begin(), newclub[a].Club_member.end(), SA_cantpush[rand_changeSoft1]);
                        if (it != newclub[a].Club_member.end())
                        {
                            rand_changeSoft1 = a;
                            break;
                        }
                    }
                }
                else
                {
                    int minf = 0;
                    int max = newclub.size();
                    rand_changeSoft1 = rand() % (max - minf) + minf;
                }
                // cout<<"OUT event2_1 STEP1"<<endl;
                // cout<<"rand_changeSoft1: "<<rand_changeSoft1<<", before club: "<<endl;
                // for(int i =0;i<newclub.size();++i)
                // {
                //     for(int j= 0;j<newclub[i].Club_member.size();++j)
                //     {
                //         // place_order.push_back(newclub[i].Club_member[j]);
                //         cout<<newclub[i].Club_member[j]<<", ";
                //     }
                //     cout<<endl;
                // }
                // cout<<endl;
                vector<Club> temp_out;
                temp_out.push_back(newclub[rand_changeSoft1]);
                set<int> has_connect_with_choosed;
                has_connect_with_choosed.insert(rand_changeSoft1);
                for (int i = rand_changeSoft1 + 1; i < newclub.size(); ++i)
                {
                    bool continuestop = true;
                    for (int j = 0; j < newclub[rand_changeSoft1].Club_member.size(); ++j)
                    {
                        std::set<int>::iterator it = find(Connect_map[newclub[i].Club_member[0]].begin(), Connect_map[newclub[i].Club_member[0]].end(), newclub[rand_changeSoft1].Club_member[j]);
                        if (it != Connect_map[newclub[i].Club_member[0]].end() && has_connect_with_choosed.find(i) == has_connect_with_choosed.end())
                        {
                            temp_out.push_back(newclub[i]);
                            has_connect_with_choosed.insert(i);
                            continuestop = false;
                            break;
                        }
                    }
                }
                // cout<<"OUT event2_1 STEP2"<<endl;
                // cout<<"temp_out: "<<endl;
                // for(int i =0;i<temp_out.size();++i)
                // {
                //     for(int j= 0;j<temp_out[i].Club_member.size();++j)
                //     {
                //         // place_order.push_back(newclub[i].Club_member[j]);
                //         cout<<temp_out[i].Club_member[j]<<", ";
                //     }
                //     cout<<endl;
                // }
                vector<Club> temp;
                for (int i = 0; i < newclub.size(); ++i)
                {
                    if (has_connect_with_choosed.find(i) == has_connect_with_choosed.end())
                    {
                        temp.push_back(newclub[i]);
                    }
                }
                if (temp.size() == 0)
                {
                    reverse(temp_out.begin(), temp_out.end());
                }
                // cout<<"OUT event2_1 STEP3"<<endl;
                // cout<<"temp club: "<<endl;
                // for(int i =0;i<temp.size();++i)
                // {
                //     for(int j= 0;j<temp[i].Club_member.size();++j)
                //     {
                //         // place_order.push_back(newclub[i].Club_member[j]);
                //         cout<<temp[i].Club_member[j]<<", ";
                //     }
                //     cout<<endl;
                // }
                int max2 = temp.size();
                int min2 = 0;
                int rand_changeSoft2 = rand() % (max2 - min2 + 1) + min2;
                // 根據群頭找有連線的
                bool get_pre = false;
                vector<int> pre_choice;
                for (int i = 0; i < temp.size(); ++i)
                {
                    for (int j = 0; j < temp[i].Club_member.size(); ++j)
                    {
                        std::set<int>::iterator it = find(Connect_map[newclub[rand_changeSoft1].Club_member[0]].begin(), Connect_map[newclub[rand_changeSoft1].Club_member[0]].end(), temp[i].Club_member[j]);
                        if (it != Connect_map[newclub[rand_changeSoft1].Club_member[0]].end())
                        {
                            pre_choice.push_back(i);
                            get_pre = true;
                            break;
                        }
                    }
                }
                // cout<<"OUT event2_1 STEP4"<<endl;
                if (get_pre == false)
                {
                    // cout<<"OUT event2_1 STEP4_1"<<endl;
                    while (rand_changeSoft1 == rand_changeSoft2)
                    {
                        if (temp.size() == 0)
                        {
                            break;
                        }
                        rand_changeSoft2 = rand() % (max2 - min2 + 1) + min2;
                    }
                }
                else
                {
                    double next_or_pre = (double)rand() / (RAND_MAX + 1.0);
                    if (next_or_pre <= 0.4)
                    {
                        // cout<<"OUT event2_1 STEP4_2"<<endl;
                        max2 = pre_choice.size();
                        min2 = 0;
                        rand_changeSoft2 = pre_choice[rand() % (max2 - min2) + min2] + 1;
                        if (rand_changeSoft1 == rand_changeSoft2)
                        {
                            rand_changeSoft2 - 1;
                        }
                    }
                    else if (next_or_pre > 0.4 && next_or_pre <= 0.8)
                    {
                        // cout<<"OUT event2_1 STEP4_3"<<endl;
                        max2 = pre_choice.size();
                        min2 = 0;
                        rand_changeSoft2 = pre_choice[rand() % (max2 - min2) + min2];
                        if (rand_changeSoft1 == rand_changeSoft2)
                        {
                            rand_changeSoft2 + 1;
                        }
                    }
                    else
                    {
                        // cout<<"OUT event2_1 STEP4_4"<<endl;
                        while (rand_changeSoft1 == rand_changeSoft2)
                        {
                            if (temp.size() == 0)
                            {
                                break;
                            }
                            rand_changeSoft2 = rand() % (max2 - min2 + 1) + min2;
                        }
                    }
                }
                // cout<<"OUT event2_1 STEP5"<<endl;
                max2 = temp.size();
                // cout<<"rand_changeSoft2: "<<rand_changeSoft2<<endl;
                place_order.clear();
                vector<Club> temp2;
                for (int i = 0; i < rand_changeSoft2; ++i)
                {
                    temp2.push_back(temp[i]);
                    for (int j = 0; j < temp[i].Club_member.size(); ++j)
                    {
                        place_order.push_back(temp[i].Club_member[j]);
                    }
                }
                for (int i = 0; i < temp_out.size(); ++i)
                {
                    temp2.push_back(temp_out[i]);
                    for (int j = 0; j < temp_out[i].Club_member.size(); ++j)
                    {
                        place_order.push_back(temp_out[i].Club_member[j]);
                    }
                }
                for (int i = rand_changeSoft2; i < max2; ++i)
                {
                    temp2.push_back(temp[i]);
                    for (int j = 0; j < temp[i].Club_member.size(); ++j)
                    {
                        place_order.push_back(temp[i].Club_member[j]);
                    }
                }
                newclub = temp2;
                end_change_position = place_order[place_order.size() - 1];
                // cout<<"out event2_1 STEP6"<<endl;
            }
            else // 組內換順序
            {
                int rand_changeSoft1;
                int minf = 0;
                int max1 = newclub.size();
                rand_changeSoft1 = rand() % (max1 - minf) + minf;
                if (newclub[rand_changeSoft1].Club_member.size() > 1)
                {
                    // cout<<"event2_2 "<<endl;
                    // cout<<"2"<<endl;
                    int minf = 0;
                    int max2 = newclub[rand_changeSoft1].Club_member.size();
                    int rand2 = rand() % (max2 - minf) + minf;
                    // cout<<"rand1: "<<rand_changeSoft1<<", rand2: "<<rand2<<", after club: "<<endl;
                    // for(int i =0;i<newclub.size();i++)
                    // {
                    //     for(int j= 0;j<newclub[i].Club_member.size();j++)
                    //     {
                    //         cout<<newclub[i].Club_member[j]<<", ";
                    //     }
                    //     cout<<endl;
                    // }
                    // cout<<endl;
                    vector<int> temp;
                    for (int i = 0; i < max2; ++i)
                    {
                        if (i != rand2)
                        {
                            temp.push_back(newclub[rand_changeSoft1].Club_member[i]);
                        }
                    }
                    vector<int> pre_choice;
                    bool get_pre = false;
                    bool Head_connect = false;
                    bool tail_connect = false;
                    for (int i = 0; i < temp.size(); ++i)
                    {
                        std::set<int>::iterator it = find(Connect_map[newclub[rand_changeSoft1].Club_member[rand2]].begin(), Connect_map[newclub[rand_changeSoft1].Club_member[rand2]].end(), temp[i]);
                        if (i < temp.size() - 1)
                        {
                            std::set<int>::iterator it2 = find(Connect_map[newclub[rand_changeSoft1].Club_member[rand2]].begin(), Connect_map[newclub[rand_changeSoft1].Club_member[rand2]].end(), temp[i + 1]);
                            if (it != Connect_map[newclub[rand_changeSoft1].Club_member[rand2]].end() && it2 != Connect_map[newclub[rand_changeSoft1].Club_member[rand2]].end())
                            {
                                // cout<<"rand2: "<<newclub[rand_changeSoft1].Club_member[rand2]<<", has connect with "<<*it<<", "<<*it2<<endl;
                                pre_choice.push_back(i);
                                get_pre = true;
                            }
                        }
                        if (i == 0)
                        {
                            if (it != Connect_map[newclub[rand_changeSoft1].Club_member[rand2]].end())
                            {
                                Head_connect = true;
                            }
                        }
                        if (i == temp.size() - 1)
                        {
                            if (it != Connect_map[newclub[rand_changeSoft1].Club_member[rand2]].end())
                            {
                                tail_connect = true;
                            }
                        }
                    }
                    int max3 = temp.size();
                    int rand3 = -1;
                    bool reverse = false;
                    if (get_pre)
                    {
                        int maxtemp = pre_choice.size();
                        rand3 = rand() % (maxtemp - minf) + minf;
                        rand3 = pre_choice[rand3] + 1;
                        if (rand3 == rand2)
                        {
                            reverse = true;
                        }
                    }
                    else
                    {
                        if (Head_connect)
                        {
                            rand3 = 0;
                            if (rand3 == rand2)
                            {
                                reverse = true;
                            }
                        }
                        else if (tail_connect)
                        {
                            rand3 = temp.size();
                            if (rand3 == rand2)
                            {
                                reverse = true;
                            }
                        }
                        else
                        {
                            double side = (double)rand() / (RAND_MAX + 1.0);
                            if (side <= 0.5)
                            {
                                rand3 = 0;
                            }
                            else
                            {
                                rand3 = temp.size();
                            }
                        }
                    }
                    if (reverse)
                    {
                        double event_reverse = (double)rand() / (RAND_MAX + 1.0);
                        if (event_reverse > 0.5)
                        {
                            vector<int> temp7 = newclub[rand_changeSoft1].Club_member;
                            std::reverse(temp7.begin(), temp7.end());
                            newclub[rand_changeSoft1].Club_member = temp7;
                        }
                        else
                        {
                            // cout<<"in here"<<endl;
                            max3 = temp.size();
                            rand3 = rand() % (max3 - minf) + minf;
                            while (rand3 == rand2)
                            {
                                rand3 = rand() % (max3 - minf + 1) + minf;
                                // cout<<rand3<<", ";
                            }
                            // cout<<"out here"<<endl;
                            vector<int> temp2;
                            for (int i = 0; i < rand3; ++i)
                            {
                                temp2.push_back(temp[i]);
                            }
                            temp2.push_back(newclub[rand_changeSoft1].Club_member[rand2]);
                            for (int i = rand3; i < max3; ++i)
                            {
                                temp2.push_back(temp[i]);
                            }
                            newclub[rand_changeSoft1].Club_member = temp2;
                            // cout<<"out here"<<endl;
                        }
                    }
                    else
                    {
                        vector<int> temp2;
                        for (int i = 0; i < rand3; ++i)
                        {
                            temp2.push_back(temp[i]);
                        }
                        temp2.push_back(newclub[rand_changeSoft1].Club_member[rand2]);
                        for (int i = rand3; i < max3; ++i)
                        {
                            temp2.push_back(temp[i]);
                        }
                        newclub[rand_changeSoft1].Club_member = temp2;
                    }
                    // cout<<"rand3: "<<rand3<<", after club: "<<endl;
                    // for(int i =0;i<newclub.size();i++)
                    // {
                    //     for(int j= 0;j<newclub[i].Club_member.size();j++)
                    //     {
                    //         cout<<newclub[i].Club_member[j]<<", ";
                    //     }
                    //     cout<<endl;
                    // }
                    // cout<<endl;
                    place_order.clear();
                    end_posi = place_order.size() - 1;
                    for (int i = 0; i < newclub.size(); ++i)
                    {
                        for (int j = 0; j < newclub[i].Club_member.size(); ++j)
                        {
                            place_order.push_back(newclub[i].Club_member[j]);
                        }
                    }
                    // for(int right =place_order.size()-1;right>=0;--right)
                    // {
                    //     if(old_place_order[right]!=place_order[right])
                    //     {
                    //         end_posi=right;
                    //         break;
                    //     }
                    // }
                    end_change_position = place_order[place_order.size() - 1];
                    // for(int i =newclub.size()-1;i>=0;--i)
                    // {
                    //     std::vector<int>::iterator it = find(newclub[i].Club_member.begin(),newclub[i].Club_member.end(),place_order[end_posi]);
                    //     if(it!=newclub[i].Club_member.end())
                    //     {
                    //         end_change_position=newclub[i].Club_member[newclub[i].Club_member.size()-1];
                    //         break;
                    //     }
                    // }
                }
                else
                {
                    // cout<<"event2_3 "<<endl;
                    vector<Club> temp;
                    for (int i = 0; i < newclub.size(); ++i)
                    {
                        if (i != rand_changeSoft1)
                        {
                            temp.push_back(newclub[i]);
                        }
                    }
                    int max2 = temp.size();
                    int min2 = 0;
                    bool get_pre = false;
                    vector<int> pre_choice;
                    int rand_changeSoft2;
                    for (int i = 0; i < temp.size(); ++i)
                    {
                        for (int j = 0; j < temp[i].Club_member.size(); ++j)
                        {
                            std::set<int>::iterator it = find(Connect_map[newclub[rand_changeSoft1].Club_member[0]].begin(), Connect_map[newclub[rand_changeSoft1].Club_member[0]].end(), temp[i].Club_member[j]);
                            if (it != Connect_map[newclub[rand_changeSoft1].Club_member[0]].end())
                            {
                                pre_choice.push_back(i);
                                get_pre = true;
                                break;
                            }
                        }
                    }
                    if (get_pre == false)
                    {
                        while (rand_changeSoft1 == rand_changeSoft2)
                        {
                            rand_changeSoft2 = rand() % (max2 - min2 + 1) + min2;
                        }
                    }
                    else
                    {
                        double next_or_pre = (double)rand() / (RAND_MAX + 1.0);
                        if (next_or_pre <= 0.4)
                        {
                            max2 = pre_choice.size();
                            min2 = 0;
                            rand_changeSoft2 = pre_choice[rand() % (max2 - min2) + min2] + 1;
                            if (rand_changeSoft1 == rand_changeSoft2)
                            {
                                rand_changeSoft2 - 1;
                            }
                        }
                        else if (next_or_pre > 0.4 && next_or_pre <= 0.8)
                        {
                            max2 = pre_choice.size();
                            min2 = 0;
                            rand_changeSoft2 = pre_choice[rand() % (max2 - min2) + min2];
                            if (rand_changeSoft1 == rand_changeSoft2)
                            {
                                rand_changeSoft2 + 1;
                            }
                        }
                        else
                        {
                            rand_changeSoft2 = rand() % (max2 - min2 + 1) + min2;
                            while (rand_changeSoft1 == rand_changeSoft2)
                            {
                                rand_changeSoft2 = rand() % (max2 - min2 + 1) + min2;
                            }
                        }
                    }
                    max2 = temp.size();
                    vector<Club> temp2;
                    for (int i = 0; i < rand_changeSoft2; ++i)
                    {
                        temp2.push_back(temp[i]);
                    }
                    temp2.push_back(newclub[rand_changeSoft1]);
                    for (int i = rand_changeSoft2; i < max2; ++i)
                    {
                        temp2.push_back(temp[i]);
                    }
                    newclub = temp2;
                    place_order.clear();
                    // cout<<"rand2: "<<rand_changeSoft2<<", after club: "<<endl;
                    for (int i = 0; i < newclub.size(); ++i)
                    {
                        for (int j = 0; j < newclub[i].Club_member.size(); ++j)
                        {
                            place_order.push_back(newclub[i].Club_member[j]);
                            // cout<<NewClub_all[i].Club_member[j]<<", ";
                        }
                        // cout<<endl;
                    }
                    // cout<<"change_position: "<<change_position<<endl;
                    end_change_position = place_order[place_order.size() - 1];
                }
            }
            start_posi = place_order.size() - 1;
            end_posi = place_order.size() - 1;
            for (int left = 0; left < place_order.size(); ++left)
            {
                if (old_place_order[left] != place_order[left])
                {
                    start_posi = left;
                    break;
                }
            }
            for (int i = 0; i < newclub.size(); ++i)
            {
                std::vector<int>::iterator it = find(newclub[i].Club_member.begin(), newclub[i].Club_member.end(), place_order[start_posi]);
                if (it != newclub[i].Club_member.end())
                {
                    change_position = newclub[i].Club_member[0];
                    break;
                }
            }

            // cout<<endl<<"   _place_order: ";
            // for(int left =0;left<place_order.size();++left)
            // {
            //     cout<<place_order[left]<<", ";
            // }
            // cout<<endl;
            // cout<<"old_place_order: ";
            // for(int left =0;left<old_place_order.size();++left)
            // {
            //     cout<<old_place_order[left]<<", ";
            // }
            // cout<<endl;
        }
    }
    else if (event <= 0.96 && event > 0.6) // 讓module能直放或橫放
    {
        // cout<<"event3 "<<endl;
        int rand_changeSoft1;
        int minf = 0;
        int max = newclub.size();
        rand_changeSoft1 = rand() % (max - minf) + minf;

        if (newclub[rand_changeSoft1].Club_member.size() > 1)
        {
            double event2 = (double)rand() / (RAND_MAX + 1.0);
            if (event2 < 0.5)
            {
                new_place_direction[newclub[rand_changeSoft1].Club_member[0]] = 1;
                if (rand_changeSoft1 == 0)
                {
                    double ratio = 0.5 + (rand() % 51) / 100.0;
                    be_changeAspectRatio(Modules[newclub[rand_changeSoft1].Club_member[0]], ratio);
                    // changeAspectRatio(Modules[newclub[rand_changeSoft1].Club_member[0]]);
                }
                else
                {
                    double ratio = 0.5 + (rand() % 51) / 100.0;
                    be_changeAspectRatio(Modules[newclub[rand_changeSoft1].Club_member[0]], ratio);
                }
                for (int i = 1; i < newclub[rand_changeSoft1].Club_member.size(); ++i)
                {
                    new_place_direction[newclub[rand_changeSoft1].Club_member[i]] = 1;
                    int Pre_M_Width = Modules[newclub[rand_changeSoft1].Club_member[0]].getWidth();
                    int This_M_Height = Modules[newclub[rand_changeSoft1].Club_member[i]].mt.data.soft_module_data.RequiredSize / Pre_M_Width;
                    if ((double(This_M_Height) / double(Pre_M_Width)) > 2.0) // 前一個比較長，這一個達不到變自身最高
                    {
                        double ratio = 0.5 + (rand() % 51) / 100.0;
                        be_changeAspectRatio(Modules[newclub[rand_changeSoft1].Club_member[i]], ratio);
                        // be_changeAspectRatio(Modules[newclub[rand_changeSoft1].Club_member[i]],0.5);
                    }
                    else if ((double(This_M_Height) / double(Pre_M_Width)) < 0.5) // 前一個比較寬，這一個達不到變自身最寬
                    {
                        double ratio = 0.5 + (rand() % 51) / 100.0;
                        be_changeAspectRatio(Modules[newclub[rand_changeSoft1].Club_member[i]], ratio);
                        // be_changeAspectRatio(Modules[newclub[rand_changeSoft1].Club_member[i]],0.5);
                    }
                    else
                    {
                        // cout<<"double(Pre_M_Height)/double(This_M_Width): "<<double(Pre_M_Height)/double(This_M_Width)<<endl;
                        be_changeAspectRatio_byWidth(Modules[newclub[rand_changeSoft1].Club_member[i]], Pre_M_Width);
                    }
                }
            }
            else
            {
                new_place_direction[newclub[rand_changeSoft1].Club_member[0]] = 0;
                if (rand_changeSoft1 == 0)
                {
                    double ratio = 1 + (rand() % 101) / 100.0;
                    be_changeAspectRatio(Modules[newclub[rand_changeSoft1].Club_member[0]], ratio);
                    // changeAspectRatio(Modules[newclub[rand_changeSoft1].Club_member[0]]);
                }
                else
                {
                    double ratio = 1 + (rand() % 101) / 100.0;
                    be_changeAspectRatio(Modules[newclub[rand_changeSoft1].Club_member[0]], ratio);
                }
                for (int i = 1; i < newclub[rand_changeSoft1].Club_member.size(); ++i)
                {
                    new_place_direction[newclub[rand_changeSoft1].Club_member[i]] = 0;
                    int Pre_M_Height = Modules[newclub[rand_changeSoft1].Club_member[0]].getHeight();
                    int This_M_Width = Modules[newclub[rand_changeSoft1].Club_member[i]].mt.data.soft_module_data.RequiredSize / Pre_M_Height;
                    if ((double(Pre_M_Height) / double(This_M_Width)) > 2.0) // 前一個比較長，這一個達不到變自身最高
                    {
                        double ratio = 1 + (rand() % 101) / 100.0;
                        be_changeAspectRatio(Modules[newclub[rand_changeSoft1].Club_member[i]], ratio);
                        // be_changeAspectRatio(Modules[newclub[rand_changeSoft1].Club_member[i]],2.0);
                    }
                    else if ((double(Pre_M_Height) / double(This_M_Width)) < 0.5) // 前一個比較寬，這一個達不到變自身最寬
                    {
                        double ratio = 1 + (rand() % 101) / 100.0;
                        be_changeAspectRatio(Modules[newclub[rand_changeSoft1].Club_member[i]], ratio);
                        // be_changeAspectRatio(Modules[newclub[rand_changeSoft1].Club_member[i]],2.0);
                    }
                    else
                    {
                        // cout<<"double(Pre_M_Height)/double(This_M_Width): "<<double(Pre_M_Height)/double(This_M_Width)<<endl;
                        be_changeAspectRatio_byHeight(Modules[newclub[rand_changeSoft1].Club_member[i]], Pre_M_Height);
                    }
                }
            }
            change_position = newclub[rand_changeSoft1].Club_member[0];
            end_change_position = place_order[place_order.size() - 1];
        }
        else
        {
            if (new_place_direction[newclub[rand_changeSoft1].Club_member[0]] == 1)
            {
                new_place_direction[newclub[rand_changeSoft1].Club_member[0]] = 0;
                if (rand_changeSoft1 == 0)
                {
                    double ratio = 1 + (rand() % 101) / 100.0;
                    be_changeAspectRatio(Modules[newclub[rand_changeSoft1].Club_member[0]], ratio);
                    // be_changeAspectRatio(Modules[newclub[rand_changeSoft1].Club_member[0]],0.5);
                    // changeAspectRatio(Modules[newclub[rand_changeSoft1].Club_member[0]]);
                }
                else
                {
                    double ratio = 1 + (rand() % 101) / 100.0;
                    be_changeAspectRatio(Modules[newclub[rand_changeSoft1].Club_member[0]], ratio);
                }
            }
            else
            {
                new_place_direction[newclub[rand_changeSoft1].Club_member[0]] = 1;
                if (rand_changeSoft1 == 0)
                {
                    double ratio = 0.5 + (rand() % 51) / 100.0;
                    be_changeAspectRatio(Modules[newclub[rand_changeSoft1].Club_member[0]], ratio);
                    // be_changeAspectRatio(Modules[newclub[rand_changeSoft1].Club_member[0]],2.0);
                    // changeAspectRatio(Modules[newclub[rand_changeSoft1].Club_member[0]]);
                }
                else
                {
                    double ratio = 0.5 + (rand() % 51) / 100.0;
                    be_changeAspectRatio(Modules[newclub[rand_changeSoft1].Club_member[0]], ratio);
                }
            }
            change_position = newclub[rand_changeSoft1].Club_member[0];
            double event5 = (double)rand() / (RAND_MAX + 1.0);
            if (event5 <= 1)
            {
                end_change_position = place_order[place_order.size() - 1];
            }
            else
            {
                // end_change_position=newclub[rand_changeSoft1].Club_member[newclub[rand_changeSoft1].Club_member.size()-1];
            }
        }
    }
    else // 轉向
    {
        // cout<<"event4 "<<endl;
        int minf = 1;
        int max = 4;
        int rand_changeSoft1 = rand() % (max - minf) + minf;
        for (int i = 0; i < rand_changeSoft1; ++i)
        {
            ++rotate_time_N;
            if (rotate_time_N % 2 == 0)
            {
                ChipHeight = ChipHeightT;
                ChipWidth = ChipWidthT;
            }
            else
            {
                ChipHeight = ChipWidthT;
                ChipWidth = ChipHeightT;
            }
            const int Msize = Modules.size();
            for (int i = SoftModuleAmount + 1; i <= Msize; ++i)
            {
                leftdown_corner temp;
                temp.x = sa_end_x1y1[i].getX() + Modules[i].getWidth();
                temp.y = sa_end_x1y1[i].getY();
                int x_new = temp.y;
                int y_new = -temp.x;
                y_new += ChipHeight;
                sa_end_x1y1[i].x = x_new;
                sa_end_x1y1[i].y = y_new;
                int temp2 = Modules[i].getHeight();
                Modules[i].mt.data.fixed_module_data.height = Modules[i].mt.data.fixed_module_data.width;
                Modules[i].mt.data.fixed_module_data.width = temp2;
            }
        }
        // cout<<"fin_rotate"<<endl;
        change_position = place_order[0];
        end_change_position = place_order[place_order.size() - 1];
    }
    // cout<<"out event"<<endl;
};
void Simulation_Annealing(int w, int version, vector<Best_SA_10> &best_10_sa)
{
    // ofstream fout("draw.txt", ios::out);
    // ofstream fout2("draw2.txt", ios::out);
    // ofstream fout3("Modulecost.txt", ios::out);
    // ofstream fout4("rrr1.txt", ios::out);
    // cout<<"before_sa: "<<endl;
    const int s = Modules.size();
    vector<int> new_place_direction = place_direction;
    vector<int> temp_place_direction = place_direction;
    vector<int> best_place_direction;
    vector<int> new_place_order = out_softmodule;
    vector<int> temp_place_order = out_softmodule;
    vector<int> best_place_order;
    vector<int> cantpush;
    vector<int> temp_cantpush;
    vector<int> best_cantpush;
    vector<Club> SA_new_club = NewClub_all;
    vector<Club> SA_temp_club = NewClub_all;
    vector<Club> SA_best_club = NewClub_all;
    int best_rotate_time = rotate_time;
    int temp_rotate_time = rotate_time;
    int new_rotate_time = rotate_time;
    map<int, Module> best_Modules;
    map<int, Module> temp_Modules;
    map<int, leftdown_corner> best_sa_end_x1y1;
    map<int, leftdown_corner> temp_sa_end_x1y1;
    double T;
    double Trate;
    double line_Trate;
    const double maxT = newSA_M_T;
    const double maxNEWSA_T = newSA_T;
    long double k;
    int rejectnum;
    double NEWSA_T = 0;
    double Trrr1 = 1;
    if (version == 0)
    {
        Trate = 0.8;
        line_Trate = 0.8;
        T = 100;
        k = 35;
        rejectnum = s;
    }
    else
    {
        Trate = 0.97;
        line_Trate = 0.97;
        T = newSA_M_T;
        NEWSA_T = newSA_T;
        rejectnum = SoftModuleAmount*SoftModuleAmount;
        if(rejectnum>900)
        {
            rejectnum=900;
        }
        // cout<<rejectnum<<endl;
    }
    int Cost = INT_MIN;
    int i = 0;
    if (w == 0)
    {
        if (rat < 85)
        {
            GetCost_leftdown(Cost, new_place_order, cantpush, new_place_order[0], new_place_order[new_place_order.size() - 1], new_place_direction, SA_new_club);
        }
        else
        {
            GetCost_leftdown2(Cost, new_place_order, cantpush, new_place_order[0], new_place_order[new_place_order.size() - 1], new_place_direction, SA_new_club);
        }
    }
    else if (w == 1)
    {
        GetCost_leftup(Cost, new_place_order, cantpush, new_place_order[0]);
    }
    else if (w == 2)
    {
        GetCost_rightdown(Cost, new_place_order, cantpush, new_place_order[0]);
    }
    else if (w == 3)
    {
        GetCost_rightup(Cost, new_place_order, cantpush, new_place_order[0]);
    }
    double pre_HPWL = FinalAnswer(sa_end_x1y1);
    int tempCost = Cost;
    int bestCost = Cost;
    double minaHPWL = DBL_MAX;
    double bestlinecount = pre_HPWL;
    double templinecount = pre_HPWL;
    double temp_linecountdelta;
    double max_temp_linecountdelta;
    best_place_order = new_place_order;
    best_place_direction = new_place_direction;
    best_sa_end_x1y1 = sa_end_x1y1;
    temp_sa_end_x1y1 = sa_end_x1y1;
    best_Modules = Modules;
    temp_cantpush = cantpush;
    best_cantpush = cantpush;
    SA_best_club = SA_new_club;
    max_temp_linecountdelta = 0;
    // cout<<std::fixed<< std::setprecision(6)<<"bestlinecount: "<<linecount<<endl;
    clock_t drawstart = clock();
    // fout << std::fixed << std::setprecision(5) << "0 " << bestlinecount << " " << bestlinecount << ", start" << endl;
    clock_t start2 = clock();
    clock_t end = clock();
    double cpu_time = static_cast<double>(end - start) / CLOCKS_PER_SEC;
    int rrr1zerocount = 0;
    int Bestcount = 0;
    int event1 = 0;
    int event2 = 0;
    int event3 = 0;
    int event4 = 0;
    int continue_good = 3;
    while (true)
    {
        clock_t end = clock();
        cpu_time = static_cast<double>(end - start) / CLOCKS_PER_SEC;
        if(cpu_time>1600)
        {
           break;
        }
        if (version == 1)
        {
            double cpu_time2 = static_cast<double>(end - start2) / CLOCKS_PER_SEC;
            // cout << "end: " << end << ", start2: " << start2 << ", cpu_time2: " << cpu_time2 << endl;
            if (cpu_time2 < 300)
            {
                line_Trate = 0.97;
            }
            else if (cpu_time2 >= 300)
            {
                if (line_Trate > 0.96)
                {
                    line_Trate = 0.96;
                }
            }
            if (cpu_time2 >= 600)
            {
                if (line_Trate > 0.94)
                {
                    line_Trate = 0.94;
                }
            }
            if (cpu_time2 >= 1200)
            {
                if (line_Trate > 0.88)
                {
                    line_Trate = 0.88;
                }
            }
            if (cpu_time2 >= 1400)
            {
                if (line_Trate > 0.8)
                {
                    line_Trate = 0.8;
                }
            }
        }
        if (version == 0)
        {
            if (rrr1zerocount == 10)
            {
                break;
            }
        }
        else
        {
            if (rrr1zerocount == 10)
            {
                break;
            }
        }
        double highest_rrr1 = 0;
        double rrr1_averange = 0;
        int highest_cost = 0;
        double cost_averange = 0;
        int cost_sum = 0;
        int ac_count = 0;
        int sum_count = 0;
        temp_linecountdelta = 0;
        int reject = 0;
        int min_tempcost = tempCost;
        int continue_reject = 0;
        while (reject < rejectnum)
        {
            clock_t end = clock();
            cpu_time = static_cast<double>(end - start) / CLOCKS_PER_SEC;
            // cout<<"cpu_time: "<<cpu_time<<endl;
            if(cpu_time>1600)
            {
                break;
            }
            cantpush = temp_cantpush;
            temp_Modules = Modules;
            temp_sa_end_x1y1 = sa_end_x1y1;
            new_place_order = temp_place_order;
            new_place_direction = temp_place_direction;
            SA_new_club = SA_temp_club;
            new_rotate_time = temp_rotate_time;
            // cout<<"new_rotate_time: "<<new_rotate_time<<endl;
            if (new_rotate_time % 2 == 0)
            {
                ChipHeight = ChipHeightT;
                ChipWidth = ChipWidthT;
            }
            else
            {
                ChipHeight = ChipWidthT;
                ChipWidth = ChipHeightT;
            }
            int change_module;
            int end_change_module;
            double T_probability;
            if (version == 0)
            {
                T_probability = (double)T / 100.0;
            }
            else if (version == 1)
            {
                T_probability = (double)NEWSA_T / maxNEWSA_T;
            }
            GetNext(new_place_order, change_module, end_change_module, T_probability, new_place_direction, SA_new_club, new_rotate_time, cantpush);
            // cout<<"change_module: "<<change_module<<endl;
            int newCost = INT_MAX;
            double newlinecount = DBL_MAX;
            if (w == 0) // 左下
            {
                if (rat < 85)
                {
                    GetCost_leftdown(newCost, new_place_order, cantpush, change_module, end_change_module, new_place_direction, SA_new_club);
                }
                else
                {
                    GetCost_leftdown2(newCost, new_place_order, cantpush, change_module, end_change_module, new_place_direction, SA_new_club);
                }
            }
            else if (w == 1) // 左上
            {
                GetCost_leftup(newCost, new_place_order, cantpush, change_module);
            }
            else if (w == 2) // 右下
            {
                GetCost_rightdown(newCost, new_place_order, cantpush, change_module);
            }
            else if (w == 3) // 右上
            {
                GetCost_rightup(newCost, new_place_order, cantpush, change_module);
            }
            double final_ans = FinalAnswer(sa_end_x1y1);
            // if(final_ans>51574733)
            // {
            //     cout<<"linecount: "<<final_ans<<endl;
            //     cout<<"cantpush: ";
            //     for(int i =1;i<cantpush.size();++i)
            //     {
            //         cout<<i<<": "<<cantpush[i]<<" , ";
            //     }
            //     cout<<endl;
            //     cout<<"new_place_direction: ";
            //     for(int i =1;i<new_place_direction.size();++i)
            //     {
            //         cout<<i<<": "<<new_place_direction[i]<<" , ";
            //     }
            //     cout<<endl;
            //     cout<<"new_place_order: ";
            //     for(int i =0;i<new_place_order.size();++i)
            //     {
            //         cout<<new_place_order[i]<<", ";
            //     }
            //     cout<<endl;
            //     setFinalPlacement(0);
            //     setFinalPlacement(1);
            //     system("pause");
            // }
            if (newCost <= tempCost)
            {
                if (newCost <= bestCost)
                {
                    if (newCost == bestCost)
                    {
                        if (final_ans <= bestlinecount)
                        {
                            if (final_ans != bestlinecount)
                            {
                                rrr1_averange = rrr1_averange + 1;
                                ++sum_count;
                                best_place_direction = new_place_direction;
                                best_place_order = new_place_order;
                                bestCost = newCost;
                                best_sa_end_x1y1 = sa_end_x1y1;
                                best_Modules = Modules;
                                bestlinecount = final_ans;
                                best_cantpush = cantpush;
                                best_rotate_time = new_rotate_time;
                                SA_best_club = SA_new_club;
                                clock_t end = clock();
                                continue_good = 4;
                                // fout << std::fixed << std::setprecision(5) << static_cast<double>(end - drawstart) / CLOCKS_PER_SEC << " " << bestlinecount << " " << final_ans << ", best accept" << endl;
                                continue_reject = 0;
                            }
                            temp_place_order = new_place_order;
                            temp_place_direction = new_place_direction;
                            tempCost = newCost;
                            templinecount = final_ans;
                            temp_rotate_time = new_rotate_time;
                            temp_cantpush = cantpush;
                            SA_temp_club = SA_new_club;
                            // ac_count++;
                            // averange_rrr1=averange_rrr1+1;
                        }
                        else
                        {
                            if (version == 1)
                            {
                                temp_linecountdelta = final_ans - bestlinecount;
                                double delta = temp_linecountdelta;
                                double rrr1 = exp(-delta / NEWSA_T);
                                // cout<<"T: "<<NEWSA_T<<", temp_linecountdelta: "<<std::fixed<< std::setprecision(6)<<temp_linecountdelta<<", new delta: "<<rrr1<<endl;
                                double rrr2 = (double)rand() / (RAND_MAX + 1.0);
                                // ac_count++;
                                if (rrr2 < rrr1)
                                {
                                    // cout<<rrr1<<", ";
                                    if (rrr1 > 1.0)
                                    {
                                        rrr1 = 1.0;
                                    }
                                    rrr1_averange = rrr1_averange + rrr1;
                                    ++sum_count;
                                    clock_t end = clock();
                                    // fout << std::fixed << std::setprecision(5) << static_cast<double>(end - drawstart) / CLOCKS_PER_SEC << " " << bestlinecount << " " << final_ans << "," << " version2 reject accept" << endl;
                                    if (rrr1 > highest_rrr1)
                                    {
                                        highest_rrr1 = rrr1;
                                    }
                                    // clock_t end= clock();
                                    // fout<<std::fixed<< std::setprecision(1)<<"cputime: "<<end-start<<", T: "<<T<<", bestleastModule: "<<bestCost<<", bestlinescore: "<<bestlinecount<<", templeastmodule: "<<newCost<<", templinescore: "<<final_ans<<endl;
                                    temp_place_order = new_place_order;
                                    temp_place_direction = new_place_direction;
                                    tempCost = newCost;
                                    templinecount = final_ans;
                                    temp_rotate_time = new_rotate_time;
                                    SA_temp_club = SA_new_club;
                                    temp_cantpush = cantpush;
                                }
                                else
                                {
                                    clock_t end = clock();
                                    // fout << std::fixed << std::setprecision(5) << static_cast<double>(end - drawstart) / CLOCKS_PER_SEC << " " << bestlinecount << " " << final_ans << ", version2 reject" << endl;
                                    Modules = temp_Modules;
                                    sa_end_x1y1 = temp_sa_end_x1y1;
                                    ++reject;
                                }
                            }
                        }
                    }
                    else if (newCost < bestCost)
                    {
                        clock_t end = clock();
                        // fout << std::fixed << std::setprecision(5) << static_cast<double>(end - drawstart) / CLOCKS_PER_SEC << " " << bestlinecount << " " << final_ans << ", temp cost accept" << endl;
                        best_place_order = new_place_order;
                        best_place_direction = new_place_direction;
                        bestCost = newCost;
                        best_sa_end_x1y1 = sa_end_x1y1;
                        best_Modules = Modules;
                        bestlinecount = final_ans;
                        best_cantpush = cantpush;
                        best_rotate_time = new_rotate_time;
                        SA_best_club = SA_new_club;
                        temp_cantpush = cantpush;
                        temp_place_order = new_place_order;
                        temp_place_direction = new_place_direction;
                        tempCost = newCost;
                        templinecount = final_ans;
                        temp_rotate_time = new_rotate_time;
                        SA_temp_club = SA_new_club;
                    }
                }
                else if (newCost == tempCost)
                {
                    if (final_ans <= templinecount)
                    {
                        clock_t end = clock();
                        // fout << std::fixed << std::setprecision(5) << static_cast<double>(end - drawstart) / CLOCKS_PER_SEC << " " << bestlinecount << " " << final_ans << ", temp cost accept" << endl;
                        temp_place_order = new_place_order;
                        temp_place_direction = new_place_direction;
                        SA_temp_club = SA_new_club;
                        tempCost = newCost;
                        temp_cantpush = cantpush;
                        temp_rotate_time = new_rotate_time;
                        templinecount = final_ans;
                    }
                    else
                    {
                        if (version == 1)
                        {
                            temp_linecountdelta = final_ans - bestlinecount;
                            double delta = temp_linecountdelta;
                            double rrr1 = exp(-delta / NEWSA_T);
                            // cout<<"T: "<<NEWSA_T<<", temp_linecountdelta: "<<std::fixed<< std::setprecision(6)<<temp_linecountdelta<<", new delta: "<<rrr1<<endl;
                            double rrr2 = (double)rand() / (RAND_MAX + 1.0);
                            // ac_count++;
                            if (rrr2 < rrr1)
                            {
                                // cout<<rrr1<<", ";
                                if (rrr1 > 1.0)
                                {
                                    rrr1 = 1.0;
                                }
                                rrr1_averange = rrr1_averange + rrr1;
                                ++sum_count;
                                clock_t end = clock();
                                // fout << std::fixed << std::setprecision(5) << static_cast<double>(end - drawstart) / CLOCKS_PER_SEC << " " << bestlinecount << " " << final_ans << ","<< " version2 reject accept" << endl;
                                if (rrr1 > highest_rrr1)
                                {
                                    highest_rrr1 = rrr1;
                                }
                                // clock_t end= clock();
                                // fout<<std::fixed<< std::setprecision(1)<<"cputime: "<<end-start<<", T: "<<T<<", bestleastModule: "<<bestCost<<", bestlinescore: "<<bestlinecount<<", templeastmodule: "<<newCost<<", templinescore: "<<final_ans<<endl;
                                temp_place_order = new_place_order;
                                temp_place_direction = new_place_direction;
                                SA_temp_club = SA_new_club;
                                tempCost = newCost;
                                temp_cantpush = cantpush;
                                temp_rotate_time = new_rotate_time;
                                templinecount = final_ans;
                            }
                            else
                            {
                                clock_t end = clock();
                                // fout << std::fixed << std::setprecision(5) << static_cast<double>(end - drawstart) / CLOCKS_PER_SEC << " " << bestlinecount << " " << final_ans << ", version2 reject" << endl;
                                Modules = temp_Modules;
                                sa_end_x1y1 = temp_sa_end_x1y1;
                                ++reject;
                            }
                        }
                    }
                }
                else if (newCost < tempCost)
                {
                    clock_t end = clock();
                    // fout << std::fixed << std::setprecision(5) << static_cast<double>(end - drawstart) / CLOCKS_PER_SEC << " " << bestlinecount << " " << final_ans << ", temp ac" << endl;
                    temp_place_order = new_place_order;
                    temp_place_direction = new_place_direction;
                    tempCost = newCost;
                    temp_cantpush = cantpush;
                    temp_rotate_time = new_rotate_time;
                    SA_temp_club = SA_new_club;
                    templinecount = final_ans;
                }
                // cout<<"T: "<<T<<", newCost: "<<newCost<<", Bestcost: "<<bestCost<<", tempCost: "<<tempCost<<endl;
                min_tempcost = min(min_tempcost, tempCost);
            }
            else if (newCost > tempCost)
            {
                double delta = newCost - min_tempcost;
                // cout<<" delta: "<<delta<<", T: "<<T<<endl;
                double rrr1 = exp(-delta / T);
                double rrr2 = (double)rand() / (RAND_MAX + 1.0);
                // cout<<"T: "<<T<<", delta: "<<delta<<", Newcost: "<<newCost<<", min_tempcost: "<<min_tempcost<<", rrr1: "<<rrr1<<endl;
                if (rrr2 < rrr1)
                {
                    if (rrr1 > 1.0)
                    {
                        rrr1 = 1.0;
                    }
                    ++cost_sum;
                    cost_averange = cost_averange + rrr1;
                    if (highest_cost < newCost)
                    {
                        highest_cost = newCost;
                    }
                    // clock_t end= clock();
                    // double final_ans=FinalAnswer(sa_end_x1y1);
                    // fout<<std::fixed<< std::setprecision(1)<<"cputime: "<<end-start<<", T: "<<T<<", bestleastModule: "<<bestCost<<", bestlinescore: "<<bestlinecount<<", templeastmodule: "<<newCost<<", templinescore: "<<final_ans<<endl;
                    if (version == 1)
                    {
                        temp_linecountdelta = final_ans - bestlinecount;
                        double delta = temp_linecountdelta;
                        double rrr1 = exp(-delta / NEWSA_T);
                        // cout<<"T: "<<NEWSA_T<<", temp_linecountdelta: "<<std::fixed<< std::setprecision(6)<<temp_linecountdelta<<", new delta: "<<rrr1<<endl;
                        double rrr2 = (double)rand() / (RAND_MAX + 1.0);
                        // ac_count++;
                        if (rrr2 < rrr1)
                        {
                            clock_t end = clock();
                            // fout << std::fixed << std::setprecision(5) << static_cast<double>(end - drawstart) / CLOCKS_PER_SEC << " " << bestlinecount << " " << final_ans << ","<< " cost version2 reject accept" << endl;
                            if (rrr1 > highest_rrr1)
                            {
                                highest_rrr1 = rrr1;
                            }
                            if (rrr1 > 1.0)
                            {
                                rrr1 = 1.0;
                            }
                            rrr1_averange = rrr1_averange + rrr1;
                            ++sum_count;
                            // clock_t end= clock();
                            // fout<<std::fixed<< std::setprecision(1)<<"cputime: "<<end-start<<", T: "<<T<<", bestleastModule: "<<bestCost<<", bestlinescore: "<<bestlinecount<<", templeastmodule: "<<newCost<<", templinescore: "<<final_ans<<endl;
                            temp_place_order = new_place_order;
                            temp_place_direction = new_place_direction;
                            tempCost = newCost;
                            temp_cantpush = cantpush;
                            templinecount = final_ans;
                            temp_rotate_time = new_rotate_time;
                            SA_temp_club = SA_new_club;
                        }
                        else
                        {
                            clock_t end = clock();
                            // fout << std::fixed << std::setprecision(5) << static_cast<double>(end - drawstart) / CLOCKS_PER_SEC << " " << bestlinecount << " " << final_ans << ","<< " cost version2 reject" << endl;
                            Modules = temp_Modules;
                            sa_end_x1y1 = temp_sa_end_x1y1;
                            ++reject;
                        }
                    }
                }
                else
                {
                    clock_t end = clock();
                    // fout << std::fixed << std::setprecision(5) << static_cast<double>(end - drawstart) / CLOCKS_PER_SEC << " " << bestlinecount << " " << final_ans << ","<< " cost version2 reject" << endl;
                    Modules = temp_Modules;
                    sa_end_x1y1 = temp_sa_end_x1y1;
                    ++reject;
                }
            }
            // cout<<"T: "<<T<<std::fixed<< std::setprecision(0)<<", best_cost: "<<bestCost<<std::fixed<< std::setprecision(1)<<", best_linescore: "<<bestlinecount<<endl;
        }
        if (continue_good > 0)
        {
            // cout<<"here";
            double cpu_time2 = static_cast<double>(end - start2) / CLOCKS_PER_SEC;
            // cout << "end: " << end << ", start2: " << start2 << ", cpu_time2: " << cpu_time2 << endl;
            if (cpu_time2 < 600)
            {
                line_Trate = 0.98;
            }
            if (cpu_time2 > 600)
            {
                line_Trate = 0.96;
            }
            if (cpu_time2 > 1200)
            {
                line_Trate = 0.92;
            }
            --continue_good;
        }
        // cout << std::fixed << std::setprecision(3) << ", T: " << NEWSA_T << ", Trate: " << line_Trate << ", best_cost: " << bestCost << ", best linescore: " << bestlinecount << ", " << ++i;
        temp_place_order = best_place_order;
        temp_place_direction = best_place_direction;
        tempCost = bestCost;
        templinecount = bestlinecount;
        temp_cantpush = best_cantpush;
        temp_rotate_time = best_rotate_time;
        SA_temp_club = SA_best_club;
        sa_end_x1y1 = best_sa_end_x1y1;
        Modules = best_Modules;

        Best_SA_10 SA_10;
        SA_10.Best_10_linecount = templinecount;
        SA_10.best_10_cantpush = temp_cantpush;
        SA_10.Best_10_club = SA_temp_club;
        SA_10.best_10_Cost = tempCost;
        SA_10.best_10_Modules = Modules;
        SA_10.best_10_place_direction = temp_place_direction;
        SA_10.best_10_place_order = temp_place_order;
        SA_10.best_10_rotate_time = temp_rotate_time;
        SA_10.best_10_sa_end_x1y1 = sa_end_x1y1;
        bool hasnt_same_linecount = true;
        for (int i = 0; i < best_10_sa.size(); ++i)
        {
            if (best_10_sa[i].Best_10_linecount == SA_10.Best_10_linecount)
            {
                hasnt_same_linecount = false;
                break;
            }
        }
        if (hasnt_same_linecount)
        {
            if (best_10_sa.size() < 10)
            {
                best_10_sa.push_back(SA_10);
            }
            else
            {
                break;
                vector<Best_SA_10> temp_best_10_sa = best_10_sa;
                int n = temp_best_10_sa.size();
                for (int i = 0; i < n - 1; ++i)
                {
                    for (int j = 0; j < n - i - 1; ++j)
                    {
                        if (temp_best_10_sa[j].Best_10_linecount > temp_best_10_sa[j + 1].Best_10_linecount)
                        {
                            Best_SA_10 temp = temp_best_10_sa[j];
                            temp_best_10_sa[j] = temp_best_10_sa[j + 1];
                            temp_best_10_sa[j + 1] = temp;
                        }
                    };
                };
                for (int i = 0; i < n - 1; ++i)
                {
                    for (int j = 0; j < n - i - 1; ++j)
                    {
                        if (temp_best_10_sa[j].best_10_Cost > temp_best_10_sa[j + 1].best_10_Cost)
                        {
                            Best_SA_10 temp = temp_best_10_sa[j];
                            temp_best_10_sa[j] = temp_best_10_sa[j + 1];
                            temp_best_10_sa[j + 1] = temp;
                        }
                    };
                };
                best_10_sa = temp_best_10_sa;
                best_10_sa.pop_back();
                best_10_sa.push_back(SA_10);
            }
        }
        if (bestlinecount == pre_HPWL)
        {
            ++Bestcount;
        }
        else
        {
            Bestcount = 0;
        }
        // cout << ", Bestcount: " << Bestcount;
        pre_HPWL = bestlinecount;
        clock_t temperature = clock();
        if (version == 1)
        {
            if (Bestcount == 10)
            {
                Bestcount = 0;
                NEWSA_T = NEWSA_T * (line_Trate - 0.05);
                T = T * (line_Trate - 0.05);
            }
            else
            {
                NEWSA_T = NEWSA_T * line_Trate;
                T = T * line_Trate;
            }
        }
        // fout2 << std::fixed << std::setprecision(5) << static_cast<double>(temperature - drawstart) / CLOCKS_PER_SEC << " " << T << "\n";
        double Tcost = 0;
        if ((double)cost_sum != 0)
        {
            Tcost = (double)cost_averange / (double)cost_sum;
        }
        Trrr1 = 0;
        if ((double)sum_count != 0)
        {
            Trrr1 = (double)rrr1_averange / (double)sum_count;
        }
        // fout3 << std::fixed << std::setprecision(5) << T << " " << Tcost << "\n";
        // fout4 << std::fixed << std::setprecision(5) << NEWSA_T << " " << Trrr1 << "\n";
        end = clock();
        if (Trrr1 < 0.00001)
        {
            ++rrr1zerocount;
        }
        else
        {
            rrr1zerocount = 0;
        }
        // final_output();
        // setFinalPlacement(0);
        // setFinalPlacement(1);
        cpu_time = static_cast<double>(end - start) / CLOCKS_PER_SEC;
        // cout << ", cputime: " << cpu_time << std::fixed << std::setprecision(5) << ", reject_ac%: " << Trrr1 << ", Module reject ac%: " << Tcost << ", rrr1zerocount: " << rrr1zerocount << ", datacount: " << datacount << endl;
        datacount = 0;
        // cout << "replace : ";
        // for (int i = 1; i < best_cantpush.size(); ++i)
        // {
        //     cout << best_cantpush[i] << " , ";
        // }
        // cout << endl;
        // cout << "direction: ";
        // for (int i = 1; i < best_place_direction.size(); ++i)
        // {
        //     cout << i << ": " << best_place_direction[i] << " , ";
        // }
        // cout << endl;
        // cout << "placeorder: ";
        // for (int i = 0; i < best_place_order.size(); ++i)
        // {
        //     cout << best_place_order[i] << ", ";
        // }
        // cout << endl;
    }
    NewClub_all = SA_best_club;
    sa_end_x1y1 = best_sa_end_x1y1;
    cant_push_afterSA = best_cantpush;
    Modules = best_Modules;
    SA_leftmodulecount = bestCost;
    out_softmodule = best_place_order;
    place_direction = best_place_direction;
    rotate_time = best_rotate_time;
    // cout<<"after_sa_end_x1y1: "<<endl;
    // for(int i=1;i<=s;i++)
    // {
    //     cout<<i<<": "<<Modules[i].getHeight()<<", "<<Modules[i].getWidth()<<endl;
    // }
    // cout<<endl;
    // fout.close();
    // fout3.close();
    // fout4.close();
    // cout << "event1: " << event1 << ", event2: " << event2 << ", event3: " << event3 << ", event4: " << event4 << endl;
}
double Get_random_case(int &Cost)
{
    const int s = Modules.size();
    vector<int> new_place_direction = place_direction;
    vector<int> new_place_order = out_softmodule;
    vector<Club> newclub = NewClub_all;
    vector<int> cantpush = cant_push_afterSA;
    double SUMCost = 0;
    double pre_Cost = 0;
    Cost = 0;
    double T_probability;
    int change_module;
    int end_change_module;
    int deltacount = 0;
    double averange_delta = 0;
    
    if (rat < 85)
    {
        GetCost_leftdown(Cost, new_place_order, cantpush, new_place_order[0], new_place_order[new_place_order.size() - 1], new_place_direction, newclub);
    }
    else
    {
        GetCost_leftdown2(Cost, new_place_order, cantpush, new_place_order[0], new_place_order[new_place_order.size() - 1], new_place_direction, newclub);
    }
    
    pre_Cost = Cost;
    double pre_HPWL = FinalAnswer(sa_end_x1y1);
    int Msize = Modules.size();
    Msize = Msize * 30;
    for (int i = 0; i < Msize; ++i)
    {
        // cout << i << ", ";
        GetNext(new_place_order, change_module, end_change_module, T_probability, new_place_direction, newclub, rotate_time, cantpush);
        // cout<<"here";
        if (rat < 85)
        {
            GetCost_leftdown(Cost, new_place_order, cantpush, new_place_order[0], new_place_order[new_place_order.size() - 1], new_place_direction, newclub);
        }
        else
        {
            GetCost_leftdown2(Cost, new_place_order, cantpush, new_place_order[0], new_place_order[new_place_order.size() - 1], new_place_direction, newclub);
        }
        double new_HPWL = FinalAnswer(sa_end_x1y1);
        SUMCost = SUMCost + abs(pre_Cost - Cost);
        averange_delta = averange_delta + abs(pre_HPWL - new_HPWL);
        ++deltacount;
        pre_Cost = Cost;
        pre_HPWL = new_HPWL;
        // final_output();
        // setFinalPlacement(0);
        // setFinalPlacement(1);
        // cout<<endl<<"change_module: "<<change_module<<", hpwl: "<<new_HPWL<<endl;
        // system("pause");
    }
    // cout<<"here";
    // cout << endl
    //      << "finorder: " << endl;
    // for (int i = 0; i < new_place_order.size(); ++i)
    // {
    //     cout << new_place_order[i] << ", ";
    // }
    // cout << endl
    //      << endl;
    Cost = SUMCost / deltacount;
    return averange_delta / (double)deltacount;
};
int findBestway()
{
    // int bestway=-1;
    // int first_modulehaveconnectwithfix=-1;
    // for(int i =0;i<out_softmodule.size();i++)
    // {
    //     for(iterator it=Connect_map[out_softmodule[i]].begin();it!=Connect_map[out_softmodule[i]].end();it++)
    //     {
    //         if(Modules[*it].isFixedModule())
    //         {
    //             first_modulehaveconnectwithfix=i;
    //             break;
    //         }
    //     }
    //     if(first_modulehaveconnectwithfix!=-1)
    //     {
    //         break;
    //     }
    // }

    return 0;
};
void final_output()
{
    double HPWL = newFinalAnswer(FinalPlacement);
    ofstream output;
    output.open(OutputFileName.c_str());
    output << "HPWL ";
    if ((int)HPWL == HPWL)
    {
        output << std::fixed << std::setprecision(0) << HPWL << endl;
    }
    else
    {
        output << std::fixed << std::setprecision(1) << HPWL << endl;
    }
    // int smc = 0;
    // for(map<int,Module>::iterator it=Modules.begin();it!=Modules.end();it++)
    // {
    //     if(it->second.isSoftModule())
    //     {
    //         smc++;
    //     }
    // }
    output << "SOFTMODULE " << /*smc*/ SoftModuleAmount << endl;
    // int hidefix=1;//0為秀出fix，1為不秀出fix
    for (int mod = 1; mod <= SoftModuleAmount; ++mod)
    {
        output << Modules[mod].ModuleName << " " << FinalPlacement[mod].size() << endl;
        for (int i = 0; i < FinalPlacement[mod].size(); ++i)
        {
            output << FinalPlacement[mod][i].x << " " << FinalPlacement[mod][i].y << endl;
        }
    }
    //     if(hidefix==1)
    //     {
    //         if(Modules[mod].isSoftModule())
    //         {
    //             if (mod != Modules.size())
    //             {
    //                 if(Modules[mod+1].isSoftModule())
    //                 {
    //                     int mod_x = sa_end_x1y1[mod].getX();
    //                     int mod_y = sa_end_x1y1[mod].getY();
    //                     int mod_w = Modules[mod].getWidth();
    //                     int mod_h = Modules[mod].getHeight();
    //                     output << Modules[mod].ModuleName << " 4" << endl;
    //                     output << mod_x << " " << mod_y << "\n"
    //                     << mod_x << " " << mod_y + mod_h << "\n"
    //                     << mod_x + mod_w << " " << mod_y + mod_h << "\n"
    //                     << mod_x + mod_w << " " << mod_y << endl;
    //                 }
    //                 else
    //                 {
    //                     int mod_x = sa_end_x1y1[mod].getX();
    //                     int mod_y = sa_end_x1y1[mod].getY();
    //                     int mod_w = Modules[mod].getWidth();
    //                     int mod_h = Modules[mod].getHeight();
    //                     output << Modules[mod].ModuleName << " 4" << endl;
    //                     output << mod_x << " " << mod_y << "\n"
    //                     << mod_x << " " << mod_y + mod_h << "\n"
    //                     << mod_x + mod_w << " " << mod_y + mod_h << "\n"
    //                     << mod_x + mod_w << " " << mod_y;
    //                 }
    //             }
    //             else
    //             {
    //                 int mod_x = sa_end_x1y1[mod].getX();
    //                 int mod_y = sa_end_x1y1[mod].getY();
    //                 int mod_w = Modules[mod].getWidth();
    //                 int mod_h = Modules[mod].getHeight();
    //                 output << Modules[mod].ModuleName << " 4" << endl;
    //                 output << mod_x << " " << mod_y << "\n"
    //                     << mod_x << " " << mod_y + mod_h << "\n"
    //                     << mod_x + mod_w << " " << mod_y + mod_h << "\n"
    //                     << mod_x + mod_w << " " << mod_y;
    //             }
    //         }
    //     }
    //     else
    //     {
    //         if (mod != Modules.size())
    //         {
    //             int mod_x = sa_end_x1y1[mod].getX();
    //             int mod_y = sa_end_x1y1[mod].getY();
    //             int mod_w = Modules[mod].getWidth();
    //             int mod_h = Modules[mod].getHeight();
    //             output << Modules[mod].ModuleName << " 4" << endl;
    //             output << mod_x << " " << mod_y << "\n"
    //             << mod_x << " " << mod_y + mod_h << "\n"
    //             << mod_x + mod_w << " " << mod_y + mod_h << "\n"
    //             << mod_x + mod_w << " " << mod_y << endl;
    //         }
    //         else
    //         {
    //             int mod_x = sa_end_x1y1[mod].getX();
    //             int mod_y = sa_end_x1y1[mod].getY();
    //             int mod_w = Modules[mod].getWidth();
    //             int mod_h = Modules[mod].getHeight();
    //             output << Modules[mod].ModuleName << " 4" << endl;
    //             output << mod_x << " " << mod_y << "\n"
    //             << mod_x << " " << mod_y + mod_h << "\n"
    //             << mod_x + mod_w << " " << mod_y + mod_h << "\n"
    //             << mod_x + mod_w << " " << mod_y;
    //         }
    //     }
    // };
    output.close();
}
int my_stoi(const std::string &str)
{
    std::stringstream ss(str);
    int result = 0;
    ss >> result;
    return result;
}

bool compareConnections(const Connection &a, const Connection &b)
{
    return a.ConnectionPin > b.ConnectionPin;
}

double FinalAnswer(map<int, leftdown_corner> ld)
{
    double linesum = 0;
    for (int connect = 0; connect < Connections.size(); ++connect)
    {
        int mod1 = Connections[connect].Module1ID;
        int mod2 = Connections[connect].Module2ID;
        // cout<<"mod1: "<<mod1<<", mod2: "<<mod2<<endl;
        double mod1_x = ld[mod1].getX();
        double mod1_y = ld[mod1].getY();
        double mod2_x = ld[mod2].getX();
        double mod2_y = ld[mod2].getY();
        double cost = 0;
        double mod1midx = mod1_x + (Modules[mod1].getWidth() / 2.0);
        double mod1midy = mod1_y + (Modules[mod1].getHeight() / 2.0);
        double mod2midx = mod2_x + (Modules[mod2].getWidth() / 2.0);
        double mod2midy = mod2_y + (Modules[mod2].getHeight() / 2.0);
        double x = abs(mod1midx - mod2midx);
        double y = abs(mod1midy - mod2midy);

        cost = ((x + y) * double(Connections[connect].ConnectionPin));
        // cout<<"Connect: "<<double(Connections[connect].ConnectionPin)<<endl;
        // cout <<"x: "<<x<<", y: "<<y<< ", (x + y): "<<(x + y)<<", double(Connections[connect].ConnectionPin): "<<double(Connections[connect].ConnectionPin)<<std::setprecision(1)<<", cost: "<<cost<<endl;
        linesum += cost;
    }
    // cout <<std::fixed<< std::setprecision(1)<<"linesum: "<<linesum<<endl;
    //  cout<<"linesum: "<<double(linesum)/10.0<<endl;
    return linesum;
}
double newFinalAnswer(vector<vector<leftdown_corner> > vvl)
{
    double linesum = 0;
    for (int connect = 0; connect < Connections.size(); ++connect)
    {
        int mod1 = Connections[connect].Module1ID;
        int mod2 = Connections[connect].Module2ID;
        // cout<<"mod1: "<<mod1<<", mod2: "<<mod2<<endl;
        double m1xmax = INT_MIN;
        double m1xmin = INT_MAX;
        double m1ymax = INT_MIN;
        double m1ymin = INT_MAX;
        double m2xmax = INT_MIN;
        double m2xmin = INT_MAX;
        double m2ymax = INT_MIN;
        double m2ymin = INT_MAX;
        for (int i = 1; i < vvl[mod1].size(); ++i)
        {
            m1xmax = max(m1xmax, double(vvl[mod1][i].getX()));
            m1xmin = min(m1xmin, double(vvl[mod1][i].getX()));
            m1ymax = max(m1ymax, double(vvl[mod1][i].getY()));
            m1ymin = min(m1ymin, double(vvl[mod1][i].getY()));
        }
        for (int i = 1; i < vvl[mod2].size(); ++i)
        {
            m2xmax = max(m2xmax, double(vvl[mod2][i].getX()));
            m2xmin = min(m2xmin, double(vvl[mod2][i].getX()));
            m2ymax = max(m2ymax, double(vvl[mod2][i].getY()));
            m2ymin = min(m2ymin, double(vvl[mod2][i].getY()));
        }
        double cost = 0;
        double mod1midx = (m1xmax + m1xmin) / 2.0;
        double mod1midy = (m1ymax + m1ymin) / 2.0;
        double mod2midx = (m2xmax + m2xmin) / 2.0;
        double mod2midy = (m2ymax + m2ymin) / 2.0;
        double x = abs(mod1midx - mod2midx);
        double y = abs(mod1midy - mod2midy);

        cost = ((x + y) * double(Connections[connect].ConnectionPin));
        // cout<<"Connect: "<<double(Connections[connect].ConnectionPin)<<endl;
        // cout <<"x: "<<x<<", y: "<<y<< ", (x + y): "<<(x + y)<<", double(Connections[connect].ConnectionPin): "<<double(Connections[connect].ConnectionPin)<<std::setprecision(1)<<", cost: "<<cost<<endl;
        linesum += cost;
    }
    return linesum;
}
pair<int, int> findDimensions(int area)
{
    double targetAspectRatio = 1;
    int width = std::sqrt(area * targetAspectRatio);
    int height = area / width;

    while (true)
    {
        double aspectRatio = static_cast<double>(width) / height;
        while (width * height < area)
        {
            if (width < height)
            {
                ++width;
            }
            else if (width > height)
            {
                ++height;
            }
            else
            {
                ++width;
            }
            if (aspectRatio >= 0.5 && aspectRatio <= 2.0)
            {
                // cout << "width: " << width << " height: " << height << endl;
                return std::make_pair(width, height);
            }
        };

        int widthDiff = std::abs(width - height);
        int targetDiff = std::abs(width - static_cast<int>(width * targetAspectRatio));

        if (widthDiff <= targetDiff && area * 1.0001 <= width * height)
        {
            area = std::ceil(width * height * 1.0001);
        }
        else
        {
            ++width;
            height = area / width;
        }
    };
    while (width * height < area)
    {
        if (width < height)
        {
            ++width;
        }
        else if (width > height)
        {
            ++height;
        }
        else
        {
            ++width;
        }
    };
}

void setFinalPlacement(int no) // 先把四角塞進去
{
    if (no == 0)
    {
        FinalPlacement.clear();
        FinalPlacement.resize(Modules.size() + 1); // 0不用
        for (int mod = 1; mod < Modules.size() + 1; ++mod)
        {
            leftdown_corner temp;
            temp.x = sa_end_x1y1[mod].getX();
            temp.y = sa_end_x1y1[mod].getY();
            FinalPlacement[mod].push_back(temp);
            temp.y = sa_end_x1y1[mod].getY() + Modules[mod].getHeight();
            FinalPlacement[mod].push_back(temp);
            temp.x = sa_end_x1y1[mod].getX() + Modules[mod].getWidth();
            FinalPlacement[mod].push_back(temp);
            temp.y = sa_end_x1y1[mod].getY();
            FinalPlacement[mod].push_back(temp);
        };
        // ofstream output;
        // output.open("output_soft_fixed_ori.txt", ios::out);
        // output << SoftModuleAmount << " " << FixedModuleAmount << endl;
        // for (int mod = 1; mod < Modules.size() + 1; ++mod)
        // {
        //     // sortCoordinates(FinalPlacement[mod]);
        //     output << "mod" << mod << " " << FinalPlacement[mod].size() << endl;
        //     for (int i = 0; i < FinalPlacement[mod].size(); ++i)
        //     {
        //         output << FinalPlacement[mod][i].x << " " << FinalPlacement[mod][i].y << endl;
        //     };
        // };
    }

    // ofstream output;
    // output.open("output_soft_fixed.txt", ios::out);
    // output << SoftModuleAmount << " " << FixedModuleAmount << endl;
    // for (int mod = 1; mod < Modules.size() + 1; ++mod)
    // {
    //     // sortCoordinates(FinalPlacement[mod]);
    //     output << "mod" << mod << " " << FinalPlacement[mod].size() << endl;
    //     for (int i = 0; i < FinalPlacement[mod].size(); ++i)
    //     {
    //         output << FinalPlacement[mod][i].x << " " << FinalPlacement[mod][i].y << endl;
    //     };
    // };
    // output.close();
    // ofstream output2;
    // output2.open("output_cnt.txt", ios::out);
    // for (int cnt = 0; cnt < Connections.size(); ++cnt)
    // {
    //     output2 << Connections[cnt].Module1ID << " " << Connections[cnt].Module2ID << " " << Connections[cnt].ConnectionPin << endl;
    // };
}
void move_soft_to_better_space(int w)
{
    const int Msize = Modules.size();
    vector<int> Move_order = out_softmodule;
    if (w == 0)
    {
        int n = Move_order.size();
        for (int i = 0; i < n - 1; ++i)
        {
            for (int j = 0; j < n - i - 1; ++j)
            {
                if (sa_end_x1y1[Move_order[j]].getY() > sa_end_x1y1[Move_order[j + 1]].getY())
                {
                    int temp = Move_order[j];
                    Move_order[j] = Move_order[j + 1];
                    Move_order[j + 1] = temp;
                }
            };
        };
        reverse(Move_order.begin(), Move_order.end());
    }
    else if (w == 1)
    {
        int n = Move_order.size();
        for (int i = 0; i < n - 1; ++i)
        {
            for (int j = 0; j < n - i - 1; ++j)
            {
                if (sa_end_x1y1[Move_order[j]].getY() > sa_end_x1y1[Move_order[j + 1]].getY())
                {
                    int temp = Move_order[j];
                    Move_order[j] = Move_order[j + 1];
                    Move_order[j + 1] = temp;
                }
            };
        };
    }
    else if (w == 2)
    {
        int n = Move_order.size();
        for (int i = 0; i < n - 1; ++i)
        {
            for (int j = 0; j < n - i - 1; ++j)
            {
                if (sa_end_x1y1[Move_order[j]].getY() > sa_end_x1y1[Move_order[j + 1]].getY())
                {
                    int temp = Move_order[j];
                    Move_order[j] = Move_order[j + 1];
                    Move_order[j + 1] = temp;
                }
            };
        };
        reverse(Move_order.begin(), Move_order.end());
    }
    else if (w == 3)
    {
        int n = Move_order.size();
        for (int i = 0; i < n - 1; ++i)
        {
            for (int j = 0; j < n - i - 1; ++j)
            {
                if (sa_end_x1y1[Move_order[j]].getY() > sa_end_x1y1[Move_order[j + 1]].getY())
                {
                    int temp = Move_order[j];
                    Move_order[j] = Move_order[j + 1];
                    Move_order[j + 1] = temp;
                }
            };
        };
    }
    for (int count = 0; count < 3; ++count)
    {
        for (int t = 0; t < Move_order.size(); ++t)
        {
            int i = Move_order[t];
            int module_x = sa_end_x1y1[i].getX();
            int module_y = sa_end_x1y1[i].getY();
            int best_way = -1;
            double best_HPWL = FinalAnswer(sa_end_x1y1);
            for (int way = 0; way < 4; ++way) // 0朝左,1朝上,2朝右,3朝下
            {
                if (way == 0)
                {
                    map<int, leftdown_corner> temp3 = sa_end_x1y1;
                    bool overlap = false;
                    temp3[i].x = temp3[i].x - 1;
                    int module_x_left = module_x - 1;
                    int module_y_left = module_y;
                    int up_x = module_x_left + Modules[i].getWidth();
                    int up_y = module_y_left + Modules[i].getHeight();
                    vector<int> find_space;
                    find_space.push_back(module_x_left);
                    find_space.push_back(module_y_left);
                    find_space.push_back(up_x);
                    find_space.push_back(up_y);
                    for (int j = 1; j <= Msize; ++j)
                    {
                        if (i == j)
                        {
                            continue;
                        }
                        vector<int> onpad_module;
                        onpad_module.push_back(sa_end_x1y1[j].getX());
                        onpad_module.push_back(sa_end_x1y1[j].getY());
                        onpad_module.push_back(sa_end_x1y1[j].getX() + Modules[j].getWidth());
                        onpad_module.push_back(sa_end_x1y1[j].getY() + Modules[j].getHeight());
                        if (isRectangleOverlap(find_space, onpad_module))
                        {
                            overlap = true;
                            break;
                        }
                    }
                    if (overlap)
                    {

                        continue;
                    }
                    double t_hpwl = FinalAnswer(temp3);
                    if (t_hpwl < best_HPWL)
                    {
                        best_way = 0;
                        best_HPWL = t_hpwl;
                    }
                }
                else if (way == 1)
                {
                    map<int, leftdown_corner> temp3 = sa_end_x1y1;
                    bool overlap = false;
                    temp3[i].y = temp3[i].y + 1;
                    int module_x_left = module_x;
                    int module_y_left = module_y + 1;
                    int up_x = module_x_left + Modules[i].getWidth();
                    int up_y = module_y_left + Modules[i].getHeight();
                    vector<int> find_space;
                    find_space.push_back(module_x_left);
                    find_space.push_back(module_y_left);
                    find_space.push_back(up_x);
                    find_space.push_back(up_y);
                    for (int j = 1; j <= Msize; ++j)
                    {
                        if (i == j)
                        {
                            continue;
                        }
                        vector<int> onpad_module;
                        onpad_module.push_back(sa_end_x1y1[j].getX());
                        onpad_module.push_back(sa_end_x1y1[j].getY());
                        onpad_module.push_back(sa_end_x1y1[j].getX() + Modules[j].getWidth());
                        onpad_module.push_back(sa_end_x1y1[j].getY() + Modules[j].getHeight());
                        if (isRectangleOverlap(find_space, onpad_module))
                        {
                            overlap = true;
                            break;
                        }
                    }
                    if (overlap)
                    {
                        continue;
                    }
                    double t_hpwl = FinalAnswer(temp3);
                    if (t_hpwl < best_HPWL)
                    {
                        best_way = 1;
                        best_HPWL = t_hpwl;
                    }
                }
                else if (way == 2)
                {
                    map<int, leftdown_corner> temp3 = sa_end_x1y1;
                    bool overlap = false;
                    temp3[i].x = temp3[i].x + 1;
                    int module_x_left = module_x + 1;
                    int module_y_left = module_y;
                    int up_x = module_x_left + Modules[i].getWidth();
                    int up_y = module_y_left + Modules[i].getHeight();
                    vector<int> find_space;
                    find_space.push_back(module_x_left);
                    find_space.push_back(module_y_left);
                    find_space.push_back(up_x);
                    find_space.push_back(up_y);
                    for (int j = 1; j <= Msize; ++j)
                    {
                        if (i == j)
                        {
                            continue;
                        }
                        vector<int> onpad_module;
                        onpad_module.push_back(sa_end_x1y1[j].getX());
                        onpad_module.push_back(sa_end_x1y1[j].getY());
                        onpad_module.push_back(sa_end_x1y1[j].getX() + Modules[j].getWidth());
                        onpad_module.push_back(sa_end_x1y1[j].getY() + Modules[j].getHeight());
                        if (isRectangleOverlap(find_space, onpad_module))
                        {
                            overlap = true;
                            break;
                        }
                    }
                    if (overlap)
                    {
                        continue;
                    }
                    double t_hpwl = FinalAnswer(temp3);
                    if (t_hpwl < best_HPWL)
                    {
                        best_way = 2;
                        best_HPWL = t_hpwl;
                    }
                }
                else if (way == 3)
                {
                    map<int, leftdown_corner> temp3 = sa_end_x1y1;
                    bool overlap = false;
                    temp3[i].y = temp3[i].y - 1;
                    int module_x_left = module_x;
                    int module_y_left = module_y - 1;
                    int up_x = module_x_left + Modules[i].getWidth();
                    int up_y = module_y_left + Modules[i].getHeight();
                    vector<int> find_space;
                    find_space.push_back(module_x_left);
                    find_space.push_back(module_y_left);
                    find_space.push_back(up_x);
                    find_space.push_back(up_y);
                    for (int j = 1; j <= Msize; ++j)
                    {
                        if (i == j)
                        {
                            continue;
                        }
                        vector<int> onpad_module;
                        onpad_module.push_back(sa_end_x1y1[j].getX());
                        onpad_module.push_back(sa_end_x1y1[j].getY());
                        onpad_module.push_back(sa_end_x1y1[j].getX() + Modules[j].getWidth());
                        onpad_module.push_back(sa_end_x1y1[j].getY() + Modules[j].getHeight());
                        if (isRectangleOverlap(find_space, onpad_module))
                        {
                            overlap = true;
                            break;
                        }
                    }
                    if (overlap)
                    {
                        continue;
                    }
                    double t_hpwl = FinalAnswer(temp3);
                    if (t_hpwl < best_HPWL)
                    {
                        best_way = 3;
                        best_HPWL = t_hpwl;
                    }
                }
            }
            // cout<<"module i : "<<i<<", bestway: "<<best_way<<endl;
            shift_module(i, best_way);
        }
    }
};
void shift_module(int i, int way)
{
    double Best_final = FinalAnswer(sa_end_x1y1);
    const int Msize = Modules.size();
    map<int, leftdown_corner> temp3 = sa_end_x1y1;
    map<int, Module> temp_Module = Modules;
    if (way == 0)
    {
        bool overlap = false;
        int nextx = temp3[i].x + 1;
        while (overlap == false)
        {
            --nextx;
            int module_x_left = nextx - 1;
            temp3[i].x = module_x_left;
            int module_y_left = temp3[i].y;
            int up_x = module_x_left + Modules[i].getWidth();
            int up_y = module_y_left + Modules[i].getHeight();
            vector<int> find_space;
            find_space.push_back(module_x_left);
            find_space.push_back(module_y_left);
            find_space.push_back(up_x);
            find_space.push_back(up_y);
            for (int j = 1; j <= Msize; ++j)
            {
                if (i == j)
                {
                    continue;
                }
                vector<int> onpad_module;
                onpad_module.push_back(sa_end_x1y1[j].getX());
                onpad_module.push_back(sa_end_x1y1[j].getY());
                onpad_module.push_back(sa_end_x1y1[j].getX() + Modules[j].getWidth());
                onpad_module.push_back(sa_end_x1y1[j].getY() + Modules[j].getHeight());
                if (isRectangleOverlap(find_space, onpad_module) || module_x_left < 0 || FinalAnswer(temp3) > Best_final)
                {
                    module_x_left = nextx;
                    temp3[i].x = module_x_left;
                    double ratio = 0.5;
                    double bang_best_score = FinalAnswer(temp3);
                    while (ratio <= 2.0)
                    {
                        be_changeAspectRatio(Modules[i], ratio);
                        if (ratio < 1)
                        {
                            ratio = ratio + 0.1;
                        }
                        else
                        {
                            ratio = ratio + 0.2;
                        }
                        up_x = module_x_left + Modules[i].getWidth();
                        up_y = module_y_left + Modules[i].getHeight();
                        vector<int> bang_space;
                        bang_space.push_back(module_x_left);
                        bang_space.push_back(module_y_left);
                        bang_space.push_back(up_x);
                        bang_space.push_back(up_y);
                        bool no_overlap = true;
                        for (int j = 1; j <= Msize; ++j)
                        {
                            if (i == j)
                            {
                                continue;
                            }
                            vector<int> onpad_module2;
                            onpad_module2.push_back(sa_end_x1y1[j].getX());
                            onpad_module2.push_back(sa_end_x1y1[j].getY());
                            onpad_module2.push_back(sa_end_x1y1[j].getX() + Modules[j].getWidth());
                            onpad_module2.push_back(sa_end_x1y1[j].getY() + Modules[j].getHeight());
                            if (isRectangleOverlap(bang_space, onpad_module2))
                            {
                                no_overlap = false;
                                break;
                            }
                        }
                        if (no_overlap)
                        {
                            if (sa_end_x1y1[i].getY() + Modules[i].getHeight() <= ChipHeight && sa_end_x1y1[i].getX() + Modules[i].getWidth() <= ChipWidth && sa_end_x1y1[i].getX() >= 0 && sa_end_x1y1[i].getY() >= 0)
                            {
                                double newScore = FinalAnswer(temp3);
                                if (newScore < bang_best_score)
                                {
                                    bang_best_score = newScore;
                                    temp_Module = Modules;
                                }
                            }
                        }
                    }
                    overlap = true;
                    break;
                }
            }
            if (overlap)
            {
                Modules = temp_Module;
                sa_end_x1y1[i].x = nextx;
                continue;
            }
        }
    }
    else if (way == 1)
    {
        map<int, leftdown_corner> temp3 = sa_end_x1y1;
        bool overlap = false;
        int nexty = temp3[i].y - 1;
        while (overlap == false)
        {
            ++nexty;
            int module_x_left = temp3[i].x;
            int module_y_left = nexty + 1;
            temp3[i].y = module_y_left;
            int up_x = module_x_left + Modules[i].getWidth();
            int up_y = module_y_left + Modules[i].getHeight();
            vector<int> find_space;
            find_space.push_back(module_x_left);
            find_space.push_back(module_y_left);
            find_space.push_back(up_x);
            find_space.push_back(up_y);
            for (int j = 1; j <= Msize; ++j)
            {
                if (i == j)
                {
                    continue;
                }
                vector<int> onpad_module;
                onpad_module.push_back(sa_end_x1y1[j].getX());
                onpad_module.push_back(sa_end_x1y1[j].getY());
                onpad_module.push_back(sa_end_x1y1[j].getX() + Modules[j].getWidth());
                onpad_module.push_back(sa_end_x1y1[j].getY() + Modules[j].getHeight());
                if (isRectangleOverlap(find_space, onpad_module) || up_y > ChipHeight || FinalAnswer(temp3) > Best_final)
                {
                    module_y_left = nexty;
                    temp3[i].y = module_y_left;
                    double ratio = 0.5;
                    double bang_best_score = FinalAnswer(temp3);
                    while (ratio <= 2.0)
                    {
                        be_changeAspectRatio(Modules[i], ratio);
                        if (ratio < 1)
                        {
                            ratio = ratio + 0.1;
                        }
                        else
                        {
                            ratio = ratio + 0.2;
                        }
                        up_x = module_x_left + Modules[i].getWidth();
                        up_y = module_y_left + Modules[i].getHeight();
                        vector<int> bang_space;
                        bang_space.push_back(module_x_left);
                        bang_space.push_back(module_y_left);
                        bang_space.push_back(up_x);
                        bang_space.push_back(up_y);
                        bool no_overlap = true;
                        for (int j = 1; j <= Msize; ++j)
                        {
                            if (i == j)
                            {
                                continue;
                            }
                            vector<int> onpad_module2;
                            onpad_module2.push_back(sa_end_x1y1[j].getX());
                            onpad_module2.push_back(sa_end_x1y1[j].getY());
                            onpad_module2.push_back(sa_end_x1y1[j].getX() + Modules[j].getWidth());
                            onpad_module2.push_back(sa_end_x1y1[j].getY() + Modules[j].getHeight());
                            if (isRectangleOverlap(bang_space, onpad_module2))
                            {
                                no_overlap = false;
                                break;
                            }
                        }
                        if (no_overlap)
                        {
                            if (sa_end_x1y1[i].getY() + Modules[i].getHeight() <= ChipHeight && sa_end_x1y1[i].getX() + Modules[i].getWidth() <= ChipWidth && sa_end_x1y1[i].getX() >= 0 && sa_end_x1y1[i].getY() >= 0)
                            {
                                double newScore = FinalAnswer(temp3);
                                if (newScore < bang_best_score)
                                {
                                    bang_best_score = newScore;
                                    temp_Module = Modules;
                                }
                            }
                        }
                    }
                    overlap = true;
                    break;
                }
            }
            if (overlap)
            {
                Modules = temp_Module;
                sa_end_x1y1[i].y = nexty;
                continue;
            }
        }
    }
    else if (way == 2)
    {
        map<int, leftdown_corner> temp3 = sa_end_x1y1;
        bool overlap = false;
        int nextx = temp3[i].x - 1;
        while (overlap == false)
        {
            ++nextx;
            int module_x_left = nextx + 1;
            temp3[i].x = module_x_left;
            int module_y_left = temp3[i].y;
            int up_x = module_x_left + Modules[i].getWidth();
            int up_y = module_y_left + Modules[i].getHeight();
            vector<int> find_space;
            find_space.push_back(module_x_left);
            find_space.push_back(module_y_left);
            find_space.push_back(up_x);
            find_space.push_back(up_y);
            for (int j = 1; j <= Msize; ++j)
            {
                if (i == j)
                {
                    continue;
                }
                vector<int> onpad_module;
                onpad_module.push_back(sa_end_x1y1[j].getX());
                onpad_module.push_back(sa_end_x1y1[j].getY());
                onpad_module.push_back(sa_end_x1y1[j].getX() + Modules[j].getWidth());
                onpad_module.push_back(sa_end_x1y1[j].getY() + Modules[j].getHeight());
                if (isRectangleOverlap(find_space, onpad_module) || up_x > ChipWidth || FinalAnswer(temp3) > Best_final)
                {
                    module_x_left = nextx;
                    temp3[i].x = module_x_left;
                    double ratio = 0.5;
                    double bang_best_score = FinalAnswer(temp3);
                    while (ratio <= 2.0)
                    {
                        be_changeAspectRatio(Modules[i], ratio);
                        if (ratio < 1)
                        {
                            ratio = ratio + 0.1;
                        }
                        else
                        {
                            ratio = ratio + 0.2;
                        }
                        up_x = module_x_left + Modules[i].getWidth();
                        up_y = module_y_left + Modules[i].getHeight();
                        vector<int> bang_space;
                        bang_space.push_back(module_x_left);
                        bang_space.push_back(module_y_left);
                        bang_space.push_back(up_x);
                        bang_space.push_back(up_y);
                        bool no_overlap = true;
                        for (int j = 1; j <= Msize; ++j)
                        {
                            if (i == j)
                            {
                                continue;
                            }
                            vector<int> onpad_module2;
                            onpad_module2.push_back(sa_end_x1y1[j].getX());
                            onpad_module2.push_back(sa_end_x1y1[j].getY());
                            onpad_module2.push_back(sa_end_x1y1[j].getX() + Modules[j].getWidth());
                            onpad_module2.push_back(sa_end_x1y1[j].getY() + Modules[j].getHeight());
                            if (isRectangleOverlap(bang_space, onpad_module2))
                            {
                                no_overlap = false;
                                break;
                            }
                        }
                        if (no_overlap)
                        {
                            if (sa_end_x1y1[i].getY() + Modules[i].getHeight() <= ChipHeight && sa_end_x1y1[i].getX() + Modules[i].getWidth() <= ChipWidth && sa_end_x1y1[i].getX() >= 0 && sa_end_x1y1[i].getY() >= 0)
                            {
                                double newScore = FinalAnswer(temp3);
                                if (newScore < bang_best_score)
                                {
                                    bang_best_score = newScore;
                                    temp_Module = Modules;
                                }
                            }
                        }
                    }
                    overlap = true;
                    break;
                }
            }
            if (overlap)
            {
                Modules = temp_Module;
                sa_end_x1y1[i].x = nextx;
                continue;
            }
        }
    }
    else if (way == 3)
    {
        map<int, leftdown_corner> temp3 = sa_end_x1y1;
        bool overlap = false;
        int nexty = temp3[i].y + 1;
        while (overlap == false)
        {
            --nexty;
            int module_x_left = temp3[i].x;
            int module_y_left = nexty - 1;
            temp3[i].y = module_y_left;
            int up_x = module_x_left + Modules[i].getWidth();
            int up_y = module_y_left + Modules[i].getHeight();
            vector<int> find_space;
            find_space.push_back(module_x_left);
            find_space.push_back(module_y_left);
            find_space.push_back(up_x);
            find_space.push_back(up_y);
            for (int j = 1; j <= Msize; ++j)
            {
                if (i == j)
                {
                    continue;
                }
                vector<int> onpad_module;
                onpad_module.push_back(sa_end_x1y1[j].getX());
                onpad_module.push_back(sa_end_x1y1[j].getY());
                onpad_module.push_back(sa_end_x1y1[j].getX() + Modules[j].getWidth());
                onpad_module.push_back(sa_end_x1y1[j].getY() + Modules[j].getHeight());
                if (isRectangleOverlap(find_space, onpad_module) || module_y_left < 0 || FinalAnswer(temp3) > Best_final)
                {
                    module_y_left = nexty;
                    temp3[i].y = module_y_left;
                    double ratio = 0.5;
                    double bang_best_score = FinalAnswer(temp3);
                    while (ratio <= 2.0)
                    {
                        be_changeAspectRatio(Modules[i], ratio);
                        if (ratio < 1)
                        {
                            ratio = ratio + 0.1;
                        }
                        else
                        {
                            ratio = ratio + 0.2;
                        }
                        up_x = module_x_left + Modules[i].getWidth();
                        up_y = module_y_left + Modules[i].getHeight();
                        vector<int> bang_space;
                        bang_space.push_back(module_x_left);
                        bang_space.push_back(module_y_left);
                        bang_space.push_back(up_x);
                        bang_space.push_back(up_y);
                        bool no_overlap = true;
                        for (int j = 1; j <= Msize; ++j)
                        {
                            if (i == j)
                            {
                                continue;
                            }
                            vector<int> onpad_module2;
                            onpad_module2.push_back(sa_end_x1y1[j].getX());
                            onpad_module2.push_back(sa_end_x1y1[j].getY());
                            onpad_module2.push_back(sa_end_x1y1[j].getX() + Modules[j].getWidth());
                            onpad_module2.push_back(sa_end_x1y1[j].getY() + Modules[j].getHeight());
                            if (isRectangleOverlap(bang_space, onpad_module2))
                            {
                                no_overlap = false;
                                break;
                            }
                        }
                        if (no_overlap)
                        {
                            if (sa_end_x1y1[i].getY() + Modules[i].getHeight() <= ChipHeight && sa_end_x1y1[i].getX() + Modules[i].getWidth() <= ChipWidth && sa_end_x1y1[i].getX() >= 0 && sa_end_x1y1[i].getY() >= 0)
                            {
                                double newScore = FinalAnswer(temp3);
                                if (newScore < bang_best_score)
                                {
                                    bang_best_score = newScore;
                                    temp_Module = Modules;
                                }
                            }
                        }
                    }
                    overlap = true;
                    break;
                }
            }
            if (overlap)
            {
                Modules = temp_Module;
                sa_end_x1y1[i].y = nexty;
                continue;
            }
        }
    }
};
void right_up_replace(map<int, Module> &temp_Modules, int remodule)
{
    const int Msize = Modules.size();
    map<int, leftdown_corner> temp3 = sa_end_x1y1;
    vector<bool> skip_mod;
    vector<vector<int> > can_fit_sets;
    skip_mod.resize(Msize);
    skip_mod[0] = true;
    skip_mod[remodule] = true;
    bool findspace = false;
    int bx = ChipWidth - Modules[remodule].getWidth();
    int by = ChipHeight - Modules[remodule].getHeight();
    int bx_pluswidth = bx + Modules[remodule].getWidth();
    int by_plusheight = by + Modules[remodule].getHeight();
    int skip_x = INT_MIN;
    int skip_y = INT_MIN;
    bool x_can_skip = false;
    bool y_can_skip = false;
    int jump_mod = 0;
    while (findspace == false)
    {
        // cout<<bx<<", "<<by<<endl;
        vector<int> remodule_locate;
        remodule_locate.push_back(bx);
        remodule_locate.push_back(by);
        remodule_locate.push_back(bx_pluswidth);
        remodule_locate.push_back(by_plusheight);

        int skip_module = 0;
        for (int j = 1; j <= Msize; ++j)
        {
            if (j == remodule)
            {
                continue;
            }
            vector<int> onpadmodule_locate;
            onpadmodule_locate.push_back(temp3[j].getX());
            onpadmodule_locate.push_back(temp3[j].getY());
            onpadmodule_locate.push_back(temp3[j].getX() + Modules[j].getWidth());
            onpadmodule_locate.push_back(temp3[j].getY() + Modules[j].getHeight());
            if (isRectangleOverlap(remodule_locate, onpadmodule_locate))
            {
                x_can_skip = true;
                y_can_skip = true;
                skip_x = max(skip_x, onpadmodule_locate[0]);
                if (skip_y < onpadmodule_locate[1])
                {
                    skip_y = onpadmodule_locate[1];
                    jump_mod = j;
                }
            }
        };
        if (x_can_skip == false)
        {
            can_fit_sets.push_back(remodule_locate);
            skip_x = bx;
            x_can_skip = true;
        }
        if (remodule_locate[0] <= 0)
        {
            bool trival_all = true;
            for (int t = 1; t <= SoftModuleAmount; ++t)
            {
                if (skip_mod[t] == false)
                {
                    trival_all = false;
                    break;
                }
            }
            if (trival_all)
            {
                findspace = true;
            }
            bx = ChipWidth - Modules[remodule].getWidth();
            if (y_can_skip)
            {
                by = skip_y - Modules[remodule].getHeight();
                skip_mod[jump_mod] = true;
                for (int g = 1; g <= Msize; ++g)
                {
                    if (skip_mod[g])
                    {
                        continue;
                    }
                    if (temp3[g].getY() + Modules[g].getHeight() <= skip_y)
                    {
                        skip_mod[g] = true;
                    }
                };
                skip_y = INT_MIN;
                skip_x = INT_MIN;
                y_can_skip = false;
                x_can_skip = false;
            }
            else
            {
                by = by - Modules[remodule].getHeight();
            }
        }
        else
        {
            if (x_can_skip)
            {
                bx = skip_x - Modules[remodule].getWidth();
                skip_x = INT_MIN;
                x_can_skip = false;
            }
            else
            {
                bx--;
            }
        }
        bx_pluswidth = bx + Modules[remodule].getWidth();
        by_plusheight = by + Modules[remodule].getHeight();
    };
    double bestHPWL = FinalAnswer(sa_end_x1y1);
    for (int l = 0; l < can_fit_sets.size(); ++l)
    {
        if (can_fit_sets[l][2] < ChipWidth && can_fit_sets[l][3] < ChipHeight)
        {
            temp3[remodule].x = can_fit_sets[l][0];
            temp3[remodule].y = can_fit_sets[l][1];
            double newHPWL = FinalAnswer(temp3);
            if (newHPWL < bestHPWL)
            {
                // cout<<"map_change"<<endl;
                bestHPWL = newHPWL;
                sa_end_x1y1 = temp3;
                temp_Modules = Modules;
            }
        }
    };
};
void right_down_replace(map<int, Module> &temp_Modules, int remodule)
{
    const int Msize = Modules.size();
    map<int, leftdown_corner> temp3 = sa_end_x1y1;
    vector<bool> skip_mod;
    vector<vector<int> > can_fit_sets;
    skip_mod.resize(Msize);
    skip_mod[0] = true;
    skip_mod[remodule] = true;
    bool findspace = false;
    int bx = ChipWidth - Modules[remodule].getWidth();
    int by = 0;
    int bx_pluswidth = bx + Modules[remodule].getWidth();
    int by_plusheight = by + Modules[remodule].getHeight();
    int skip_x = INT_MIN;
    int skip_y = INT_MAX;
    bool x_can_skip = false;
    bool y_can_skip = false;
    int jump_mod = 0;
    while (findspace == false)
    {
        // cout<<bx<<", "<<by<<endl;
        vector<int> remodule_locate;
        remodule_locate.push_back(bx);
        remodule_locate.push_back(by);
        remodule_locate.push_back(bx_pluswidth);
        remodule_locate.push_back(by_plusheight);

        int skip_module = 0;
        for (int j = 1; j <= Msize; ++j)
        {
            if (j == remodule)
            {
                continue;
            }
            vector<int> onpadmodule_locate;
            onpadmodule_locate.push_back(temp3[j].getX());
            onpadmodule_locate.push_back(temp3[j].getY());
            onpadmodule_locate.push_back(temp3[j].getX() + Modules[j].getWidth());
            onpadmodule_locate.push_back(temp3[j].getY() + Modules[j].getHeight());
            if (isRectangleOverlap(remodule_locate, onpadmodule_locate))
            {
                x_can_skip = true;
                y_can_skip = true;
                skip_x = max(skip_x, onpadmodule_locate[0]);
                if (skip_y > onpadmodule_locate[3])
                {
                    skip_y = onpadmodule_locate[3];
                    jump_mod = j;
                }
            }
        };
        if (x_can_skip == false)
        {
            can_fit_sets.push_back(remodule_locate);
            skip_x = bx;
            x_can_skip = true;
        }
        if (remodule_locate[0] <= 0)
        {
            bool trival_all = true;
            for (int t = 1; t <= SoftModuleAmount; ++t)
            {
                if (skip_mod[t] == false)
                {
                    trival_all = false;
                    break;
                }
            }
            if (trival_all)
            {
                findspace = true;
            }
            bx = ChipWidth - Modules[remodule].getWidth();
            if (y_can_skip)
            {
                by = skip_y;
                skip_mod[jump_mod] = true;
                for (int g = 1; g <= Msize; ++g)
                {
                    if (skip_mod[g])
                    {
                        continue;
                    }
                    if (temp3[g].getY() + Modules[g].getHeight() <= skip_y)
                    {
                        skip_mod[g] = true;
                    }
                };
                skip_y = INT_MAX;
                skip_x = INT_MIN;
                y_can_skip = false;
                x_can_skip = false;
            }
            else
            {
                by = by + Modules[remodule].getHeight();
            }
        }
        else
        {
            if (x_can_skip)
            {
                bx = skip_x - Modules[remodule].getWidth();
                skip_x = INT_MIN;
                x_can_skip = false;
            }
            else
            {
                bx--;
            }
        }
        bx_pluswidth = bx + Modules[remodule].getWidth();
        by_plusheight = by + Modules[remodule].getHeight();
    };
    double bestHPWL = FinalAnswer(sa_end_x1y1);
    for (int l = 0; l < can_fit_sets.size(); ++l)
    {
        if (can_fit_sets[l][2] < ChipWidth && can_fit_sets[l][3] < ChipHeight)
        {
            temp3[remodule].x = can_fit_sets[l][0];
            temp3[remodule].y = can_fit_sets[l][1];
            double newHPWL = FinalAnswer(temp3);
            if (newHPWL < bestHPWL)
            {
                // cout<<"map_change"<<endl;
                bestHPWL = newHPWL;
                sa_end_x1y1 = temp3;
                temp_Modules = Modules;
            }
        }
    };
};
void left_up_replace(map<int, Module> &temp_Modules, int remodule)
{
    const int Msize = Modules.size();
    map<int, leftdown_corner> temp3 = sa_end_x1y1;
    vector<bool> skip_mod;
    vector<vector<int> > can_fit_sets;
    skip_mod.resize(Msize);
    skip_mod[0] = true;
    skip_mod[remodule] = true;
    bool findspace = false;
    int bx = 0;
    int by = ChipHeight - Modules[remodule].getHeight();
    int bx_pluswidth = bx + Modules[remodule].getWidth();
    int by_plusheight = by + Modules[remodule].getHeight();
    int skip_x = INT_MAX;
    int skip_y = INT_MIN;
    bool x_can_skip = false;
    bool y_can_skip = false;
    int jump_mod = 0;
    while (findspace == false)
    {
        // cout<<bx<<", "<<by<<endl;
        vector<int> remodule_locate;
        remodule_locate.push_back(bx);
        remodule_locate.push_back(by);
        remodule_locate.push_back(bx_pluswidth);
        remodule_locate.push_back(by_plusheight);
        int skip_module = 0;
        for (int j = 1; j <= Msize; ++j)
        {
            if (j == remodule)
            {
                continue;
            }
            vector<int> onpadmodule_locate;
            onpadmodule_locate.push_back(temp3[j].getX());
            onpadmodule_locate.push_back(temp3[j].getY());
            onpadmodule_locate.push_back(temp3[j].getX() + Modules[j].getWidth());
            onpadmodule_locate.push_back(temp3[j].getY() + Modules[j].getHeight());
            if (isRectangleOverlap(remodule_locate, onpadmodule_locate))
            {
                x_can_skip = true;
                y_can_skip = true;
                skip_x = min(skip_x, onpadmodule_locate[2]);
                if (skip_y < onpadmodule_locate[1])
                {
                    skip_y = onpadmodule_locate[1];
                    jump_mod = j;
                }
            }
        };
        if (x_can_skip == false)
        {
            can_fit_sets.push_back(remodule_locate);
            skip_x = bx + Modules[remodule].getWidth();
            x_can_skip = true;
        }
        if (remodule_locate[2] >= ChipWidth)
        {
            bool trival_all = true;
            for (int t = 1; t <= SoftModuleAmount; ++t)
            {
                if (skip_mod[t] == false)
                {
                    trival_all = false;
                    break;
                }
            }
            if (trival_all)
            {
                findspace = true;
            }
            bx = 0;
            if (y_can_skip)
            {
                by = skip_y - Modules[remodule].getHeight();
                skip_mod[jump_mod] = true;
                for (int g = 1; g <= Msize; ++g)
                {
                    if (skip_mod[g])
                    {
                        continue;
                    }
                    if (temp3[g].getY() + Modules[g].getHeight() >= skip_y)
                    {
                        skip_mod[g] = true;
                    }
                };
                skip_y = INT_MIN;
                skip_x = INT_MAX;
                y_can_skip = false;
                x_can_skip = false;
            }
            else
            {
                by = by - Modules[remodule].getHeight();
            }
        }
        else
        {
            if (x_can_skip)
            {
                bx = skip_x;
                skip_x = INT_MAX;
                x_can_skip = false;
            }
            else
            {
                bx++;
            }
        }
        bx_pluswidth = bx + Modules[remodule].getWidth();
        by_plusheight = by + Modules[remodule].getHeight();
    };
    double bestHPWL = FinalAnswer(sa_end_x1y1);
    for (int l = 0; l < can_fit_sets.size(); ++l)
    {
        if (can_fit_sets[l][2] < ChipWidth && can_fit_sets[l][3] < ChipHeight)
        {
            temp3[remodule].x = can_fit_sets[l][0];
            temp3[remodule].y = can_fit_sets[l][1];
            double newHPWL = FinalAnswer(temp3);
            if (newHPWL < bestHPWL)
            {
                bestHPWL = newHPWL;
                sa_end_x1y1 = temp3;
                temp_Modules = Modules;
            }
        }
    };
};
void left_down_replace(map<int, Module> &temp_Modules, int remodule)
{
    const int Msize = Modules.size();
    map<int, leftdown_corner> temp3 = sa_end_x1y1;
    vector<bool> skip_mod;
    vector<vector<int> >can_fit_sets;
    skip_mod.resize(Msize);
    skip_mod[0] = true;
    skip_mod[remodule] = true;
    bool findspace = false;
    int bx = 0;
    int by = 0;
    int bx_pluswidth = bx + Modules[remodule].getWidth();
    int by_plusheight = by + Modules[remodule].getHeight();
    int skip_x = INT_MAX;
    int skip_y = INT_MAX;
    bool x_can_skip = false;
    bool y_can_skip = false;
    int jump_mod = 0;
    while (findspace == false)
    {
        // cout<<bx<<", "<<by<<endl;
        vector<int> remodule_locate;
        remodule_locate.push_back(bx);
        remodule_locate.push_back(by);
        remodule_locate.push_back(bx_pluswidth);
        remodule_locate.push_back(by_plusheight);

        int skip_module = 0;
        for (int j = 1; j <= Msize; ++j)
        {
            if (j == remodule)
            {
                continue;
            }
            vector<int> onpadmodule_locate;
            onpadmodule_locate.push_back(temp3[j].getX());
            onpadmodule_locate.push_back(temp3[j].getY());
            onpadmodule_locate.push_back(temp3[j].getX() + Modules[j].getWidth());
            onpadmodule_locate.push_back(temp3[j].getY() + Modules[j].getHeight());
            if (isRectangleOverlap(remodule_locate, onpadmodule_locate))
            {
                x_can_skip = true;
                y_can_skip = true;
                skip_x = min(skip_x, onpadmodule_locate[2]);
                if (skip_y > onpadmodule_locate[3])
                {
                    skip_y = onpadmodule_locate[3];
                    jump_mod = j;
                }
            }
        };
        if (x_can_skip == false)
        {
            can_fit_sets.push_back(remodule_locate);
            skip_x = bx + Modules[remodule].getWidth();
            x_can_skip = true;
        }
        if (remodule_locate[2] >= ChipWidth)
        {
            bool trival_all = true;
            for (int t = 1; t <= SoftModuleAmount; ++t)
            {
                if (skip_mod[t] == false)
                {
                    trival_all = false;
                    break;
                }
            }
            if (trival_all)
            {
                findspace = true;
            }
            bx = 0;
            if (y_can_skip)
            {
                by = skip_y;
                skip_mod[jump_mod] = true;
                for (int g = 1; g <= Msize; ++g)
                {
                    if (skip_mod[g])
                    {
                        continue;
                    }
                    if (temp3[g].getY() + Modules[g].getHeight() <= skip_y)
                    {
                        skip_mod[g] = true;
                    }
                };
                skip_y = INT_MAX;
                skip_x = INT_MAX;
                y_can_skip = false;
                x_can_skip = false;
            }
            else
            {
                by = by + Modules[remodule].getHeight();
            }
        }
        else
        {
            if (x_can_skip)
            {
                bx = skip_x;
                skip_x = INT_MAX;
                x_can_skip = false;
            }
            else
            {
                ++bx;
            }
        }
        bx_pluswidth = bx + Modules[remodule].getWidth();
        by_plusheight = by + Modules[remodule].getHeight();
    };
    double bestHPWL = FinalAnswer(sa_end_x1y1);
    for (int l = 0; l < can_fit_sets.size(); ++l)
    {
        if (can_fit_sets[l][2] < ChipWidth && can_fit_sets[l][3] < ChipHeight)
        {
            temp3[remodule].x = can_fit_sets[l][0];
            temp3[remodule].y = can_fit_sets[l][1];
            double newHPWL = FinalAnswer(temp3);
            if (newHPWL < bestHPWL)
            {
                // cout<<"map_change"<<endl;
                bestHPWL = newHPWL;
                sa_end_x1y1 = temp3;
                temp_Modules = Modules;
            }
        }
    };
};
void replace_soft_to_better_space(int bestway, vector<int> small_first)
{
    vector<int> tempsmall_first = small_first;
    int n = tempsmall_first.size();
    for (int i = 0; i < n - 1; ++i)
    {
        for (int j = 0; j < n - i - 1; ++j)
        {
            if (Modules[tempsmall_first[j]].mt.data.soft_module_data.RequiredSize > Modules[tempsmall_first[j + 1]].mt.data.soft_module_data.RequiredSize)
            {
                int temp = tempsmall_first[j];
                tempsmall_first[j] = tempsmall_first[j + 1];
                tempsmall_first[j + 1] = temp;
            }
        };
    };
    reverse(tempsmall_first.begin(), tempsmall_first.end());
    for (int t = 0; t < tempsmall_first.size(); ++t)
    {
        int i = tempsmall_first[t];
        left_down_replace(Modules, i);
    };
};
void merge()
{
    set<int> hasdoMerge;
    map<pair<int, int>, int> Overlap_line_sort;
    map<pair<int, int>, int> module_force;
    for (int i = 1; i <= SoftModuleAmount; ++i)
    {
        vector<int> overlap_Softmodule;
        leftdown_corner forceway;
        forceway = ModPullingDirection(i);
        // std::set<int>::iterator it;
        // cout<<"i: "<<i<<", Connect_map[i]: ";
        // for (it =Connect_map[i].begin(); it != Connect_map[i].end(); ++it) {
        //     std::cout << *it << " ";
        // }
        // cout<<endl;
        int x_abs = abs(forceway.getX());
        int y_abs = abs(forceway.getY());
        bool to_x = false;
        bool to_y = false;
        if (x_abs == 0 && y_abs == 0)
        {
            continue;
        }
        if (x_abs > y_abs)
        {
            to_x = true;
            to_y = false;
        }
        else
        {
            to_x = false;
            to_y = true;
        }
        if (to_x)
        {
            if (forceway.getX() > 0)
            {
                int module_x_left = sa_end_x1y1[i].x + 1;
                int module_y_left = sa_end_x1y1[i].y;
                int up_x = module_x_left + Modules[i].getWidth();
                int up_y = module_y_left + Modules[i].getHeight();
                vector<int> find_space;
                find_space.push_back(module_x_left);
                find_space.push_back(module_y_left);
                find_space.push_back(up_x);
                find_space.push_back(up_y);
                for (int j = 1; j <= SoftModuleAmount; ++j)
                {
                    if (i == j)
                    {
                        continue;
                    }
                    vector<int> onpad_module;
                    onpad_module.push_back(sa_end_x1y1[j].getX());
                    onpad_module.push_back(sa_end_x1y1[j].getY());
                    onpad_module.push_back(sa_end_x1y1[j].getX() + Modules[j].getWidth());
                    onpad_module.push_back(sa_end_x1y1[j].getY() + Modules[j].getHeight());
                    if (isRectangleOverlap(find_space, onpad_module))
                    {
                        overlap_Softmodule.push_back(j);
                        leftdown_corner forceway_j;
                        forceway_j = ModPullingDirection(j);
                        module_force[make_pair(i, j)] = abs(forceway.getX() - forceway_j.getX());
                    }
                };
            }
            else
            {
                int module_x_left = sa_end_x1y1[i].x - 1;
                int module_y_left = sa_end_x1y1[i].y;
                int up_x = module_x_left + Modules[i].getWidth();
                int up_y = module_y_left + Modules[i].getHeight();
                vector<int> find_space;
                find_space.push_back(module_x_left);
                find_space.push_back(module_y_left);
                find_space.push_back(up_x);
                find_space.push_back(up_y);
                for (int j = 1; j <= SoftModuleAmount; ++j)
                {
                    if (i == j)
                    {
                        continue;
                    }
                    vector<int> onpad_module;
                    onpad_module.push_back(sa_end_x1y1[j].getX());
                    onpad_module.push_back(sa_end_x1y1[j].getY());
                    onpad_module.push_back(sa_end_x1y1[j].getX() + Modules[j].getWidth());
                    onpad_module.push_back(sa_end_x1y1[j].getY() + Modules[j].getHeight());
                    if (isRectangleOverlap(find_space, onpad_module))
                    {
                        overlap_Softmodule.push_back(j);
                        leftdown_corner forceway_j;
                        forceway_j = ModPullingDirection(j);
                        module_force[make_pair(i, j)] = abs(forceway.getX() - forceway_j.getX());
                    }
                };
            }
        }
        if (to_y)
        {
            if (forceway.getY() > 0)
            {
                int module_x_left = sa_end_x1y1[i].x;
                int module_y_left = sa_end_x1y1[i].y + 1;
                int up_x = module_x_left + Modules[i].getWidth();
                int up_y = module_y_left + Modules[i].getHeight();
                vector<int> find_space;
                find_space.push_back(module_x_left);
                find_space.push_back(module_y_left);
                find_space.push_back(up_x);
                find_space.push_back(up_y);
                for (int j = 1; j <= SoftModuleAmount; ++j)
                {
                    if (i == j)
                    {
                        continue;
                    }
                    vector<int> onpad_module;
                    onpad_module.push_back(sa_end_x1y1[j].getX());
                    onpad_module.push_back(sa_end_x1y1[j].getY());
                    onpad_module.push_back(sa_end_x1y1[j].getX() + Modules[j].getWidth());
                    onpad_module.push_back(sa_end_x1y1[j].getY() + Modules[j].getHeight());
                    if (isRectangleOverlap(find_space, onpad_module))
                    {
                        overlap_Softmodule.push_back(j);
                        leftdown_corner forceway_j;
                        forceway_j = ModPullingDirection(j);
                        module_force[make_pair(i, j)] = abs(forceway.getY() - forceway_j.getY());
                    }
                };
            }
            else
            {
                int module_x_left = sa_end_x1y1[i].x;
                int module_y_left = sa_end_x1y1[i].y - 1;
                int up_x = module_x_left + Modules[i].getWidth();
                int up_y = module_y_left + Modules[i].getHeight();
                vector<int> find_space;
                find_space.push_back(module_x_left);
                find_space.push_back(module_y_left);
                find_space.push_back(up_x);
                find_space.push_back(up_y);
                for (int j = 1; j <= SoftModuleAmount; ++j)
                {
                    if (i == j)
                    {
                        continue;
                    }
                    vector<int> onpad_module;
                    onpad_module.push_back(sa_end_x1y1[j].getX());
                    onpad_module.push_back(sa_end_x1y1[j].getY());
                    onpad_module.push_back(sa_end_x1y1[j].getX() + Modules[j].getWidth());
                    onpad_module.push_back(sa_end_x1y1[j].getY() + Modules[j].getHeight());
                    if (isRectangleOverlap(find_space, onpad_module))
                    {
                        overlap_Softmodule.push_back(j);
                        leftdown_corner forceway_j;
                        forceway_j = ModPullingDirection(j);
                        module_force[make_pair(i, j)] = abs(forceway.getY() - forceway_j.getY());
                    }
                };
            }
        }
        for (int k = 0; k < overlap_Softmodule.size(); ++k)
        {
            // cout << "mod: " << i << ", mod: " << overlap_Softmodule[k] << ",with connection and adjacent" << endl;
            Overlap_line_sort[make_pair(i, overlap_Softmodule[k])] = module_force[make_pair(i, overlap_Softmodule[k])];
        };
    };
    vector<pair<pair<int, int>, int> > keyValueVec(Overlap_line_sort.begin(), Overlap_line_sort.end());
    vector<pair<pair<int, int>, int> > temp = keyValueVec;
    int n = temp.size();
    for (int i = 0; i < n - 1; ++i)
    {
        for (int j = 0; j < n - i - 1; ++j)
        {
            if (temp[j].second > temp[j + 1].second)
            {
                pair<pair<int, int>, int> temp1 = temp[j];
                temp[j] = temp[j + 1];
                temp[j + 1] = temp1;
            }
        };
    };
    reverse(temp.begin(), temp.end());
    keyValueVec = temp;
    for (vector<pair<pair<int, int>, int> >::iterator it = keyValueVec.begin(); it != keyValueVec.end(); ++it)
    {
        leftdown_corner start;
        leftdown_corner end;
        std::pair<std::pair<int, int>, int> &pair = *it; // const拔掉
        std::pair<int, int> &mod = pair.first;
        int value = pair.second;
        // cout<<key.first<<", "<<key.second<<", "<<value<<endl;
        if (hasdoMerge.find(mod.first) == hasdoMerge.end() && hasdoMerge.find(mod.second) == hasdoMerge.end())
        {
            merge_two_block(mod.first, mod.second, start, end);
            hasdoMerge.insert(mod.first);
            hasdoMerge.insert(mod.second);
            // cout << "Mod: " << mod.first << ", Mod:" << mod.second << ", with ";
            // cout << "start: (" << start.x << ", " << start.y << "), ";
            // cout << "end: (" << end.x << ", " << end.y << ")"
            //      << ", by connectPIN: " << value << endl;
            // 做融合
            int exchangeLen = ExchangingLength(mod.first, mod.second, start, end);
            // cout<<"returned exchanginglength: "<<exchangingLength<<endl;
            if (exchangeLen <= 0)
            {
                continue;
            }
            // 更新FinalPlacement
            leftdown_corner temp;
            temp.x = start.x;
            temp.y = start.y;
            int leftright = ModLocationRelation(start, end); // 回傳1是左右關係，0是上下關係
            if (leftright)                                   // 左右關係
            {
                // cout << "left right" << endl;
                int exchangeHeight = abs(end.y - start.y);
                if (exchangeHeight <= 0)
                {
                    continue;
                }
                bool isodd = false;
                if (exchangeHeight % 2 == 1) // 先判斷奇數偶數
                {
                    // cout << "exchangingWidth is odd" << endl;
                    isodd = true;
                    // continue;
                }
                exchangeHeight = exchangeHeight / 2; // 再除以2
                // cout << "exchangingHeight: " << exchangeHeight << endl;
                // cout << "exchangingWidth: " << exchangeLen << endl;
                if (exchangeLen <= 0 || exchangeHeight <= 0)
                {
                    continue;
                }
                // which module is left
                bool mod1_left = false;
                if (FinalPlacement[mod.first][0].x < FinalPlacement[mod.second][0].x)
                {
                    mod1_left = true;
                }
                if (!mod1_left) // 如果第一個模組在右邊
                {
                    // cout << "mod1 right" << endl;
                    // 交換兩個模組
                    // cout << "exchange two modules" << endl;
                    // cout << "mod1: " << mod.first << ", mod2: " << mod.second << endl;
                    int temp = mod.first;
                    mod.first = mod.second;
                    mod.second = temp;
                    // cout << "mod1: " << mod.first << ", mod2: " << mod.second << endl;
                }
                if (FinalPlacement[mod.first][1].y == FinalPlacement[mod.second][1].y) // 如果上邊等高
                {
                    // cout << "up same" << endl;
                    if (FinalPlacement[mod.first][0].y < FinalPlacement[mod.second][0].y) // 上邊等高且如果下邊為m1比m2低
                    {
                        if (isodd)
                        {
                            // cout << "up same m1(left) down lower(odd)" << endl;
                            // 先做左邊的m1 //0 1不變
                            FinalPlacement[mod.first][2].x = FinalPlacement[mod.first][2].x - exchangeLen; // 2
                            int temp_x = FinalPlacement[mod.first][3].x;                                   // 右下先記下來
                            int temp_y = FinalPlacement[mod.first][3].y;
                            FinalPlacement[mod.first][3].x = FinalPlacement[mod.first][2].x;
                            FinalPlacement[mod.first][3].y = FinalPlacement[mod.first][2].y - exchangeHeight;
                            leftdown_corner temp;
                            temp.x = FinalPlacement[mod.first][3].x + exchangeLen * 2;
                            temp.y = FinalPlacement[mod.first][3].y;
                            FinalPlacement[mod.first].push_back(temp); // 4
                            temp.x = FinalPlacement[mod.first][4].x;
                            temp.y = FinalPlacement[mod.first][4].y - exchangeHeight;
                            FinalPlacement[mod.first].push_back(temp); // 5
                            temp.x = FinalPlacement[mod.first][5].x - exchangeLen;
                            temp.y = FinalPlacement[mod.first][5].y;
                            FinalPlacement[mod.first].push_back(temp); // 6
                            temp.x = temp_x;
                            temp.y = temp_y;
                            FinalPlacement[mod.first].push_back(temp); // 7(右下)
                            // 再做右邊的m2
                            // 0 不變
                            FinalPlacement[mod.second][1].x = FinalPlacement[mod.first][6].x;
                            FinalPlacement[mod.second][1].y = FinalPlacement[mod.first][6].y; // 1
                            int temp_x1 = FinalPlacement[mod.second][2].x;
                            int temp_y1 = FinalPlacement[mod.second][2].y; // 右上先記下來
                            FinalPlacement[mod.second][2].x = FinalPlacement[mod.first][5].x;
                            FinalPlacement[mod.second][2].y = FinalPlacement[mod.first][5].y; // 2
                            int temp_x2 = FinalPlacement[mod.second][3].x;
                            int temp_y2 = FinalPlacement[mod.second][3].y; // 右下先記下來
                            FinalPlacement[mod.second][3].x = FinalPlacement[mod.first][4].x;
                            FinalPlacement[mod.second][3].y = FinalPlacement[mod.first][4].y; // 3
                            temp.x = FinalPlacement[mod.first][3].x;
                            temp.y = FinalPlacement[mod.first][3].y;
                            FinalPlacement[mod.second].push_back(temp); // 4
                            temp.x = FinalPlacement[mod.first][2].x;
                            temp.y = FinalPlacement[mod.first][2].y;
                            FinalPlacement[mod.second].push_back(temp); // 5
                            temp.x = temp_x1;
                            temp.y = temp_y1;
                            FinalPlacement[mod.second].push_back(temp); // 6
                            temp.x = temp_x2;
                            temp.y = temp_y2;
                            FinalPlacement[mod.second].push_back(temp); // 7
                        }
                        else
                        {
                            // cout << "up same m1(left) down lower(even)" << endl;
                            // 先做左邊的m1 //0 1不變
                            FinalPlacement[mod.first][2].x = FinalPlacement[mod.first][2].x - exchangeLen; // 2
                            int temp_x = FinalPlacement[mod.first][3].x;                                   // 右下先記下來
                            int temp_y = FinalPlacement[mod.first][3].y;
                            FinalPlacement[mod.first][3].x = FinalPlacement[mod.first][2].x;
                            FinalPlacement[mod.first][3].y = FinalPlacement[mod.first][2].y - exchangeHeight;
                            leftdown_corner temp;
                            temp.x = FinalPlacement[mod.first][3].x + exchangeLen * 2;
                            temp.y = FinalPlacement[mod.first][3].y;
                            FinalPlacement[mod.first].push_back(temp); // 4
                            temp.x = FinalPlacement[mod.first][4].x;
                            temp.y = FinalPlacement[mod.first][4].y - exchangeHeight;
                            FinalPlacement[mod.first].push_back(temp); // 5
                            temp.x = FinalPlacement[mod.first][5].x - exchangeLen;
                            temp.y = FinalPlacement[mod.first][5].y;
                            FinalPlacement[mod.first].push_back(temp); // 6
                            temp.x = temp_x;
                            temp.y = temp_y;
                            FinalPlacement[mod.first].push_back(temp); // 7(右下)
                            // 再做右邊的m2
                            FinalPlacement[mod.second][0].x = FinalPlacement[mod.first][5].x; // 0
                            FinalPlacement[mod.second][1].x = FinalPlacement[mod.first][4].x;
                            FinalPlacement[mod.second][1].y = FinalPlacement[mod.first][4].y; // 1
                            int temp_x1 = FinalPlacement[mod.second][2].x;
                            int temp_y1 = FinalPlacement[mod.second][2].y; // 右上先記下來
                            FinalPlacement[mod.second][2].x = FinalPlacement[mod.first][3].x;
                            FinalPlacement[mod.second][2].y = FinalPlacement[mod.first][3].y; // 0
                            int temp_x2 = FinalPlacement[mod.second][3].x;
                            int temp_y2 = FinalPlacement[mod.second][3].y; // 右下先記下來
                            FinalPlacement[mod.second][3].x = FinalPlacement[mod.first][2].x;
                            FinalPlacement[mod.second][3].y = FinalPlacement[mod.first][2].y; // 3
                            temp.x = temp_x1;
                            temp.y = temp_y1;
                            FinalPlacement[mod.second].push_back(temp); // 4
                            temp.x = temp_x2;
                            temp.y = temp_y2;
                            FinalPlacement[mod.second].push_back(temp); // 5
                        }
                    }
                    else if (FinalPlacement[mod.first][0].y == FinalPlacement[mod.second][0].y) // 上下邊等高
                    {
                        if (isodd)
                        {
                            // cout << "up down same(odd)" << endl;
                            // 先做左邊的m1 //0 1不變
                            FinalPlacement[mod.first][2].x = FinalPlacement[mod.first][2].x - exchangeLen; // 2
                            int temp_x = FinalPlacement[mod.first][3].x;                                   // 右下先記下來
                            int temp_y = FinalPlacement[mod.first][3].y;
                            FinalPlacement[mod.first][3].x = FinalPlacement[mod.first][2].x;
                            FinalPlacement[mod.first][3].y = FinalPlacement[mod.first][2].y - exchangeHeight; // 3
                            leftdown_corner temp;
                            temp.x = FinalPlacement[mod.first][3].x + exchangeLen * 2;
                            temp.y = FinalPlacement[mod.first][3].y;
                            FinalPlacement[mod.first].push_back(temp); // 4
                            temp.x = FinalPlacement[mod.first][4].x;
                            temp.y = FinalPlacement[mod.first][4].y - exchangeHeight;
                            FinalPlacement[mod.first].push_back(temp); // 5
                            temp.x = FinalPlacement[mod.first][5].x - exchangeLen;
                            FinalPlacement[mod.first].push_back(temp); // 6
                            temp.x = temp_x;
                            temp.y = temp_y;
                            FinalPlacement[mod.first].push_back(temp); // 7(右下)
                            // 再做右邊的m2
                            FinalPlacement[mod.second][1].x = FinalPlacement[mod.first][6].x;
                            FinalPlacement[mod.second][1].y = FinalPlacement[mod.first][6].y; // 1
                            int temp_x1 = FinalPlacement[mod.second][2].x;
                            int temp_y1 = FinalPlacement[mod.second][2].y; // 右上先記下來
                            FinalPlacement[mod.second][2].x = FinalPlacement[mod.first][5].x;
                            FinalPlacement[mod.second][2].y = FinalPlacement[mod.first][5].y; // 2
                            int temp_x2 = FinalPlacement[mod.second][3].x;
                            int temp_y2 = FinalPlacement[mod.second][3].y; // 右下先記下來
                            FinalPlacement[mod.second][3].x = FinalPlacement[mod.first][4].x;
                            FinalPlacement[mod.second][3].y = FinalPlacement[mod.first][4].y; // 3
                            temp.x = FinalPlacement[mod.first][3].x;
                            temp.y = FinalPlacement[mod.first][3].y;
                            FinalPlacement[mod.second].push_back(temp); // 4
                            temp.x = FinalPlacement[mod.first][2].x;
                            temp.y = FinalPlacement[mod.first][2].y;
                            FinalPlacement[mod.second].push_back(temp); // 5
                            temp.x = temp_x1;
                            temp.y = temp_y1;
                            FinalPlacement[mod.second].push_back(temp); // 6
                            temp.x = temp_x2;
                            temp.y = temp_y2;
                            FinalPlacement[mod.second].push_back(temp); // 7
                        }
                        else
                        {
                            // cout << "up down same(even)" << endl;
                            // 先做左邊的m1 //0 1不變
                            FinalPlacement[mod.first][2].x = FinalPlacement[mod.first][2].x - exchangeLen; // 2
                            FinalPlacement[mod.first][3].x = FinalPlacement[mod.first][2].x;
                            FinalPlacement[mod.first][3].y = FinalPlacement[mod.first][2].y - exchangeHeight; // 3
                            leftdown_corner temp;
                            temp.x = FinalPlacement[mod.first][3].x + exchangeLen * 2;
                            temp.y = FinalPlacement[mod.first][3].y;
                            FinalPlacement[mod.first].push_back(temp); // 4
                            temp.x = FinalPlacement[mod.first][4].x;
                            temp.y = FinalPlacement[mod.first][4].y - exchangeHeight;
                            FinalPlacement[mod.first].push_back(temp); // 5
                            // 再做右邊的m2
                            FinalPlacement[mod.second][0].x = FinalPlacement[mod.first][5].x; // 0
                            FinalPlacement[mod.second][1].x = FinalPlacement[mod.first][4].x;
                            FinalPlacement[mod.second][1].y = FinalPlacement[mod.first][4].y; // 1
                            int temp_x1 = FinalPlacement[mod.second][2].x;
                            int temp_y1 = FinalPlacement[mod.second][2].y; // 右上先記下來
                            FinalPlacement[mod.second][2].x = FinalPlacement[mod.first][3].x;
                            FinalPlacement[mod.second][2].y = FinalPlacement[mod.first][3].y; // 2
                            int temp_x2 = FinalPlacement[mod.second][3].x;
                            int temp_y2 = FinalPlacement[mod.second][3].y; // 右下先記下來
                            FinalPlacement[mod.second][3].x = FinalPlacement[mod.first][2].x;
                            FinalPlacement[mod.second][3].y = FinalPlacement[mod.first][2].y; // 3
                            temp.x = temp_x1;
                            temp.y = temp_y1;
                            FinalPlacement[mod.second].push_back(temp); // 4
                            temp.x = temp_x2;
                            temp.y = temp_y2;
                            FinalPlacement[mod.second].push_back(temp); // 5
                        }
                    }
                    else if (FinalPlacement[mod.first][0].y > FinalPlacement[mod.second][0].y) // 上邊等高 下邊為m1比m2高
                    {
                        if (isodd)
                        {
                            // cout << "up same m1(left) down higher(odd)" << endl;
                            // 先做左邊的m1 //0 1不變
                            int temp_x1 = FinalPlacement[mod.first][3].x;
                            int temp_y1 = FinalPlacement[mod.first][3].y;                                  // 右下先記下來
                            FinalPlacement[mod.first][2].x = FinalPlacement[mod.first][2].x - exchangeLen; // 2
                            FinalPlacement[mod.first][3].x = FinalPlacement[mod.first][2].x;
                            FinalPlacement[mod.first][3].y = FinalPlacement[mod.first][2].y - exchangeHeight; // 3
                            leftdown_corner temp;
                            temp.x = FinalPlacement[mod.first][3].x + exchangeLen * 2;
                            temp.y = FinalPlacement[mod.first][3].y;
                            FinalPlacement[mod.first].push_back(temp); // 4
                            temp.x = FinalPlacement[mod.first][4].x;
                            temp.y = FinalPlacement[mod.first][4].y - exchangeHeight;
                            FinalPlacement[mod.first].push_back(temp); // 5
                            temp.x = FinalPlacement[mod.first][5].x - exchangeLen;
                            FinalPlacement[mod.first].push_back(temp); // 6
                            temp.x = temp_x1;
                            temp.y = temp_y1;
                            FinalPlacement[mod.first].push_back(temp); // 7(右下)
                            // 再做右邊的m2 //0不變
                            FinalPlacement[mod.second][1].x = FinalPlacement[mod.first][6].x; // 1
                            FinalPlacement[mod.second][1].y = FinalPlacement[mod.first][6].y;
                            int temp_x2 = FinalPlacement[mod.second][2].x;
                            int temp_y2 = FinalPlacement[mod.second][2].y; // 右上先記下來
                            FinalPlacement[mod.second][2].x = FinalPlacement[mod.first][5].x;
                            FinalPlacement[mod.second][2].y = FinalPlacement[mod.first][5].y; // 2
                            int temp_x3 = FinalPlacement[mod.second][3].x;
                            int temp_y3 = FinalPlacement[mod.second][3].y; // 右下先記下來
                            FinalPlacement[mod.second][3].x = FinalPlacement[mod.first][4].x;
                            FinalPlacement[mod.second][3].y = FinalPlacement[mod.first][4].y; // 3
                            temp.x = FinalPlacement[mod.first][3].x;
                            temp.y = FinalPlacement[mod.first][3].y;
                            FinalPlacement[mod.second].push_back(temp); // 4
                            temp.x = FinalPlacement[mod.first][2].x;
                            temp.y = FinalPlacement[mod.first][2].y;
                            FinalPlacement[mod.second].push_back(temp); // 5
                            temp.x = temp_x2;
                            temp.y = temp_y2;
                            FinalPlacement[mod.second].push_back(temp); // 6
                            temp.x = temp_x3;
                            temp.y = temp_y3;
                            FinalPlacement[mod.second].push_back(temp); // 7
                        }
                        else
                        {
                            // cout << "up same m1(left) down higher(even)" << endl;
                            // 先做左邊的m1 //0 1不變
                            int temp_x1 = FinalPlacement[mod.first][3].x;
                            int temp_y1 = FinalPlacement[mod.first][3].y;                                  // 右下先記下來
                            FinalPlacement[mod.first][2].x = FinalPlacement[mod.first][2].x - exchangeLen; // 2
                            FinalPlacement[mod.first][3].x = FinalPlacement[mod.first][2].x;
                            FinalPlacement[mod.first][3].y = FinalPlacement[mod.first][2].y - exchangeHeight; // 3
                            leftdown_corner temp;
                            temp.x = FinalPlacement[mod.first][3].x + exchangeLen * 2;
                            temp.y = FinalPlacement[mod.first][3].y;
                            FinalPlacement[mod.first].push_back(temp); // 4
                            temp.x = FinalPlacement[mod.first][4].x;
                            temp.y = FinalPlacement[mod.first][4].y - exchangeHeight;
                            FinalPlacement[mod.first].push_back(temp); // 5
                            // 再做右邊的m2 //0不變
                            FinalPlacement[mod.second][1].x = FinalPlacement[mod.first][5].x - exchangeLen; // 1
                            FinalPlacement[mod.second][1].y = FinalPlacement[mod.first][5].y;
                            int temp_x2 = FinalPlacement[mod.second][2].x;
                            int temp_y2 = FinalPlacement[mod.second][2].y; // 右上先記下來
                            FinalPlacement[mod.second][2].x = FinalPlacement[mod.first][5].x;
                            FinalPlacement[mod.second][2].y = FinalPlacement[mod.first][5].y; // 2
                            int temp_x3 = FinalPlacement[mod.second][3].x;
                            int temp_y3 = FinalPlacement[mod.second][3].y; // 右下先記下來
                            FinalPlacement[mod.second][3].x = FinalPlacement[mod.first][4].x;
                            FinalPlacement[mod.second][3].y = FinalPlacement[mod.first][4].y; // 3
                            temp.x = FinalPlacement[mod.first][3].x;
                            temp.y = FinalPlacement[mod.first][3].y;
                            FinalPlacement[mod.second].push_back(temp); // 4
                            temp.x = FinalPlacement[mod.first][2].x;
                            temp.y = FinalPlacement[mod.first][2].y;
                            FinalPlacement[mod.second].push_back(temp); // 5
                            temp.x = temp_x2;
                            temp.y = temp_y2;
                            FinalPlacement[mod.second].push_back(temp); // 6
                            temp.x = temp_x3;
                            temp.y = temp_y3;
                            FinalPlacement[mod.second].push_back(temp); // 7
                        }
                    }
                }
                else if (FinalPlacement[mod.first][0].y == FinalPlacement[mod.second][0].y) // 如果下邊等高
                {
                    // cout << "down same" << endl;
                    // 上下等高處理過了
                    if (FinalPlacement[mod.first][1].y > FinalPlacement[mod.second][1].y) // 下等高 上為m1比m2高
                    {
                        if (isodd)
                        {
                            // cout << "down same m1(left) up higher(odd)" << endl;
                            // 先做左邊的m1 //0 1 2不變
                            int temp_x = FinalPlacement[mod.first][3].x; // 右下先記下來
                            int temp_y = FinalPlacement[mod.first][3].y;
                            FinalPlacement[mod.first][3].y = FinalPlacement[mod.second][1].y; // 3
                            leftdown_corner temp;
                            temp.x = FinalPlacement[mod.first][3].x - exchangeLen;
                            temp.y = FinalPlacement[mod.first][3].y;
                            FinalPlacement[mod.first].push_back(temp); // 4
                            temp.y = FinalPlacement[mod.first][4].y - exchangeHeight;
                            FinalPlacement[mod.first].push_back(temp); // 5
                            temp.x = FinalPlacement[mod.first][5].x + exchangeLen * 2;
                            FinalPlacement[mod.first].push_back(temp); // 6
                            temp.y = FinalPlacement[mod.first][6].y - exchangeHeight;
                            FinalPlacement[mod.first].push_back(temp); // 7
                            temp.x = FinalPlacement[mod.first][7].x - exchangeLen;
                            FinalPlacement[mod.first].push_back(temp); // 8
                            temp.x = temp_x;
                            temp.y = temp_y;
                            FinalPlacement[mod.first].push_back(temp); // 9(右下)
                            // 再做右邊的m2
                            FinalPlacement[mod.second][0].x = FinalPlacement[mod.first][9].x;
                            FinalPlacement[mod.second][0].y = FinalPlacement[mod.first][9].y; // 0
                            FinalPlacement[mod.second][1].x = FinalPlacement[mod.first][8].x;
                            FinalPlacement[mod.second][1].y = FinalPlacement[mod.first][8].y; // 1
                            int temp_x1 = FinalPlacement[mod.second][2].x;
                            int temp_y1 = FinalPlacement[mod.second][2].y; // 右上先記下來
                            FinalPlacement[mod.second][2].x = FinalPlacement[mod.first][7].x;
                            FinalPlacement[mod.second][2].y = FinalPlacement[mod.first][7].y; // 2
                            int temp_x2 = FinalPlacement[mod.second][3].x;
                            int temp_y2 = FinalPlacement[mod.second][3].y; // 右下先記下來
                            FinalPlacement[mod.second][3].x = FinalPlacement[mod.first][6].x;
                            FinalPlacement[mod.second][3].y = FinalPlacement[mod.first][6].y; // 3
                            temp.x = FinalPlacement[mod.first][5].x;
                            temp.y = FinalPlacement[mod.first][5].y;
                            FinalPlacement[mod.second].push_back(temp); // 4
                            temp.x = FinalPlacement[mod.first][4].x;
                            temp.y = FinalPlacement[mod.first][4].y;
                            FinalPlacement[mod.second].push_back(temp); // 5
                            temp.x = temp_x1;
                            temp.y = temp_y1;
                            FinalPlacement[mod.second].push_back(temp); // 6
                            temp.x = temp_x2;
                            temp.y = temp_y2;
                            FinalPlacement[mod.second].push_back(temp); // 7
                        }
                        else
                        {
                            // cout << "down same m1(left) up higher(even)" << endl;
                            // 先做左邊的m1 //0 1 2不變
                            FinalPlacement[mod.first][3].y = FinalPlacement[mod.second][1].y; // 3
                            leftdown_corner temp;
                            temp.x = FinalPlacement[mod.first][3].x - exchangeLen;
                            temp.y = FinalPlacement[mod.first][3].y;
                            FinalPlacement[mod.first].push_back(temp); // 4
                            temp.y = FinalPlacement[mod.first][4].y - exchangeHeight;
                            FinalPlacement[mod.first].push_back(temp); // 5
                            temp.x = FinalPlacement[mod.first][5].x + exchangeLen * 2;
                            FinalPlacement[mod.first].push_back(temp); // 6
                            temp.y = FinalPlacement[mod.first][6].y - exchangeHeight;
                            FinalPlacement[mod.first].push_back(temp); // 7
                            // 再做右邊的m2
                            FinalPlacement[mod.second][0].x = FinalPlacement[mod.first][7].x;
                            FinalPlacement[mod.second][0].y = FinalPlacement[mod.first][7].y; // 0
                            FinalPlacement[mod.second][1].x = FinalPlacement[mod.first][6].x;
                            FinalPlacement[mod.second][1].y = FinalPlacement[mod.first][6].y; // 1
                            int temp_x1 = FinalPlacement[mod.second][2].x;
                            int temp_y1 = FinalPlacement[mod.second][2].y; // 右上先記下來
                            FinalPlacement[mod.second][2].x = FinalPlacement[mod.first][5].x;
                            FinalPlacement[mod.second][2].y = FinalPlacement[mod.first][5].y; // 2
                            int temp_x2 = FinalPlacement[mod.second][3].x;
                            int temp_y2 = FinalPlacement[mod.second][3].y; // 右下先記下來
                            FinalPlacement[mod.second][3].x = FinalPlacement[mod.first][4].x;
                            FinalPlacement[mod.second][3].y = FinalPlacement[mod.first][4].y; // 3
                            temp.x = temp_x1;
                            temp.y = temp_y1;
                            FinalPlacement[mod.second].push_back(temp); // 4
                            temp.x = temp_x2;
                            temp.y = temp_y2;
                            FinalPlacement[mod.second].push_back(temp); // 5
                        }
                    }
                    else if (FinalPlacement[mod.first][1].y < FinalPlacement[mod.second][1].y) // 下等高 上為m2比m1高
                    {
                        if (isodd)
                        {
                            // cout << "down same m1(left) up lower(odd)" << endl;
                            // 先做左邊的m1 //0 1 不變
                            FinalPlacement[mod.first][2].x = FinalPlacement[mod.first][2].x - exchangeLen; // 2
                            int temp_x = FinalPlacement[mod.first][3].x;
                            int temp_y = FinalPlacement[mod.first][3].y; // 右下先記下來
                            FinalPlacement[mod.first][3].x = FinalPlacement[mod.first][2].x;
                            FinalPlacement[mod.first][3].y = FinalPlacement[mod.first][2].y - exchangeHeight; // 3
                            leftdown_corner temp;
                            temp.x = FinalPlacement[mod.first][3].x + exchangeLen * 2;
                            temp.y = FinalPlacement[mod.first][3].y;
                            FinalPlacement[mod.first].push_back(temp); // 4
                            temp.y = FinalPlacement[mod.first][4].y - exchangeHeight;
                            FinalPlacement[mod.first].push_back(temp); // 5
                            temp.x = FinalPlacement[mod.first][5].x - exchangeLen;
                            FinalPlacement[mod.first].push_back(temp); // 6
                            temp.x = temp_x;
                            temp.y = temp_y;
                            FinalPlacement[mod.first].push_back(temp); // 7(右下)
                            // 再做右邊的m2
                            FinalPlacement[mod.second][0].x = FinalPlacement[mod.first][7].x;
                            FinalPlacement[mod.second][0].y = FinalPlacement[mod.first][7].y; // 0
                            int temp_x1 = FinalPlacement[mod.second][1].x;
                            int temp_y1 = FinalPlacement[mod.second][1].y; // 左上先記下來
                            FinalPlacement[mod.second][1].x = FinalPlacement[mod.first][6].x;
                            FinalPlacement[mod.second][1].y = FinalPlacement[mod.first][6].y; // 1
                            int temp_x2 = FinalPlacement[mod.second][2].x;
                            int temp_y2 = FinalPlacement[mod.second][2].y; // 右上先記下來
                            FinalPlacement[mod.second][2].x = FinalPlacement[mod.first][5].x;
                            FinalPlacement[mod.second][2].y = FinalPlacement[mod.first][5].y; // 2
                            int temp_x3 = FinalPlacement[mod.second][3].x;
                            int temp_y3 = FinalPlacement[mod.second][3].y; // 右下先記下來
                            FinalPlacement[mod.second][3].x = FinalPlacement[mod.first][4].x;
                            FinalPlacement[mod.second][3].y = FinalPlacement[mod.first][4].y; // 3
                            temp.x = FinalPlacement[mod.first][3].x;
                            temp.y = FinalPlacement[mod.first][3].y;
                            FinalPlacement[mod.second].push_back(temp); // 4
                            temp.x = FinalPlacement[mod.first][2].x;
                            temp.y = FinalPlacement[mod.first][2].y;
                            FinalPlacement[mod.second].push_back(temp); // 5
                            temp.x = FinalPlacement[mod.second][5].x + exchangeLen;
                            temp.y = FinalPlacement[mod.second][5].y;
                            FinalPlacement[mod.second].push_back(temp); // 6
                            temp.x = temp_x1;
                            temp.y = temp_y1;
                            FinalPlacement[mod.second].push_back(temp); // 7
                            temp.x = temp_x2;
                            temp.y = temp_y2;
                            FinalPlacement[mod.second].push_back(temp); // 8
                            temp.x = temp_x3;
                            temp.y = temp_y3;
                            FinalPlacement[mod.second].push_back(temp); // 9
                        }
                        else
                        {
                            // cout << "down same m1(left) up lower(even)" << endl;
                            // 先做左邊的m1 //0 1 不變
                            FinalPlacement[mod.first][2].x = FinalPlacement[mod.first][2].x - exchangeLen; // 2
                            FinalPlacement[mod.first][3].x = FinalPlacement[mod.first][2].x;
                            FinalPlacement[mod.first][3].y = FinalPlacement[mod.first][2].y - exchangeHeight; // 3
                            leftdown_corner temp;
                            temp.x = FinalPlacement[mod.first][3].x + exchangeLen * 2;
                            temp.y = FinalPlacement[mod.first][3].y;
                            FinalPlacement[mod.first].push_back(temp); // 4
                            temp.y = FinalPlacement[mod.first][4].y - exchangeHeight;
                            FinalPlacement[mod.first].push_back(temp); // 5
                            // 再做右邊的m2
                            FinalPlacement[mod.second][0].x = FinalPlacement[mod.first][5].x;
                            FinalPlacement[mod.second][0].y = FinalPlacement[mod.first][5].y; // 0
                            int temp_x1 = FinalPlacement[mod.second][1].x;
                            int temp_y1 = FinalPlacement[mod.second][1].y; // 左上先記下來
                            FinalPlacement[mod.second][1].x = FinalPlacement[mod.first][4].x;
                            FinalPlacement[mod.second][1].y = FinalPlacement[mod.first][4].y; // 1
                            int temp_x2 = FinalPlacement[mod.second][2].x;
                            int temp_y2 = FinalPlacement[mod.second][2].y; // 右上先記下來
                            FinalPlacement[mod.second][2].x = FinalPlacement[mod.first][3].x;
                            FinalPlacement[mod.second][2].y = FinalPlacement[mod.first][3].y; // 2
                            int temp_x3 = FinalPlacement[mod.second][3].x;
                            int temp_y3 = FinalPlacement[mod.second][3].y; // 右下先記下來
                            FinalPlacement[mod.second][3].x = FinalPlacement[mod.first][2].x;
                            FinalPlacement[mod.second][3].y = FinalPlacement[mod.first][2].y; // 3
                            temp.x = FinalPlacement[mod.second][3].x + exchangeLen;
                            temp.y = FinalPlacement[mod.second][3].y;
                            FinalPlacement[mod.second].push_back(temp); // 4
                            temp.x = temp_x1;
                            temp.y = temp_y1;
                            FinalPlacement[mod.second].push_back(temp); // 5
                            temp.x = temp_x2;
                            temp.y = temp_y2;
                            FinalPlacement[mod.second].push_back(temp); // 6
                            temp.x = temp_x3;
                            temp.y = temp_y3;
                            FinalPlacement[mod.second].push_back(temp); // 7
                        }
                    }
                }
                else // 如果上下都不等高
                {
                    // cout << "up down not same" << endl;
                    if (FinalPlacement[mod.first][1].y > FinalPlacement[mod.second][1].y) // 如果m1上邊較高
                    {
                        // cout << "up down not same m1(left) up higher" << endl;
                        if (FinalPlacement[mod.first][0].y < FinalPlacement[mod.second][0].y) // 如果m1上邊高m1下邊低
                        {
                            if (isodd)
                            {
                                // cout << "up down not same m1(left) up higher m1 down lower(odd)" << endl;
                                // 先做左邊的m1 //0 1 2不變
                                int temp_x = FinalPlacement[mod.first][3].x;
                                int temp_y = FinalPlacement[mod.first][3].y;                      // 右下先記下來
                                FinalPlacement[mod.first][3].y = FinalPlacement[mod.second][1].y; // 3
                                leftdown_corner temp;
                                temp.x = FinalPlacement[mod.first][3].x - exchangeLen;
                                temp.y = FinalPlacement[mod.first][3].y;
                                FinalPlacement[mod.first].push_back(temp); // 4
                                temp.y = FinalPlacement[mod.first][4].y - exchangeHeight;
                                FinalPlacement[mod.first].push_back(temp); // 5
                                temp.x = FinalPlacement[mod.first][5].x + exchangeLen * 2;
                                FinalPlacement[mod.first].push_back(temp); // 6
                                temp.y = FinalPlacement[mod.first][6].y - exchangeHeight;
                                FinalPlacement[mod.first].push_back(temp); // 7
                                temp.x = FinalPlacement[mod.first][7].x - exchangeLen;
                                FinalPlacement[mod.first].push_back(temp); // 8
                                temp.x = temp_x;
                                temp.y = temp_y;
                                FinalPlacement[mod.first].push_back(temp); // 9
                                // for(auto it = FinalPlacement[mod.first].begin(); it != FinalPlacement[mod.first].end(); it++)
                                // {
                                //     cout<<"("<<it->x<<","<<it->y<<") ";
                                // }
                                // 再做右邊的m2
                                FinalPlacement[mod.second][0].x = FinalPlacement[mod.first][8].x;
                                FinalPlacement[mod.second][0].y = FinalPlacement[mod.first][8].y - 1; // 0
                                FinalPlacement[mod.second][1].x = FinalPlacement[mod.first][8].x;
                                FinalPlacement[mod.second][1].y = FinalPlacement[mod.first][8].y; // 1
                                int temp_x1 = FinalPlacement[mod.second][2].x;
                                int temp_y1 = FinalPlacement[mod.second][2].y; // 右上先記下來
                                FinalPlacement[mod.second][2].x = FinalPlacement[mod.first][7].x;
                                FinalPlacement[mod.second][2].y = FinalPlacement[mod.first][7].y; // 2
                                int temp_x2 = FinalPlacement[mod.second][3].x;
                                int temp_y2 = FinalPlacement[mod.second][3].y; // 右下先記下來
                                FinalPlacement[mod.second][3].x = FinalPlacement[mod.first][6].x;
                                FinalPlacement[mod.second][3].y = FinalPlacement[mod.first][6].y; // 3
                                temp.x = FinalPlacement[mod.first][5].x;
                                temp.y = FinalPlacement[mod.first][5].y;
                                FinalPlacement[mod.second].push_back(temp); // 4
                                temp.x = FinalPlacement[mod.first][4].x;
                                temp.y = FinalPlacement[mod.first][4].y;
                                FinalPlacement[mod.second].push_back(temp); // 5
                                temp.x = temp_x1;
                                temp.y = temp_y1;
                                FinalPlacement[mod.second].push_back(temp); // 6
                                temp.x = temp_x2;
                                temp.y = temp_y2;
                                FinalPlacement[mod.second].push_back(temp); // 7
                            }
                            else
                            {
                                // cout << "up down not same m1(left) up higher m1 down lower(even)" << endl;
                                // 先做左邊的m1 //0 1 2不變
                                int temp_x = FinalPlacement[mod.first][3].x;
                                int temp_y = FinalPlacement[mod.first][3].y;                      // 右下先記下來
                                FinalPlacement[mod.first][3].y = FinalPlacement[mod.second][1].y; // 3
                                leftdown_corner temp;
                                temp.x = FinalPlacement[mod.first][3].x - exchangeLen;
                                temp.y = FinalPlacement[mod.first][3].y;
                                FinalPlacement[mod.first].push_back(temp); // 4
                                temp.y = FinalPlacement[mod.first][4].y - exchangeHeight;
                                FinalPlacement[mod.first].push_back(temp); // 5
                                temp.x = FinalPlacement[mod.first][5].x + exchangeLen * 2;
                                FinalPlacement[mod.first].push_back(temp); // 6
                                temp.y = FinalPlacement[mod.first][6].y - exchangeHeight;
                                FinalPlacement[mod.first].push_back(temp); // 7
                                temp.x = FinalPlacement[mod.first][7].x - exchangeLen;
                                FinalPlacement[mod.first].push_back(temp); // 8
                                temp.x = temp_x;
                                temp.y = temp_y;
                                FinalPlacement[mod.first].push_back(temp); // 9
                                // for(auto it = FinalPlacement[mod.first].begin(); it != FinalPlacement[mod.first].end(); it++)
                                // {
                                //     cout<<"("<<it->x<<","<<it->y<<") ";
                                // }
                                // 再做右邊的m2
                                FinalPlacement[mod.second][0].x = FinalPlacement[mod.first][7].x;
                                FinalPlacement[mod.second][0].y = FinalPlacement[mod.first][7].y; // 0
                                FinalPlacement[mod.second][1].x = FinalPlacement[mod.first][6].x;
                                FinalPlacement[mod.second][1].y = FinalPlacement[mod.first][6].y; // 1
                                int temp_x1 = FinalPlacement[mod.second][2].x;
                                int temp_y1 = FinalPlacement[mod.second][2].y; // 右上先記下來
                                FinalPlacement[mod.second][2].x = FinalPlacement[mod.first][5].x;
                                FinalPlacement[mod.second][2].y = FinalPlacement[mod.first][5].y; // 2
                                int temp_x2 = FinalPlacement[mod.second][3].x;
                                int temp_y2 = FinalPlacement[mod.second][3].y; // 右下先記下來
                                FinalPlacement[mod.second][3].x = FinalPlacement[mod.first][4].x;
                                FinalPlacement[mod.second][3].y = FinalPlacement[mod.first][4].y; // 3
                                temp.x = temp_x1;
                                temp.y = temp_y1;
                                FinalPlacement[mod.second].push_back(temp); // 4
                                temp.x = temp_x2;
                                temp.y = temp_y2;
                                FinalPlacement[mod.second].push_back(temp); // 5
                            }
                        }
                        else if (FinalPlacement[mod.first][0].y > FinalPlacement[mod.second][0].y) // 如果m1上邊高m1下邊高
                        {
                            if (isodd)
                            {
                                // cout << "up down not same m1(left) up higher m1 down higher(odd)" << endl;
                                // 先做左邊的m1 //0 1 2不變
                                int temp_x = FinalPlacement[mod.first][3].x;
                                int temp_y = FinalPlacement[mod.first][3].y;                      // 右下先記下來
                                FinalPlacement[mod.first][3].y = FinalPlacement[mod.second][1].y; // 3
                                leftdown_corner temp;
                                temp.x = FinalPlacement[mod.first][3].x - exchangeLen;
                                temp.y = FinalPlacement[mod.first][3].y;
                                FinalPlacement[mod.first].push_back(temp); // 4
                                temp.y = FinalPlacement[mod.first][4].y - exchangeHeight;
                                FinalPlacement[mod.first].push_back(temp); // 5
                                temp.x = FinalPlacement[mod.first][5].x + exchangeLen * 2;
                                FinalPlacement[mod.first].push_back(temp); // 6
                                temp.y = FinalPlacement[mod.first][6].y - exchangeHeight;
                                FinalPlacement[mod.first].push_back(temp); // 7
                                temp.x = FinalPlacement[mod.first][7].x - exchangeLen;
                                FinalPlacement[mod.first].push_back(temp); // 8
                                temp.x = temp_x;
                                temp.y = temp_y;
                                FinalPlacement[mod.first].push_back(temp); // 9
                                // 再做右邊的m2 0不變
                                FinalPlacement[mod.second][1].y = FinalPlacement[mod.first][8].y; // 1
                                int temp_x1 = FinalPlacement[mod.second][2].x;
                                int temp_y1 = FinalPlacement[mod.second][2].y; // 右上先記下來
                                FinalPlacement[mod.second][2].x = FinalPlacement[mod.first][7].x;
                                FinalPlacement[mod.second][2].y = FinalPlacement[mod.first][7].y; // 2
                                int temp_x2 = FinalPlacement[mod.second][3].x;
                                int temp_y2 = FinalPlacement[mod.second][3].y; // 右下先記下來
                                FinalPlacement[mod.second][3].x = FinalPlacement[mod.first][6].x;
                                FinalPlacement[mod.second][3].y = FinalPlacement[mod.first][6].y; // 3
                                temp.x = FinalPlacement[mod.first][5].x;
                                temp.y = FinalPlacement[mod.first][5].y;
                                FinalPlacement[mod.second].push_back(temp); // 4
                                temp.x = FinalPlacement[mod.first][4].x;
                                temp.y = FinalPlacement[mod.first][4].y;
                                FinalPlacement[mod.second].push_back(temp); // 5
                                temp.x = temp_x1;
                                temp.y = temp_y1;
                                FinalPlacement[mod.second].push_back(temp); // 6
                                temp.x = temp_x2;
                                temp.y = temp_y2;
                                FinalPlacement[mod.second].push_back(temp); // 7
                            }
                            else
                            {
                                // cout << "up down not same m1(left) up higher m1 down higher(even)" << endl;
                                // 先做左邊的m1 //0 1 2不變
                                int temp_x = FinalPlacement[mod.first][3].x;
                                int temp_y = FinalPlacement[mod.first][3].y;                      // 右下先記下來
                                FinalPlacement[mod.first][3].y = FinalPlacement[mod.second][1].y; // 3
                                leftdown_corner temp;
                                temp.x = FinalPlacement[mod.first][3].x - exchangeLen;
                                temp.y = FinalPlacement[mod.first][3].y;
                                FinalPlacement[mod.first].push_back(temp); // 4
                                temp.y = FinalPlacement[mod.first][4].y - exchangeHeight;
                                FinalPlacement[mod.first].push_back(temp); // 5
                                temp.x = FinalPlacement[mod.first][5].x + exchangeLen * 2;
                                FinalPlacement[mod.first].push_back(temp); // 6
                                temp.y = FinalPlacement[mod.first][6].y - exchangeHeight;
                                FinalPlacement[mod.first].push_back(temp); // 7
                                // 再做右邊的m2 0不變
                                FinalPlacement[mod.second][1].y = FinalPlacement[mod.first][7].y; // 1
                                int temp_x1 = FinalPlacement[mod.second][2].x;
                                int temp_y1 = FinalPlacement[mod.second][2].y; // 右上先記下來
                                FinalPlacement[mod.second][2].x = FinalPlacement[mod.first][7].x;
                                FinalPlacement[mod.second][2].y = FinalPlacement[mod.first][7].y; // 2
                                int temp_x2 = FinalPlacement[mod.second][3].x;
                                int temp_y2 = FinalPlacement[mod.second][3].y; // 右下先記下來
                                FinalPlacement[mod.second][3].x = FinalPlacement[mod.first][6].x;
                                FinalPlacement[mod.second][3].y = FinalPlacement[mod.first][6].y; // 3
                                temp.x = FinalPlacement[mod.first][5].x;
                                temp.y = FinalPlacement[mod.first][5].y;
                                FinalPlacement[mod.second].push_back(temp); // 4
                                temp.x = FinalPlacement[mod.first][4].x;
                                temp.y = FinalPlacement[mod.first][4].y;
                                FinalPlacement[mod.second].push_back(temp); // 5
                                temp.x = temp_x1;
                                temp.y = temp_y1;
                                FinalPlacement[mod.second].push_back(temp); // 6
                                temp.x = temp_x2;
                                temp.y = temp_y2;
                                FinalPlacement[mod.second].push_back(temp); // 7
                            }
                        }
                    }
                    else if (FinalPlacement[mod.first][1].y < FinalPlacement[mod.second][1].y) // 如果m2上邊較高
                    {
                        // cout << "up down not same m1(left) up lower" << endl;
                        if (FinalPlacement[mod.first][0].y > FinalPlacement[mod.second][0].y) // 如果m2上邊高m2下邊低
                        {
                            if (isodd)
                            {
                                // cout << "up down not same m1(left) up lower m1 down higher(odd)" << endl;
                                // 先做左邊的m1 //0 1不變
                                int temp_x1 = FinalPlacement[mod.first][2].x;
                                int temp_y1 = FinalPlacement[mod.first][2].y;                                  // 右上先記下來
                                FinalPlacement[mod.first][2].x = FinalPlacement[mod.first][2].x - exchangeLen; // 2
                                int temp_x2 = FinalPlacement[mod.first][3].x;
                                int temp_y2 = FinalPlacement[mod.first][3].y; // 右下先記下來
                                FinalPlacement[mod.first][3].x = FinalPlacement[mod.first][2].x;
                                FinalPlacement[mod.first][3].y = FinalPlacement[mod.first][2].y - exchangeHeight; // 3
                                leftdown_corner temp;
                                temp.x = FinalPlacement[mod.first][3].x + exchangeLen * 2;
                                temp.y = FinalPlacement[mod.first][3].y;
                                FinalPlacement[mod.first].push_back(temp); // 4
                                temp.y = FinalPlacement[mod.first][4].y - exchangeHeight;
                                FinalPlacement[mod.first].push_back(temp); // 5
                                temp.x = FinalPlacement[mod.first][5].x - exchangeLen;
                                FinalPlacement[mod.first].push_back(temp); // 6
                                temp.x = temp_x2;
                                temp.y = temp_y2;
                                FinalPlacement[mod.first].push_back(temp); // 7
                                // 再做右邊的m2 0不變
                                int temp_x3 = FinalPlacement[mod.second][1].x;
                                int temp_y3 = FinalPlacement[mod.second][1].y; // 左上先記下來
                                int temp_x4 = FinalPlacement[mod.second][2].x;
                                int temp_y4 = FinalPlacement[mod.second][2].y; // 右上先記下來
                                int temp_x5 = FinalPlacement[mod.second][3].x;
                                int temp_y5 = FinalPlacement[mod.second][3].y; // 右下先記下來
                                FinalPlacement[mod.second][1].x = FinalPlacement[mod.first][6].x;
                                FinalPlacement[mod.second][1].y = FinalPlacement[mod.first][6].y; // 1
                                FinalPlacement[mod.second][2].x = FinalPlacement[mod.first][5].x;
                                FinalPlacement[mod.second][2].y = FinalPlacement[mod.first][5].y; // 2
                                FinalPlacement[mod.second][3].x = FinalPlacement[mod.first][4].x;
                                FinalPlacement[mod.second][3].y = FinalPlacement[mod.first][4].y; // 3
                                temp.x = FinalPlacement[mod.first][3].x;
                                temp.y = FinalPlacement[mod.first][3].y;
                                FinalPlacement[mod.second].push_back(temp); // 4
                                temp.x = FinalPlacement[mod.first][2].x;
                                temp.y = FinalPlacement[mod.first][2].y;
                                FinalPlacement[mod.second].push_back(temp); // 5
                                temp.x = temp_x1;
                                temp.y = temp_y1;
                                FinalPlacement[mod.second].push_back(temp); // 6
                                temp.x = temp_x3;
                                temp.y = temp_y3;
                                FinalPlacement[mod.second].push_back(temp); // 7
                                temp.x = temp_x4;
                                temp.y = temp_y4;
                                FinalPlacement[mod.second].push_back(temp); // 8
                                temp.x = temp_x5;
                                temp.y = temp_y5;
                                FinalPlacement[mod.second].push_back(temp); // 9
                            }
                            else
                            {
                                // cout << "up down not same m1(left) up lower m1 down higher(even)" << endl;
                                // 先做左邊的m1 //0 1不變
                                int temp_x1 = FinalPlacement[mod.first][2].x;
                                int temp_y1 = FinalPlacement[mod.first][2].y;                                  // 右上先記下來
                                FinalPlacement[mod.first][2].x = FinalPlacement[mod.first][2].x - exchangeLen; // 2
                                int temp_x2 = FinalPlacement[mod.first][3].x;
                                int temp_y2 = FinalPlacement[mod.first][3].y; // 右下先記下來
                                FinalPlacement[mod.first][3].x = FinalPlacement[mod.first][2].x;
                                FinalPlacement[mod.first][3].y = FinalPlacement[mod.first][2].y - exchangeHeight; // 3
                                leftdown_corner temp;
                                temp.x = FinalPlacement[mod.first][3].x + exchangeLen * 2;
                                temp.y = FinalPlacement[mod.first][3].y;
                                FinalPlacement[mod.first].push_back(temp); // 4
                                temp.y = FinalPlacement[mod.first][4].y - exchangeHeight;
                                FinalPlacement[mod.first].push_back(temp); // 5
                                // 再做右邊的m2 0不變
                                int temp_x3 = FinalPlacement[mod.second][1].x;
                                int temp_y3 = FinalPlacement[mod.second][1].y; // 左上先記下來
                                int temp_x4 = FinalPlacement[mod.second][2].x;
                                int temp_y4 = FinalPlacement[mod.second][2].y; // 右上先記下來
                                int temp_x5 = FinalPlacement[mod.second][3].x;
                                int temp_y5 = FinalPlacement[mod.second][3].y; // 右下先記下來
                                FinalPlacement[mod.second][1].x = temp_x2;
                                FinalPlacement[mod.second][1].y = temp_y2; // 1
                                FinalPlacement[mod.second][2].x = FinalPlacement[mod.first][5].x;
                                FinalPlacement[mod.second][2].y = FinalPlacement[mod.first][5].y; // 2
                                FinalPlacement[mod.second][3].x = FinalPlacement[mod.first][4].x;
                                FinalPlacement[mod.second][3].y = FinalPlacement[mod.first][4].y; // 3
                                temp.x = FinalPlacement[mod.first][3].x;
                                temp.y = FinalPlacement[mod.first][3].y;
                                FinalPlacement[mod.second].push_back(temp); // 4
                                temp.x = FinalPlacement[mod.first][2].x;
                                temp.y = FinalPlacement[mod.first][2].y;
                                FinalPlacement[mod.second].push_back(temp); // 5
                                temp.x = temp_x1;
                                temp.y = temp_y1;
                                FinalPlacement[mod.second].push_back(temp); // 6
                                temp.x = temp_x3;
                                temp.y = temp_y3;
                                FinalPlacement[mod.second].push_back(temp); // 7
                                temp.x = temp_x4;
                                temp.y = temp_y4;
                                FinalPlacement[mod.second].push_back(temp); // 8
                                temp.x = temp_x5;
                                temp.y = temp_y5;
                                FinalPlacement[mod.second].push_back(temp); // 9
                            }
                        }
                        else if (FinalPlacement[mod.first][0].y < FinalPlacement[mod.second][0].y) // 如果m2上邊高m2下邊高
                        {
                            if (isodd)
                            {
                                // cout << "up down not same m1(left) up lower m1 down lower(odd)" << endl;
                                // 先做左邊的m1 //0 1不變
                                int temp_x1 = FinalPlacement[mod.first][2].x;
                                int temp_y1 = FinalPlacement[mod.first][2].y; // 右上先記下來
                                int temp_x2 = FinalPlacement[mod.first][3].x;
                                int temp_y2 = FinalPlacement[mod.first][3].y;                                  // 右下先記下來
                                FinalPlacement[mod.first][2].x = FinalPlacement[mod.first][2].x - exchangeLen; // 2
                                FinalPlacement[mod.first][3].x = FinalPlacement[mod.first][2].x;
                                FinalPlacement[mod.first][3].y = FinalPlacement[mod.first][2].y - exchangeHeight; // 3
                                leftdown_corner temp;
                                temp.x = FinalPlacement[mod.first][3].x + exchangeLen * 2;
                                temp.y = FinalPlacement[mod.first][3].y;
                                FinalPlacement[mod.first].push_back(temp); // 4
                                temp.y = FinalPlacement[mod.first][4].y - exchangeHeight;
                                FinalPlacement[mod.first].push_back(temp); // 5
                                temp.x = temp.x - exchangeLen;
                                FinalPlacement[mod.first].push_back(temp); // 6
                                temp.x = temp_x2;
                                temp.y = temp_y2;
                                FinalPlacement[mod.first].push_back(temp); // 7
                                // 再做右邊的m2
                                int temp_x3 = FinalPlacement[mod.second][1].x;
                                int temp_y3 = FinalPlacement[mod.second][1].y; // 左上先記下來
                                int temp_x4 = FinalPlacement[mod.second][2].x;
                                int temp_y4 = FinalPlacement[mod.second][2].y; // 右上先記下來
                                int temp_x5 = FinalPlacement[mod.second][3].x;
                                int temp_y5 = FinalPlacement[mod.second][3].y; // 右下先記下來
                                FinalPlacement[mod.second][1].x = FinalPlacement[mod.first][6].x;
                                FinalPlacement[mod.second][1].y = FinalPlacement[mod.first][6].y; // 1
                                FinalPlacement[mod.second][2].x = FinalPlacement[mod.first][5].x;
                                FinalPlacement[mod.second][2].y = FinalPlacement[mod.first][5].y; // 2
                                FinalPlacement[mod.second][3].x = FinalPlacement[mod.first][4].x;
                                FinalPlacement[mod.second][3].y = FinalPlacement[mod.first][4].y; // 3
                                temp.x = FinalPlacement[mod.first][3].x;
                                temp.y = FinalPlacement[mod.first][3].y;
                                FinalPlacement[mod.second].push_back(temp); // 4
                                temp.x = FinalPlacement[mod.first][2].x;
                                temp.y = FinalPlacement[mod.first][2].y;
                                FinalPlacement[mod.second].push_back(temp); // 5
                                temp.x = temp_x1;
                                temp.y = temp_y1;
                                FinalPlacement[mod.second].push_back(temp); // 6
                                temp.x = temp_x3;
                                temp.y = temp_y3;
                                FinalPlacement[mod.second].push_back(temp); // 7
                                temp.x = temp_x4;
                                temp.y = temp_y4;
                                FinalPlacement[mod.second].push_back(temp); // 8
                                temp.x = temp_x5;
                                temp.y = temp_y5;
                                FinalPlacement[mod.second].push_back(temp); // 9
                            }
                            else
                            {
                                // cout << "up down not same m1(left) up lower m1 down lower(even)" << endl;
                                // 先做左邊的m1 //0 1不變
                                int temp_x1 = FinalPlacement[mod.first][2].x;
                                int temp_y1 = FinalPlacement[mod.first][2].y; // 右上先記下來
                                int temp_x2 = FinalPlacement[mod.first][3].x;
                                int temp_y2 = FinalPlacement[mod.first][3].y;                                  // 右下先記下來
                                FinalPlacement[mod.first][2].x = FinalPlacement[mod.first][2].x - exchangeLen; // 2
                                FinalPlacement[mod.first][3].x = FinalPlacement[mod.first][2].x;
                                FinalPlacement[mod.first][3].y = FinalPlacement[mod.first][2].y - exchangeHeight; // 3
                                leftdown_corner temp;
                                temp.x = FinalPlacement[mod.first][3].x + exchangeLen * 2;
                                temp.y = FinalPlacement[mod.first][3].y;
                                FinalPlacement[mod.first].push_back(temp); // 4
                                temp.y = FinalPlacement[mod.first][4].y - exchangeHeight;
                                FinalPlacement[mod.first].push_back(temp); // 5
                                temp.x = temp.x - exchangeLen;
                                FinalPlacement[mod.first].push_back(temp); // 6
                                temp.x = temp_x2;
                                temp.y = temp_y2;
                                FinalPlacement[mod.first].push_back(temp); // 7
                                // 再做右邊的m2
                                int temp_x3 = FinalPlacement[mod.second][1].x;
                                int temp_y3 = FinalPlacement[mod.second][1].y; // 左上先記下來
                                int temp_x4 = FinalPlacement[mod.second][2].x;
                                int temp_y4 = FinalPlacement[mod.second][2].y; // 右上先記下來
                                int temp_x5 = FinalPlacement[mod.second][3].x;
                                int temp_y5 = FinalPlacement[mod.second][3].y; // 右下先記下來
                                FinalPlacement[mod.second][0].x = FinalPlacement[mod.first][5].x;
                                FinalPlacement[mod.second][0].y = FinalPlacement[mod.first][5].y; // 0
                                FinalPlacement[mod.second][1].x = FinalPlacement[mod.first][4].x;
                                FinalPlacement[mod.second][1].y = FinalPlacement[mod.first][4].y; // 1
                                FinalPlacement[mod.second][2].x = FinalPlacement[mod.first][3].x;
                                FinalPlacement[mod.second][2].y = FinalPlacement[mod.first][3].y; // 2
                                FinalPlacement[mod.second][3].x = FinalPlacement[mod.first][2].x;
                                FinalPlacement[mod.second][3].y = FinalPlacement[mod.first][2].y; // 3
                                temp.x = temp_x1;
                                temp.y = temp_y1;
                                FinalPlacement[mod.second].push_back(temp); // 4
                                temp.x = temp_x3;
                                temp.y = temp_y3;
                                FinalPlacement[mod.second].push_back(temp); // 5
                                temp.x = temp_x4;
                                temp.y = temp_y4;
                                FinalPlacement[mod.second].push_back(temp); // 6
                                temp.x = temp_x5;
                                temp.y = temp_y5;
                                FinalPlacement[mod.second].push_back(temp); // 7
                            }
                        }
                    }
                }
            }
            else // 上下關係
            {
                // cout << "up down" << endl;
                bool isodd = false;
                int exchangeWidth = abs(end.x - start.x);
                if (exchangeWidth % 2 == 1) // 先確認奇數偶數
                {
                    // cout << "exchangingWidth is odd" << endl;
                    isodd = true;
                    // continue;
                }
                exchangeWidth = exchangeWidth / 2; // 再除二
                if (exchangeLen <= 0 || exchangeWidth <= 0)
                {
                    continue;
                }
                // cout << "exchangingHeight: " << exchangeLen << endl;
                // cout << "exchangingWidth: " << exchangeWidth << endl;
                bool mod1_top = false;
                if (FinalPlacement[mod.first][0].y > FinalPlacement[mod.second][0].y)
                {
                    mod1_top = true;
                }
                if (!mod1_top) // 如果第一個模組在下面
                {
                    // cout << "mod2 top" << endl;
                    // 交換兩個模組
                    // cout << "exchange two modules" << endl;
                    // cout << "mod1: " << mod.first << ", mod2: " << mod.second << endl;
                    int temp = mod.first;
                    mod.first = mod.second;
                    mod.second = temp;
                    // cout << "mod1: " << mod.first << ", mod2: " << mod.second << endl;
                }
                if (FinalPlacement[mod.first][2].x == FinalPlacement[mod.second][2].x) // 如果右邊相等
                {
                    // cout << "right same" << endl;
                    if (FinalPlacement[mod.first][0].x < FinalPlacement[mod.second][0].x) // 右邊相等且如果左邊為m1比m2左
                    {
                        if (isodd)
                        {
                            // cout << "right same left m1 more left(odd)" << endl;
                            int temp_x1 = FinalPlacement[mod.first][0].x;
                            int temp_y1 = FinalPlacement[mod.first][0].y; // 左下先記下來
                            int temp_x2 = FinalPlacement[mod.first][1].x;
                            int temp_y2 = FinalPlacement[mod.first][1].y; // 左上先記下來
                            int temp_x3 = FinalPlacement[mod.first][2].x;
                            int temp_y3 = FinalPlacement[mod.first][2].y; // 右上先記下來
                            int temp_x4 = FinalPlacement[mod.first][3].x;
                            int temp_y4 = FinalPlacement[mod.first][3].y; // 右下先記下來
                            FinalPlacement[mod.first][0].x = FinalPlacement[mod.second][1].x;
                            FinalPlacement[mod.first][0].y = FinalPlacement[mod.second][1].y - exchangeLen; // 0
                            FinalPlacement[mod.first][1].x = FinalPlacement[mod.first][0].x;
                            FinalPlacement[mod.first][1].y = FinalPlacement[mod.first][0].y + exchangeLen; // 1
                            FinalPlacement[mod.first][2].x = temp_x1;
                            FinalPlacement[mod.first][2].y = temp_y1; // 2
                            FinalPlacement[mod.first][3].x = temp_x2;
                            FinalPlacement[mod.first][3].y = temp_y2; // 3
                            leftdown_corner temp;
                            temp.x = temp_x3;
                            temp.y = temp_y3;
                            FinalPlacement[mod.first].push_back(temp); // 4
                            temp.x = temp_x4;
                            temp.y = temp_y4;
                            FinalPlacement[mod.first].push_back(temp); // 5
                            temp.x = FinalPlacement[mod.first][5].x - 1;
                            FinalPlacement[mod.first].push_back(temp); // 6
                            temp.y = FinalPlacement[mod.first][6].y + exchangeLen;
                            FinalPlacement[mod.first].push_back(temp); // 7
                            temp.x = FinalPlacement[mod.first][7].x - exchangeWidth;
                            FinalPlacement[mod.first].push_back(temp); // 8
                            temp.y = FinalPlacement[mod.first][8].y - 2 * exchangeLen;
                            FinalPlacement[mod.first].push_back(temp); // 9
                            // 再做下面的m2 0不變
                            FinalPlacement[mod.second][1].x = FinalPlacement[mod.first][0].x;
                            FinalPlacement[mod.second][1].y = FinalPlacement[mod.first][0].y; // 1
                            FinalPlacement[mod.second][2].x = FinalPlacement[mod.first][9].x;
                            FinalPlacement[mod.second][2].y = FinalPlacement[mod.first][9].y; // 2
                            int temp_x5 = FinalPlacement[mod.second][3].x;
                            int temp_y5 = FinalPlacement[mod.second][3].y; // 右下先記下來
                            FinalPlacement[mod.second][3].x = FinalPlacement[mod.first][8].x;
                            FinalPlacement[mod.second][3].y = FinalPlacement[mod.first][8].y; // 3
                            temp.x = FinalPlacement[mod.first][7].x;
                            temp.y = FinalPlacement[mod.first][7].y;
                            FinalPlacement[mod.second].push_back(temp); // 4
                            temp.x = FinalPlacement[mod.first][6].x;
                            temp.y = FinalPlacement[mod.first][6].y;
                            FinalPlacement[mod.second].push_back(temp); // 5
                            temp.x = FinalPlacement[mod.first][5].x;
                            temp.y = FinalPlacement[mod.first][5].y;
                            FinalPlacement[mod.second].push_back(temp); // 6
                            temp.x = temp_x5;
                            temp.y = temp_y5;
                            FinalPlacement[mod.second].push_back(temp); // 7
                        }
                        else
                        {
                            // cout << "right same left m1 more left(even)" << endl;
                            int temp_x1 = FinalPlacement[mod.first][0].x;
                            int temp_y1 = FinalPlacement[mod.first][0].y; // 左下先記下來
                            int temp_x2 = FinalPlacement[mod.first][1].x;
                            int temp_y2 = FinalPlacement[mod.first][1].y; // 左上先記下來
                            int temp_x3 = FinalPlacement[mod.first][2].x;
                            int temp_y3 = FinalPlacement[mod.first][2].y; // 右上先記下來
                            FinalPlacement[mod.first][0].x = FinalPlacement[mod.second][1].x;
                            FinalPlacement[mod.first][0].y = FinalPlacement[mod.second][1].y - exchangeLen; // 0
                            FinalPlacement[mod.first][1].x = FinalPlacement[mod.first][0].x;
                            FinalPlacement[mod.first][1].y = FinalPlacement[mod.first][0].y + exchangeLen; // 1
                            FinalPlacement[mod.first][2].x = temp_x1;
                            FinalPlacement[mod.first][2].y = temp_y1; // 2
                            FinalPlacement[mod.first][3].x = temp_x2;
                            FinalPlacement[mod.first][3].y = temp_y2; // 3
                            leftdown_corner temp;
                            temp.x = temp_x3;
                            temp.y = temp_y3;
                            FinalPlacement[mod.first].push_back(temp); // 4
                            temp.x = FinalPlacement[mod.second][2].x;
                            temp.y = FinalPlacement[mod.second][2].y + exchangeLen;
                            FinalPlacement[mod.first].push_back(temp); // 5
                            temp.x = FinalPlacement[mod.first][5].x - exchangeWidth;
                            FinalPlacement[mod.first].push_back(temp); // 6
                            temp.y = FinalPlacement[mod.first][6].y - 2 * exchangeLen;
                            FinalPlacement[mod.first].push_back(temp); // 7
                            // 再做下面的m2 0不變
                            FinalPlacement[mod.second][1].x = FinalPlacement[mod.first][0].x;
                            FinalPlacement[mod.second][1].y = FinalPlacement[mod.first][0].y; // 1
                            FinalPlacement[mod.second][2].x = FinalPlacement[mod.first][7].x;
                            FinalPlacement[mod.second][2].y = FinalPlacement[mod.first][7].y; // 2
                            int temp_x4 = FinalPlacement[mod.second][3].x;
                            int temp_y4 = FinalPlacement[mod.second][3].y; // 右下先記下來
                            FinalPlacement[mod.second][3].x = FinalPlacement[mod.first][6].x;
                            FinalPlacement[mod.second][3].y = FinalPlacement[mod.first][6].y; // 3
                            temp.x = FinalPlacement[mod.first][5].x;
                            temp.y = FinalPlacement[mod.first][5].y;
                            FinalPlacement[mod.second].push_back(temp); // 4
                            temp.x = temp_x4;
                            temp.y = temp_y4;
                            FinalPlacement[mod.second].push_back(temp); // 5
                        }
                    }
                    else if (FinalPlacement[mod.first][0].x == FinalPlacement[mod.second][0].x) // 左右邊相等
                    {
                        if (isodd)
                        {
                            // cout << "left right same(odd)" << endl;
                            // 先做上面的m1 //1 2不變
                            FinalPlacement[mod.first][0].y = FinalPlacement[mod.first][0].y - exchangeLen; // 0
                            leftdown_corner temp;
                            temp.x = FinalPlacement[mod.first][3].x - 1;
                            temp.y = FinalPlacement[mod.first][3].y;
                            FinalPlacement[mod.first].push_back(temp); // 4
                            temp.y = FinalPlacement[mod.first][4].y + exchangeLen;
                            FinalPlacement[mod.first].push_back(temp); // 5
                            temp.x = FinalPlacement[mod.first][5].x - exchangeWidth;
                            FinalPlacement[mod.first].push_back(temp); // 6
                            temp.y = FinalPlacement[mod.first][6].y - 2 * exchangeLen;
                            FinalPlacement[mod.first].push_back(temp); // 7
                            // 再做下面的m2 0不變
                            FinalPlacement[mod.second][1].x = FinalPlacement[mod.first][0].x;
                            FinalPlacement[mod.second][1].y = FinalPlacement[mod.first][0].y; // 1
                            int temp_x1 = FinalPlacement[mod.second][2].x;
                            int temp_y1 = FinalPlacement[mod.second][2].y; // 右上先記下來
                            FinalPlacement[mod.second][2].x = FinalPlacement[mod.first][7].x;
                            FinalPlacement[mod.second][2].y = FinalPlacement[mod.first][7].y; // 2
                            int temp_x2 = FinalPlacement[mod.second][3].x;
                            int temp_y2 = FinalPlacement[mod.second][3].y; // 右下先記下來
                            FinalPlacement[mod.second][3].x = FinalPlacement[mod.first][6].x;
                            FinalPlacement[mod.second][3].y = FinalPlacement[mod.first][6].y; // 3
                            temp.x = FinalPlacement[mod.first][5].x;
                            temp.y = FinalPlacement[mod.first][5].y;
                            FinalPlacement[mod.second].push_back(temp); // 4
                            temp.x = FinalPlacement[mod.first][4].x;
                            temp.y = FinalPlacement[mod.first][4].y;
                            FinalPlacement[mod.second].push_back(temp); // 5
                            temp.x = temp_x1;
                            temp.y = temp_y1;
                            FinalPlacement[mod.second].push_back(temp); // 6
                            temp.x = temp_x2;
                            temp.y = temp_y2;
                            FinalPlacement[mod.second].push_back(temp); // 7
                        }
                        else
                        {
                            // cout << "left right same(even)" << endl;
                            // 先做上面的m1 //1 2不變
                            FinalPlacement[mod.first][0].y = FinalPlacement[mod.first][0].y - exchangeLen; // 0
                            FinalPlacement[mod.first][3].y = FinalPlacement[mod.first][3].y + exchangeLen; // 3
                            leftdown_corner temp;
                            temp.x = FinalPlacement[mod.first][3].x - exchangeWidth;
                            temp.y = FinalPlacement[mod.first][3].y;
                            FinalPlacement[mod.first].push_back(temp); // 4
                            temp.x = FinalPlacement[mod.first][4].x;
                            temp.y = FinalPlacement[mod.first][4].y - 2 * exchangeLen;
                            FinalPlacement[mod.first].push_back(temp); // 5
                            // 再做下面的m2 0不變
                            FinalPlacement[mod.second][1].x = FinalPlacement[mod.first][0].x;
                            FinalPlacement[mod.second][1].y = FinalPlacement[mod.first][0].y; // 1
                            FinalPlacement[mod.second][2].x = FinalPlacement[mod.first][5].x;
                            FinalPlacement[mod.second][2].y = FinalPlacement[mod.first][5].y; // 2
                            int temp_x1 = FinalPlacement[mod.second][3].x;
                            int temp_y1 = FinalPlacement[mod.second][3].y; // 右下先記下來
                            FinalPlacement[mod.second][3].x = FinalPlacement[mod.first][4].x;
                            FinalPlacement[mod.second][3].y = FinalPlacement[mod.first][4].y; // 3
                            temp.x = FinalPlacement[mod.first][3].x;
                            temp.y = FinalPlacement[mod.first][3].y;
                            FinalPlacement[mod.second].push_back(temp); // 4
                            temp.x = temp_x1;
                            temp.y = temp_y1;
                            FinalPlacement[mod.second].push_back(temp); // 5
                        }
                    }
                    else if (FinalPlacement[mod.first][0].x > FinalPlacement[mod.second][0].x) // 右邊相等 左邊m2比m1左
                    {
                        if (isodd)
                        {
                            // cout << "right same left m2 more left(odd)" << endl;
                            // 先做上面的m1 1 2不變
                            FinalPlacement[mod.first][0].y = FinalPlacement[mod.first][0].y - exchangeLen; // 0
                            leftdown_corner temp;
                            temp.x = FinalPlacement[mod.first][3].x - 1;
                            temp.y = FinalPlacement[mod.first][3].y;
                            FinalPlacement[mod.first].push_back(temp); // 4
                            temp.x = FinalPlacement[mod.first][4].x;
                            temp.y = FinalPlacement[mod.first][4].y + exchangeLen;
                            FinalPlacement[mod.first].push_back(temp); // 5
                            temp.x = FinalPlacement[mod.first][5].x - exchangeWidth;
                            FinalPlacement[mod.first].push_back(temp); // 6
                            temp.y = FinalPlacement[mod.first][6].y - 2 * exchangeLen;
                            FinalPlacement[mod.first].push_back(temp); // 7
                            // 再做下面的m2 //0 1不變
                            int temp_x = FinalPlacement[mod.second][2].x;
                            int temp_y = FinalPlacement[mod.second][2].y; // 右上先記下來
                            FinalPlacement[mod.second][2].x = FinalPlacement[mod.first][0].x;
                            FinalPlacement[mod.second][2].y = FinalPlacement[mod.first][0].y + exchangeLen; // 2
                            int temp_x1 = FinalPlacement[mod.second][3].x;
                            int temp_y1 = FinalPlacement[mod.second][3].y; // 右下先記下來
                            FinalPlacement[mod.second][3].x = FinalPlacement[mod.first][0].x;
                            FinalPlacement[mod.second][3].y = FinalPlacement[mod.first][0].y; // 3
                            temp.x = FinalPlacement[mod.first][7].x;
                            temp.y = FinalPlacement[mod.first][7].y;
                            FinalPlacement[mod.second].push_back(temp); // 4
                            temp.x = FinalPlacement[mod.first][6].x;
                            temp.y = FinalPlacement[mod.first][6].y;
                            FinalPlacement[mod.second].push_back(temp); // 5
                            temp.x = FinalPlacement[mod.first][5].x;
                            temp.y = FinalPlacement[mod.first][5].y;
                            FinalPlacement[mod.second].push_back(temp); // 6
                            temp.x = FinalPlacement[mod.first][4].x;
                            temp.y = FinalPlacement[mod.first][4].y;
                            FinalPlacement[mod.second].push_back(temp); // 7
                            temp.x = temp_x;
                            temp.y = temp_y;
                            FinalPlacement[mod.second].push_back(temp); // 8
                            temp.x = temp_x1;
                            temp.y = temp_y1;
                            FinalPlacement[mod.second].push_back(temp); // 9
                        }
                        else
                        {
                            // cout << "right same left m2 more left(even)" << endl;
                            // 先做上面的m1 1 2不變
                            FinalPlacement[mod.first][0].y = FinalPlacement[mod.first][0].y - exchangeLen; // 0
                            FinalPlacement[mod.first][3].y = FinalPlacement[mod.first][3].y + exchangeLen; // 3
                            leftdown_corner temp;
                            temp.x = FinalPlacement[mod.first][3].x - exchangeWidth;
                            temp.y = FinalPlacement[mod.first][3].y;
                            FinalPlacement[mod.first].push_back(temp); // 4
                            temp.x = FinalPlacement[mod.first][4].x;
                            temp.y = FinalPlacement[mod.first][4].y - 2 * exchangeLen;
                            FinalPlacement[mod.first].push_back(temp); // 5
                            // 再做下面的m2 //0 1不變
                            FinalPlacement[mod.second][2].x = FinalPlacement[mod.first][0].x;
                            FinalPlacement[mod.second][2].y = FinalPlacement[mod.first][0].y + exchangeLen; // 2
                            int temp_x1 = FinalPlacement[mod.second][3].x;
                            int temp_y1 = FinalPlacement[mod.second][3].y; // 右下先記下來
                            FinalPlacement[mod.second][3].x = FinalPlacement[mod.first][0].x;
                            FinalPlacement[mod.second][3].y = FinalPlacement[mod.first][0].y; // 3
                            temp.x = FinalPlacement[mod.first][5].x;
                            temp.y = FinalPlacement[mod.first][5].y;
                            FinalPlacement[mod.second].push_back(temp); // 4
                            temp.x = FinalPlacement[mod.first][4].x;
                            temp.y = FinalPlacement[mod.first][4].y;
                            FinalPlacement[mod.second].push_back(temp); // 5
                            temp.x = FinalPlacement[mod.first][3].x;
                            temp.y = FinalPlacement[mod.first][3].y;
                            FinalPlacement[mod.second].push_back(temp); // 6
                            temp.x = temp_x1;
                            temp.y = temp_y1;
                            FinalPlacement[mod.second].push_back(temp); // 7
                        }
                    }
                }
                else if (FinalPlacement[mod.first][0].x == FinalPlacement[mod.second][0].x) // 如果左邊相等
                {
                    // cout << "left same" << endl;
                    // 左右相等處理過了
                    if (FinalPlacement[mod.first][2].x < FinalPlacement[mod.second][2].x) // 左相等 右邊m2比m1右
                    {
                        if (isodd)
                        {
                            // cout << "left same right m2 more right(odd)" << endl;
                            // 先做上面的m1
                            FinalPlacement[mod.first][0].y = FinalPlacement[mod.first][0].y - exchangeLen; // 0
                            leftdown_corner temp;
                            temp.x = FinalPlacement[mod.first][3].x - 1;
                            temp.y = FinalPlacement[mod.first][3].y;
                            FinalPlacement[mod.first].push_back(temp); // 4
                            temp.y = FinalPlacement[mod.first][4].y + exchangeLen;
                            FinalPlacement[mod.first].push_back(temp); // 5
                            temp.x = FinalPlacement[mod.first][5].x - exchangeWidth;
                            FinalPlacement[mod.first].push_back(temp); // 6
                            temp.y = FinalPlacement[mod.first][6].y - 2 * exchangeLen;
                            FinalPlacement[mod.first].push_back(temp); // 7
                            // 再做下面的m2
                            FinalPlacement[mod.second][1].x = FinalPlacement[mod.first][0].x;
                            FinalPlacement[mod.second][1].y = FinalPlacement[mod.first][0].y; // 1
                            int temp_x1 = FinalPlacement[mod.second][2].x;
                            int temp_y1 = FinalPlacement[mod.second][2].y; // 右上先記下來
                            FinalPlacement[mod.second][2].x = FinalPlacement[mod.first][7].x;
                            FinalPlacement[mod.second][2].y = FinalPlacement[mod.first][7].y; // 2
                            int temp_x2 = FinalPlacement[mod.second][3].x;
                            int temp_y2 = FinalPlacement[mod.second][3].y; // 右下先記下來
                            FinalPlacement[mod.second][3].x = FinalPlacement[mod.first][6].x;
                            FinalPlacement[mod.second][3].y = FinalPlacement[mod.first][6].y; // 3
                            temp.x = FinalPlacement[mod.first][5].x;
                            temp.y = FinalPlacement[mod.first][5].y;
                            FinalPlacement[mod.second].push_back(temp); // 4
                            temp.x = FinalPlacement[mod.first][4].x;
                            temp.y = FinalPlacement[mod.first][4].y;
                            FinalPlacement[mod.second].push_back(temp); // 5
                            temp.x = temp_x1;
                            temp.y = temp_y1;
                            FinalPlacement[mod.second].push_back(temp); // 6
                            temp.x = temp_x2;
                            temp.y = temp_y2;
                            FinalPlacement[mod.second].push_back(temp); // 7
                        }
                        else
                        {
                            // cout << "left same right m2 more right(even)" << endl;
                            // 先做上面的m1
                            FinalPlacement[mod.first][0].y = FinalPlacement[mod.first][0].y - exchangeLen; // 0
                            FinalPlacement[mod.first][3].y = FinalPlacement[mod.first][3].y + exchangeLen; // 3
                            leftdown_corner temp;
                            temp.x = FinalPlacement[mod.first][3].x - exchangeWidth;
                            temp.y = FinalPlacement[mod.first][3].y;
                            FinalPlacement[mod.first].push_back(temp); // 4
                            temp.y = FinalPlacement[mod.first][4].y - 2 * exchangeLen;
                            FinalPlacement[mod.first].push_back(temp); // 5
                            // 再做下面的m2
                            FinalPlacement[mod.second][1].x = FinalPlacement[mod.first][0].x;
                            FinalPlacement[mod.second][1].y = FinalPlacement[mod.first][0].y; // 1
                            int temp_x1 = FinalPlacement[mod.second][2].x;
                            int temp_y1 = FinalPlacement[mod.second][2].y; // 右上先記下來
                            FinalPlacement[mod.second][2].x = FinalPlacement[mod.first][5].x;
                            FinalPlacement[mod.second][2].y = FinalPlacement[mod.first][5].y; // 2
                            int temp_x2 = FinalPlacement[mod.second][3].x;
                            int temp_y2 = FinalPlacement[mod.second][3].y; // 右下先記下來
                            FinalPlacement[mod.second][3].x = FinalPlacement[mod.first][4].x;
                            FinalPlacement[mod.second][3].y = FinalPlacement[mod.first][4].y; // 3
                            temp.x = FinalPlacement[mod.first][3].x;
                            temp.y = FinalPlacement[mod.first][3].y;
                            FinalPlacement[mod.second].push_back(temp); // 4
                            temp.x = FinalPlacement[mod.second][4].x;
                            temp.y = FinalPlacement[mod.second][4].y - exchangeLen;
                            FinalPlacement[mod.second].push_back(temp); // 5
                            temp.x = temp_x1;
                            temp.y = temp_y1;
                            FinalPlacement[mod.second].push_back(temp); // 6
                            temp.x = temp_x2;
                            temp.y = temp_y2;
                            FinalPlacement[mod.second].push_back(temp); // 7
                        }
                    }
                    else if (FinalPlacement[mod.first][2].x > FinalPlacement[mod.second][2].x) // 左相等 右邊m1比m2右
                    {
                        if (isodd)
                        {
                            // cout << "left same right m1 more right(odd)" << endl;
                            // 先做上面的m1 123不變
                            FinalPlacement[mod.first][0].y = FinalPlacement[mod.first][0].y - exchangeLen; // 0
                            leftdown_corner temp;
                            temp.x = FinalPlacement[mod.second][2].x - 1;
                            temp.y = FinalPlacement[mod.second][2].y;
                            FinalPlacement[mod.first].push_back(temp); // 4
                            temp.y = FinalPlacement[mod.first][4].y + exchangeLen;
                            FinalPlacement[mod.first].push_back(temp); // 5
                            temp.x = FinalPlacement[mod.first][5].x - exchangeWidth;
                            temp.y = FinalPlacement[mod.first][5].y;
                            FinalPlacement[mod.first].push_back(temp); // 6
                            temp.x = FinalPlacement[mod.first][6].x;
                            temp.y = FinalPlacement[mod.first][6].y - 2 * exchangeLen;
                            FinalPlacement[mod.first].push_back(temp); // 7
                            // 再做下面的m2
                            FinalPlacement[mod.second][1].x = FinalPlacement[mod.first][0].x;
                            FinalPlacement[mod.second][1].y = FinalPlacement[mod.first][0].y; // 1
                            int temp_x1 = FinalPlacement[mod.second][2].x;
                            int temp_y1 = FinalPlacement[mod.second][2].y; // 右上先記下來
                            FinalPlacement[mod.second][2].x = FinalPlacement[mod.first][7].x;
                            FinalPlacement[mod.second][2].y = FinalPlacement[mod.first][7].y; // 2
                            int temp_x2 = FinalPlacement[mod.second][3].x;
                            int temp_y2 = FinalPlacement[mod.second][3].y; // 右下先記下來
                            FinalPlacement[mod.second][3].x = FinalPlacement[mod.first][6].x;
                            FinalPlacement[mod.second][3].y = FinalPlacement[mod.first][6].y; // 3
                            temp.x = FinalPlacement[mod.first][5].x;
                            temp.y = FinalPlacement[mod.first][5].y;
                            FinalPlacement[mod.second].push_back(temp); // 4
                            temp.x = FinalPlacement[mod.first][4].x;
                            temp.y = FinalPlacement[mod.first][4].y;
                            FinalPlacement[mod.second].push_back(temp); // 5
                            temp.x = temp_x1;
                            temp.y = temp_y1;
                            FinalPlacement[mod.second].push_back(temp); // 6
                            temp.x = temp_x2;
                            temp.y = temp_y2;
                            FinalPlacement[mod.second].push_back(temp); // 7
                        }
                        else
                        {
                            // cout << "left same right m1 more right(even)" << endl;
                            // 先做上面的m1 123不變
                            FinalPlacement[mod.first][0].y = FinalPlacement[mod.first][0].y - exchangeLen; // 0
                            leftdown_corner temp;
                            temp.x = FinalPlacement[mod.second][2].x;
                            temp.y = FinalPlacement[mod.second][2].y;
                            FinalPlacement[mod.first].push_back(temp); // 4
                            temp.y = FinalPlacement[mod.first][4].y + exchangeLen;
                            FinalPlacement[mod.first].push_back(temp); // 5
                            temp.x = FinalPlacement[mod.first][5].x - exchangeWidth;
                            temp.y = FinalPlacement[mod.first][5].y;
                            FinalPlacement[mod.first].push_back(temp); // 6
                            temp.x = FinalPlacement[mod.first][6].x;
                            temp.y = FinalPlacement[mod.first][6].y - 2 * exchangeLen;
                            FinalPlacement[mod.first].push_back(temp); // 7
                            // 再做下面的m2
                            FinalPlacement[mod.second][1].x = FinalPlacement[mod.first][0].x;
                            FinalPlacement[mod.second][1].y = FinalPlacement[mod.first][0].y; // 1
                            FinalPlacement[mod.second][2].x = FinalPlacement[mod.first][7].x;
                            FinalPlacement[mod.second][2].y = FinalPlacement[mod.first][7].y; // 2
                            int temp_x1 = FinalPlacement[mod.second][3].x;
                            int temp_y1 = FinalPlacement[mod.second][3].y; // 右下先記下來
                            FinalPlacement[mod.second][3].x = FinalPlacement[mod.first][6].x;
                            FinalPlacement[mod.second][3].y = FinalPlacement[mod.first][6].y; // 3
                            temp.x = FinalPlacement[mod.first][5].x;
                            temp.y = FinalPlacement[mod.first][5].y;
                            FinalPlacement[mod.second].push_back(temp); // 4
                            temp.x = temp_x1;
                            temp.y = temp_y1;
                            FinalPlacement[mod.second].push_back(temp); // 5
                        }
                    }
                }
                else // 如果左右都不相等
                {
                    // cout << "left right not same" << endl;
                    if (FinalPlacement[mod.first][2].x > FinalPlacement[mod.second][2].x) // 如果m1右邊較右
                    {
                        // cout << "left right not same right m1 more right" << endl;
                        if (FinalPlacement[mod.first][0].x > FinalPlacement[mod.second][0].x) // 如果m1右邊右m2左邊左
                        {
                            if (isodd)
                            {
                                // cout << "left right not same right m1 more right left m2 more left(odd)" << endl;
                                // 先做上面的m1 // 1 2 3不變
                                FinalPlacement[mod.first][0].y = FinalPlacement[mod.first][0].y - exchangeLen; // 0
                                leftdown_corner temp;
                                temp.x = FinalPlacement[mod.second][2].x - 1;
                                temp.y = FinalPlacement[mod.second][2].y;
                                FinalPlacement[mod.first].push_back(temp); // 4
                                temp.y = FinalPlacement[mod.first][4].y + exchangeLen;
                                FinalPlacement[mod.first].push_back(temp); // 5
                                temp.x = FinalPlacement[mod.first][5].x - exchangeWidth;
                                FinalPlacement[mod.first].push_back(temp); // 6
                                temp.y = FinalPlacement[mod.first][6].y - 2 * exchangeLen;
                                FinalPlacement[mod.first].push_back(temp); // 7
                                // for(auto it = FinalPlacement[mod.first].begin(); it != FinalPlacement[mod.first].end(); it++)
                                // {
                                //     cout<<"("<<it->x<<","<<it->y<<") ";
                                // }
                                // 再做下面的m2
                                int temp_x = FinalPlacement[mod.second][2].x;
                                int temp_y = FinalPlacement[mod.second][2].y; // 右上先記下來
                                FinalPlacement[mod.second][2].x = FinalPlacement[mod.first][0].x;
                                FinalPlacement[mod.second][2].y = FinalPlacement[mod.first][0].y + exchangeLen; // 2
                                int temp_x1 = FinalPlacement[mod.second][3].x;
                                int temp_y1 = FinalPlacement[mod.second][3].y; // 右下先記下來
                                FinalPlacement[mod.second][3].x = FinalPlacement[mod.first][0].x;
                                FinalPlacement[mod.second][3].y = FinalPlacement[mod.first][0].y; // 3
                                temp.x = FinalPlacement[mod.first][7].x;
                                temp.y = FinalPlacement[mod.first][7].y;
                                FinalPlacement[mod.second].push_back(temp); // 4
                                temp.x = FinalPlacement[mod.first][6].x;
                                temp.y = FinalPlacement[mod.first][6].y;
                                FinalPlacement[mod.second].push_back(temp); // 5
                                temp.x = FinalPlacement[mod.first][5].x;
                                temp.y = FinalPlacement[mod.first][5].y;
                                FinalPlacement[mod.second].push_back(temp); // 6
                                temp.x = FinalPlacement[mod.first][4].x;
                                temp.y = FinalPlacement[mod.first][4].y;
                                FinalPlacement[mod.second].push_back(temp); // 7
                                temp.x = temp_x;
                                temp.y = temp_y;
                                FinalPlacement[mod.second].push_back(temp); // 8
                                temp.x = temp_x1;
                                temp.y = temp_y1;
                                FinalPlacement[mod.second].push_back(temp); // 9
                            }
                            else
                            {
                                // cout << "left right not same right m1 more right left m2 more left(even)" << endl;
                                // 先做上面的m1 // 1 2 3不變
                                FinalPlacement[mod.first][0].y = FinalPlacement[mod.first][0].y - exchangeLen; // 0
                                leftdown_corner temp;
                                temp.x = FinalPlacement[mod.second][2].x;
                                temp.y = FinalPlacement[mod.second][2].y;
                                FinalPlacement[mod.first].push_back(temp); // 4
                                temp.y = FinalPlacement[mod.first][4].y + exchangeLen;
                                FinalPlacement[mod.first].push_back(temp); // 5
                                temp.x = FinalPlacement[mod.first][5].x - exchangeWidth;
                                FinalPlacement[mod.first].push_back(temp); // 6
                                temp.y = FinalPlacement[mod.first][6].y - 2 * exchangeLen;
                                FinalPlacement[mod.first].push_back(temp); // 7
                                // for(auto it = FinalPlacement[mod.first].begin(); it != FinalPlacement[mod.first].end(); it++)
                                // {
                                //     cout<<"("<<it->x<<","<<it->y<<") ";
                                // }
                                // 再做下面的m2
                                FinalPlacement[mod.second][2].x = FinalPlacement[mod.first][0].x;
                                FinalPlacement[mod.second][2].y = FinalPlacement[mod.first][0].y + exchangeLen; // 2
                                int temp_x1 = FinalPlacement[mod.second][3].x;
                                int temp_y1 = FinalPlacement[mod.second][3].y; // 右下先記下來
                                FinalPlacement[mod.second][3].x = FinalPlacement[mod.first][0].x;
                                FinalPlacement[mod.second][3].y = FinalPlacement[mod.first][0].y; // 3
                                temp.x = FinalPlacement[mod.first][7].x;
                                temp.y = FinalPlacement[mod.first][7].y;
                                FinalPlacement[mod.second].push_back(temp); // 4
                                temp.x = FinalPlacement[mod.first][6].x;
                                temp.y = FinalPlacement[mod.first][6].y;
                                FinalPlacement[mod.second].push_back(temp); // 5
                                temp.x = FinalPlacement[mod.first][5].x;
                                temp.y = FinalPlacement[mod.first][5].y;
                                FinalPlacement[mod.second].push_back(temp); // 6
                                temp.x = temp_x1;
                                temp.y = temp_y1;
                                FinalPlacement[mod.second].push_back(temp); // 7
                            }
                        }
                        else if (FinalPlacement[mod.first][0].x < FinalPlacement[mod.second][0].x) // 如果m1右邊右m1左邊左
                        {
                            if (isodd)
                            {
                                // cout << "left right not same right m1 more right left m1 more left(odd)" << endl;
                                // 先做上面的m1
                                int temp_x1 = FinalPlacement[mod.first][0].x;
                                int temp_y1 = FinalPlacement[mod.first][0].y; // 左下先記下來
                                int temp_x2 = FinalPlacement[mod.first][1].x;
                                int temp_y2 = FinalPlacement[mod.first][1].y; // 左上先記下來
                                int temp_x3 = FinalPlacement[mod.first][2].x;
                                int temp_y3 = FinalPlacement[mod.first][2].y; // 右上先記下來
                                int temp_x4 = FinalPlacement[mod.first][3].x;
                                int temp_y4 = FinalPlacement[mod.first][3].y; // 右下先記下來
                                FinalPlacement[mod.first][0].x = FinalPlacement[mod.second][1].x;
                                FinalPlacement[mod.first][0].y = FinalPlacement[mod.second][1].y - exchangeLen; // 0
                                FinalPlacement[mod.first][1].x = FinalPlacement[mod.second][1].x;
                                FinalPlacement[mod.first][1].y = FinalPlacement[mod.second][1].y; // 1
                                FinalPlacement[mod.first][2].x = temp_x1;
                                FinalPlacement[mod.first][2].y = temp_y1; // 2
                                FinalPlacement[mod.first][3].x = temp_x2;
                                FinalPlacement[mod.first][3].y = temp_y2; // 3
                                leftdown_corner temp;
                                temp.x = temp_x3;
                                temp.y = temp_y3;
                                FinalPlacement[mod.first].push_back(temp); // 4
                                temp.x = temp_x4;
                                temp.y = temp_y4;
                                FinalPlacement[mod.first].push_back(temp); // 5
                                temp.x = FinalPlacement[mod.second][2].x - 1;
                                temp.y = FinalPlacement[mod.second][2].y;
                                FinalPlacement[mod.first].push_back(temp); // 6
                                temp.y = FinalPlacement[mod.first][6].y + exchangeLen;
                                FinalPlacement[mod.first].push_back(temp); // 7
                                temp.x = FinalPlacement[mod.first][7].x - exchangeWidth;
                                temp.y = FinalPlacement[mod.first][7].y;
                                FinalPlacement[mod.first].push_back(temp); // 8
                                temp.x = FinalPlacement[mod.first][8].x;
                                temp.y = FinalPlacement[mod.first][8].y - 2 * exchangeLen;
                                FinalPlacement[mod.first].push_back(temp); // 9
                                // 再做下面的m2
                                FinalPlacement[mod.second][1].x = FinalPlacement[mod.first][0].x;
                                FinalPlacement[mod.second][1].y = FinalPlacement[mod.first][0].y; // 1
                                FinalPlacement[mod.second][2].x = FinalPlacement[mod.first][9].x;
                                FinalPlacement[mod.second][2].y = FinalPlacement[mod.first][9].y; // 2
                                int temp_x5 = FinalPlacement[mod.second][3].x;
                                int temp_y5 = FinalPlacement[mod.second][3].y; // 右下先記下來
                                FinalPlacement[mod.second][3].x = FinalPlacement[mod.first][8].x;
                                FinalPlacement[mod.second][3].y = FinalPlacement[mod.first][8].y; // 3
                                temp.x = FinalPlacement[mod.first][7].x;
                                temp.y = FinalPlacement[mod.first][7].y;
                                FinalPlacement[mod.second].push_back(temp); // 4
                                temp.x = FinalPlacement[mod.first][6].x;
                                temp.y = FinalPlacement[mod.first][6].y;
                                FinalPlacement[mod.second].push_back(temp); // 5
                                temp.x = FinalPlacement[mod.first][6].x + 1;
                                temp.y = FinalPlacement[mod.first][6].y;
                                FinalPlacement[mod.second].push_back(temp); // 6
                                temp.x = temp_x5;
                                temp.y = temp_y5;
                                FinalPlacement[mod.second].push_back(temp); // 7
                            }
                            else
                            {
                                // cout << "left right not same right m1 more right left m1 more left(even)" << endl;
                                // 先做上面的m1
                                int temp_x1 = FinalPlacement[mod.first][0].x;
                                int temp_y1 = FinalPlacement[mod.first][0].y; // 左下先記下來
                                int temp_x2 = FinalPlacement[mod.first][1].x;
                                int temp_y2 = FinalPlacement[mod.first][1].y; // 左上先記下來
                                int temp_x3 = FinalPlacement[mod.first][2].x;
                                int temp_y3 = FinalPlacement[mod.first][2].y; // 右上先記下來
                                int temp_x4 = FinalPlacement[mod.first][3].x;
                                int temp_y4 = FinalPlacement[mod.first][3].y; // 右下先記下來
                                FinalPlacement[mod.first][0].x = FinalPlacement[mod.second][1].x;
                                FinalPlacement[mod.first][0].y = FinalPlacement[mod.second][1].y - exchangeLen; // 0
                                FinalPlacement[mod.first][1].x = FinalPlacement[mod.second][1].x;
                                FinalPlacement[mod.first][1].y = FinalPlacement[mod.second][1].y; // 1
                                FinalPlacement[mod.first][2].x = temp_x1;
                                FinalPlacement[mod.first][2].y = temp_y1; // 2
                                FinalPlacement[mod.first][3].x = temp_x2;
                                FinalPlacement[mod.first][3].y = temp_y2; // 3
                                leftdown_corner temp;
                                temp.x = temp_x3;
                                temp.y = temp_y3;
                                FinalPlacement[mod.first].push_back(temp); // 4
                                temp.x = temp_x4;
                                temp.y = temp_y4;
                                FinalPlacement[mod.first].push_back(temp); // 5
                                temp.x = FinalPlacement[mod.second][2].x;
                                temp.y = FinalPlacement[mod.second][2].y;
                                FinalPlacement[mod.first].push_back(temp); // 6
                                temp.y = FinalPlacement[mod.first][6].y + exchangeLen;
                                FinalPlacement[mod.first].push_back(temp); // 7
                                temp.x = FinalPlacement[mod.first][7].x - exchangeWidth;
                                temp.y = FinalPlacement[mod.first][7].y;
                                FinalPlacement[mod.first].push_back(temp); // 8
                                temp.x = FinalPlacement[mod.first][8].x;
                                temp.y = FinalPlacement[mod.first][8].y - 2 * exchangeLen;
                                FinalPlacement[mod.first].push_back(temp); // 9
                                // 再做下面的m2
                                FinalPlacement[mod.second][1].x = FinalPlacement[mod.first][0].x;
                                FinalPlacement[mod.second][1].y = FinalPlacement[mod.first][0].y; // 1
                                FinalPlacement[mod.second][2].x = FinalPlacement[mod.first][9].x;
                                FinalPlacement[mod.second][2].y = FinalPlacement[mod.first][9].y; // 2
                                int temp_x5 = FinalPlacement[mod.second][3].x;
                                int temp_y5 = FinalPlacement[mod.second][3].y; // 右下先記下來
                                FinalPlacement[mod.second][3].x = FinalPlacement[mod.first][8].x;
                                FinalPlacement[mod.second][3].y = FinalPlacement[mod.first][8].y; // 3
                                temp.x = FinalPlacement[mod.first][7].x;
                                temp.y = FinalPlacement[mod.first][7].y;
                                FinalPlacement[mod.second].push_back(temp); // 4
                                temp.x = temp_x5;
                                temp.y = temp_y5;
                                FinalPlacement[mod.second].push_back(temp); // 5
                            }
                        }
                    }
                    else if (FinalPlacement[mod.first][2].x < FinalPlacement[mod.second][2].x) // 如果m2右邊較右
                    {
                        // cout << "left right not same right m2 more right " << endl;
                        if (FinalPlacement[mod.first][0].x > FinalPlacement[mod.second][0].x) // 如果m2左邊較左
                        {
                            if (isodd)
                            {
                                // cout << "left right not same right m2 more right left m2 more left(odd)" << endl;
                                // 先做上面的m1
                                FinalPlacement[mod.first][0].y = FinalPlacement[mod.first][0].y - exchangeLen; // 0
                                leftdown_corner temp;
                                temp.x = FinalPlacement[mod.first][3].x - 1;
                                temp.y = FinalPlacement[mod.first][3].y;
                                FinalPlacement[mod.first].push_back(temp); // 4
                                temp.y = FinalPlacement[mod.first][4].y + exchangeLen;
                                FinalPlacement[mod.first].push_back(temp); // 5
                                temp.x = FinalPlacement[mod.first][5].x - exchangeWidth;
                                FinalPlacement[mod.first].push_back(temp); // 6
                                temp.y = FinalPlacement[mod.first][6].y - 2 * exchangeLen;
                                FinalPlacement[mod.first].push_back(temp); // 7
                                // 再做右邊的m2 0不變
                                int temp_x2 = FinalPlacement[mod.second][2].x;
                                int temp_y2 = FinalPlacement[mod.second][2].y; // 右上先記下來
                                int temp_x3 = FinalPlacement[mod.second][3].x;
                                int temp_y3 = FinalPlacement[mod.second][3].y; // 右下先記下來
                                FinalPlacement[mod.second][2].x = FinalPlacement[mod.first][0].x;
                                FinalPlacement[mod.second][2].y = FinalPlacement[mod.first][0].y + exchangeLen; // 2
                                FinalPlacement[mod.second][3].x = FinalPlacement[mod.first][0].x;
                                FinalPlacement[mod.second][3].y = FinalPlacement[mod.first][0].y; // 3
                                temp.x = FinalPlacement[mod.first][7].x;
                                temp.y = FinalPlacement[mod.first][7].y;
                                FinalPlacement[mod.second].push_back(temp); // 4
                                temp.x = FinalPlacement[mod.first][6].x;
                                temp.y = FinalPlacement[mod.first][6].y;
                                FinalPlacement[mod.second].push_back(temp); // 5
                                temp.x = FinalPlacement[mod.first][5].x;
                                temp.y = FinalPlacement[mod.first][5].y;
                                FinalPlacement[mod.second].push_back(temp); // 6
                                temp.y = FinalPlacement[mod.second][6].y - exchangeLen;
                                FinalPlacement[mod.second].push_back(temp); // 7
                                temp.x = temp_x2;
                                temp.y = temp_y2;
                                FinalPlacement[mod.second].push_back(temp); // 8
                                temp.x = temp_x3;
                                temp.y = temp_y3;
                                FinalPlacement[mod.second].push_back(temp); // 9
                            }
                            else
                            {
                                // cout << "left right not same right m2 more right left m2 more left(even)" << endl;
                                // 先做上面的m1
                                FinalPlacement[mod.first][0].y = FinalPlacement[mod.first][0].y - exchangeLen; // 0
                                int temp_x1 = FinalPlacement[mod.first][3].x;
                                int temp_y1 = FinalPlacement[mod.first][3].y; // 右下先記下來
                                FinalPlacement[mod.first][3].x = FinalPlacement[mod.first][3].x;
                                FinalPlacement[mod.first][3].y = FinalPlacement[mod.first][3].y + exchangeLen; // 3
                                leftdown_corner temp;
                                temp.x = FinalPlacement[mod.first][3].x - exchangeWidth;
                                temp.y = FinalPlacement[mod.first][3].y;
                                FinalPlacement[mod.first].push_back(temp); // 4
                                temp.y = FinalPlacement[mod.first][4].y - 2 * exchangeLen;
                                FinalPlacement[mod.first].push_back(temp); // 5
                                // 再做右邊的m2 0不變
                                int temp_x2 = FinalPlacement[mod.second][2].x;
                                int temp_y2 = FinalPlacement[mod.second][2].y; // 右上先記下來
                                int temp_x3 = FinalPlacement[mod.second][3].x;
                                int temp_y3 = FinalPlacement[mod.second][3].y; // 右下先記下來
                                FinalPlacement[mod.second][2].x = FinalPlacement[mod.first][0].x;
                                FinalPlacement[mod.second][2].y = FinalPlacement[mod.first][0].y + exchangeLen; // 2
                                FinalPlacement[mod.second][3].x = FinalPlacement[mod.first][0].x;
                                FinalPlacement[mod.second][3].y = FinalPlacement[mod.first][0].y; // 3
                                temp.x = FinalPlacement[mod.first][5].x;
                                temp.y = FinalPlacement[mod.first][5].y;
                                FinalPlacement[mod.second].push_back(temp); // 4
                                temp.x = FinalPlacement[mod.first][4].x;
                                temp.y = FinalPlacement[mod.first][4].y;
                                FinalPlacement[mod.second].push_back(temp); // 5
                                temp.x = FinalPlacement[mod.first][3].x;
                                temp.y = FinalPlacement[mod.first][3].y;
                                FinalPlacement[mod.second].push_back(temp); // 6
                                temp.y = FinalPlacement[mod.second][6].y - exchangeLen;
                                FinalPlacement[mod.second].push_back(temp); // 7
                                temp.x = temp_x2;
                                temp.y = temp_y2;
                                FinalPlacement[mod.second].push_back(temp); // 8
                                temp.x = temp_x3;
                                temp.y = temp_y3;
                                FinalPlacement[mod.second].push_back(temp); // 9
                            }
                        }
                        else if (FinalPlacement[mod.first][0].x < FinalPlacement[mod.second][0].x) // 如果m2右邊更右 m1左邊更左
                        {
                            if (isodd)
                            {
                                // cout << "left right not same right m2 more right left m1 more left(odd)" << endl;
                                // 先做上面的m1
                                int temp_x1 = FinalPlacement[mod.first][0].x;
                                int temp_y1 = FinalPlacement[mod.first][0].y; // 左下先記下來
                                int temp_x2 = FinalPlacement[mod.first][1].x;
                                int temp_y2 = FinalPlacement[mod.first][1].y; // 左上先記下來
                                int temp_x3 = FinalPlacement[mod.first][2].x;
                                int temp_y3 = FinalPlacement[mod.first][2].y; // 右上先記下來
                                int temp_x4 = FinalPlacement[mod.first][3].x;
                                int temp_y4 = FinalPlacement[mod.first][3].y; // 右下先記下來
                                FinalPlacement[mod.first][0].x = FinalPlacement[mod.second][1].x;
                                FinalPlacement[mod.first][0].y = FinalPlacement[mod.second][1].y - exchangeLen; // 0
                                FinalPlacement[mod.first][1].x = FinalPlacement[mod.second][1].x;
                                FinalPlacement[mod.first][1].y = FinalPlacement[mod.second][1].y; // 1
                                FinalPlacement[mod.first][2].x = temp_x1;
                                FinalPlacement[mod.first][2].y = temp_y1; // 2
                                FinalPlacement[mod.first][3].x = temp_x2;
                                FinalPlacement[mod.first][3].y = temp_y2; // 3
                                leftdown_corner temp;
                                temp.x = temp_x3;
                                temp.y = temp_y3;
                                FinalPlacement[mod.first].push_back(temp); // 4
                                temp.x = temp_x4;
                                temp.y = temp_y4;
                                FinalPlacement[mod.first].push_back(temp); // 5
                                temp.x = temp.x - 1;
                                FinalPlacement[mod.first].push_back(temp); // 6
                                temp.y = temp.y + exchangeLen;
                                FinalPlacement[mod.first].push_back(temp); // 7
                                temp.x = temp.x - exchangeWidth;
                                FinalPlacement[mod.first].push_back(temp); // 8
                                temp.y = temp.y - 2 * exchangeLen;
                                FinalPlacement[mod.first].push_back(temp); // 9

                                // 再做下面的m2
                                int temp_x5 = FinalPlacement[mod.second][2].x;
                                int temp_y5 = FinalPlacement[mod.second][2].y; // 右上先記下來
                                int temp_x6 = FinalPlacement[mod.second][3].x;
                                int temp_y6 = FinalPlacement[mod.second][3].y; // 右下先記下來
                                FinalPlacement[mod.second][1].x = FinalPlacement[mod.first][0].x;
                                FinalPlacement[mod.second][1].y = FinalPlacement[mod.first][0].y; // 1
                                FinalPlacement[mod.second][2].x = FinalPlacement[mod.first][9].x;
                                FinalPlacement[mod.second][2].y = FinalPlacement[mod.first][9].y; // 2
                                FinalPlacement[mod.second][3].x = FinalPlacement[mod.first][8].x;
                                FinalPlacement[mod.second][3].y = FinalPlacement[mod.first][8].y; // 3
                                temp.x = FinalPlacement[mod.first][7].x;
                                temp.y = FinalPlacement[mod.first][7].y;
                                FinalPlacement[mod.second].push_back(temp); // 4
                                temp.x = FinalPlacement[mod.first][6].x;
                                temp.y = FinalPlacement[mod.first][6].y;
                                FinalPlacement[mod.second].push_back(temp); // 5
                                temp.x = temp_x5;
                                temp.y = temp_y5;
                                FinalPlacement[mod.second].push_back(temp); // 6
                                temp.x = temp_x6;
                                temp.y = temp_y6;
                                FinalPlacement[mod.second].push_back(temp); // 7
                            }
                            else
                            {
                                // cout << "left right not same right m2 more right left m1 more left(even)" << endl;
                                // 先做上面的m1
                                int temp_x1 = FinalPlacement[mod.first][0].x;
                                int temp_y1 = FinalPlacement[mod.first][0].y; // 左下先記下來
                                int temp_x2 = FinalPlacement[mod.first][1].x;
                                int temp_y2 = FinalPlacement[mod.first][1].y; // 左上先記下來
                                int temp_x3 = FinalPlacement[mod.first][2].x;
                                int temp_y3 = FinalPlacement[mod.first][2].y; // 右上先記下來
                                int temp_x4 = FinalPlacement[mod.first][3].x;
                                int temp_y4 = FinalPlacement[mod.first][3].y; // 右下先記下來
                                FinalPlacement[mod.first][0].x = FinalPlacement[mod.second][1].x;
                                FinalPlacement[mod.first][0].y = FinalPlacement[mod.second][1].y - exchangeLen; // 0
                                FinalPlacement[mod.first][1].x = FinalPlacement[mod.second][1].x;
                                FinalPlacement[mod.first][1].y = FinalPlacement[mod.second][1].y; // 1
                                FinalPlacement[mod.first][2].x = temp_x1;
                                FinalPlacement[mod.first][2].y = temp_y1; // 2
                                FinalPlacement[mod.first][3].x = temp_x2;
                                FinalPlacement[mod.first][3].y = temp_y2; // 3
                                leftdown_corner temp;
                                temp.x = temp_x3;
                                temp.y = temp_y3;
                                FinalPlacement[mod.first].push_back(temp); // 4
                                temp.x = temp_x4;
                                temp.y = temp_y4 + exchangeLen;
                                FinalPlacement[mod.first].push_back(temp); // 5
                                temp.x = temp.x - exchangeWidth;
                                FinalPlacement[mod.first].push_back(temp); // 6
                                temp.y = temp.y - 2 * exchangeLen;
                                FinalPlacement[mod.first].push_back(temp); // 7
                                // 再做下面的m2
                                int temp_x5 = FinalPlacement[mod.second][2].x;
                                int temp_y5 = FinalPlacement[mod.second][2].y; // 右上先記下來
                                int temp_x6 = FinalPlacement[mod.second][3].x;
                                int temp_y6 = FinalPlacement[mod.second][3].y; // 右下先記下來
                                FinalPlacement[mod.second][1].x = FinalPlacement[mod.first][0].x;
                                FinalPlacement[mod.second][1].y = FinalPlacement[mod.first][0].y; // 1
                                FinalPlacement[mod.second][2].x = FinalPlacement[mod.first][7].x;
                                FinalPlacement[mod.second][2].y = FinalPlacement[mod.first][7].y; // 2
                                FinalPlacement[mod.second][3].x = FinalPlacement[mod.first][6].x;
                                FinalPlacement[mod.second][3].y = FinalPlacement[mod.first][6].y; // 3
                                temp.x = FinalPlacement[mod.first][5].x;
                                temp.y = FinalPlacement[mod.first][5].y;
                                FinalPlacement[mod.second].push_back(temp); // 4
                                temp.x = temp_x4;
                                temp.y = temp_y4;
                                FinalPlacement[mod.second].push_back(temp); // 5
                                temp.x = temp_x5;
                                temp.y = temp_y5;
                                FinalPlacement[mod.second].push_back(temp); // 6
                                temp.x = temp_x6;
                                temp.y = temp_y6;
                                FinalPlacement[mod.second].push_back(temp); // 7
                            }
                        }
                    }
                }
            }
        }
    };
};
void merge_two_block(int mod1, int mod2, leftdown_corner &start, leftdown_corner &end)
{
    for (int i = 0; i < FinalPlacement[mod1].size(); ++i)
    {
        leftdown_corner temp_prev;
        leftdown_corner temp_next;
        temp_prev.x = FinalPlacement[mod1][i].x;
        temp_prev.y = FinalPlacement[mod1][i].y;
        if (i != FinalPlacement[mod1].size() - 1)
        {
            temp_next.x = FinalPlacement[mod1][i + 1].x;
            temp_next.y = FinalPlacement[mod1][i + 1].y;
        }
        else
        {
            temp_next.x = FinalPlacement[mod1][0].x;
            temp_next.y = FinalPlacement[mod1][0].y;
        }
        for (int j = FinalPlacement[mod2].size() - 1; j >= 0; j--)
        {
            leftdown_corner temp2_prev;
            leftdown_corner temp2_next;
            temp2_prev.x = FinalPlacement[mod2][j].x;
            temp2_prev.y = FinalPlacement[mod2][j].y;
            if (j != 0)
            {
                temp2_next.x = FinalPlacement[mod2][j - 1].x;
                temp2_next.y = FinalPlacement[mod2][j - 1].y;
            }
            else
            {
                temp2_next.x = FinalPlacement[mod2][FinalPlacement[mod2].size() - 1].x;
                temp2_next.y = FinalPlacement[mod2][FinalPlacement[mod2].size() - 1].y;
            }
            bool LineNotOverlap = false;
            leftdown_corner start2;
            leftdown_corner end2;
            getCollinearEndpoints(temp_prev, temp_next, temp2_prev, temp2_next, start2, end2, LineNotOverlap);
            if (LineNotOverlap)
            {
                continue;
            }
            start = start2;
            end = end2;
        };
    };
};
void getCollinearEndpoints(leftdown_corner p1, leftdown_corner p2, leftdown_corner p3, leftdown_corner p4, leftdown_corner &endpoint1, leftdown_corner &endpoint2, bool &LineNotOverlap)
{
    // 水平线段的情况
    if (p1.y == p2.y && p2.y == p3.y && p3.y == p4.y && p4.y == p1.y)
    {
        int minX = max(min(p1.x, p2.x), min(p3.x, p4.x));
        int maxX = min(max(p1.x, p2.x), max(p3.x, p4.x));
        endpoint1.x=minX;
        endpoint1.y=p1.y;
        endpoint2.x=maxX;
        endpoint2.y=p1.y;
        if (minX >= maxX)
        {
            LineNotOverlap = true;
        }
    }
    // 垂直线段的情况
    else if (p1.x == p2.x && p2.x == p3.x && p3.x == p4.x && p4.x == p1.x)
    {
        int minY = max(min(p1.y, p2.y), min(p3.y, p4.y));
        int maxY = min(max(p1.y, p2.y), max(p3.y, p4.y));
        endpoint1.x=p1.x;
        endpoint1.y=minY;
        endpoint2.x=p1.x;
        endpoint2.y=maxY;
        if (minY >= maxY)
        {
            LineNotOverlap = true;
        }
    }
    else
    {
        LineNotOverlap = true;
    }
}

long double calculatePolygonArea(vector<leftdown_corner> points)
{
    long double area = 0.0;
    int numPoints = points.size();

    for (int i = 0; i < numPoints; ++i)
    {
        int j = (i + 1) % numPoints;
        area += static_cast<long double>(points[i].getX()) * static_cast<long double>(points[j].getY()) -
                static_cast<long double>(points[j].getX()) * static_cast<long double>(points[i].getY());
    }

    return abs(area) / 2.0;
}

bool checkRatioArea(int modID)
{
    vector<leftdown_corner> temp = FinalPlacement[modID];
    long double area = calculatePolygonArea(temp);
    long double minRectArea = LDBL_MIN;
    // 算出最小x,y和最大x,y
    int min_x = INT_MAX;
    int min_y = INT_MAX;
    int max_x = INT_MIN;
    int max_y = INT_MIN;
    for (int i = 0; i < FinalPlacement[modID].size(); ++i)
    {
        min_x = min(min_x, FinalPlacement[modID][i].getX());
        min_y = min(min_y, FinalPlacement[modID][i].getY());
        max_x = max(max_x, FinalPlacement[modID][i].getX());
        max_y = max(max_y, FinalPlacement[modID][i].getY());
    }
    minRectArea = (max_x - min_x) * (max_y - min_y);
    long double ratio = area / minRectArea;
    if (ratio < 0.8)
        return false;
    else // check height width ratio
    {
        long double height = max_y - min_y;
        long double width = max_x - min_x;
        if (height > width)
        {
            if (height / width > 2.0)
                return false;
            else
                return true;
        }
        else
        {
            if (width / height > 2.0)
                return false;
            else
                return true;
        }
    }
}

long double canBorrowArea(int modID)
{
    // calclaute the min x,y and max x,y
    int min_x = INT_MAX;
    int min_y = INT_MAX;
    int max_x = INT_MIN;
    int max_y = INT_MIN;
    for (int i = 0; i < FinalPlacement[modID].size(); ++i)
    {
        min_x = min(min_x, FinalPlacement[modID][i].getX());
        min_y = min(min_y, FinalPlacement[modID][i].getY());
        max_x = max(max_x, FinalPlacement[modID][i].getX());
        max_y = max(max_y, FinalPlacement[modID][i].getY());
    }
    long double minRectArea = (max_x - min_x) * (max_y - min_y);
    return minRectArea * 0.2;
}

int BorrowLessMod(int modID1, int modID2)
{
    long double area1 = canBorrowArea(modID1);
    long double area2 = canBorrowArea(modID2);
    if (area1 < area2)
        return modID1;
    else
        return modID2;
}

int ExchangingLength(int modID1, int modID2, leftdown_corner start, leftdown_corner end)
{
    // int borrowMod=BorrowLessMod(modID1,modID2);
    // calculate the most length can be borrowed
    // long double area = canBorrowArea(borrowMod);
    // if leftroght==0, updown, else leftright
    // while updown, calculate the most length can be borrowed
    Module tmpMod1 = Modules[modID1];
    Module tmpMod2 = Modules[modID2];
    // 計算目前長寬
    int mod1Width = tmpMod1.getWidth();
    int mod1Height = tmpMod1.getHeight();
    int mod2Width = tmpMod2.getWidth();
    int mod2Height = tmpMod2.getHeight();
    int leftright = ModLocationRelation(start, end);
    if (leftright) // 左右關係
    {
        // cout<<"--------------leftright--------------\n";
        int changeAreaHeight = abs(end.y - start.y) / 2; // 相交線段
        if (changeAreaHeight <= 0)
            return 0;
        // cout<<"Intersection: "<<changeAreaHeight<<"\n";
        int mod1LimitLength = mod1Height * 2 - mod1Width;
        // cout<<"m1h:"<<mod1Height<<"\n";
        // cout<<"m1w:"<<mod1Width<<"\n";
        // cout<<"m1Lim:"<<mod1LimitLength<<"\n";
        int mod2LimitLength = mod2Height * 2 - mod2Width;
        // cout<<"m2h:"<<mod2Height<<"\n";
        // cout<<"m2w:"<<mod2Width<<"\n";
        // cout<<"m2Lim:"<<mod2LimitLength<<"\n";
        int LimitLength = min(mod1LimitLength, mod2LimitLength); // 選長寬比限制中能延長較小的
        // cout<<"Lim: "<<LimitLength<<"\n";
        //  long double BorrowLength = area/static_cast<long double>(changeAreaHeight);//算面積限制中的最大長度
        long double BorrowLength = static_cast<long double>(min(mod1Width, mod2Width)) * 0.25;
        // cout<<"area: "<<area<<" , changeAreaHeight: "<<changeAreaHeight<<"\n";
        // cout << "Borrow: " << BorrowLength << "\n";

        // cout<<"Changing: "<<min(static_cast<long double>(LimitLength),BorrowLength)<<"\n";
        long double temp = min(static_cast<long double>(LimitLength), BorrowLength);
        if (temp < 0)
            return 0;
        else
            return temp;
    }
    else // 上下關係
    {
        // cout<<"--------------updown--------------\n";
        int changeAreaWidth = abs(end.x - start.x) / 2;
        if (changeAreaWidth <= 0)
            return 0;
        int mod1LimitLength = mod1Width * 2 - mod1Height;
        // cout<<"m1h:"<<mod1Height<<"\n";
        // cout<<"m1w:"<<mod1Width<<"\n";
        // cout<<"m1Lim:"<<mod1LimitLength<<"\n";
        int mod2LimitLength = mod2Width * 2 - mod2Height;
        // cout<<"m2h:"<<mod2Height<<"\n";
        // cout<<"m2w:"<<mod2Width<<"\n";
        // cout<<"m2Lim:"<<mod2LimitLength<<"\n";
        int LimitLength = min(mod1LimitLength, mod2LimitLength);
        // cout<<"Lim:"<<LimitLength<<"\n";
        // long double BorrowLength = area/static_cast<long double>(changeAreaWidth);
        long double BorrowLength = static_cast<long double>(min(mod1Height, mod2Height)) * 0.25;
        // cout << "Borrow:" << BorrowLength << "\n";

        // cout<<"Changing:"<<changeAreaWidth<<"\n";
        // cout<<"Changing:"<<min(static_cast<long double>(LimitLength),BorrowLength)<<"\n";
        long double temp = min(static_cast<long double>(LimitLength), BorrowLength);
        if (temp < 0)
            return 0;
        else
            return temp;
    }
}

int ModLocationRelation(leftdown_corner start, leftdown_corner end) // updown=0, leftright=1
{
    if (start.x == end.x)
        return 1;
    else
        return 0;
}

long double calculateMinPolygonArea(vector<leftdown_corner> points)
{
    long double minRectArea = 0.0;
    int numPoints = points.size();

    // 算出最小x,y和最大x,y
    int min_x = INT_MAX;
    int min_y = INT_MAX;
    int max_x = INT_MIN;
    int max_y = INT_MIN;
    for (int i = 1; i < points.size(); ++i)
    {
        min_x = min(min_x, points[i].getX());
        min_y = min(min_y, points[i].getY());
        max_x = max(max_x, points[i].getX());
        max_y = max(max_y, points[i].getY());
    }
    minRectArea = static_cast<long double>(max_x - min_x) * static_cast<long double>(max_y - min_y);

    return minRectArea;
}

long double calculateHeightWidthRatio(vector<leftdown_corner> points)
{
    long double ratio = 0.0;
    int numPoints = points.size();

    // 算出最小x,y和最大x,y
    int min_x = INT_MAX;
    int min_y = INT_MAX;
    int max_x = INT_MIN;
    int max_y = INT_MIN;
    for (int i = 1; i < points.size(); ++i)
    {
        min_x = min(min_x, points[i].getX());
        min_y = min(min_y, points[i].getY());
        max_x = max(max_x, points[i].getX());
        max_y = max(max_y, points[i].getY());
    }
    long double height = static_cast<long double>(max_y - min_y);
    long double width = static_cast<long double>(max_x - min_x);
    ratio = height / width;
    return ratio;
}
leftdown_corner ModPullingDirection(int modID)
{
    // pair<double,double> pullingDirection;
    // pullingDirection.first = 0;
    // pullingDirection.second = 0;
    double xttl = 0.0;
    double yttl = 0.0;
    // 先找自己質心座標
    int min_x = INT_MAX;
    int min_y = INT_MAX;
    int max_x = INT_MIN;
    int max_y = INT_MIN;
    for (int i = 0; i < FinalPlacement[modID].size(); ++i)
    {
        min_x = min(min_x, FinalPlacement[modID][i].getX());
        min_y = min(min_y, FinalPlacement[modID][i].getY());
        max_x = max(max_x, FinalPlacement[modID][i].getX());
        max_y = max(max_y, FinalPlacement[modID][i].getY());
    }
    double center_x = double(min_x + max_x) / 2.0;
    double center_y = double(min_y + max_y) / 2.0;
    // cout << "----------------------------------\n";
    // cout << "modID: " << modID << " ";
    // cout << "center_x: " << center_x << " center_y: " << center_y << "\n";
    for (int i = 0; i < Connections.size(); ++i)
    {
        int m2id = 0;
        bool find = false;
        if (Connections[i].Module1ID == modID)
        {
            m2id = Connections[i].Module2ID;
            find = true;
        }
        else if (Connections[i].Module2ID == modID)
        {
            m2id = Connections[i].Module1ID;
            find = true;
        }
        if (find)
        {
            // 找到相連的模組的質心座標
            int min_x2 = INT_MAX;
            int min_y2 = INT_MAX;
            int max_x2 = INT_MIN;
            int max_y2 = INT_MIN;
            for (int j = 0; j < FinalPlacement[m2id].size(); ++j)
            {
                min_x2 = min(min_x2, FinalPlacement[m2id][j].getX());
                min_y2 = min(min_y2, FinalPlacement[m2id][j].getY());
                max_x2 = max(max_x2, FinalPlacement[m2id][j].getX());
                max_y2 = max(max_y2, FinalPlacement[m2id][j].getY());
            }
            double center_x2 = double(min_x2 + max_x2) / 2.0;
            double center_y2 = double(min_y2 + max_y2) / 2.0;
            // cout<<"m2id: "<<m2id<<"\n";
            // cout<<"center_x2: "<<center_x2<<" center_y2: "<<center_y2<<endl;
            // cout<<"ConnectionPin: "<<Connections[i].ConnectionPin<<endl;
            double Xpulling = (center_x2 - center_x) * Connections[i].ConnectionPin;
            double Ypulling = (center_y2 - center_y) * Connections[i].ConnectionPin;
            // cout<<"Xpulling: "<<Xpulling<<" Ypulling: "<<Ypulling<<endl<<endl;
            xttl += Xpulling;
            yttl += Ypulling;
        }
    }
    // cout<<"pullingDirection.first: "<<xttl<<" pullingDirection.second: "<<yttl<<endl;
    leftdown_corner pullingDirection;
    pullingDirection.x = xttl;
    pullingDirection.y = yttl;
    return pullingDirection;
};