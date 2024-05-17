#include "NebulaCore/Log/Log.hpp"
#include "NebulaCore/Profile/Profile.hpp"
#include <algorithm>

// Sample function to print "Hello"
void PrintHello()
{
    NEBULA_PROFILE_FUNCTION();
    NEB_CORE_LOG_INFO("Hello from Nebula Editor");
}

// Sample function to calculate Fibonacci numbers
int FibonacciRecursive(int n)
{
    NEBULA_PROFILE_FUNCTION();
    if (n <= 1)
        return n;
    return FibonacciRecursive(n - 1) + FibonacciRecursive(n - 2);
}

int FibonacciRecursiveNoProfile(int n)
{
    if (n <= 1)
        return n;
    return FibonacciRecursiveNoProfile(n - 1) + FibonacciRecursiveNoProfile(n - 2);
}

int Fibonacci(int n) {
    NEBULA_PROFILE_FUNCTION();

    if (n <= 1) return n;
    int a = 0, b = 1;
    for (int i = 2; i <= n; ++i) {
        int temp = a + b;
        a = b;
        b = temp;
    }
    return b;
}

// Sample function to perform a bubble sort
void BubbleSort(std::vector<int>& arr)
{
    NEBULA_PROFILE_FUNCTION();
    bool swapped;
    for (size_t i = 0; i < arr.size() - 1; ++i)
    {
        swapped = false;
        for (size_t j = 0; j < arr.size() - i - 1; ++j)
        {
            if (arr[j] > arr[j + 1])
            {
                std::swap(arr[j], arr[j + 1]);
                swapped = true;
            }
        }
        if (!swapped)
            break;
    }
}

// Sample function to simulate heavy computation
void HeavyComputation()
{
    NEBULA_PROFILE_FUNCTION();
    std::vector<int> v(10000);
    std::generate(v.begin(), v.end(), std::rand);
    BubbleSort(v);
}

int main(int argc, char** argv)
{
    Nebula::Log::Init();
    NEBULA_PROFILE_ADD_INSTRUMENTORS();
    NEBULA_PROFILE_RECURSION_LIMIT(1);

    NEBULA_PROFILE_BEGIN_SESSION("Startup", "NebulaProfile-Startup.json");
    NEB_CORE_LOG_INFO("Nebula Editor Started");
    PrintHello();
    NEBULA_PROFILE_END_SESSION();

    // Run Fibonacci calculation and log the result
    NEBULA_PROFILE_BEGIN_SESSION("Fibonacci", "NebulaProfile-Fibonacci.json");
    int fibResultRecursive = FibonacciRecursive(20);
    NEB_CORE_LOG_INFO("FibonacciRecursive(20) = {0}", fibResultRecursive);
    {
        NEBULA_PROFILE_SCOPE("FibonacciRecursiveNoProfile");
        int fibResultRecursiveNoProfile = FibonacciRecursiveNoProfile(20);
    }
    int fibResult = Fibonacci(20);
    NEB_CORE_LOG_INFO("Fibonacci(20) = {0}", fibResult);
    NEBULA_PROFILE_END_SESSION();

    // Run BubbleSort and log the result
    NEBULA_PROFILE_BEGIN_SESSION("Sorting", "NebulaProfile-Sorting.json");
    std::vector<int> arr = {5, 2, 9, 1, 5, 6};
    BubbleSort(arr);
    NEB_CORE_LOG_INFO("Sorted array: ");
    for (int num : arr) {
        NEB_CORE_LOG_INFO("{0}", num);
    }
    NEBULA_PROFILE_END_SESSION();

    // Run heavy computation
    NEBULA_PROFILE_BEGIN_SESSION("HeavyComputation", "NebulaProfile-HeavyComputation.json");
    HeavyComputation();
    NEBULA_PROFILE_END_SESSION();

    NEB_CORE_LOG_INFO("Nebula Editor Shutting Down");
    return 0;
}
