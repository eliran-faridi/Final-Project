#include <stdint.h>
#include <xtensa/tie/partial_product.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>


int multiplication_C(int a, int b) __attribute__((noinline)) {
    return a * b;
}

int approximate_multiplication_TIE_v1(int a, int b) __attribute__((noinline)) {
    int res;
	res = approximate_mult_8_x_8_v1(a, b);
	return res;
}

int approximate_multiplication_TIE_v2(int a, int b) __attribute__((noinline)) {
    int res;
	res = approximate_mult_8_x_8_v2(a, b);
	return res;
}

int approximate_multiplication_TIE_v3(int a, int b) __attribute__((noinline)) {
    int res;
	res = approximate_mult_8_x_8_v3(a, b);
	return res;
}

void print_binary(int num, char *buffer, size_t buffer_size) {

    for (int i = 0; i < 16; i++) {
        buffer[15 - i] = (num & (1 << i)) ? '1' : '0';
    }
    buffer[16] = '\0'; // Null-terminate the string
}

void print_binary_32bit(int num, char *buffer, size_t buffer_size) {
    // Ensure the buffer is large enough
    if (buffer_size < 33) {
        return; // Avoid buffer overflow
    }

    for (int i = 0; i < 32; i++) {
        buffer[31 - i] = (num & (1 << i)) ? '1' : '0';
    }
    buffer[32] = '\0'; // Null-terminate the string
}


int main() {
    srand(time(0));
    const int iterations = 200;

    FILE *outputFile = fopen("multiplication_results.txt", "w");
    if (outputFile == NULL) {
        perror("Error opening file");
        return EXIT_FAILURE;
    }

    for (int i = 0; i < iterations; i++) {
        int num1, num2;

        do {
            num1 = rand() % 256 - 128; // Generate signed 8-bit numbers
        } while (num1 == 0); // Regenerate if num1 is zero

        do {
            num2 = rand() % 256 - 128; // Generate signed 8-bit numbers
        } while (num2 == 0); // Regenerate if num2 is zero

        int output_C = multiplication_C(num1, num2);
        int output_TIE_v1 = approximate_multiplication_TIE_v1(num1, num2);
        int output_TIE_v2 = approximate_multiplication_TIE_v2(num1, num2);
        int output_TIE_v3 = approximate_multiplication_TIE_v3(num1, num2);

        int diff_v1 = abs(output_C - output_TIE_v1);
        int diff_v2 = abs(output_C - output_TIE_v2);
        int diff_v3 = abs(output_C - output_TIE_v3);

        double error_percentage_v1 = (double)diff_v1 / abs(output_C) * 100;
        double error_percentage_v2 = (double)diff_v2 / abs(output_C) * 100;
        double error_percentage_v3 = (double)diff_v3 / abs(output_C) * 100;

        // Convert to binary representation
        char binary_num1[17], binary_num2[17], binary_num3[17], binary_num4[17];
        print_binary(num1, binary_num1, sizeof(binary_num1));
        print_binary(num2, binary_num2, sizeof(binary_num2));

        /////////////////////////////////////////////////////////////////////////////////////

        fprintf(outputFile, "num1: %d \n", num1);
        fprintf(outputFile, "num2: %d \n", num2);

        fprintf(outputFile, "output_C (exact): %d\n", output_C);
        fprintf(outputFile, "output_TIE_v1: %d\n\n", output_TIE_v1);
        fprintf(outputFile, "difference output_C - output_TIE_v1 = %d, error percentage_v1 = %.2f%%\n\n", diff_v1, error_percentage_v1);

        fprintf(outputFile, "output_C (exact): %d\n", output_C);
		    fprintf(outputFile, "output_TIE_v2: %d\n\n", output_TIE_v2);
		    fprintf(outputFile, "difference output_C - output_TIE_v2 = %d, error percentage_v2 = %.2f%%\n\n", diff_v2, error_percentage_v2);

        fprintf(outputFile, "output_C (exact): %d\n", output_C);
		    fprintf(outputFile, "output_TIE_v3: %d\n\n", output_TIE_v3);
		    fprintf(outputFile, "difference output_C - output_TIE_v3 = %d, error percentage_v3 = %.2f%%\n\n", diff_v3, error_percentage_v3);

		    fprintf(outputFile, "******************************************************************************************************\n\n");

        /////////////////////////////////////////////////////////////////////////////////////


    }
    fclose(outputFile);
    printf("Results written to multiplication_results.txt\n");
    return 0;
}
