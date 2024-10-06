#include <iostream>
#include<Windows.h>
#include <intrin.h>
#include <vector>
#include <chrono>
#include <omp.h>
#include<ctime>


class Matrix {
public:
    int n;
    std::vector<std::vector<int>> data;

    Matrix(int size) : n(size), data(size, std::vector<int>(size, 0)) {}

    // Операція множення матриць
    Matrix operator*(const Matrix& other) {
        Matrix result(n);
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n; ++j) {
                result.data[i][j] = 0;
                for (int k = 0; k < n; ++k) {
                    result.data[i][j] += data[i][k] * other.data[k][j];
                }
            }
        }
        return result;
    }
};


void time_tToSysTime(time_t t, SYSTEMTIME& st);
void measure_time();
void measure_clock();
void measure_system_time();
void measure_precise_system_time();
void measure_tick_count();
void measure_rdtsc();
void measure_qpc();
void measure_chrono();
void measure_omp_wtime();
long long sumArrayT4(int* arr, size_t size);
void absoluteTime(int* arr, size_t size);
int relativeTime(int* arr, size_t size);
void Task1();
void Task2();
void Task3();
void Task4();
void Task5();
void multiplyMatrices(int** A, int** B, int** C, int n);
int** createMatrix(int n);
void initializeMatrix(int** matrix, int n, int value);
void freeMatrix(int** matrix, int n);

int main()
{
    Task5();
}

void Task1() {
    time_t t = 0x7FFFFFFF;
    SYSTEMTIME sysTime;
    time_tToSysTime(t, sysTime);
    //std::cout << "Latest 32bit time is:";
    //std::cout << "Year: " << sysTime.wYear << "\n";
    //std::cout << "Month: " << sysTime.wMonth << "\n";
    //std::cout << "Day: " << sysTime.wDay << "\n";
    //std::cout << "Hour: " << sysTime.wHour << "\n";
    //std::cout << "Minute: " << sysTime.wMinute << "\n";
    //std::cout << "Second: " << sysTime.wSecond << "\n";
    //std::cout << "Milliseconds: " << sysTime.wMilliseconds << "\n";
    //std::cout << "Day of Week: " << sysTime.wDayOfWeek << "\n";
    printf("%4.4d:%2.2d:%2.2d\n", sysTime.wYear, sysTime.wMonth, sysTime.wDay);
}

void time_tToSysTime(time_t t, SYSTEMTIME& st) {
    struct tm timeinfo;

    localtime_s(&timeinfo, &t);

    st.wYear = timeinfo.tm_year + 1900;
    st.wMonth = timeinfo.tm_mon + 1;
    st.wDay = timeinfo.tm_wday;
    st.wHour = timeinfo.tm_hour;
    st.wMinute = timeinfo.tm_min;
    st.wSecond = timeinfo.tm_sec;
    st.wMilliseconds = 0;
    st.wDayOfWeek = timeinfo.tm_wday;
}

void Task2() {
    measure_time();
    measure_clock();
    measure_system_time();
    measure_precise_system_time();
    measure_tick_count();
    measure_rdtsc();
    measure_qpc();
    measure_chrono();
    measure_omp_wtime();
}

void measure_time() {
    time_t start_time, end_time;
    start_time = time(NULL);
    Sleep(1000);
    end_time = time(NULL);
    std::cout << "time() elapsed seconds: " << difftime(end_time, start_time) << " s" << std::endl;
}

void measure_clock() {
    clock_t start_clock = clock();
    Sleep(1000);
    clock_t end_clock = clock();
    std::cout << "clock() elapsed milliseconds: " << 1000.0 * (end_clock - start_clock) / CLOCKS_PER_SEC << " ms" << std::endl;
}

void measure_system_time() {
    FILETIME ft;
    ULARGE_INTEGER start, end;

    GetSystemTimeAsFileTime(&ft);
    start.LowPart = ft.dwLowDateTime;
    start.HighPart = ft.dwHighDateTime;

    Sleep(1000);

    GetSystemTimeAsFileTime(&ft);
    end.LowPart = ft.dwLowDateTime;
    end.HighPart = ft.dwHighDateTime;

    std::cout << "GetSystemTimeAsFileTime() elapsed 100-nanoseconds: " << (end.QuadPart - start.QuadPart) << " *100 ns" << std::endl;
}

