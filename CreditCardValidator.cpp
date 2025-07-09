#include <bits/stdc++.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include <cstring>

using namespace std;

int main()
{
    long long CreditNumber;
    int attempts = 0;          // Counter for attempts
    const int maxAttempts = 3; // Maximum allowed attempts

    while (attempts < maxAttempts)
    {
        cout << "Enter Credit Card Number: ";
        cin >> CreditNumber;

        if (CreditNumber <= 0)
        {
            cout << "Invalid input. Please enter a positive credit card number." << endl;
            attempts++;
            continue; // Skip to the next iteration if input is invalid
        }

        long long pcc = CreditNumber;
        int Sum = 0;
        int Count1 = 0;
        long long divisor = 10;
        char types[20];

        // Condition 1
        while (pcc > 0)
        {
            int LastDigit = pcc % 10;
            Sum = Sum + LastDigit;
            pcc = pcc / 100;
        }

        // Condition 2
        pcc = CreditNumber / 20;
        while (pcc > 0)
        {
            int LastSecDigit = pcc % 10;
            int Db = LastSecDigit * 2;
            Sum = Sum + (Db % 10) + (Db / 10);
            pcc = pcc / 100;
        }

        // Length of Card Number
        pcc = CreditNumber;
        while (pcc != 0)
        {
            pcc = pcc / 10;
            Count1++;
        }
        for (int i = 0; i < Count1 - 2; i++)
        {
            divisor = divisor * 10;
        }

        int FirstDigit = CreditNumber / divisor;
        int FirstTwo = CreditNumber / (divisor / 10);

        // if divisible then valid
        if (Sum % 10 == 0)
        {
            if (FirstDigit == 4 && (Count1 == 13 || Count1 == 16))
            {
                cout << "VALID\n";
                cout << "CARD TYPE: ";
                strcpy(types, "VISA");
            }
            else if ((FirstTwo == 34 || FirstTwo == 37) && Count1 == 15)
            {
                cout << "VALID\n";
                cout << "CARD TYPE: ";
                strcpy(types, "AMERICAN EXPRESS");
            }
            else if ((50 < FirstTwo && FirstTwo < 56) && Count1 == 16)
            {
                cout << "VALID\n";
                cout << "CARD TYPE: ";
                strcpy(types, "MASTERCARD");
            }
            else
            {
                cout << "VALID\n";
                strcpy(types, "OTHER CARD TYPE");
            }
            cout << types << endl;
            break; // Exit the loop if the card is valid
        }
        else
        {
            strcpy(types, "INVALID");
            cout << types << endl;
            attempts++; // Increment attempts for invalid card
        }
    }

    if (attempts == maxAttempts)
    {
        cout << "Maximum attempts reached Reconfirm then come back. Exiting." << endl;
    }

    return 0;
}
