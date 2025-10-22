#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <errno.h>

typedef struct {
    double face_value;
    double coupon_rate;
    double ytm;
    int periods;
    int frequency;
} BondParams;

typedef struct {
    double price;
    double macaulay_duration;
    double modified_duration;
    double convexity;
    double dv01;
    double yield_to_maturity;
} BondMetrics;

/* Fast power function for repeated calculations
static inline double fast_pow(double base, int exp) {
    double result = 1.0;
    double current = base;

    while (exp > 0) {
        if (exp & 1) result *= current;
        current *= current;
        exp >>= 1;
    }
    return result;
}*/

// Calculate bond price and all metrics in a single pass for efficiency
void calculate_bond_metrics(const BondParams *params, BondMetrics *metrics) {
    const double fv = params->face_value;
    const double c = params->coupon_rate / params->frequency;
    const double y = params->ytm / params->frequency;
    const int n = params->periods * params->frequency;
    const double coupon_pmt = fv * c;

    // Initialize accumulators
    double price = 0.0;
    double weighted_pv = 0.0;
    double convexity_sum = 0.0;

    // Precompute discount factor
    const double discount = 1.0 / (1.0 + y);

    // Single loop to calculate all metrics simultaneously
    double pv_factor = discount;
    for (int t = 1; t <= n; t++) {
        const double cf = (t == n) ? (coupon_pmt + fv) : coupon_pmt;
        const double pv = cf * pv_factor;

        price += pv;
        weighted_pv += t * pv;
        convexity_sum += t * (t + 1) * pv;

        pv_factor *= discount;
    }

    // Calculate final metrics
    metrics->price = price;
    metrics->macaulay_duration = weighted_pv / (price * params->frequency);
    metrics->modified_duration = metrics->macaulay_duration / (1.0 + params->ytm / params->frequency);
    metrics->convexity = convexity_sum / (price * params->frequency * params->frequency * (1.0 + y) * (1.0 + y));
    metrics->dv01 = metrics->modified_duration * price / 10000.0;
    metrics->yield_to_maturity = params->ytm;
}

// Validate and parse command line arguments
int parse_arguments(int argc, char *argv[], BondParams *params) {
    if (argc != 6) {
        fprintf(stderr, "Usage: %s <face_value> <coupon_rate> <ytm> <years> <frequency>\n", argv[0]);
        fprintf(stderr, "\nArguments:\n");
        fprintf(stderr, "  face_value  : Face value of the bond (e.g., 1000)\n");
        fprintf(stderr, "  coupon_rate : Annual coupon rate as decimal (e.g., 0.05 for 5%%)\n");
        fprintf(stderr, "  ytm         : Yield to maturity as decimal (e.g., 0.06 for 6%%)\n");
        fprintf(stderr, "  years       : Years to maturity (e.g., 10)\n");
        fprintf(stderr, "  frequency   : Payment frequency per year (1=annual, 2=semi-annual, 4=quarterly)\n");
        return 0;
    }

    char *endptr;

    // Parse face value
    errno = 0;
    params->face_value = strtod(argv[1], &endptr);
    if (errno != 0 || *endptr != '\0' || params->face_value <= 0) {
        fprintf(stderr, "Error: Invalid face value. Must be positive number.\n");
        return 0;
    }

    // Parse coupon rate
    errno = 0;
    params->coupon_rate = strtod(argv[2], &endptr);
    if (errno != 0 || *endptr != '\0' || params->coupon_rate < 0 || params->coupon_rate > 1) {
        fprintf(stderr, "Error: Invalid coupon rate. Must be between 0 and 1.\n");
        return 0;
    }

    // Parse YTM
    errno = 0;
    params->ytm = strtod(argv[3], &endptr);
    if (errno != 0 || *endptr != '\0' || params->ytm < 0) {
        fprintf(stderr, "Error: Invalid YTM. Must be non-negative.\n");
        return 0;
    }

    // Parse periods
    errno = 0;
    long periods = strtol(argv[4], &endptr, 10);
    if (errno != 0 || *endptr != '\0' || periods <= 0 || periods > 100) {
        fprintf(stderr, "Error: Invalid years to maturity. Must be between 1 and 100.\n");
        return 0;
    }
    params->periods = (int)periods;

    // Parse frequency
    errno = 0;
    long freq = strtol(argv[5], &endptr, 10);
    if (errno != 0 || *endptr != '\0' || (freq != 1 && freq != 2 && freq != 4 && freq != 12)) {
        fprintf(stderr, "Error: Invalid frequency. Must be 1, 2, 4, or 12.\n");
        return 0;
    }
    params->frequency = (int)freq;

    return 1;
}

void print_results(const BondParams *params, const BondMetrics *metrics) {
    printf("\n");
    printf("═══════════════════════════════════════════════════════════════\n");
    printf("                    BOND ANALYSIS REPORT                       \n");
    printf("═══════════════════════════════════════════════════════════════\n");
    printf("\n");

    printf("BOND PARAMETERS:\n");
    printf("───────────────────────────────────────────────────────────────\n");
    printf("  Face Value              : $%.2f\n", params->face_value);
    printf("  Coupon Rate             : %.4f%% (%.4f)\n", params->coupon_rate * 100, params->coupon_rate);
    printf("  Yield to Maturity       : %.4f%% (%.4f)\n", params->ytm * 100, params->ytm);
    printf("  Years to Maturity       : %d years\n", params->periods);
    printf("  Payment Frequency       : %d times per year\n", params->frequency);
    printf("  Total Payments          : %d\n", params->periods * params->frequency);
    printf("\n");

    printf("PRICING METRICS:\n");
    printf("───────────────────────────────────────────────────────────────\n");
    printf("  Bond Price              : $%.4f\n", metrics->price);
    printf("  Price as %% of Par       : %.4f%%\n", (metrics->price / params->face_value) * 100);
    printf("\n");

    printf("RISK METRICS:\n");
    printf("───────────────────────────────────────────────────────────────\n");
    printf("  Macaulay Duration       : %.4f years\n", metrics->macaulay_duration);
    printf("  Modified Duration       : %.4f years\n", metrics->modified_duration);
    printf("  Convexity               : %.4f\n", metrics->convexity);
    printf("  DV01 (Dollar Duration)  : $%.4f\n", metrics->dv01);
    printf("\n");

    printf("INTERPRETATION:\n");
    printf("───────────────────────────────────────────────────────────────\n");
    printf("  • A 1%% yield change implies:\n");
    printf("    - Price change (duration): $%.2f (%.2f%%)\n",
           -metrics->modified_duration * metrics->price * 0.01,
           -metrics->modified_duration * 1.0);
    printf("    - Convexity adjustment   : $%.2f\n",
           0.5 * metrics->convexity * metrics->price * 0.01 * 0.01);
    printf("  • Bond is trading at a %s\n",
           metrics->price > params->face_value ? "premium" :
           metrics->price < params->face_value ? "discount" : "par");
    printf("═══════════════════════════════════════════════════════════════\n");
    printf("\n");
}

int main(int argc, char *argv[]) {
    BondParams params;
    BondMetrics metrics;

    // Parse and validate arguments
    if (!parse_arguments(argc, argv, &params)) {
        return EXIT_FAILURE;
    }

    // Calculate all bond metrics
    calculate_bond_metrics(&params, &metrics);

    // Display results
    print_results(&params, &metrics);

    return EXIT_SUCCESS;
}