void measure_precise_system_time() {
    FILETIME ft;
    ULARGE_INTEGER start, end;

    GetSystemTimePreciseAsFileTime(&ft);
    start.LowPart = ft.dwLowDateTime;
    start.HighPart = ft.dwHighDateTime;

    Sleep(1000);

    GetSystemTimePreciseAsFileTime(&ft);
    end.LowPart = ft.dwLowDateTime;
    end.HighPart = ft.dwHighDateTime;

    std::cout << "GetSystemTimePreciseAsFileTime() elapsed 100-nanoseconds: " << (end.QuadPart - start.QuadPart) << " *100 ns" << std::endl;
}

void measure_tick_count() {
    DWORD start = GetTickCount();
    Sleep(1000);
    DWORD end = GetTickCount();
    std::cout << "GetTickCount() elapsed milliseconds: " << (end - start) << " ms" << std::endl;
}

void measure_rdtsc() {
    unsigned __int64 start = __rdtsc();
    Sleep(1000);
    unsigned __int64 end = __rdtsc();
    std::cout << "__rdtsc() elapsed ticks: " << (end - start) << " ticks" << std::endl;
}

void measure_qpc() {
    LARGE_INTEGER frequency;
    LARGE_INTEGER start, end;

    QueryPerformanceFrequency(&frequency);
    QueryPerformanceCounter(&start);

    Sleep(1000);

    QueryPerformanceCounter(&end);
    std::cout << "QueryPerformanceCounter() elapsed time: " << static_cast<double>(end.QuadPart - start.QuadPart) * 1000.0 / frequency.QuadPart << " ms" << std::endl;
}

void measure_chrono() {
    auto start = std::chrono::high_resolution_clock::now();
    Sleep(1000);
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
    std::cout << "chrono elapsed nanoseconds: " << duration << " ns" << std::endl;
}

void measure_omp_wtime() {
    double start = omp_get_wtime();
    Sleep(1000);
    double end = omp_get_wtime();
    std::cout << "omp_get_wtime() elapsed seconds: " << (end - start) << " s" << std::endl;
}

void Task3() {

    // __rdtsc PART

    int array1[1000];
    int array2[1000];

    // Array filling
    for (int i = 0; i < 1000; i++) {
        array1[i] = i + 1;
    }
    for (int i = 0; i < 1000; i++) {
        array2[i] = i + 1;
    }

    LARGE_INTEGER frequency, start_qpc, end_qpc;
    QueryPerformanceFrequency(&frequency);

    // Calculate __rdtsc elapsed time
    unsigned __int64 start_ticks = __rdtsc();
    int result1 = 0;
    for (int i = 0; i < 1000; i++) {
        result1 += array1[i];
    }
    unsigned __int64 end_ticks = __rdtsc();
    unsigned __int64 ticks_elapsed = end_ticks - start_ticks;

    // QueryPerformanceCounter for a reference time measurement in ms
    QueryPerformanceCounter(&start_qpc);
    int result2 = 0;
    for (int i = 0; i < 1000; i++) {
        result2 += array2[i];
    }
    QueryPerformanceCounter(&end_qpc);

    // Output results
    // 
    // 
    // BUG WITH QPC.
    double qpc_elapsed_ms = static_cast<double>(end_qpc.QuadPart - start_qpc.QuadPart) * 1000.0 / frequency.QuadPart;
    std::cout << "QueryPerformanceCounter() elapsed time: " << qpc_elapsed_ms << " ms" << std::endl;

    // Assuming your CPU frequency (in Hz) is close to the QueryPerformanceFrequency frequency
    double cpu_frequency_ghz = static_cast<double>(frequency.QuadPart) / 1e9; // Convert to GHz
    double elapsed_ms = static_cast<double>(ticks_elapsed) / (cpu_frequency_ghz * 1e6); // Convert ticks to ms

    std::cout << "__rdtsc() elapsed ticks: " << ticks_elapsed << " ticks" << std::endl;
    std::cout << "__rdtsc() elapsed time: " << elapsed_ms << " ms" << std::endl;
}

