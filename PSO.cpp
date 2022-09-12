
#include <math.h>
#include <random>
#include <iostream>

#include "PSO.h"

struct particle //单个粒子
{
    double pBest[NVARS];
    double v[NVARS]; // 粒子在各个维度上的速度
    double x[NVARS]; // 粒子在各个维度上的位置
    double upper[NVARS];
    double lower[NVARS];
};

double w;                    //惯量权重
double c1 = 2.0;             //加速因子1
double c2 = 2.0;             //加速因子2
double absbound;             //上下界绝对值
double vmax;                 //最大速度
double gBest[NVARS];         //全局最优解
particle particles[POPSIZE]; //粒子群

/// <summary>
/// 适应度计算
/// </summary>
/// <param name="parameter"> 粒子 NVARS 个参数位置 </param>
/// <param name="evalFUNC"> 适应度函数（可根据系统自定义） </param>
/// <returns></returns>
double evalfunc(double parameter[], int evalFUNC = 1)
{
    if (evalFUNC == 1)
    {
        double val = 0;
        for (int i = 0; i < NVARS; i++)
        {
            val += parameter[i] * parameter[i];
        }
        return val;
    }

    if (evalFUNC == 2)
    {
        double val1 = 0;
        double val2 = 0;
        for (int i = 0; i < NVARS; i++)
        {
            val1 += abs(parameter[i]);
            val2 = val2 * abs(parameter[i]);
        }
        return (val1 + val2);
    }

    if (evalFUNC == 3)
    {
        double val1 = 0;
        int i, j;
        for (i = 0; i < NVARS; i++)
        {
            double val2 = 0;
            for (j = 0; j < i; j++)
            {
                val2 += parameter[j];
            }
            val1 += val2 * val2;
        }
        return val1;
    }

    if (evalFUNC == 4)
    {
        double val = 0;
        for (int i = 0; i < NVARS; i++)
        {
            if (abs(parameter[i]) > val)
                val = abs(parameter[i]);
        }
    }

    if (evalFUNC == 5)
    {
        double val = 0;
        for (int i = 0; i < NVARS - 1; i++)
        {
            val += 100 * (parameter[i + 1] - parameter[i] * parameter[i]) * (parameter[i + 1] - parameter[i] * parameter[i]) + (parameter[i] - 1) * (parameter[i] - 1);
        }
        return val;
    }

    if (evalFUNC == 6)
    {
        double val = 0;
        for (int i = 0; i < NVARS; i++)
        {
            val += floor(parameter[i] + 0.5) * floor(parameter[i] + 0.5);
        }
        return val;
    }

    if (evalFUNC == 7)
    {
        double val = 0;
        for (int i = 0; i < NVARS; i++)
        {
            val += i * pow(parameter[i], double(i));
        }
        return (val + randval(0, 1));
    }

    if (evalFUNC == 8)
    {
        double val = 0;
        for (int i = 0; i < NVARS; i++)
        {
            val += (-(parameter[i] * sin(sqrt(abs(parameter[i])) / 180 * PI)));
        }
        return val;
    }

    if (evalFUNC == 9)
    {
        double val = 0;
        for (int i = 0; i < NVARS; i++)
        {
            val += (parameter[i] * parameter[i] - 10 * cos(2 * PI * parameter[i] / 180 * PI) + 10.0);
        }
        return val;
    }

    if (evalFUNC == 10)
    {
        double val1 = 0;
        double val2 = 0;
        for (int i = 0; i < NVARS; i++)
        {
            val1 += parameter[i] * parameter[i];
            val2 += cos(2 * PI * parameter[i] / 180 * PI);
        }
        return ((-20) * exp((-0.2) * sqrt(val1 / NVARS)) - exp(val2 / NVARS) + 20 + exp(1));
    }
}

/// <summary>
/// 平均值计算
/// </summary>
/// <param name="parameter"> 数据集 </param>
/// <param name="n"> 数据个数 </param>
/// <returns></returns>
double avg(double parameter[], int n)
{
    double num = 0;
    for (int i = 0; i < n; i++)
    {
        num += parameter[i];
    }
    return num / n;
}

/// <summary>
/// 标准差计算
/// </summary>
/// <param name="parameter"> 数据集 </param>
/// <param name="n"> 数据个数 </param>
/// <returns></returns>
double stddev(double parameter[], int n)
{
    double average = avg(parameter, n);
    double sum = 0.0;
    for (int i = 0; i < n; i++)
    {
        sum += (parameter[i] - average) * (parameter[i] - average);
    }
    return sqrt(sum / n);
}

double randval(double low, double high)
{ // 随机权重
    return (low + (high - low) * rand() * 1.0 / RAND_MAX);
}

