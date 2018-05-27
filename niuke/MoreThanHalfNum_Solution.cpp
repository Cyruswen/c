#include <stdio.h>

class Solution {
public:
    int MoreThanHalfNum_Solution(vector<int> numbers) {
        size_t size = numbers.size();
        for(size_t i = 0; i < size; i++){
            int tmp = 0;
            size_t count = 0;
            for(size_t j = 0; j < size; j++){
                tmp = numbers[i];
                if(numbers[j] == tmp){
                    count ++;
                }
            }
            if(count > size / 2){
                return tmp;
            }
        }
        return 0;
    }
};
