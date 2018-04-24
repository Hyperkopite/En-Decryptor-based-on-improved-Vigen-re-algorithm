#include <iostream>
#include <fstream>
#include <stdlib.h>

using namespace std;

int main()
{
    char sub [10];
    while(true)
        {
            cout << "subprocess is running" << endl;
            ifstream in("sub.dat");
            if (!in.is_open())
                {
                    cout << "Failed to open sub.dat" << endl;
                    system("taskkill /f /im Cipher.exe");
                    exit(1);
                }
            in.getline (sub, 10);
            if (sub[0] == 'r')
                {
                    in.close();
                    remove ("sub.dat");
                    system("taskkill /f /im Cipher.exe");
                    system("Cipher.exe");
                    exit(0);
                }
        }
}



