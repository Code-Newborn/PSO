#ifndef _PSO_H_
#define _PSO_H_

#define PI 3.14159265358979323846
#define POPSIZE 20                         //粒子个数
#define MAXINTERATION 2000                 //最大迭代次数
#define NVARS 10                           //参数个数
#define WMAX 0.9                           //惯量权重最大值
#define WMIN 0.4                           //惯量权重最小值

extern double absbound;             //上下界绝对值
extern double vmax;                 //最大速度
extern double gBest[NVARS];         //全局最优解

double evalfunc(double[], int);            //适应度计算
double avg(double[], int);                 //求平均数
double stddev(double[], int);              //求标准差
void initialize(int);                      //初始化函数
double randval(double, double);            //求范围（lower，upper）之间的随机数
void evaluate(int);                        //评估粒子适应值
void update(int, int);                     //利用更新公式更新每个粒子的速度和位置以及历史最优位置
void fit(void);                            //更新群体历史最优位置

#endif