// DYING ON MY PC
void Task4() {
    size_t size100k = 100000;
    size_t size200k = 200000;
    size_t size300k = 300000;

    // Виділення пам'яті під масиви
    int* arr100k = (int*)malloc(size100k * sizeof(int));
    int* arr200k = (int*)malloc(size200k * sizeof(int));
    int* arr300k = (int*)malloc(size300k * sizeof(int));

    // Ініціалізація масивів
    for (size_t i = 0; i < size100k; ++i) arr100k[i] = 1;
    for (size_t i = 0; i < size200k; ++i) arr200k[i] = 1;
    for (size_t i = 0; i < size300k; ++i) arr300k[i] = 1;

    // Абсолютне вимірювання часу
    std::cout << "Absolute time measurement:\n";
    absoluteTime(arr100k, size100k);
    absoluteTime(arr200k, size200k);
    absoluteTime(arr300k, size300k);

    // Відносне вимірювання часу
    std::cout << "\nRelative time measurement:\n";
    int iterations100k = relativeTime(arr100k, size100k);
    int iterations200k = relativeTime(arr200k, size200k);
    int iterations300k = relativeTime(arr300k, size300k);

    // Порівняння результатів
    std::cout << "\nComparisons (Relative method):\n";
    std::cout << "T(200000)/T(100000): " << static_cast<double>(iterations100k) / iterations200k << "\n";
    std::cout << "T(300000)/T(100000): " << static_cast<double>(iterations100k) / iterations300k << "\n";

    // Звільнення пам'яті
    free(arr100k);
    free(arr200k);
    free(arr300k);

}

long long sumArray(int* arr, size_t size) {
    long long sum = 0;
    for (size_t i = 0; i < size; ++i) {
        sum += arr[i];
    }
    return sum;
}

// Абсолютний вимір часу з omp_get_wtime
void absoluteTime(int* arr, size_t size) {
    double start = omp_get_wtime();
    long long result = sumArray(arr, size);
    double end = omp_get_wtime();
    std::cout << "Sum: " << result << ", Absolute time (omp_get_wtime): " << (end - start) << " seconds\n";
}

// Відносний вимір часу з GetTickCount
int relativeTime(int* arr, size_t size) {
    DWORD start = GetTickCount();
    int iterations = 0;
    long long result = 0;

    // Проводимо операції до моменту, поки не пройдуть 2 секунди
    while (GetTickCount() - start < 2000) { // 2 секунди
        result = sumArray(arr, size);
        ++iterations;
    }

    std::cout << "Sum: " << result << ", Iterations in 2 seconds: " << iterations << "\n";
    return iterations;
}

void Task5() {
    int sizes[] = { 512, 1024, 2048 };

    // Тест для множення без об'єктів
    for (int size : sizes) {
        int** A = createMatrix(size);
        int** B = createMatrix(size);
        int** C = createMatrix(size);

        initializeMatrix(A, size, 1);
        initializeMatrix(B, size, 2);

        auto start = std::chrono::high_resolution_clock::now();
        multiplyMatrices(A, B, C, size);
        auto stop = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);

        std::cout << "Size: " << size << " | Time without objects: " << duration.count() << " ms\n";

        freeMatrix(A, size);
        freeMatrix(B, size);
        freeMatrix(C, size);
    }

    // Тест для множення з об'єктами
    for (int size : sizes) {
        Matrix A(size);
        Matrix B(size);

        for (int i = 0; i < size; ++i) {
            for (int j = 0; j < size; ++j) {
                A.data[i][j] = 1;
                B.data[i][j] = 2;
            }
        }

        auto start = std::chrono::high_resolution_clock::now();
        Matrix C = A * B;
        auto stop = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);

        std::cout << "Size: " << size << " | Time with objects: " << duration.count() << " ms\n";
    }
}
void multiplyMatrices(int** A, int** B, int** C, int n) {
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            C[i][j] = 0;
            for (int k = 0; k < n; ++k) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
}
int** createMatrix(int n) {
    int** matrix = new int*[n];
    for (int i = 0; i < n; ++i) {
        matrix[i] = new int[n];
    }
    return matrix;
}

// Функція для ініціалізації матриці
void initializeMatrix(int** matrix, int n, int value) {
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            matrix[i][j] = value;
        }
    }
}

// Функція для звільнення пам'яті
void freeMatrix(int** matrix, int n) {
    for (int i = 0; i < n; ++i) {
        delete[] matrix[i];
    }
    delete[] matrix;
}
