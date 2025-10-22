# Bond Pricing Calculator

A high-performance C program for calculating bond prices and comprehensive risk metrics used in fixed-income portfolio management and trading.

#### Video Demo: <https://youtu.be/C6xUCh6hoig>

## 📋 Table of Contents

- [Features](#features)
- [Quick Start](#quick-start)
- [Installation](#installation)
- [Usage](#usage)
- [Calculated Metrics](#calculated-metrics)
- [Examples](#examples)
- [Build Script Guide](#build-script-guide)
- [Technical Details](#technical-details)
- [Troubleshooting](#troubleshooting)
- [Contributing](#contributing)
- [License](#license)

---

## ✨ Features

- **Fast**: Single-pass O(n) algorithm with aggressive optimizations
- **Comprehensive**: Calculates price, duration, modified duration, convexity, and DV01
- **Robust**: Extensive input validation with clear error messages
- **Professional**: Formatted output suitable for reports and presentations
- **Portable**: Pure C99 with no external dependencies except standard libraries
- **Memory Efficient**: Stack-only allocation, no heap usage

### Performance Characteristics

- **Time Complexity**: O(n) where n is the number of payment periods
- **Space Complexity**: O(1) constant space
- **Typical Speed**: ~10-50 nanoseconds per cash flow on modern CPUs
- **Optimization Level**: Compiled with `-O3` and `-march=native`

---

## 🚀 Quick Start

### Prerequisites

- C compiler (lang)
- Standard C library with math support
- Unix-like environment (Linux, macOS, WSL on Windows)

### Three-Step Setup

```bash
# 1. Make build script executable
chmod +x build.sh

# 2. Compile the program
./build.sh

# 3. Run your first calculation
./bond_pricer 1000 0.05 0.06 10 2
```

That's it! You should see a detailed bond analysis report.

---

## 📦 Installation

### Method 1: Using the Build Script (Recommended)

1. **Download the files**:
   ```bash
   # Place these files in your project directory:
   # - bond_pricer.c
   # - build.sh
   ```

2. **Make the build script executable**:
   ```bash
   chmod +x build.sh
   ```

3. **Run the build script**:
   ```bash
   ./build.sh
   ```

   The script will:
   - Check for the source file
   - Verify compiler availability
   - Compile with optimal flags
   - Create the executable
   - Run a test calculation

### Method 2: Manual Compilation

#### Basic Compilation
```bash
gcc bond_pricer.c -lm -o bond_pricer
```

#### Optimized Compilation (Recommended)
```bash
gcc -O3 -march=native -Wall -Wextra -std=c99 bond_pricer.c -lm -o bond_pricer
```

#### Using Clang
```bash
clang -O3 -march=native -Wall -Wextra -std=c99 bond_pricer.c -lm -o bond_pricer
```

#### Debug Build
```bash
gcc -g -O0 -Wall -Wextra -std=c99 bond_pricer.c -lm -o bond_pricer_debug
```

### Compilation Used Flags Explained

| Flag | Purpose |
|------|---------|
| `-O3` | Maximum optimization for speed |
| `-march=native` | CPU-specific optimizations (SSE, AVX, etc.) |
| `-Wall` | Enable all compiler warnings |
| `-Wextra` | Enable extra warnings |
| `-std=c99` | Use C99 standard |
| `-lm` | Link the math library |
| `-o bond_pricer` | Specify output executable name |

---

## 💻 Usage

### Command Syntax

```bash
./bond_pricer <face_value> <coupon_rate> <ytm> <years> <frequency>
```

### Parameters

| Parameter | Description | Example | Valid Range |
|-----------|-------------|---------|-------------|
| `face_value` | Principal/par value of the bond | `1000` | > 0 |
| `coupon_rate` | Annual coupon rate (as decimal) | `0.05` (5%) | 0 to 1 |
| `ytm` | Yield to maturity (as decimal) | `0.06` (6%) | ≥ 0 |
| `years` | Years to maturity | `10` | 1 to 100 |
| `frequency` | Payments per year | `2` | 1, 2, 4, or 12 |

### Payment Frequencies

| Value | Frequency | Common Use |
|-------|-----------|------------|
| `1` | Annual | Some government bonds |
| `2` | Semi-annual | US Treasuries, most corporate bonds |
| `4` | Quarterly | Some corporate bonds |
| `12` | Monthly | Mortgage-backed securities |

---

## 📊 Calculated Metrics

### 1. Bond Price
Present value of all future cash flows discounted at the yield to maturity.

**Formula**:
```
Price = Σ(t=1 to n) [C / (1+y)^t] + [F / (1+y)^n]
```

### 2. Macaulay Duration
Weighted average time until cash flows are received (in years).

**Formula**:
```
D_mac = [Σ(t=1 to n) t × PV(CF_t)] / Price
```

**Interpretation**: Measures the bond's interest rate sensitivity and approximate time to recover the initial investment.

### 3. Modified Duration
First-order approximation of price sensitivity to yield changes.

**Formula**:
```
D_mod = D_mac / (1 + y/f)
```

**Interpretation**: A bond with modified duration of 7.5 will decrease ~7.5% in price if yields increase by 1%.

### 4. Convexity
Second-order measure of price-yield relationship curvature.

**Formula**:
```
Convexity = [Σ(t=1 to n) t(t+1) × PV(CF_t)] / [Price × (1+y)²]
```

**Interpretation**: Positive convexity means price increases more when yields fall than it decreases when yields rise (beneficial to bondholders).

### 5. DV01 (Dollar Duration)
Dollar change in bond price for a 1 basis point (0.01%) change in yield.

**Formula**:
```
DV01 = (Modified Duration × Price) / 10,000
```

**Interpretation**: Risk metric for portfolio managers; indicates dollar exposure to interest rate moves.

---

## 📝 Examples

### Example 1: 10-Year Treasury Note
**Scenario**: Standard US Treasury note with semi-annual payments

```bash
./bond_pricer 1000 0.05 0.06 10 2
```

**Output**:
```
═══════════════════════════════════════════════════════════════
                    BOND ANALYSIS REPORT
═══════════════════════════════════════════════════════════════

BOND PARAMETERS:
───────────────────────────────────────────────────────────────
  Face Value              : $1,000.00
  Coupon Rate             : 5.0000% (0.0500)
  Yield to Maturity       : 6.0000% (0.0600)
  Years to Maturity       : 10 years
  Payment Frequency       : 2 times per year
  Total Payments          : 20

PRICING METRICS:
───────────────────────────────────────────────────────────────
  Bond Price              : $926.3990
  Price as % of Par       : 92.6399%

RISK METRICS:
───────────────────────────────────────────────────────────────
  Macaulay Duration       : 7.8856 years
  Modified Duration       : 7.6569 years
  Convexity               : 68.4162
  DV01 (Dollar Duration)  : $0.7094

INTERPRETATION:
───────────────────────────────────────────────────────────────
  • A 1% yield change implies:
    - Price change (duration): $-70.94 (-7.66%)
    - Convexity adjustment   : $3.17
  • Bond is trading at a discount
═══════════════════════════════════════════════════════════════
```

**Analysis**: This bond trades below par (discount) because its coupon rate (5%) is lower than the market yield (6%). The modified duration of 7.66 indicates significant interest rate risk.

---

### Example 2: 5-Year Corporate Bond at Par
**Scenario**: Corporate bond priced at par with annual payments

```bash
./bond_pricer 1000 0.04 0.04 5 1
```

**Key Insights**:
- Price = $1,000 (at par, since coupon rate = YTM)
- Lower duration than Example 1 (shorter maturity)
- Lower convexity

---

### Example 3: 30-Year Premium Bond
**Scenario**: Long-term bond trading at premium with quarterly payments

```bash
./bond_pricer 1000 0.07 0.05 30 4
```

**Key Insights**:
- Price > $1,000 (premium, since coupon rate > YTM)
- High duration (long maturity, high rate sensitivity)
- Significant convexity benefit

---

### Example 4: Zero-Coupon Bond
**Scenario**: Pure discount bond with no coupon payments

```bash
./bond_pricer 1000 0.0 0.05 10 1
```

**Key Insights**:
- Maximum duration (equals maturity for zero-coupon bonds)
- Price = Present value of face value only
- Highest interest rate sensitivity

---

### Example 5: High-Yield Bond
**Scenario**: Junk bond with high coupon, monthly payments

```bash
./bond_pricer 1000 0.12 0.15 7 12
```

**Key Insights**:
- Trading at discount (high yield required by market)
- Monthly cash flows reduce duration
- Higher default risk not reflected in price (model assumes no default)

---

## 🔧 Build Script Guide

### What the Build Script Does

The `build.sh` script automates the compilation process:

1. ✅ Checks for source file existence
2. ✅ Verifies compiler installation and version
3. ✅ Compiles with optimal performance flags
4. ✅ Applies executable permissions automatically
5. ✅ Displays usage examples
6. ✅ Runs a test calculation

### Using the Build Script

#### Basic Usage
```bash
./build.sh
```

#### Using a Different Compiler
```bash
CC=clang ./build.sh
```

#### Environment Variables

| Variable | Default | Purpose |
|----------|---------|---------|
| `CC` | `gcc` | Specify compiler (gcc, clang, etc.) |

### Build Script Output

The script provides color-coded feedback:

- **🔵 Blue**: Informational messages and headers
- **🟡 Yellow**: Commands and important details
- **🟢 Green**: Success messages
- **🔴 Red**: Error messages

---

## 🔬 Technical Details

### Algorithm Complexity

**Time Complexity**: O(n)
- Single pass through all payment periods
- Cannot be improved (must process each cash flow)

**Space Complexity**: O(1)
- Fixed memory usage regardless of input
- No dynamic allocation
- All data on the stack

### Optimization Techniques

1. **Single-Pass Calculation**: All metrics computed in one loop
2. **Precomputation**: Invariant values calculated once before the loop
3. **Multiplicative Updates**: Avoid expensive division and exponentiation
4. **Inline Functions**: Eliminate function call overhead
5. **Const Correctness**: Enable compiler optimizations
6. **Cache Locality**: Related data stored together in structs

### Memory Layout

```
Stack Frame (~200 bytes):
├── BondParams (40 bytes)
│   ├── face_value (8)
│   ├── coupon_rate (8)
│   ├── ytm (8)
│   ├── periods (4)
│   └── frequency (4)
└── BondMetrics (48 bytes)
    ├── price (8)
    ├── macaulay_duration (8)
    ├── modified_duration (8)
    ├── convexity (8)
    ├── dv01 (8)
    └── yield_to_maturity (8)
```

### Input Validation

The program performs comprehensive validation:

- **Type Checking**: Ensures numeric inputs
- **Range Validation**: Enforces logical bounds
- **Error Detection**: Uses `errno` to catch overflow/underflow
- **Clear Messages**: Provides specific guidance on errors

### Floating-Point Considerations

- Uses IEEE 754 double precision (64-bit)
- Precision: ~15-17 decimal digits
- Range: ~10^-308 to 10^308
- Sufficient for financial calculations (pennies on millions of dollars)

---

## 🐛 Troubleshooting

### Common Errors and Solutions

#### 1. "Source file not found"

**Problem**: The build script can't locate `bond_pricer.c`

**Solution**:
```bash
ls -la bond_pricer.c  # Verify file exists
pwd                    # Check current directory
```

Ensure both `bond_pricer.c` and `build.sh` are in the same directory.

---

#### 2. "gcc: command not found"

**Problem**: C compiler not installed

**Solutions**:

**Ubuntu/Debian**:
```bash
sudo apt-get update
sudo apt-get install build-essential
```

**macOS**:
```bash
xcode-select --install
```

**Fedora/RHEL/CentOS**:
```bash
sudo dnf install gcc
```

**Windows (WSL)**:
```bash
sudo apt-get install build-essential
```

---

#### 3. "Permission denied" when running build.sh

**Problem**: Build script lacks execute permission

**Solution**:
```bash
chmod +x build.sh
```

---

#### 4. "undefined reference to 'pow'" or "undefined reference to 'sqrt'"

**Problem**: Math library not linked

**Solution**: Ensure `-lm` flag comes **after** the source file:
```bash
gcc bond_pricer.c -lm -o bond_pricer  # Correct
gcc -lm bond_pricer.c -o bond_pricer  # Wrong!
```

---

#### 5. "Invalid coupon rate" with correct input

**Problem**: Using percentage instead of decimal

**Solution**: Enter rates as decimals:
```bash
./bond_pricer 1000 5 6 10 2     # Wrong (5% as integer)
./bond_pricer 1000 0.05 0.06 10 2  # Correct (5% as 0.05)
```

---

#### 6. Unexpected Results or Precision Issues

**Problem**: Extreme parameter values or numerical instability

**Checks**:
- Verify inputs are reasonable (YTM < 100%, maturity < 100 years)
- Ensure coupon rate and YTM are in decimal form
- Check for typos in command-line arguments

---

### Platform-Specific Issues

#### macOS: "xcrun: error: invalid active developer path"

**Solution**: Install Xcode Command Line Tools:
```bash
xcode-select --install
```

#### Windows (using WSL): "Cannot execute binary file"

**Solution**: Ensure you're using WSL (Linux subsystem), not Windows CMD/PowerShell:
```bash
wsl  # Enter WSL environment
./build.sh
```

---

## 🧪 Testing

### Unit Tests

Create a test script `test_bond_pricer.sh`:

```bash
#!/bin/bash

echo "Running Bond Pricer Tests..."

# Test 1: Bond at par
echo "Test 1: Bond at par (coupon = YTM)"
./bond_pricer 1000 0.05 0.05 10 2

# Test 2: Zero-coupon bond
echo "Test 2: Zero-coupon bond"
./bond_pricer 1000 0.0 0.06 10 1

# Test 3: Premium bond
echo "Test 3: Premium bond"
./bond_pricer 1000 0.07 0.05 15 2

# Test 4: Input validation
echo "Test 4: Invalid input (should fail)"
./bond_pricer 1000 1.5 0.06 10 2

echo "Tests complete!"
```

Run tests:
```bash
chmod +x test_bond_pricer.sh
./test_bond_pricer.sh
```

---

## 📚 Mathematical Background

### Bond Pricing Theory

The fundamental principle: **A bond's price equals the present value of its future cash flows**.

**Cash Flows**:
- Periodic coupon payments: C = Face Value × (Coupon Rate / Frequency)
- Final principal repayment: Face Value at maturity

**Discount Rate**: Yield to Maturity (YTM) represents the market's required return.

### Duration Interpretation

**Macaulay Duration**:
- Time-weighted average of cash flows
- Approximates the investment payback period
- Expressed in years

**Modified Duration**:
- Percentage price change for 1% yield change
- Linear approximation of price-yield relationship
- Always positive for bonds

**Rule of Thumb**: For small yield changes (< 100 bps), price change ≈ -Modified Duration × Δy

### Convexity Benefits

Positive convexity means:
- Bonds gain more value when yields fall than they lose when yields rise
- Provides cushion against interest rate volatility
- More valuable in volatile interest rate environments

---

## 🔄 Advanced Usage

### Performance Benchmarking

Create a benchmark script:

```bash
#!/bin/bash

echo "Benchmarking bond_pricer..."

# Compile with profiling
gcc -O3 -march=native -pg bond_pricer.c -lm -o bond_pricer_profile

# Run multiple iterations
time for i in {1..10000}; do
    ./bond_pricer_profile 1000 0.05 0.06 30 2 > /dev/null
done

# Generate profile report
gprof bond_pricer_profile gmon.out > profile_report.txt

echo "Profile report saved to profile_report.txt"
```

---

### Integration with Python

Create a Python wrapper:

```python
import subprocess
import json

def price_bond(face_value, coupon_rate, ytm, years, frequency):
    """Calculate bond metrics using C program."""
    cmd = ['./bond_pricer', str(face_value), str(coupon_rate),
           str(ytm), str(years), str(frequency)]

    result = subprocess.run(cmd, capture_output=True, text=True)

    if result.returncode != 0:
        raise ValueError(f"Bond pricer error: {result.stderr}")

    # Parse output (customize based on your needs)
    return result.stdout

# Example usage
output = price_bond(1000, 0.05, 0.06, 10, 2)
print(output)
```

---

## ⚡ Quick Reference

### Command Template
```bash
./bond_pricer <face_value> <coupon_rate> <ytm> <years> <frequency>
```

### Common Bond Types

| Bond Type | Example Command |
|-----------|-----------------|
| 10Y Treasury | `./bond_pricer 1000 0.05 0.06 10 2` |
| 30Y Treasury | `./bond_pricer 1000 0.04 0.045 30 2` |
| Corporate (5Y) | `./bond_pricer 1000 0.06 0.065 5 2` |
| Zero-Coupon | `./bond_pricer 1000 0.0 0.05 10 1` |
| High-Yield | `./bond_pricer 1000 0.10 0.12 7 4` |

---

**Happy Bond Pricing! 📈**
