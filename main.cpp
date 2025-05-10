#include <iostream>
#include <chrono>
#include <iomanip>
#include <thread>
#include "mstd/threadPool.hpp"
#include "mstd/fileCache.hpp"
#include "mstd/vector.hpp"
#include "mstd/string.hpp"
#include "mstd/lockFreeQueue.hpp"
#include "mstd/yaml.hpp"


int main() {
    system("chcp 65001 > nul");
    auto start = std::chrono::high_resolution_clock::now();

    
    // 在此插入测试代码
    mstd::YamlReader reader("test.yaml");
    auto server = reader.getObject("server");
    std::string host = server.getValue<std::string>("host");
    int port = server.getValue<int>("port");
    auto features = server.getObject("features");
    std::cout << "Server_Features_enable_feature_x:" << features.getValue<bool>("enable_feature_x") << std::endl;
    std::cout << "Server_Features_enable_feature_y:" << features.getValue<bool>("enable_feature_y") << std::endl;
    
    std::cout << "Server_Host: " << host << std::endl;
    std::cout << "Server_Port: " << port << std::endl;

    auto database = reader.getObject("database");
    std::string db_host = database.getValue<std::string>("host");
    int db_port = database.getValue<int>("port");
    std::string db_user = database.getValue<std::string>("username");
    std::string db_password = database.getValue<std::string>("password");
    std::string db_type = database.getValue<std::string>("type");
    std::cout << "Database_Host: " << db_host << std::endl;
    std::cout << "Database_Port: " << db_port << std::endl;
    std::cout << "Database_User: " << db_user << std::endl;
    std::cout << "Database_Password: " << db_password << std::endl;
    std::cout << "Database_Type: " << db_type << std::endl;

    
    
    auto finish = std::chrono::high_resolution_clock::now();
    // 计算时间差
    std::chrono::duration<double, std::milli> duration = finish - start;
    // 输出结果，保留六位小数
    std::cout << std::fixed << std::setprecision(6);
    std::cout << "耗时：" << duration.count() << "ms" << std::endl;
    system("pause");
    return 0;
}