#include <algorithm>
#include <chrono>
#include <iostream>
#include <vector>
#include <list>
#include <set>

std::vector<int> results_global;

template <typename T>
std::vector<T> intersection(std::vector<T>& v1, std::vector<T>& v2)
{
    auto result = std::vector<T>{};
    result.reserve(v1.size());
    for (const auto& x1 : v1)
    {
        auto it = std::find(v2.begin(), v2.end(), x1);
        if (it != v2.end())
            result.push_back(*it);
    }
    return result;
}

template <typename T>
std::list<T> intersection(std::list<T>& v1, std::list<T>& v2)
{
    auto result = std::list<T>{};
    for (const auto& x1 : v1)
    {
        auto it = std::find(v2.begin(), v2.end(), x1);
        if (it != v2.end())
            result.push_back(*it);
    }
    return result;
}

template <typename T>
std::set<T> intersection(std::set<T>& v1, std::set<T>& v2)
{
    auto result = std::set<T>{};
    for (const auto& x1 : v1)
    {
        auto it = std::find(v2.begin(), v2.end(), x1);
        if (it != v2.end())
            result.insert(*it);
    }
    return result;
}

template <typename T>
std::vector<T> intersection_erase(std::vector<T>& v1, std::vector<T>& v2)
{
    auto result = std::vector<T>{};
    result.reserve(v1.size());
    for (const auto& x1 : v1)
    {
        auto it = std::find(v2.begin(), v2.end(), x1);
        if (it != v2.end())
        {
            result.push_back(*it);
            v2.erase(it);
        }
    }
    return result;
}

template <typename T>
std::list<T> intersection_erase(std::list<T>& v1, std::list<T>& v2)
{
    auto result = std::list<T>{};
    for (const auto& x1 : v1)
    {
        auto it = std::find(v2.begin(), v2.end(), x1);
        if (it != v2.end())
        {
            result.push_back(*it);
            v2.erase(it);
        }
    }
    return result;
}

template <typename T>
std::set<T> intersection_erase(std::set<T>& v1, std::set<T>& v2)
{
    auto result = std::set<T>{};
    for (const auto& x1 : v1)
    {
        auto it = std::find(v2.begin(), v2.end(), x1);
        if (it != v2.end())
        {
            result.insert(*it);
            v2.erase(it);
        }
    }
    return result;
}

template <typename T>
void fill(std::vector<T>& v, unsigned size, T element = {})
{
    v.reserve(size);
    for (; size > 0; size--, element++)
        v.push_back(element);
}

template <typename T>
void fill(std::list<T>& v, unsigned size, T element = {})
{
    for (; size > 0; size--, element++)
        v.push_back(element);
}

template <typename T>
void fill(std::set<T>& v, unsigned size, T element = {})
{
    for (; size > 0; size--, element++)
        v.insert(element);
}

template <typename Container, typename T>
auto TEST_CONTAINER_ERASE(std::size_t size_1, std::size_t size_2, std::size_t common)
{
    Container first; fill(first, size_1);
    Container second; fill(second, size_2, T(size_1-common));
    auto t1 = std::chrono::high_resolution_clock::now();
    auto result = intersection_erase(first, second);
    auto t2 = std::chrono::high_resolution_clock::now();
    results_global.push_back(result.size());
    return std::chrono::duration_cast<std::chrono::microseconds>( t2 - t1 ).count();
}

template <typename Container, typename T>
auto TEST_CONTAINER_NO_ERASE(std::size_t size_1, std::size_t size_2, std::size_t common)
{
    Container first; fill(first, size_1);
    Container second; fill(second, size_2, T(size_1-common));
    auto t1 = std::chrono::high_resolution_clock::now();
    auto result = intersection(first, second);
    auto t2 = std::chrono::high_resolution_clock::now();
    results_global.push_back(result.size());
    return std::chrono::duration_cast<std::chrono::microseconds>( t2 - t1 ).count();
}

template <typename T>
void TEST(std::size_t size_1, std::size_t size_2, std::size_t common)
{
    auto v_ms       = TEST_CONTAINER_ERASE<std::vector<T>, T>(size_1, size_2, common);
    auto v_we_ms    = TEST_CONTAINER_NO_ERASE<std::vector<T>, T>(size_1, size_2, common);
    auto l_ms       = TEST_CONTAINER_ERASE<std::list<T>, T>(size_1, size_2, common);
    auto l_we_ms    = TEST_CONTAINER_NO_ERASE<std::list<T>, T>(size_1, size_2, common);
    auto s_ms       = TEST_CONTAINER_ERASE<std::set<T>, T>(size_1, size_2, common);
    auto s_we_ms    = TEST_CONTAINER_NO_ERASE<std::set<T>, T>(size_1, size_2, common);

    std::cout << sizeof(T) << "\t" << size_1 << "\t"
        << size_2 << "\t" << common << "\t"
        << v_ms << "\t" << v_we_ms << "\t"
        << l_ms << "\t" << l_we_ms << "\t"
        << s_ms << "\t" << s_we_ms << std::endl;
}

template <typename T>
void TEST_STRUCT()
{
    TEST<T>(10, 10, 5);
    TEST<T>(100, 100, 20);
    TEST<T>(100, 100, 80);
    TEST<T>(1000, 1000, 80);
    TEST<T>(1000, 1000, 700);
    TEST<T>(10000, 1000, 200);
    TEST<T>(10000, 1000, 800);
    TEST<T>(10000, 10000, 2000);
    TEST<T>(10000, 10000, 8000);
}

struct comparator {
    comparator() = default;
    comparator(int i) : id{i} {}
    bool operator==(const comparator& x) const { return id == x.id; }
    bool operator!=(const comparator& x) const { return not operator==(x); }
    bool operator<(const comparator& x) const { return id < x.id; }
    comparator& operator++(int) { ++id; return *this; }

    int id{0};
};

struct MediumStruct : public comparator {
    MediumStruct() = default;
    MediumStruct(int i) : comparator(i) {}
    double array[10];
};

struct BigStruct : public comparator {
    BigStruct() = default;
    BigStruct(int i) : comparator(i) {}
    double array[50]; int int_array[10];
};

struct VeryBigStruct : public comparator {
    VeryBigStruct() = default;
    VeryBigStruct(int i) : comparator(i) {}
    double array[400]; int int_array[10];
};

int main() {
    std::cout << "Results:\n ----------------------------------------" << std::endl;
    std::cout << "e_s\tc_1\tc_2\tcommon\tv_e\tv_we\tl_e\tl_we\ts_e\ts_we" << std::endl;
    std::cout << " ----------------------------------------" << std::endl;

    TEST_STRUCT<int>();
    TEST_STRUCT<MediumStruct>();
    TEST_STRUCT<BigStruct>();
    TEST_STRUCT<VeryBigStruct>();

    std::cout << "Legend:\ne_s - single element size in bytes\n"
        "c_1 - size of the first container\n"
        "c_2 - size of the second container\n"
        "common - number of same elements in both containers\n"
        "v_e - result time (in ms) on two vector conatiners with erase method\n"
        "v_we - result time (in ms) on two vector conatiners without erase\n"
        "l_e - result time (in ms) on two list conatiners with erase method\n"
        "l_we - result time (in ms) on two list conatiners without erase\n"
        "s_e - result time (in ms) on two set conatiners with erase method\n"
        "s_we - result time (in ms) on two set conatiners without erase\n";
    std::cout << " ----------------------------------------\n";
}



