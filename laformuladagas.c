#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <ctype.h>
#include <errno.h>

#define R 8.31446261815324

typedef enum {
    FIND_P = 1,
    FIND_V,
    FIND_N,
    FIND_T
} Variable;

static int read_double(const char *prompt, double *value)
{
    char buffer[128];
    char *endptr;

    while (1) {
        printf("%s", prompt);

        if (!fgets(buffer, sizeof(buffer), stdin)) {
            return 0;
        }

        errno = 0;
        *value = strtod(buffer, &endptr);

        while (isspace((unsigned char)*endptr)) {
            endptr++;
        }

        if (endptr == buffer || *endptr != '\0' || errno == ERANGE) {
            printf("Hata: Gecersiz bir sayi girdiniz.\n");
            continue;
        }

        return 1;
    }
}

static int read_positive(const char *prompt, double *value)
{
    while (1) {
        if (!read_double(prompt, value)) {
            return 0;
        }

        if (!isfinite(*value)) {
            printf("Hata: Sonsuz veya gecersiz bir deger kullanilamaz.\n");
            continue;
        }

        if (*value <= 0.0) {
            printf("Hata: Deger 0'dan buyuk olmalidir.\n");
            continue;
        }

        return 1;
    }
}

static Variable choose_variable(void)
{
    int choice;
    char buffer[32];

    while (1) {
        printf("\n==============================\n");
        printf("       PV = nRT HESAPLAYICI\n");
        printf("==============================\n");
        printf("1 - Basinc (P) bul\n");
        printf("2 - Hacim   (V) bul\n");
        printf("3 - Mol     (n) bul\n");
        printf("4 - Sicaklik(T) bul\n");
        printf("0 - Cikis\n");
        printf("==============================\n");
        printf("Seciminiz: ");

        if (!fgets(buffer, sizeof(buffer), stdin)) {
            return 0;
        }

        if (sscanf(buffer, "%d", &choice) != 1) {
            printf("Hata: Lutfen 1-4 arasinda bir secim yapin.\n");
            continue;
        }

        if (choice >= 0 && choice <= 4) {
            return (Variable)choice;
        }

        printf("Hata: Gecersiz secim.\n");
    }
}

int main(void)
{
    double P = 0.0;
    double V = 0.0;
    double n = 0.0;
    double T = 0.0;
    double result;

    while (1) {
        Variable target = choose_variable();

        if (target == 0) {
            printf("\nProgram sonlandirildi.\n");
            return EXIT_SUCCESS;
        }

        printf("\nNot: Bu program SI birimlerini kullanir.\n");
        printf("P = Pascal (Pa)\n");
        printf("V = metrekup (m^3)\n");
        printf("n = mol\n");
        printf("T = Kelvin (K)\n\n");

        switch (target) {

        case FIND_P:
            if (!read_positive("Hacim V (m^3): ", &V))
                return EXIT_FAILURE;

            if (!read_positive("Mol miktari n (mol): ", &n))
                return EXIT_FAILURE;

            if (!read_positive("Sicaklik T (K): ", &T))
                return EXIT_FAILURE;

            result = (n * R * T) / V;

            printf("\nSonuc:\n");
            printf("P = %.10f Pa\n", result);
            break;

        case FIND_V:
            if (!read_positive("Basinc P (Pa): ", &P))
                return EXIT_FAILURE;

            if (!read_positive("Mol miktari n (mol): ", &n))
                return EXIT_FAILURE;

            if (!read_positive("Sicaklik T (K): ", &T))
                return EXIT_FAILURE;

            result = (n * R * T) / P;

            printf("\nSonuc:\n");
            printf("V = %.10f m^3\n", result);
            break;

        case FIND_N:
            if (!read_positive("Basinc P (Pa): ", &P))
                return EXIT_FAILURE;

            if (!read_positive("Hacim V (m^3): ", &V))
                return EXIT_FAILURE;

            if (!read_positive("Sicaklik T (K): ", &T))
                return EXIT_FAILURE;

            result = (P * V) / (R * T);

            printf("\nSonuc:\n");
            printf("n = %.10f mol\n", result);
            break;

        case FIND_T:
            if (!read_positive("Basinc P (Pa): ", &P))
                return EXIT_FAILURE;

            if (!read_positive("Hacim V (m^3): ", &V))
                return EXIT_FAILURE;

            if (!read_positive("Mol miktari n (mol): ", &n))
                return EXIT_FAILURE;

            result = (P * V) / (n * R);

            printf("\nSonuc:\n");
            printf("T = %.10f K\n", result);
            break;

        default:
            printf("Beklenmeyen hata.\n");
            return EXIT_FAILURE;
        }

        printf("\n--------------------------------\n");
    }
}
