#include <iostream>
#include <chrono>
#include <iomanip>
#include <thread>
#include "mstd/ThreadPool.hpp"
#include "mstd/FileCache.hpp"
#include "mstd/vector.hpp"
#include "mstd/string.hpp"
#include "mstd/LockFreeQueue.hpp"


int main() {
    system("chcp 65001 > nul");
    auto start = std::chrono::high_resolution_clock::now();

    
    // 在此插入测试代码
    
    
    auto finish = std::chrono::high_resolution_clock::now();
    // 计算时间差
    std::chrono::duration<double, std::milli> duration = finish - start;
    // 输出结果，保留六位小数
    std::cout << std::fixed << std::setprecision(6);
    std::cout << "耗时：" << duration.count() << "ms" << std::endl;
    system("pause");
    return 0;
}