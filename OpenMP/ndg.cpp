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

bool isPrime(int num) {
    for (int n = 2; n <= floor(sqrt(num)); n++) {
        if ((num % n) == 0) {
            return false;
        }
    }
    return true;
}

list<int> getPrimes(int M, int N) {
    list<int> allPNumbers;

    int init;
    if (M % 2 == 0) {
        init = M + 1;
        if (M == 2) {
            allPNumbers.push_back(2);
        }
    } else {
        init = M;
    }

    #pragma omp parallel for schedule(dynamic)
    for (int number = init; number <= N; number += 2) {
        if (isPrime(number)) {
            #pragma omp critical
            {
                allPNumbers.push_back(number);
            }
        }
    }
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