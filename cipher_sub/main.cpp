#include <iostream>

using namespace std;

int main()
{
    cout << "done." << endl;
    system("taskkill /f /im Cipher.exe");
    system("Cipher.exe");
    return 0;
}



