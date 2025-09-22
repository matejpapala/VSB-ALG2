#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
using namespace std;

std::vector<int> readIntegersFromFile(const std::string &filename)
{
    std::ifstream file(filename);
    std::vector<int> numbersVec;

    if (!file.is_open())
    {
        std::cerr << "Unable to open file: " << filename << std::endl;
        return numbersVec;
    }

    std::string line;

    // change if to while to read the whole file
    if (std::getline(file, line))
    {
        std::stringstream lineAsStream(line);
        int num;
        while (lineAsStream >> num)
        { // NOTE: this assumes that the input file has the data we want
            numbersVec.push_back(num);
        }
    }

    file.close();
    return numbersVec;
}

void binarySearchPrint(const vector<int>& data, const vector<int>& targets) {
    for(const int target : targets) {
        int left = 0;
        int right = data.size() - 1;
        bool found = false;
        while(left <= right) {
            int mid = left + (right - left) / 2;
            if(data.at(mid) == target) {
                cout<< target <<": T" << endl;
                found = true;
                break;
            }
            else if(data.at(mid) < target) {
                left = mid + 1;
            }
            else {
                right = mid - 1;
            }
        }
        if(!found) {
            cout<< target <<": F" << endl;
        }
    }
}

int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        std::cerr << "Not enough arguments\n";
        return 1;
    }
    std::string data_file = argv[1];
    std::string numbers_file = argv[2];
    auto data = readIntegersFromFile(data_file);
    auto numbers = readIntegersFromFile(numbers_file);
    sort(data.begin(), data.end());

    binarySearchPrint(data, numbers);
    return 0;
}