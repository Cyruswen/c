#include <iostream>
#include <string.h>

class Solution {
public:
    string PrintMinNumber(vector<int> numbers) {
        size_t size = numbers.size;
        for(size_t i = 0; i < size; i++){
            string s;
            itoa(numbers[i], s, 10);
        }
            
    }

};