void initialize(int FUNC = 1)
{ //
    /*  初始化每个粒子的速度、位置并将  */
    /*  该位置设定为当前历史最优位置找  */
    /*  到所有粒子的最优位置并设定为当  */
    /*  前群体历史最优位置              */
    int i, j;
    double lbound[NVARS];
    double ubound[NVARS];

    for (i = 0; i < NVARS; i++)
    {
        lbound[i] = (-absbound);
        ubound[i] = absbound;
    }

    // 初始化所有粒子的位置、速度的上下界
    for (i = 0; i < NVARS; i++)
    {
        for (j = 0; j < POPSIZE; j++)
        {
            particles[j].upper[i] = ubound[i];
            particles[j].lower[i] = lbound[i];
            particles[j].v[i] = randval(lbound[i], ubound[i]);
            particles[j].x[i] = randval(lbound[i], ubound[i]);
            particles[j].pBest[i] = particles[j].x[i]; // 初始化个体历史最佳位置
        }
    }

    double pval = evalfunc(particles[0].pBest);
    int num = 0;
    for (i = 1; i < POPSIZE; i++)
    {
        if (pval > evalfunc(particles[i].pBest))
        {
            pval = evalfunc(particles[i].pBest);
            num = i; // 搜寻记录适应度函数值最小的粒子
        }
        else
            continue;
    }
    for (j = 0; j < NVARS; j++)
    {
        gBest[j] = particles[num].pBest[j]; // 全局历史最佳位置
    }
}

void evaluate(int FUNC = 1)
{
    /*  通过传入参数FUNC来调用不同的评  */
    /*  估函数并对粒子位置进行评估并更  */
    /*  新粒子历史最优位置              */
    int i, j;
    double pval[POPSIZE], nval[POPSIZE];

    for (i = 0; i < POPSIZE; i++)
    {
        pval[i] = evalfunc(particles[i].pBest, FUNC); // 粒子个体历史最佳位置的适应度
        nval[i] = evalfunc(particles[i].x, FUNC);     // 粒子个体当前位置的适应度

        if (pval[i] > nval[i])
        {
            for (j = 0; j < NVARS; j++)
            {
                particles[i].pBest[j] = particles[i].x[j]; // 更新粒子个体最佳位置
            }
        }
    }
}

void update(int interation, int w_change_method = 1)
{
    /*  通过参数w_change_method来选择不 */
    /*  同的惯性权重衡量规则来根据群体  */
    /*  历史最优位置更新粒子速度以及位置*/
    int i, j;
    double v, x;

    if (w_change_method == 1)
    { // 线性递减权重法
        w = WMAX - (WMAX - WMIN) / MAXINTERATION * (double)interation;
    }
    else if (w_change_method == 2)
    { // 随机权重法
        w = randval(0.4, 0.6);
    }
    else if (w_change_method == 3)
    { // 凹函数递减法
        w = WMAX - (WMAX - WMIN) * ((double)interation / MAXINTERATION) * ((double)interation / MAXINTERATION);
    }
    else if (w_change_method == 4)
    { // 凸函数递减法
        w = WMIN + (WMAX - WMIN) * ((double)interation / MAXINTERATION - 1) * ((double)interation / MAXINTERATION - 1);
    }
    else
    {
        std::cout << "Dont have this weight change method!";
        return;
    }

    for (i = 0; i < NVARS; i++)
    {
        for (j = 0; j < POPSIZE; j++)
        {
            v = w * particles[j].v[i] + c1 * randval(0, 1) * (particles[j].pBest[i] - particles[j].x[i]) + c2 * randval(0, 1) * (gBest[i] - particles[j].x[i]);
            if (v > vmax)
                particles[j].v[i] = vmax; // 速度上限
            else if (v < (-vmax))
                particles[j].v[i] = (-vmax); // 速度下限
            else
                particles[j].v[i] = v;

            x = particles[j].x[i] + particles[j].v[i]; // 更新位置

            if (x > particles[j].upper[i])
                particles[j].x[i] = particles[j].upper[i]; // 位置上限
            else if (x < particles[j].lower[i])
                particles[j].x[i] = particles[j].lower[i]; // 位置下限
            else
                particles[j].x[i] = x;
        }
    }
}

/// <summary>
/// 更新粒子群的全局最佳位置
/// </summary>
/// <param name=""></param>
void fit(void)
{
    int i, j;
    double gval = evalfunc(gBest);
    double pval[POPSIZE];

    for (i = 0; i < POPSIZE; i++)
    {
        pval[i] = evalfunc(particles[i].pBest);

        if (gval > pval[i])
        {
            for (j = 0; j < NVARS; j++)
            {
                gBest[j] = particles[i].pBest[j]; // 搜寻记录粒子群中处于最佳位置的粒子的位置
            }
        }
    }
}