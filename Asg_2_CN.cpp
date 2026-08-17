
#include <iostream>
using namespace std;

int main() {
    int d1, d2, d3, d4;
    int h[8]; // Using index 1 to 7
    int s1, s2, s3, error;

    // Sender Side
    cout << "===== Sender Side =====" << endl;
    cout << "Enter 4 data bits (D1 D2 D3 D4): ";
    cin >> d4 >> d3 >> d2 >> d1;

    // Generate parity bits (Even Parity)
    h[1] = d1 ^ d2 ^ d4;   // P1
    h[2] = d1 ^ d3 ^ d4;   // P2
    h[3] = d1;
    h[4] = d2 ^ d3 ^ d4;   // P3
    h[5] = d2;
    h[6] = d3;
    h[7] = d4;

    cout << "\nGenerated Hamming Code: ";
    for (int i = 7; i >0; i--)
        cout << h[i] << " ";
    cout << endl;

    // Receiver Side
    cout << "\n===== Receiver Side =====" << endl;
    cout << "Enter the received 7-bit Hamming code:\n";
    for (int i = 7; i > 0; i--)
        cin >> h[i];

    // Calculate syndrome bits
    s1 = h[1] ^ h[3] ^ h[5] ^ h[7];
    s2 = h[2] ^ h[3] ^ h[6] ^ h[7];
    s3 = h[4] ^ h[5] ^ h[6] ^ h[7];

    // Find error position
    error = s1 + (2 * s2) + (4 * s3);

    if (error == 0) {
        cout << "\nNo Error Detected." << endl;
    } else {
        cout << "\nError Detected at Bit Position: " << error << endl;

        // Correct the error
        h[error] = h[error] ^ 1;

        cout << "Corrected Hamming Code: ";
        for (int i = 7; i > 0; i--)
            cout << h[i] << " ";
        cout << endl;
    }

    // Extract original data bits
    cout << "\nOriginal Data Bits: ";
    cout << h[7] << " " << h[6] << " " << h[5] << " " << h[3] << endl;

    return 0;
}