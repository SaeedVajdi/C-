#include <iostream>
#include <chrono>
#include <pthread.h>

using namespace std;
pthread_t pthread[5];
int n;

class Sort {
public:
    static void bubbleSort(int array[], int count) {
        int temp;
        for (int i = 0; i < count - 1; ++i) {
            for (int j = 0; j < count - i - 1; ++j) {
                if (array[j] > array[j + 1]) {
                    temp = array[j];
                    array[j] = array[j + 1];
                    array[j + 1] = temp;
                }
            }
        }
    }

    static void insertionSort(int array[], int count) {
        for (int i = 1; i < count; i++) {
            int temp = array[i];
            int j = i - 1;
            while ((j > -1) && (array[j] > temp)) {
                array[j + 1] = array[j];
                j--;
            }
            array[j + 1] = temp;
        }
    }

    static void selectionSort(int array[], int count) {
        int minIndex, temp;
        for (int i = 0; i < count - 1; i++) {
            minIndex = i;
            for (int j = i + 1; j < count; j++)
                if (array[j] < array[minIndex]) {
                    minIndex = j;
                }
            if (minIndex != i) {
                temp = array[i];
                array[i] = array[minIndex];
                array[minIndex] = temp;

            }
        }
    }

    static void mergeSort0(int array[], int l, int m, int r) {
        int i, j, k, nl, nr;
        nl = m - l + 1;
        nr = r - m;
        int la[nl], ra[nr];
        for (i = 0; i < nl; i++)
            la[i] = array[l + i];
        for (j = 0; j < nr; j++)
            ra[j] = array[m + 1 + j];
        i = 0;
        j = 0;
        k = l;
        while (i < nl && j < nr) {
            if (la[i] <= ra[j]) {
                array[k] = la[i];
                i++;
            } else {
                array[k] = ra[j];
                j++;
            }
            k++;
        }
        while (i < nl) {
            array[k] = la[i];
            i++;
            k++;
        }
        while (j < nr) {
            array[k] = ra[j];
            j++;
            k++;
        }
    }

    static void mergeSort(int array[], int l, int r) {
        if (l < r) {
            int m = l + (r - l) / 2;
            mergeSort(array, l, m);
            mergeSort(array, m + 1, r);
            mergeSort0(array, l, m, r);
        }
    }

    static int partition(int a[], int l, int r) {
        int pivot = a[r];
        int P_index = l;
        int i, t;
        for (i = l; i < r; i++) {
            if (a[i] <= pivot) {
                t = a[i];
                a[i] = a[P_index];
                a[P_index] = t;
                P_index++;
            }
        }
        t = a[r];
        a[r] = a[P_index];
        a[P_index] = t;
        return P_index;
    }

    static void quickSort(int a[], int l, int r) {
        if (l < r) {
            int P_index = partition(a, l, r);
            quickSort(a, l, P_index - 1);
            quickSort(a, P_index + 1, r);
        }
    }
};

void function(void (*func)(int [], int), int array[], int count) {
    auto start = chrono::steady_clock::now();
    func(array, count);
    auto end = chrono::steady_clock::now();
    cout << chrono::duration<double, milli>(end - start).count() << "\t\t";
}

void function(void (*func)(int [], int, int), int array[], int x, int count) {
    auto start = chrono::steady_clock::now();
    func(array, x, count);
    auto end = chrono::steady_clock::now();
    cout << chrono::duration<double, milli>(end - start).count() << "\t\t";
}

void *bSort() {
    int array[n];
    for (int i = 0; i < n; ++i) {
        array[i] = rand()%1000;
    }
    function(Sort::bubbleSort, array, n);
    pthread_exit(nullptr);
    return nullptr;
}

void *sSort() {
    int array[n];
    for (int i = 0; i < n; ++i) {
        array[i] = rand()%1000;
    }
    function(Sort::selectionSort, array, n);
    pthread_exit(nullptr);
    return nullptr;
}

void *iSort() {
    int array[n];
    for (int i = 0; i < n; ++i) {
        array[i] = rand()%1000;
    }
    function(Sort::insertionSort, array, n);
    pthread_exit(nullptr);
    return nullptr;
}

void *mSort() {
    int array[n];
    for (int i = 0; i < n; ++i) {
        array[i] = rand()%1000;
    }
    function(Sort::mergeSort, array, 0, n);
    pthread_exit(nullptr);
    return nullptr;
}

void *qSort() {
    int array[n];
    for (int i = 0; i < n; ++i) {
        array[i] = rand()%1000;
    }
    function(Sort::quickSort, array, 0, n);
    pthread_exit(nullptr);
    return nullptr;
}

int main() {
    cout << "Member      Quick Sort       Merge Sort      Insertion Sort   Selection Sort  Bubble Sort" << endl;
    for (int j = 10000; j <= 50000; j += 1000) {
        int array1[j], array2[j], array3[j], array4[j], array5[j];
        n = j;
        for (int i = 0; i < j; ++i) {
            array1[i] = rand()%1000;
            array2[i] = array1[i];
            array3[i] = array1[i];
            array4[i] = array1[i];
            array5[i] = array1[i];
        }
        cout << j << "\t\t";
        function(Sort::quickSort, array5, 0, j);
        function(Sort::mergeSort, array4, 0, j);
        function(Sort::insertionSort, array2, j);
        function(Sort::selectionSort, array3, j);
        function(Sort::bubbleSort, array1, j);
        cout<<"\npthread\t\t";
        pthread_create(&pthread[4], nullptr, reinterpret_cast<void *(*)(void *)>(bSort), nullptr);
        pthread_create(&pthread[3], nullptr, reinterpret_cast<void *(*)(void *)>(sSort), nullptr);
        pthread_create(&pthread[2], nullptr, reinterpret_cast<void *(*)(void *)>(iSort), nullptr);
        pthread_create(&pthread[1], nullptr, reinterpret_cast<void *(*)(void *)>(mSort), nullptr);
        pthread_create(&pthread[0], nullptr, reinterpret_cast<void *(*)(void *)>(qSort), nullptr);
        for (auto &i : pthread) {
            pthread_join(i, nullptr);
        }
        cout<<"\n------------------------------------------------------------------------------------------"<<endl;
    }

    return 0;
}