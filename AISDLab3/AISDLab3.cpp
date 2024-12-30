#include <iostream>
#include <vector>
#include <random>
#include <chrono>
#include <iomanip>


struct stats {
    size_t comparison_count = 0;
    size_t copy_count = 0;
};

stats insertionSort(std::vector<int>& vector) {
    stats st;
    size_t size = vector.size();
    if (size < 2)
        return st;

    for (int i = 1; i < size; i++) {
        int key = vector[i];
        st.copy_count++;
        int j = i - 1;
        while (j >= 0 && key < vector[j]) {
            vector[j + 1] = vector[j];
            j--;
            st.comparison_count++;
            st.copy_count++;
        }
        if (j >= 0)
            st.comparison_count++;
        vector[j + 1] = key;
        st.copy_count++;
    }
    return st;
}

stats shellSort(std::vector<int> & vector) {
    stats st;
    size_t size = vector.size();
    for (int gap = size / 2; gap > 0; gap /= 2) {
        for (int i = gap; i < size; i++) {
            int temp = vector[i];
            st.copy_count++;
            int j = i;
            while (j >= gap && vector[j - gap] > temp) {
                vector[j] = vector[j - gap];
                j -= gap;
                st.copy_count++;
                st.comparison_count++;
            }
            if (j >= gap)
                st.comparison_count++;
            vector[j] = temp;
            st.copy_count++;
        }
    }
    return st;
}

std::vector<std::pair<int, int>> findRuns(const std::vector<int>& arr) {
    std::vector<std::pair<int, int>> runs;
    size_t size = arr.size();
    if (size <= 1) {
        if (size == 1)
            runs.push_back({ 0,0 });
        return runs;
    }

    int start = 0;
    for (int i = 1; i < size; i++) {
        if (arr[i] < arr[i - 1]) {
            runs.push_back({ start, i - 1 });
            start = i;
        }
    }
    runs.push_back({ start, size - 1 });
    return runs;
}

stats merge(std::vector<int>& arr, int left, int mid, int right) {
    stats st;
    int n1 = mid - left + 1;
    int n2 = right - mid;

    std::vector<int> L(n1);
    std::vector<int> R(n2);

    for (int i = 0; i < n1; i++) {
        L[i] = arr[left + i];
        st.copy_count++;
    }
    for (int j = 0; j < n2; j++) {
        R[j] = arr[mid + 1 + j];
        st.copy_count++;
    }

    int i = 0, j = 0, k = left;
    while (i < n1 && j < n2) {
        st.comparison_count++;
        if (L[i] <= R[j]) {
            arr[k] = L[i];
            i++;
            st.copy_count++;
        }
        else {
            arr[k] = R[j];
            j++;
            st.copy_count++;
        }
        k++;
    }

    while (i < n1) {
        arr[k] = L[i];
        i++;
        k++;
        st.copy_count++;
    }
    while (j < n2) {
        arr[k] = R[j];
        j++;
        k++;
        st.copy_count++;
    }
    return st;
}

stats naturalMergeSort(std::vector<int>& arr) {
    stats total_st;
    size_t size = arr.size();
    if (size <= 1)
        return total_st;

    std::vector<std::pair<int, int>> runs;

    do {
        runs = findRuns(arr);

        for (size_t i = 0; i < runs.size(); i += 2) {
            int left = runs[i].first;
            int mid = runs[i].second;
            int right;
            if (i + 1 < runs.size())
                right = runs[i + 1].second;
            else
                right = runs[i].second;

            stats st = merge(arr, left, mid, right);
            total_st.comparison_count += st.comparison_count;
            total_st.copy_count += st.copy_count;
        }
    } while (runs.size() > 1);
    return total_st;
}

///--------------------------2_TASK------------------------------

std::vector<int> generateRandomArray(int size) {
    unsigned seed = static_cast<unsigned>(std::chrono::system_clock::now().time_since_epoch().count());
    std::mt19937 gen(seed);
    std::uniform_int_distribution<> dis(1, size);

    std::vector<int> array(size);
    for (int i = 0; i < size; ++i)
    {
        array[i] = dis(gen);
    }
    return array;
}


std::vector<int> generateSortedArray(int size) {
    std::vector<int> arr(size);
    for (int i = 0; i < size; ++i) {
        arr[i] = i;
    }
    return arr;
}


std::vector<int> generateReverseSortedArray(int size) {
    std::vector<int> arr(size);
    for (int i = 0; i < size; ++i) {
        arr[i] = size - i - 1;
    }
    return arr;
}


stats runSort(std::vector<int> arr, const std::string& sortType) {
    if (sortType == "naturalMergeSort")
        return naturalMergeSort(arr);
    else if (sortType == "insertionSort")
        return insertionSort(arr);
    else if (sortType == "shellSort")
        return shellSort(arr);
    else
        return stats();
}


void printVector(std::vector<int>& vector) {
    for (int i = 0; i < vector.size(); i++) {
        std::cout << vector[i] << ' ';
    }
    std::cout << std::endl;
}


int main() {
    std::vector<int> sizes = { 1000, 2000, 3000, 5000, 10000, 25000, 50000, 100000};
    std::vector<std::string> sortTypes = { "insertionSort", "shellSort", "naturalMergeSort" };
    int numRandomArrays = 100;


    std::cout << "Size,SortType,DataType,AvgComparisons,AvgCopies" << std::endl;

    for (int size : sizes) {
        for (const std::string& sortType : sortTypes) {
            // a) Средние значения для случайных массивов
            long long totalComparisons = 0;
            long long totalCopies = 0;

            for (int i = 0; i < numRandomArrays; ++i) {
                std::vector<int> randomArr = generateRandomArray(size);
                stats st = runSort(randomArr, sortType);
                totalComparisons += st.comparison_count;
                totalCopies += st.copy_count;
            }
            double avgComparisons = static_cast<double>(totalComparisons) / numRandomArrays;
            double avgCopies = static_cast<double>(totalCopies) / numRandomArrays;
            std::cout << size << "," << sortType << ",random," << std::fixed << std::setprecision(0) << avgComparisons << "," << avgCopies << std::endl;

            // b) Для полностью отсортированного массива
            std::vector<int> sortedArr = generateSortedArray(size);
            stats sortedSt = runSort(sortedArr, sortType);
            std::cout << size << "," << sortType << ",sorted," << std::fixed << std::setprecision(0) << sortedSt.comparison_count << "," << sortedSt.copy_count << std::endl;

            // c) Для обратно отсортированного массива
            std::vector<int> reversedArr = generateReverseSortedArray(size);
            stats reversedSt = runSort(reversedArr, sortType);
            std::cout << size << "," << sortType << ",reversed," << std::fixed << std::setprecision(0) << reversedSt.comparison_count << "," << reversedSt.copy_count << std::endl;
        }
    }

    return 0;
}