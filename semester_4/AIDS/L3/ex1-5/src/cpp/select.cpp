#include <iostream>
#include <vector>
#include <algorithm>
#include "selects.hpp"
#include "operationCounter.hpp"

using namespace std;
using namespace selects;

void printArray(const vector<int>& arr, const string& label) {
    cout << label << ": ";
    for (int x : arr)
        cout << x << " ";
    cout << "\n";
}

int main(int argc, char* argv[]) {
    
    if (argc != 3) {
        cerr << "Usage: " << argv[0] << " <n> <k>\n";
        return 1;
    }

    int n = stoi(argv[1]);
    int k = stoi(argv[2]);

    vector<int> input(n);
    for (int i = 0; i < n; ++i) {
        cin >> input[i];
    }

    vector<int> original = input;

    OperationCounter::reset();
    int result = selects::Select(input.begin(), input.end(), k);

    if (n <= 30) {
        printArray(original, "Początkowy stan tablicy");
        cout << "Znaleziony element: " << result << "\n";

        vector<int> sorted = original;
        sort(sorted.begin(), sorted.end());
        printArray(sorted, "Posortowany ciąg");
        cout << "Spodziewana statystyka pozycyjna (k=" << k << "): " << sorted[k - 1] << "\n";
    }

    cout << "\n[Select] Porównania: " << OperationCounter::comparisons
         << ", Przestawienia: " << OperationCounter::swaps << "\n";

    return 0;
}