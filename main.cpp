#include <iomanip>
#include <iostream>
#include <chrono>

#include "PSO.h"

int main(int argc, char const *argv[])
{
    srand(time(NULL)); // 设置随机种子
    double all_gBest[100];
    int i, j, k;
    int FUNC;
    int w_change_method; // 权重更新方法
    std::cout << "********************************************************************************************************************************" << std::endl;
    std::cout << "***************************  10种" << NVARS << "适应度函数，用4种不同的权重控制方法，运行一百次的平均值及标准差"
              << "  ****************************" << std::endl;
    std::cout << " 适应度函数" << std::setw(8) << "线性递减法" << std::setw(16) << "随机权重法" << std::setw(16) << "凹函数递减法" << std::setw(16) << "凸函数递减法" << std::endl;
    std::cout << "平均值" << std::setw(16) << "标准差" << std::setw(8)
              << "平均值" << std::setw(8) << "标准差" << std::setw(8)
              << "平均值" << std::setw(8) << "标准差" << std::setw(8)
              << "平均值" << std::setw(8) << "标准差" << std::setw(8);
    std::cout << std::endl
              << std::endl;
    for (FUNC = 1; FUNC <= 10; FUNC++)
    {
        std::cout << " " << std::left << "f" << std::setw(4) << FUNC;
        switch (FUNC)
        {
        case 1:
            absbound = 100.0;
            break;
        case 2:
            absbound = 10.0;
            break;
        case 3:
            absbound = 100.0;
            break;
        case 4:
            absbound = 100.0;
            break;
        case 5:
            absbound = 30.0;
            break;
        case 6:
            absbound = 100.0;
            break;
        case 7:
            absbound = 1.28;
            break;
        case 8:
            absbound = 500.0;
            break;
        case 9:
            absbound = 5.12;
            break;
        case 10:
            absbound = 32.0;
            break;
        default:
            break;
        }
        vmax = absbound * 0.15; // 最大速度限制
        for (w_change_method = 1; w_change_method <= 4; w_change_method++)
        { // 遍历四种权重更新方法
            for (j = 0; j < 100; j++)
            {
                initialize(1);
                evaluate(FUNC);
                for (int i = 0; i < MAXINTERATION; i++)
                { // 经历过最大迭代次数后
                    update(i, w_change_method);
                    evaluate(FUNC);
                    fit();
                }
                double evalue = evalfunc(gBest,1);
                if (evalue == 0)
                { // 值错误，重新迭代
                    j--;
                    continue;
                }
                all_gBest[j] = evalue;
            }
            std::cout << std::setw(16) << avg(all_gBest, 100);
            std::cout << std::setw(16) << stddev(all_gBest, 100);
        }
        std::cout << std::endl;
    }
    std::cout << "*********************************************************  算法运行结束  ********************************************************" << std::endl;
    system("pause");
}
