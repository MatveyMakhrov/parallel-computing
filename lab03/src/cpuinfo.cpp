#include "../include/cpuinfo.h"
#include <iostream>
#include <intrin.h>
#include <iomanip>
#include <cstring>

using namespace std;

static bool bit(int reg, int b) {
    return (reg >> b) & 1;
}

// EAX = 0
void printEAX0() {
    int cpuInfo[4];
    __cpuid(cpuInfo, 0);

    char vendor[13];
    memcpy(vendor + 0, &cpuInfo[1], 4);
    memcpy(vendor + 4, &cpuInfo[3], 4);
    memcpy(vendor + 8, &cpuInfo[2], 4);
    vendor[12] = 0;

    cout << "---------------=============== EAX = 0 ===============---------------\n\n";
    cout << "Number of basic functions = " << cpuInfo[0] << endl;
    cout << "Vendor = " << vendor << endl;
}

// EAX = 1
void printEAX1() {
    int cpuInfo[4];
    __cpuid(cpuInfo, 1);

    int eax = cpuInfo[0];
    int ebx = cpuInfo[1];
    int ecx = cpuInfo[2];
    int edx = cpuInfo[3];

    cout << "\n---------------=============== EAX = 1 ===============---------------\n\n";

    cout << hex << uppercase;
    cout << "EAX info = " << setw(8) << setfill('0') << eax << "h\n";

    int stepping = eax & 0xF;
    int model = (eax >> 4) & 0xF;
    int family = (eax >> 8) & 0xF;
    int type = (eax >> 12) & 0x3;
    int extModel = (eax >> 16) & 0xF;
    int extFamily = (eax >> 20) & 0xFF;

    cout << "    Stepping ID      = " << stepping << "h\n";
    cout << "    Model            = " << model << "h\n";
    cout << "    Family           = " << family << "h\n";
    cout << "    Processor type   = " << type << "h\n";
    cout << "    Extended model   = " << extModel << "h\n";
    cout << "    Extended family  = " << extFamily << "h\n";

    // ===== EBX =====
    cout << "\nEBX info = " << setw(8) << ebx << "h\n";
    cout << "    Brand index = " << (ebx & 0xFF) << "h\n";
    cout << "    CLFLUSH line size = " << ((ebx >> 8) & 0xFF) * 8 << " Bytes\n";
    cout << "    Logical processors = " << ((ebx >> 16) & 0xFF) << endl;
    cout << "    APIC ID = " << ((ebx >> 24) & 0xFF) << endl;

    // ===== EDX =====
    cout << "\nEDX info = " << setw(8) << edx << "h\n";
    cout << "    FPU = " << bit(edx, 0) << endl;
    cout << "    VME = " << bit(edx, 1) << endl;
    cout << "    DE = " << bit(edx, 2) << endl;
    cout << "    PSE = " << bit(edx, 3) << endl;
    cout << "    TSC = " << bit(edx, 4) << endl;
    cout << "    MSR = " << bit(edx, 5) << endl;
    cout << "    PAE = " << bit(edx, 6) << endl;
    cout << "    MCE = " << bit(edx, 7) << endl;
    cout << "    CX8 = " << bit(edx, 8) << endl;
    cout << "    APIC = " << bit(edx, 9) << endl;
    cout << "    SEP = " << bit(edx, 11) << endl;
    cout << "    MTRR = " << bit(edx, 12) << endl;
    cout << "    PGE = " << bit(edx, 13) << endl;
    cout << "    MCA = " << bit(edx, 14) << endl;
    cout << "    CMOV = " << bit(edx, 15) << endl;
    cout << "    PAT = " << bit(edx, 16) << endl;
    cout << "    PSE36 = " << bit(edx, 17) << endl;
    cout << "    CLFSH = " << bit(edx, 19) << endl;
    cout << "    DS = " << bit(edx, 21) << endl;
    cout << "    ACPI = " << bit(edx, 22) << endl;
    cout << "    MMX = " << bit(edx, 23) << endl;
    cout << "    FXSR = " << bit(edx, 24) << endl;
    cout << "    SSE = " << bit(edx, 25) << endl;
    cout << "    SSE2 = " << bit(edx, 26) << endl;
    cout << "    SS = " << bit(edx, 27) << endl;
    cout << "    HTT = " << bit(edx, 28) << endl;

    // ===== ECX =====
    cout << "\nECX info = " << setw(8) << ecx << "h\n";
    cout << "    SSE3 = " << bit(ecx, 0) << endl;
    cout << "    PCLMULQDQ = " << bit(ecx, 1) << endl;
    cout << "    SSSE3 = " << bit(ecx, 9) << endl;
    cout << "    FMA3 = " << bit(ecx, 12) << endl;
    cout << "    SSE4.1 = " << bit(ecx, 19) << endl;
    cout << "    SSE4.2 = " << bit(ecx, 20) << endl;
    cout << "    POPCNT = " << bit(ecx, 23) << endl;
    cout << "    AES = " << bit(ecx, 25) << endl;
    cout << "    AVX = " << bit(ecx, 28) << endl;
    cout << "    RDRAND = " << bit(ecx, 30) << endl;

    cout << dec;
}

// EAX = 4 (CACHE FULL)
void printCache() {
    int cpuInfo[4];

    cout << "\n---------------=============== EAX = 4 ===============---------------\n\n";

    for (int i = 0;; i++) {
        __cpuidex(cpuInfo, 4, i);

        int eax = cpuInfo[0];
        if ((eax & 0x1F) == 0) break;

        cout << "New cache info portion: "
             << hex << cpuInfo[0] << ":"
             << cpuInfo[1] << ":"
             << cpuInfo[2] << ":"
             << cpuInfo[3] << dec << endl;

        int type = eax & 0x1F;
        int level = (eax >> 5) & 0x7;

        if (type == 1) cout << "    Data cache\n";
        else if (type == 2) cout << "    Instruction cache\n";
        else if (type == 3) cout << "    Unified cache\n";

        int line = (cpuInfo[1] & 0xFFF) + 1;
        int part = ((cpuInfo[1] >> 12) & 0x3FF) + 1;
        int ways = ((cpuInfo[1] >> 22) & 0x3FF) + 1;
        int sets = cpuInfo[2] + 1;

        int size = line * part * ways * sets;

        cout << "    Cache level = " << level << endl;
        cout << "    Line size = " << line << " Bytes\n";
        cout << "    Ways = " << ways << endl;
        cout << "    Sets = " << sets << endl;
        cout << "    Cache size = " << size / 1024 << " KB\n\n";
    }
}

// EAX = 7
void printEAX7() {
    int cpuInfo[4];
    __cpuidex(cpuInfo, 7, 0);

    cout << "\n---------------=============== EAX = 7, ECX = 0 ===============---------------\n\n";

    cout << "EBX info:\n";
    cout << "    AVX2 = " << bit(cpuInfo[1], 5) << endl;
    cout << "    AVX512 = " << bit(cpuInfo[1], 16) << endl;
    cout << "    SHA = " << bit(cpuInfo[1], 29) << endl;

    cout << "\nECX info:\n";
    cout << "    GFNI = " << bit(cpuInfo[2], 8) << endl;
}

// BRAND
void printBrand() {
    int cpuInfo[4];
    char brand[49];

    for (int i = 0; i < 3; i++) {
        __cpuid(cpuInfo, 0x80000002 + i);
        memcpy(brand + i * 16, cpuInfo, 16);
    }

    brand[48] = 0;

    cout << "\n------------============ BRAND ============------------\n\n";
    cout << brand << endl;
}