#include <iostream>
#include <string>
#include <unordered_map>
#include <bitset>
#include <cmath>// 包含 log2 函數
#include <chrono>

#include <fstream>
#include <sstream>
using namespace std;

int ceilLog2_atcgmap_Count; //全域變數字母表數量
int atcgStr1_Count, atcgStr2_Count;

enum DNA_Base {
    A = 0, // 對應數字 0
    T = 1, // 對應數字 1
    C = 2, // 對應數字 2
    G = 3  // 對應數字 3
};
int base_lookup(char base) {
    switch (base) {
    case 'A': return A;
    case 'T': return T;
    case 'C': return C;
    case 'G': return G;
    default: throw std::invalid_argument("Invalid DNA base");
    }
}
unsigned long long generate_WitnessBit(int m, int n, int sliceIndex) {
    unsigned long long result = 0;
    int patternSize = m + 1;  // 模式的長度，例如 100 的長度是 3
    int totalBits = patternSize * (n);  // 總位數，即模式長度乘以重複次數
    int bitsPerSlice = 64;  // 每個切片的位數（如果需要支持其他位數，這裡可以修改）

    // 計算應該跳過的位數
    int startBit = sliceIndex * bitsPerSlice;//4<<3 |4

    // 計算模式在切片中的起始位置
    int currentBit = startBit;

    int i;
    // 生成模式並填充到 result 中
    for (i = 0; i < totalBits; ++i) {
        // 如果當前位是在模式中的 '1' 的位置
        if ((currentBit - (currentBit / patternSize) * patternSize) == 0) {
            result |= (1ULL << (i));
        }
        currentBit++;

    }
    result = result << m;
    
    //unsigned long long result = 0;
    //int patternSize = m + 1;  // 模式的長度，例如 100 的長度是 3
    //int totalBits = patternSize * (n);  // 總位數，即模式長度乘以重複次數
    //int bitsPerSlice = 64;  // 每個切片的位數（如果需要支持其他位數，這裡可以修改）21一切片，不要乘開?

    return result;
}

unsigned long long convert_BinaryStringToULL(const std::string& binaryStr) {
    unsigned long long result = 0;
    for (char c : binaryStr) {
        result = (result << 1) | (c - '0');
    }
    return result;
}


int popcount(unsigned long long i) {
    i = i - ((i >> 1) & 0x5555555555555555ULL);
    i = (i & 0x3333333333333333ULL) + ((i >> 2) & 0x3333333333333333ULL);
    return (int)((((i + (i >> 4)) & 0xF0F0F0F0F0F0F0FULL) * 0x101010101010101ULL) >> 56);
}

int calculateHammingDistance(const string& bitStr1, const string& bitStr2) {
    int hammingDistance = 0;
    unsigned long long B_WitnessBit, BinvWitnessBit, num1, num2, xorResult;
    int m = ceilLog2_atcgmap_Count; // 幾個0
    int n = atcgStr1_Count; // 100重複幾次

    int totalSlices = (n * (m + 1) + 63) / 64; // 計算需要分成多少個60位切片

    string BinvWitnessBit_str, B_WitnessBit_str, chunk1, chunk2;

    for (int i = 0; i < totalSlices; ++i) {
        B_WitnessBit = generate_WitnessBit(m, n, i);

        BinvWitnessBit = ~B_WitnessBit;

        chunk1 = bitStr1.substr(i * 64, 64);
        chunk2 = bitStr2.substr(i * 64, 64);

        num1 = convert_BinaryStringToULL(chunk1);
        num2 = convert_BinaryStringToULL(chunk2);
        //num1 = strtoll(chunk1.c_str(), NULL, 10);
        //num2 = strtoll(chunk2.c_str(), NULL, 10);

        xorResult = num1 ^ num2; // XOR 運算

        B_WitnessBit = generate_WitnessBit(m, n, i);
        BinvWitnessBit = ~B_WitnessBit;
        hammingDistance += popcount((xorResult + BinvWitnessBit) & B_WitnessBit);
    }

    return hammingDistance;
}

