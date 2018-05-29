#include <iostream>
#include <string>
#include <unistd.h>

using namespace std;
int main()
{
    string s1, s2;
    cin >> s2;
    while (cin >> s1)
        s2 = s1  + " " + s2;
    cout << s2 << endl;
    return 0;
}
