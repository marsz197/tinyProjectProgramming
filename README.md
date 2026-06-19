# Tiny Projects Implementation

This repository contains the completed C++ implementations for the **Linear System & Machine Learning** module (Project 1 & 1b) and the **Smart E-Commerce System** module (Project 2).
Our Group consist of 2 members:
Lê Tấn Trường - 104240110
Đặng Ngọc Minh - 104240011
## Project Structure

### Project 1a (`project1/project1.cpp`)
Contains custom `Vector` and `Matrix` classes implemented from scratch, bypassing standard library mathematical dependencies.
- Features dynamic memory allocation, deep copying, and comprehensive operator overloading.
- Implements 1-based indexing as requested.
- Implements linear algebra routines like `determinant()`, `inverse()`, `transpose()`, and `pseudoInverse()` using Gaussian elimination.

### Project 1b (`project1/project1b.cpp`)
Extends Project 1 to solve linear regression problems (specifically predicting CPU performance using the UCI Computer Hardware dataset).
- Uses both Moore-Penrose Pseudo-Inverse and a custom **Tikhonov Regularization** method to solve $Ax=b$.
- Parses `machine.data` dynamically, splits it into an 80% training set and a 20% test set, and reports the Root Mean Square Error (RMSE).

**How to run Project 1b:**
```bash
cd project1
g++ project1b.cpp -o project1b.exe
./project1b.exe
```

### Project 2 (`project2/ecommerce.cpp`)
A fully-featured Object-Oriented Smart E-Commerce System utilizing `nlohmann/json` for persistent data storage across sessions.
- **TUI (Terminal User Interface):** Offers a full-screen, interactive graphical console menu using `<conio.h>` for arrow-key navigation.
- **Roles:** Separates concerns strictly between `Administrator` and `Customer`.
- **Recommendation Engine:** Calculates live recommendation scores via the formula: `score = view_count * 0.2 + purchase_count * 0.8 + category_bonus`.
- **Security:** Input fields are robust against space characters (using `getline`), safe against buffer overflows due to C++ standard typing, and protected against JSON injection as the API sanitizes search parameters into literal string evaluations. 

**How to run Project 2:**
```bash
cd project2
g++ ecommerce.cpp -o ecommerce.exe
./ecommerce.exe
```

## Integration Testing

An automated integration script (`run_tests.cpp`) has been provided to verify end-to-end functionality including JSON persistence, cart modifications, inventory bounds checking, security constraints, duplicate registration failures, and admin powers.

**How to run Tests:**
```bash
cd project2
g++ run_tests.cpp -o run_tests.exe
./run_tests.exe
```

### Test Logs

```text
Product added.
Product added.
Product added.

--- STEP 1: Admin adds/removes products ---
Product added.
Product added.
Product deleted.

--- STEP 2: Register Minh, Browse, Cart, Checkout ---
--- Products ---
P1 | Laptop | $999.99 | Stock: 10
P2 | Mouse | $49.99 | Stock: 50
P3 | Desk | $199.99 | Stock: 5
P4 | Dell Latt 7850 | $299.99 | Stock: 1
Added to cart.
Added to cart.
Removed from cart.
Order placed! Total: $2049.97
--- Your Orders ---
Order ID: O1 | Total: $2049.97 | Status: Completed
  - Laptop x2 ($999.99)
  - Mouse x1 ($49.99)

--- STEP 3: John Recommendation, Carts, Logout ---
--- Recommended for you ---
Insufficient stock! Only 8 left.
Added to cart.
Added to cart.
Removed from cart.

--- STEP 4: John Relog, Cart Persists, Checkout ---
Order placed! Total: $5999.94

--- STEP 5: Admin Stats, History, Updates ---
Total Users: 3
Total Products: 4
Total Orders: 2
Active Users: 2
Best selling: Laptop (8 sold)
--- All Orders ---
Order ID: O1 | Customer: U3 | Total: $2049.97
  - Laptop x2
  - Mouse x1
Order ID: O2 | Customer: U2 | Total: $5999.94
  - Laptop x6
  - Desk x0
Product updated.
--- Active Users ---
Active: john (ID: U2)
Active: Minh (ID: U3)
Password updated successfully.

--- STEP 6: Minh Relog (new pass), Security tests ---
Not found!

--- ALL TESTS PASSED SUCCESSFULLY! ---
```