# Credit Card Validator

A command-line C++ application that validates credit card numbers using the **Luhn Algorithm** and identifies the card network (Visa, Mastercard, or American Express).

---

## Table of Contents

- [Overview](#overview)
- [Features](#features)
- [How It Works](#how-it-works)
  - [Luhn Algorithm](#luhn-algorithm)
  - [Card Type Detection](#card-type-detection)
- [Getting Started](#getting-started)
  - [Prerequisites](#prerequisites)
  - [Compilation](#compilation)
  - [Running the Program](#running-the-program)
- [Usage](#usage)
  - [Example Sessions](#example-sessions)
- [Supported Card Types](#supported-card-types)
- [Input Validation & Attempt Limiting](#input-validation--attempt-limiting)
- [Limitations & Known Issues](#limitations--known-issues)
- [Potential Improvements](#potential-improvements)
- [License](#license)

---

## Overview

This program takes a credit card number as input and determines:

1. Whether the number is **mathematically valid** (passes the Luhn checksum).
2. If valid, which **card network** issued the card based on its prefix and length.

It allows up to **3 attempts** before exiting, making it suitable as a lightweight validation utility or a learning reference for the Luhn Algorithm in C++.

---

## Features

- Luhn Algorithm–based checksum validation
- Automatic card network detection (Visa, Mastercard, American Express)
- Input sanitisation — rejects zero and negative numbers
- Attempt counter with a maximum of 3 tries before the program exits
- Clean, single-file implementation with no external dependencies

---

## How It Works

### Luhn Algorithm

The [Luhn Algorithm](https://en.wikipedia.org/wiki/Luhn_algorithm) is the industry-standard checksum formula used to validate card numbers. The steps are:

1. **Starting from the rightmost digit**, sum every digit at an odd position (1st, 3rd, 5th, …).
2. **Starting from the second-to-last digit**, double every digit at an even position (2nd, 4th, 6th, …). If the doubled value exceeds 9, add its two digits together (equivalent to subtracting 9).
3. **Add both sums together**. If the total is divisible by 10, the card number is valid.

The implementation splits this into two loops over the `long long` integer:

```
Condition 1 — odd-position digits (right to left, stepping by 100):
  Sum += (pcc % 10);  pcc /= 100;

Condition 2 — even-position digits (starting one digit in, stepping by 100):
  pcc = CreditNumber / 20;
  Db = (pcc % 10) * 2;
  Sum += (Db % 10) + (Db / 10);  pcc /= 100;
```

### Card Type Detection

After the Luhn check passes, the card type is determined by the **IIN/BIN prefix** and **total digit length**:

| Card Network      | Starting Digits | Card Length |
|-------------------|-----------------|-------------|
| Visa              | 4               | 13 or 16    |
| American Express  | 34 or 37        | 15          |
| Mastercard        | 51–55           | 16          |
| Other             | Any other valid | Any         |

---

## Getting Started

### Prerequisites

- A C++ compiler supporting **C++11** or later (e.g., `g++`, `clang++`, MSVC)
- A terminal / command prompt

### Compilation

```bash
# Using g++
g++ -o credit_card_validator CreditCardValidator.cpp

# Using clang++
clang++ -o credit_card_validator CreditCardValidator.cpp
```

### Running the Program

```bash
./credit_card_validator        # Linux / macOS
credit_card_validator.exe      # Windows
```

---

## Usage

The program runs interactively. Enter the card number as a plain integer (no spaces, dashes, or other separators).

```
Enter Credit Card Number: <your card number here>
```

The output will be one of:

```
VALID
CARD TYPE: VISA

VALID
CARD TYPE: MASTERCARD

VALID
CARD TYPE: AMERICAN EXPRESS

VALID
CARD TYPE: OTHER CARD TYPE

INVALID
```

### Example Sessions

**Valid Visa card:**
```
Enter Credit Card Number: 4111111111111111
VALID
CARD TYPE: VISA
```

**Valid American Express card:**
```
Enter Credit Card Number: 378282246310005
VALID
CARD TYPE: AMERICAN EXPRESS
```

**Invalid card number:**
```
Enter Credit Card Number: 1234567890123456
INVALID
Enter Credit Card Number: ...
```

**Maximum attempts reached:**
```
Enter Credit Card Number: 0000000000000000
INVALID
Enter Credit Card Number: 9999999999999999
INVALID
Enter Credit Card Number: 1234000000000000
INVALID
Maximum attempts reached Reconfirm then come back. Exiting.
```

---

## Supported Card Types

| Card Network      | Prefixes    | Length(s) | Detection Rule                                 |
|-------------------|-------------|-----------|------------------------------------------------|
| Visa              | 4           | 13, 16    | First digit == 4 AND length is 13 or 16        |
| American Express  | 34, 37      | 15        | First two digits are 34 or 37 AND length is 15 |
| Mastercard        | 51–55       | 16        | First two digits between 51–55 AND length is 16|
| Other             | Any         | Any       | Passes Luhn but matches none of the above      |

> **Note:** Mastercard has expanded its IIN range to include 2221–2720 as of 2017. The current implementation only checks the legacy 51–55 range.

---

## Input Validation & Attempt Limiting

| Scenario                        | Behaviour                                  |
|---------------------------------|--------------------------------------------|
| Input is zero or negative       | Prints an error message; increments attempts counter |
| Card fails Luhn check           | Prints `INVALID`; increments attempts counter |
| Card passes Luhn check          | Prints `VALID` and the card type; exits loop |
| 3 consecutive invalid attempts  | Prints an exit message and terminates       |

---

## Limitations & Known Issues

- **Numeric overflow risk** — The card number is stored as `long long` (typically 64-bit). This accommodates all standard card lengths (13–19 digits), but entering extremely large numbers may cause undefined behaviour.
- **No string-based input** — Leading zeros are lost (e.g., entering `0412345678901234` is read as `412345678901234`). Real card numbers do not start with 0, but edge cases may arise.
- **Partial Mastercard support** — Only the legacy 51–55 prefix range is checked; the modern 2221–2720 range is not handled.
- **`bits/stdc++.h` header** — This GCC-specific header is not portable. For cross-platform builds, replace it with individual standard headers (`<iostream>`, `<string>`, `<cstring>`).
- **`char types[20]` buffer** — Using a fixed-size character array with `strcpy` is a common source of buffer overflows. Consider using `std::string` instead.
- **No Discover or UnionPay detection** — Other major networks are classified as `OTHER CARD TYPE`.

---

## Potential Improvements

- Replace `long long` parsing with `std::string` input to preserve leading zeros and support card numbers up to 19 digits.
- Add detection for Discover (prefix 6011, 622126–622925, 644–649, 65) and UnionPay (prefix 62).
- Expand Mastercard detection to include the 2221–2720 prefix range.
- Replace `char types[20]` with `std::string` to eliminate buffer overflow risk.
- Remove the non-portable `#include <bits/stdc++.h>` and use only standard headers.
- Add unit tests covering edge cases such as 13-digit Visa numbers, boundary Mastercard prefixes, and single-digit inputs.
- Accept card numbers as a string argument via `argv` for scripted / non-interactive use.

---

## License

This project is released for educational purposes. No warranty is provided. Feel free to use, modify, and distribute with attribution.
