#include <vector>
#include <exception>
#include <iostream>
#include <algorithm>
#include <fstream>
#include <string>

using std::vector;

class Heap{
    private:
    vector<int> data;
    size_t n = 2;
    
    size_t parent(size_t index) const {
        if(index == 0){return 0;}
        return (index - 1) / n;
    }

    void heapify(size_t index){
        size_t largest = index;
        size_t firstChild = n * index + 1;
        for(size_t k = 0; k < n; ++k){
            size_t child = firstChild + k;
            if(child >= data.size()) break;
            if(data[child] > data[largest]){
                largest = child;
            }
        }
        if(largest != index){
            std::swap(data[index], data[largest]);
            heapify(largest);
        }
    }

    void makeHeap(){
        if(data.size() < 2) return;
        long long start = static_cast<long long>((data.size() - 2) / n);
        for(long long i = start; i >= 0; --i){
            heapify(static_cast<size_t>(i));
        }
    }

    public:
    Heap() = default;

    Heap(size_t arity): n(arity) {}

    Heap(size_t arity, const vector<int>& input): n(arity){
        data = input;
        makeHeap();
    }

    Heap(size_t arity, vector<int>&& input): n(arity){
        data = std::move(input);
        makeHeap();
    }

    void insert(const int value){
        size_t currentIndex = data.size();
        data.push_back(value);
        size_t parentIndex = parent(currentIndex);
        while(currentIndex != 0 && data[currentIndex] > data[parentIndex]){
            std::swap(data[currentIndex], data[parentIndex]);
            currentIndex = parentIndex;
            parentIndex = parent(currentIndex);
        }
    }

    int getMax(){
        int maxValue = data[0];
        data[0] = data.back();
        data.pop_back();
        if( data.size() > 1){
            heapify(0);
        }
        return maxValue;
    }

    void print() const {
        for(const int item : data){
            std::cout << item << " ";
        }
        std::cout << "\n";
    }

    void clear() {
        data.clear();
    }

};

static vector<int> readIntegersFromFile(const std::string& filename){
    std::ifstream f(filename);
    vector<int> v;
    int x;
    while(f >> x) v.push_back(x);
    return v;
}

int main(int argc, char* argv[]){
    if(argc < 3) return 0;
    size_t arity = static_cast<size_t>(std::stoul(argv[1]));
    vector<int> input = readIntegersFromFile(argv[2]);
    Heap heap(arity, input);
    heap.print();
    heap.getMax();
    heap.print();
    heap.insert(42);
    heap.print();
    heap.insert(-5);
    heap.print();
    heap.getMax();
    heap.print();
    return 0;
}
