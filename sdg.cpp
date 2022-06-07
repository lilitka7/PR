#include <iostream>
#include <omp.h>
#include <math.h>
#include <list>

using namespace std;

void showPrimes(int m, int n, list<int> primes) {
    int i = 0;
    
    cout << "W zakresie od " << m << " do " << n << " znaleziono " << primes.size() << " liczb pierwszych:" << endl;
    
    for(auto num : primes) {
        cout << num << " ";
        if(i == 9) {
            cout << endl;
            i = 0;
        } else {
            ++i;
        }
    }

    if(i != 0) {
        cout << endl;
    }
}

inline void sitCrossOut(int n, bool isPrimeNumber[], int M, int N) {
    int init = ceil(M / (float)n);
    for (int mulnum = (init == 1 ? 2 : init) * n; mulnum <= N; mulnum += n) {
        if (mulnum >= M) {
            #pragma omp critical
            {
                isPrimeNumber[mulnum - M] = false;
            }
        }
    }
}

list<int> getPrimes(int M, int N) {
    const int arrSize = N - M + 1;
    bool* isPrimeNumber = (bool*)calloc(arrSize, sizeof(bool));
    fill_n(isPrimeNumber, arrSize, true);
    list<int> allPNumbers;
    
    const int limit = (int)floor(sqrt(N));
    #pragma omp parallel
    {
        #pragma omp single nowait
        {
            sitCrossOut(2, isPrimeNumber, M, N);
        }
        
        #pragma omp for schedule(dynamic)
        for (int n = 3; n <= limit; n += 2) {
            sitCrossOut(n, isPrimeNumber, M, N);
        }

        #pragma omp for
        for (int i = 0; i < N - M + 1; ++i) {
            if (isPrimeNumber[i]) {
                #pragma omp critical
                {
                    allPNumbers.push_back(M + i);
                }
            }
        }
    }

    free(isPrimeNumber);

    return allPNumbers;
}

int main(int argc, char *argv[]) {
    int m, n;
    bool showResult = false;
    list<int> result;

    if(argc < 3) {
        cerr << "Please, provide at least 2 integers (m, n values)" << endl;
        return -1;
    }

    m = atoi(argv[1]);
    n = atoi(argv[2]);

    if(argc >= 4) {
        omp_set_num_threads(
            atoi(argv[3])
        );

        if(argc >= 5) {
            showResult = atoi(argv[4]);
        }
    }

    result = getPrimes(m, n);
    
    if(showResult) {
        showPrimes(m, n, result);
    }

    return 0;
}