int main() {
    unordered_map<char, unsigned char> atcgmapToBit = {
        {'A', 0},
        {'T', 1},
        {'C', 2},
        {'G', 3}
    };
    size_t atcgmap_Count = atcgmapToBit.size();

    double log2_atcgmap_Count = log2(static_cast<double>(atcgmap_Count));
    ceilLog2_atcgmap_Count = static_cast<int>(ceil(log2_atcgmap_Count));

    //string atcgStr1 = "GGTCGAGTGAAATCTTTGAGAAGAAGGGGGGAACCTCTGTAGTTTGTGCTACTGCTAAGGGAGG";//64
    //string atcgStr2 = "GGTCGAGTGAAATCTTTGAGAAGAAGGGGGGAACCTCTGTAGTTTGTGCTACTGCTAAGGGAGG";//64

    //string atcgStr1 = "TGTAACATTGTTCTAGGGACAAGCGGGAAGCA";//32
    //string atcgStr2 = "TGTAACATTGTTCTAGGGACAAGCGGGAAGCA";//32 

    //string atcgStr1 = "AAAATTTTCCCCGGGG";// 16/12
    //string atcgStr2 = "ATCGATCGATCGATCG";// 16

    //string atcgStr1 = "ATCTGTAG";//8
    //string atcgStr2 = "ATCGATCG";//8 

    //string atcgStr1 = "AAAA";//12
    //string atcgStr2 = "ATCG";//12
    // 
    // 檔案名稱
    string filename1 = "d_4.txt";
    string filename2 = "d_4.txt";

    ifstream infile1(filename1);       // 以讀取模式打開檔案
    ifstream infile2(filename2);       // 以讀取模式打開檔案

    // 檢查檔案是否成功打開
    if (!infile1) {
        cerr << "無法打開檔案 " << filename1 << endl;
        return 1;
    }

    // 檢查檔案是否成功打開
    if (!infile2) {
        cerr << "無法打開檔案 " << filename2 << endl;
        return 1;
    }

    stringstream buffer1;
    stringstream buffer2;

    buffer1 << infile1.rdbuf();  // 將檔案內容讀入到 stringstream 中
    buffer2 << infile2.rdbuf();  // 將檔案內容讀入到 stringstream 中

    string atcgStr1 = buffer1.str();  // 將 stringstream 中的內容轉換為 string
    string atcgStr2 = buffer2.str();

    atcgStr1_Count = atcgStr1.length();
    atcgStr2_Count = atcgStr2.length();

    string bitStr1, bitStr2;
    //read str to ULL
    int buffer1Sizes = (atcgStr1_Count+21-1) / 21;//無條件進位 分為n串 1串處理21字元
    unsigned long long* atcgBitBuffer1 = new unsigned long long[buffer1Sizes];

    int jSize = (atcgStr1_Count <21)?atcgStr1_Count: 21 ;
    for (int i = 0; i < buffer1Sizes; i ++) {
        unsigned long long kk=1;
        for (int j = 0; j < jSize; j++) {
            char base = atcgStr1[i * jSize + j];
            unsigned long long baseBit=base_lookup(base)&0b111;
            if (i != 0)
                kk = baseBit;
            else
                kk = (kk << 3)| baseBit;
            //kk = (kk << ((i != 0) ? 3:0)) &baseBit;
            bitset<12> baseBitbinary(baseBit);
            bitset<12> kkbinary(kk);
            cout << base << ":" << baseBitbinary << endl;
            cout << "kk" << ":" << kkbinary << endl;
            
        }
        cout << kk;
        atcgBitBuffer1[i] = kk;
    }

    int buffer2Sizes = (atcgStr2_Count + 21 - 1) / 21;//無條件進位 分為n串 1串處理21字元
    unsigned long long* atcgBitBuffer2 = new unsigned long long[buffer2Sizes];

    int jSize = (atcgStr2_Count < 21) ? atcgStr2_Count : 21;
    for (int i = 0; i < buffer2Sizes; i++) {
        unsigned long long kk = 1;
        for (int j = 0; j < jSize; j++) {
            char base = atcgStr1[i * jSize + j];
            unsigned long long baseBit = base_lookup(base) & 0b111;
            if (i != 0)
                kk = baseBit;
            else
                kk = (kk << 3) | baseBit;
            //kk = (kk << ((i != 0) ? 3:0)) &baseBit;
            bitset<12> baseBitbinary(baseBit);
            bitset<12> kkbinary(kk);
            cout << base << ":" << baseBitbinary << endl;
            cout << "kk" << ":" << kkbinary << endl;

        }
        cout << kk;
        atcgBitBuffer2[i] = kk;
    }

    /*for (char base : atcgStr1) {
        if (atcgmapToBit.find(base) != atcgmapToBit.end()) {
            bitStr1 += atcgmapToBit[base];
        }
        else {
            cout << "無效字符: " << base << endl;
            return 1;
        }
    }*/


    //for (char base : atcgStr2) {
    //    if (atcgmapToBit.find(base) != atcgmapToBit.end()) {
    //        bitStr2 += atcgmapToBit[base];
    //    }
    //    else {
    //        cout << "無效字符: " << base << endl;
    //        return 1;
    //    }
    //}

    //if (bitStr1.length() != bitStr2.length()) {
    //    cout << "字符串長度不匹配，無法計算漢明距離" << endl;
    //    return 1;
    //}

    //double at = 0;
    //int j = 0;
    //for (j = 0; j < 1; j++) {
    //    auto start = chrono::high_resolution_clock::now();
    //    int hammingDistance = calculateHammingDistance(bitStr1, bitStr2);
    //    auto end = chrono::high_resolution_clock::now();
    //    // 輸出結果
    //    //cout << atcgStr1<< " 的二進制表示: " << bitStr1 << endl;
    //    //cout << atcgStr2<< " 的二進制表示: " << bitStr2 << endl;
    //    cout << "漢明距離: " << hammingDistance << endl;

    //    // 計算經過的時間
    //    chrono::duration<double> duration = end - start;

    //    // 輸出結果（以秒為單位）
    //    //cout << "程式執行時間: " << duration.count() << " s" << endl;
    //    at += duration.count();
    //}
    //cout << "長度為 " << atcgStr1.length() << " time : " << at << endl;
    return 0;

}