#include <iostream>
#include <vector>
#include <map>
#include <unordered_map>
using namespace std;

bool containsOnlyUniqueValues(const vector<int>& data) {
    for(size_t i = 1; i < data.size(); i++) {
        if(data.at(i) == data.at(i-1)) {
            return false;
        }
    }
    return true;
}

int modeCompute(const vector<int>& data) {
    int i = 0;
    int modeFrequency = 0;
    int modus;
    while(i <= data.size() - 1) {
        int runLength = 1;
        int runValue = data.at(i);
        while(i + runLength <= data.size() - 1 && data.at(i + runLength) == runValue) {
            runLength++;
        }
        if(runLength > modeFrequency) {
            modus = runValue;
            modeFrequency = runLength;
        }
        i += runLength;
    }
    return modus;
}

int modusByHashMap(const vector<int>& data) {
    map<int, int> frequencyMap;
    for(const int item : data) {
        frequencyMap[item]++;
    }

    int modus;
    int modeFrequency = 0;
    for(const auto& [number, frequency] : frequencyMap) {
        if(frequency > modeFrequency) {
            modus = number;
            modeFrequency = frequency;
        }
    }
    return modus;
}

int fibonaciFinder(int n) {
    if(n <= 1) {
        return n;
    }
    return fibonaciFinder(n - 1) + fibonaciFinder(n - 2);
}


int main() {
    vector<int> data = {1,1,1,2,2,3,3,3,4,4,4,4,0,1,0,1,0,1,5,5,5,5,5,5,5,5,9,9,3,4,5,2,1,2,2,2};

    sort(data.begin(), data.end());
    cout << modeCompute(data) << endl;
    cout << modusByHashMap(data) << endl;
    cout << fibonaciFinder(40) << endl;
    return 0;
}
