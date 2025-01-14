#include <iostream>
#include <thread>
using namespace std;

void Method1() {
    int x = 10;
    x += 5;
    cout << "Hello from the first thread.\n" << endl;
}
void Method2() {
    int y = 10;
    y += 5;
    cout << "Hello from the second thread.\n" << endl;
}

int main() {
    thread myThread1(Method1);
    thread myThread2(Method2);
    myThread1.join();
    myThread2.join();
}
