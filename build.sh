#!/bin/bash

# Bond Pricing Calculator - Build Script
# This script compiles the bond pricer with optimal settings

# Color codes for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# Configuration
SOURCE_FILE="bond_pricer.c"
OUTPUT_FILE="bond_pricer"
CC="${CC:-clang}"  # Use environment CC or default to gcc

echo -e "${BLUE}═══════════════════════════════════════════════════════════${NC}"
echo -e "${BLUE}    Bond Pricing Calculator - Compilation Script${NC}"
echo -e "${BLUE}═══════════════════════════════════════════════════════════${NC}"
echo ""

# Check if source file exists
if [ ! -f "$SOURCE_FILE" ]; then
    echo -e "${RED}Error: Source file '$SOURCE_FILE' not found!${NC}"
    echo "Please ensure bond_pricer.c is in the current directory."
    exit 1
fi

echo -e "${YELLOW}Compiler:${NC} $CC"
echo -e "${YELLOW}Source:${NC}   $SOURCE_FILE"
echo -e "${YELLOW}Output:${NC}   $OUTPUT_FILE"
echo ""

# Check compiler version
echo -e "${BLUE}Checking compiler version...${NC}"
$CC --version | head -n 1
echo ""

# Compilation options explained:
# -O3              : Maximum optimization (speed)
# -march=native    : Optimize for your specific CPU architecture
# -Wall            : Enable all warning messages
# -Wextra          : Enable extra warning messages
# -Werror          : Treat warnings as errors (strict mode)
# -std=c99         : Use C99 standard
# -lm              : Link math library
# -o               : Output file name

echo -e "${BLUE}Compiling with optimization flags...${NC}"
echo -e "${YELLOW}Command:${NC} $CC -O3 -march=native -Wall -Wextra -std=c99 $SOURCE_FILE -lm -o $OUTPUT_FILE"
echo ""

# Compile the program
if $CC -O3 -march=native -Wall -Wextra -std=c99 "$SOURCE_FILE" -lm -o "$OUTPUT_FILE"; then
    echo -e "${GREEN}✓ Compilation successful!${NC}"
    echo ""

    # Display file information
    if [ -f "$OUTPUT_FILE" ]; then
        FILE_SIZE=$(ls -lh "$OUTPUT_FILE" | awk '{print $5}')
        echo -e "${GREEN}Executable created:${NC} $OUTPUT_FILE ($FILE_SIZE)"

        # Make executable
        chmod +x "$OUTPUT_FILE"
        echo -e "${GREEN}Made executable:${NC} chmod +x applied"
        echo ""

        # Display usage
        echo -e "${BLUE}═══════════════════════════════════════════════════════════${NC}"
        echo -e "${GREEN}Usage Examples:${NC}"
        echo -e "${BLUE}═══════════════════════════════════════════════════════════${NC}"
        echo ""
        echo -e "${YELLOW}1. Basic usage (help):${NC}"
        echo "   ./$OUTPUT_FILE"
        echo ""
        echo -e "${YELLOW}2. 10-year bond, 5% coupon, 6% YTM, semi-annual:${NC}"
        echo "   ./$OUTPUT_FILE 1000 0.05 0.06 10 2"
        echo ""
        echo -e "${YELLOW}3. 5-year bond, 3% coupon, 2.5% YTM, annual:${NC}"
        echo "   ./$OUTPUT_FILE 1000 0.03 0.025 5 1"
        echo ""
        echo -e "${YELLOW}4. 30-year bond, 4.5% coupon, 5% YTM, quarterly:${NC}"
        echo "   ./$OUTPUT_FILE 1000 0.045 0.05 30 4"
        echo ""
        echo -e "${BLUE}═══════════════════════════════════════════════════════════${NC}"
        echo -e "${GREEN}Parameters:${NC}"
        echo "  1. Face value (e.g., 1000)"
        echo "  2. Coupon rate as decimal (e.g., 0.05 for 5%)"
        echo "  3. YTM as decimal (e.g., 0.06 for 6%)"
        echo "  4. Years to maturity (e.g., 10)"
        echo "  5. Frequency (1=annual, 2=semi-annual, 4=quarterly, 12=monthly)"
        echo ""

        # Try to run a test
        echo -e "${BLUE}═══════════════════════════════════════════════════════════${NC}"
        echo -e "${YELLOW}Running test calculation...${NC}"
        echo -e "${BLUE}═══════════════════════════════════════════════════════════${NC}"
        ./"$OUTPUT_FILE" 1000 0.05 0.06 10 2

    fi
else
    echo -e "${RED}✗ Compilation failed!${NC}"
    echo ""
    echo "Common issues:"
    echo "  1. Missing gcc/compiler: sudo apt-get install build-essential"
    echo "  2. Missing math library: Ensure -lm flag is included"
    echo "  3. Syntax errors: Check source code"
    exit 1
fi

echo ""
echo -e "${GREEN}Build script completed successfully!${NC}"
exit 0
