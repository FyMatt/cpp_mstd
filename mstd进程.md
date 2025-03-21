# mstd开发进程

## 2025.3.4

> 完成了 `funtion`、`iterator`、`string` 这三个模块

### `funtion`函数模块(代码案例)

```cpp
#include "mstd/function.hpp"

void exampleFunction(int a,int b,int c) {
    std::cout << "Called with " << a << "," << b << "," << c << std::endl;
}

// 指定函数指针类型，并且传入函数指针
mstd::Function<void(int,int,int)> func(exampleFunction);

// 调用函数对象
if (func) {
    func(1,2,3); // 输出: Called with 1,2,3
} else {
    std::cerr << "Function is not callable" << std::endl;
}
```



### `iterator`迭代器模块(代码案例)

```cpp
#include "mstd/iterator.hpp"

void testIterator(){
    int arr[] = {1, 2, 3, 4,1, 2, 3, 4,1, 2, 3, 4,1, 2, 3, 4,1, 2, 3, 4,1, 2, 3, 4,1, 2, 3, 4,1, 2, 3, 4,1, 2, 3, 4,1, 2, 3, 4,1, 2, 3, 4,1, 2, 3, 4, 5};
    // 先指定迭代器的指针类型，然后传入数组起始地址
    mstd::Iterator<int> it(arr);
    int length = sizeof(arr) / sizeof(arr[0]);
    // 这里就是取得尾部的迭代器
    mstd::Iterator<int> end(arr + length);

    while (it != end) {
        std::cout << *it << " ";
        ++it;
    }
    std::cout << std::endl;
}
```



### `string`字符串模块(代码案例)

```cpp
#include "mstd/string.hpp"

mstd::string str("Hello, World!");
char* c_str = str.c_str();

// 返回字符串长度
int length = str.size();

// 返回字符串内存大小
int capacity = str.capacity();

// 重新设置字符串的内存大小
str.reserve(20);

//重新设置字符串的长度
str.resize(10);
```





## 2025.3.8

### `vector`动态列表模块(代码案例)

```cpp
#include "mstd/vector.hpp"

mstd::vector<int> list = {1,2,3,4,5,6,7,8,9};
for(int i=10; i< 1000;i++){
    list.push_back(i);
}
for(int i=0; i< list.get_size();i++){
    std::cout << list[i] << std::endl;
}
```





## 2025.3.10

### `LockFreeQueue`无锁队列模块(代码案例)

```cpp
//	适用于多线程高并发的基于原子操作的无锁队列，按照普通队列的方式使用即可
//	用于子进程的Rector模型中，多进程监听端口，然后apccept到一个新连接后添加入无锁队列即可
```



## 2025.3.13

### `ThreadPool`线程池模块(代码案例)

```cpp
//	结合无锁队列一起使用
#include "mstd/LockFreeQueue.hpp"
#include "mstd/ThreadPool.hpp"
mstd::LockFreeQueue<int> queue;
mstd::ThreadPool pool(5); // 创建一个包含3个线程的线程池

// 生产者任务
pool.enqueue([&queue] {
    for (int i = 0; i < 1024 * 1; ++i) {
        queue.enqueue(i);
    }
});

// 消费者任务
auto consumerTask = [&queue] {
    while (!queue.empty()) {
        if (auto val = queue.dequeue()) {
            if (!val) break;
            std::cout << "thread ID: " << std::this_thread::get_id() << " dequeue: " << *val << std::endl;
        }
    }
};

pool.enqueue(consumerTask);
pool.enqueue(consumerTask);
pool.enqueue(consumerTask);
pool.enqueue(consumerTask);

// 等待所有任务完成
pool.~ThreadPool();
```



## 2025.3.17

### `FileCache`文件缓存模块(代码案例)

```cpp
mstd::FileCache file_cache(1024 * 1024 * 10); // 10MB 缓存
std::string file_path = "example.txt";
auto result = file_cache.get(file_path);
//	模拟多次请求重复获取相同的文件
for(int i=0;i < 1000;i++){
    result = file_cache.get(file_path);
}
if (result) {
    std::cout << "File content: " << std::string(result->first.begin(), result->first.end()) << std::endl;
    std::cout << "MIME type: " << result->second << std::endl;
} else {
    std::cout << "Failed to load file: " << file_path << std::endl;
}

// 输出缓存命中和未命中次数
std::cout << "Cache hits: " << file_cache.get_cache_hits() << std::endl;
std::cout << "Cache misses: " << file_cache.get_cache_misses() << std::endl;
```



```cpp
//	多线程下的文件缓存使用案例
//	这个锁只是用来控制输出的完整性和顺序性
#include "mstd/ThreadPool.hpp"
#include "mstd/FileCache.hpp"

std::mutex mtx;

void handle_request(mstd::FileCache& file_cache, const std::string& file_path) {
    auto result = file_cache.get(file_path);
    if (result) {
        std::unique_lock<std::mutex> lock(mtx);
        // 模拟返回文件内容
        std::cout << "线程ID: " << std::this_thread::get_id() << std::endl;
        std::cout << "文件内容: " << std::string(result->first.begin(), result->first.end()) << std::endl;
        std::cout << "文件类型: " << result->second << std::endl;
    } else {
        std::cout << "未找到文件: " << file_path << std::endl;
    }
}

int main(){
    mstd::FileCache file_cache(1024 * 1024 * 10); // 10MB 缓存
    std::vector<std::string> file_paths = {"example1.txt", "example2.txt", "example3.txt"};

    // 创建线程池
    mstd::ThreadPool thread_pool(4); // 假设有4个线程

    // 模拟多个用户请求多个文件
    const int num_requests = 2024;
    for (int i = 0; i < num_requests; ++i) {
        thread_pool.enqueue(handle_request, std::ref(file_cache), file_paths[rand() % 3]);
    }

    // 等待所有任务完成
    thread_pool.~ThreadPool();

    // 输出缓存命中和未命中次数
    std::cout << "文件缓存系统命中次数: " << file_cache.get_cache_hits() << std::endl;
    std::cout << "文件缓存系统未命中次数: " << file_cache.get_cache_misses() << std::endl;
}
```

