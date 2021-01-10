#include <stdio.h>
#include <string>
#include <vector>
#include <iostream>
#include <windows.h>

typedef unsigned char u8;
typedef unsigned long ulong;
static ulong s_last = 0;
static u8* s_pAll = NULL;
static std::vector<ulong> s_vecPrime;

bool renewCurrentPrime(ulong& prime)
{
    while (prime < s_last) {
        ++prime;
        if (s_pAll[prime - 1] == 1)
            return true;
    }
    return false;
}

void showDetails()
{
    ulong pos = 0;
    while (true) {
        if (pos >= s_vecPrime.size())
            break;
        std::cout << s_vecPrime[pos++];
        if (pos < s_vecPrime.size())
            std::cout << ",";
        if (pos % 500 == 0) {
            printf("\n %u primes listed, %u left to show [c] continue; <q> quit: ", pos, s_vecPrime.size() - pos);
            std::string strInput;
            getline(std::cin, strInput);
            if (strInput == "q")
                break;
            std::cout << std::endl;
        }
        if (pos % 100 == 0)
            std::cout << std::endl;
    }
}

int main()
{
    printf(" Eratosthenes sieve: a method to find out all primes below the number that you specify here please: ");
    std::string strInput;
    getline(std::cin, strInput);
    s_last = strtoul(strInput.c_str(), 0, 10);
    if (s_last <= 2) {
        printf("\n Wrong input.\n");
        return 0;
    }
    printf("\n Only the sum of all primes needed [y/n](y as default): ");
    getline(std::cin, strInput);
    bool bDetail = (strInput == "n");
    if (bDetail)
        printf("\n Start to work out all primes below %u...\n", s_last);
    else
        printf("\n Start to work out the sum of all primes below %u...\n", s_last);
    DWORD tickBegin = GetTickCount();
    s_pAll = new u8[s_last];
    if (!s_pAll) {
        printf("Lack of memory.\n");
        return 0;
    }
    
    ulong sum = 0;
    ulong curPrime = 1;
    memset(s_pAll, 1, s_last);
    s_pAll[0] = 0; // set 1 non-prime
    while (true) {
        renewCurrentPrime(curPrime);
        ++sum;
        if (bDetail)
            s_vecPrime.push_back(curPrime);
        if (curPrime * curPrime > s_last)
            break;
        for (int idx = curPrime - 1; idx <= s_last - 1; idx += curPrime) {
            s_pAll[idx] = 0;
        }
    }
    /// pick up all the left primes
    for (int idx = curPrime; idx < s_last; ++idx) {
        if (s_pAll[idx] == 1) {
            ++sum;
            if (bDetail)
                s_vecPrime.push_back(idx + 1);
        }
    }
    printf(" %u primes found in %u milliseconds.\n\n", sum, GetTickCount() - tickBegin);
    delete []s_pAll;
    if (bDetail)
        showDetails();
    return 0;
}
