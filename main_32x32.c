#include <stdint.h>
#include <xtensa/tie/partial_product.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>

typedef struct {
    int out1;
    int out2;
} MultiplyResult;

int64_t multiplication_C(int a, int b) __attribute__((noinline)) {
    return (int64_t)a * (int64_t)b;
}

MultiplyResult approximate_multiplication_TIE_v1(int a, int b) __attribute__((noinline)) {
    MultiplyResult result;
    approximate_mult_32_x_32_v1(result.out1, result.out2, a, b);
    return result;
}

MultiplyResult approximate_multiplication_TIE_v2(int a, int b) __attribute__((noinline)) {
    MultiplyResult result;
    approximate_mult_32_x_32_v2(result.out1, result.out2, a, b);
    return result;
}

MultiplyResult approximate_multiplication_TIE_v3(int a, int b) __attribute__((noinline)) {
    MultiplyResult result;
    approximate_mult_32_x_32_v3(result.out1, result.out2, a, b);
    return result;
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
    	  int32_t  num1 = ((int32_t)(rand() << 16) | rand()) - 2147483648; // Generate signed 32-bit numbers
    	  int32_t  num2 = ((int32_t)(rand() << 16) | rand()) - 2147483648; // Generate signed 32-bit numbers

        int64_t output_C = multiplication_C(num1, num2);

        MultiplyResult result_v1 = approximate_multiplication_TIE_v1(num1, num2);
        MultiplyResult result_v2 = approximate_multiplication_TIE_v2(num1, num2);
        MultiplyResult result_v3 = approximate_multiplication_TIE_v3(num1, num2);

        int64_t output_TIE_v1 = ((int64_t)result_v1.out1 << 32) | (uint32_t)result_v1.out2;
        int64_t output_TIE_v2 = ((int64_t)result_v2.out1 << 32) | (uint32_t)result_v2.out2;
        int64_t output_TIE_v3 = ((int64_t)result_v3.out1 << 32) | (uint32_t)result_v3.out2;

        int64_t diff_v1 = llabs(output_C - output_TIE_v1);
        int64_t diff_v2 = llabs(output_C - output_TIE_v2);
        int64_t diff_v3 = llabs(output_C - output_TIE_v3);

        double error_percentage_v1 = (double)diff_v1 / llabs(output_C) * 100;
        double error_percentage_v2 = (double)diff_v2 / llabs(output_C) * 100;
        double error_percentage_v3 = (double)diff_v3 / llabs(output_C) * 100;



        // Convert to binary representation
        char binary_num1[17], binary_num2[17], binary_num3[17], binary_num4[17];
        char binary_t1[33], binary_t2[33];
        print_binary(num1, binary_num1, sizeof(binary_num1));
        print_binary(num2, binary_num2, sizeof(binary_num2));

        /////////////////////////////////////////////////////////////////////////////////////

        fprintf(outputFile, "num1: %d \n", num1);
        fprintf(outputFile, "num2: %d \n", num2);

        fprintf(outputFile, "output_C (exact): %lld\n", output_C);
        fprintf(outputFile, "output_TIE_v1: %lld\n\n", output_TIE_v1);
        fprintf(outputFile, "difference C - output_TIE_v1 = %lld, error percentage = %.9f%%\n\n", diff_v1, error_percentage_v1);

        fprintf(outputFile, "output_C (exact): %lld\n", output_C);
        fprintf(outputFile, "output_TIE_v2: %lld\n\n", output_TIE_v2);
        fprintf(outputFile, "difference C - output_TIE_v2 = %lld, error percentage = %.9f%\n\n", diff_v2, error_percentage_v2);

        fprintf(outputFile, "output_C (exact): %lld\n", output_C);
        fprintf(outputFile, "output_TIE_v3: %lld\n\n", output_TIE_v3);
        fprintf(outputFile, "difference C - output_TIE_v3 = %lld, error percentage = %.9f%%\n\n", diff_v3, error_percentage_v3);

		    fprintf(outputFile, "******************************************************************************************************\n\n");

    }
    fclose(outputFile);
    printf("Results written to multiplication_results.txt\n");
    return 0;
}
