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

int multiplication_C(int a, int b) __attribute__((noinline)) {
    return a * b;
}

MultiplyResult approximate_multiplication_TIE(int a, int b) __attribute__((noinline)) {
    MultiplyResult result;
    approximate_mult_16_x_16(result.out1, result.out2, a, b);
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
    const int iterations = 100;

    double total_time_C_multiplication = 0.0;
    double total_time_TIE_multiplication = 0.0;

    FILE *outputFile = fopen("multiplication_results.txt", "w");
    if (outputFile == NULL) {
        perror("Error opening file");
        return EXIT_FAILURE;
    }

    for (int i = 0; i < iterations; i++) {
        int num1 = rand() % 65536 - 32768; // Generate signed 16-bit numbers
        int num2 = rand() % 65536 - 32768; // Generate signed 16-bit numbers
        int num3 = rand() % 65536 - 32768; // Generate signed 16-bit numbers
		int num4 = rand() % 65536 - 32768; // Generate signed 16-bit numbers

		int t1 = (num1 << 16) | (num2 & 0xFFFF);
		int t2 = (num3 << 16) | (num4 & 0xFFFF);

		clock_t start1 = clock();
		int output_C_1 = multiplication_C(num1, num2);
        int output_C_2 = multiplication_C(num3, num4);
        clock_t end1 = clock();
        total_time_C_multiplication += (double)(end1 - start1) / CLOCKS_PER_SEC;

        clock_t start2 = clock();
        MultiplyResult result = approximate_multiplication_TIE(t1, t2);
        clock_t end2 = clock();
        total_time_TIE_multiplication += (double)(end2 - start2) / CLOCKS_PER_SEC;


        int diff_A = abs(output_C_1 - result.out1);
        int diff_B = abs(output_C_2 - result.out2);

        double error_percentage_A = (double)diff_A / abs(output_C_1) * 100;
        double error_percentage_B = (double)diff_B / abs(output_C_2) * 100;


        // Convert to binary representation
        char binary_num1[17], binary_num2[17], binary_num3[17], binary_num4[17];
        char binary_t1[33], binary_t2[33];
        print_binary(num1, binary_num1, sizeof(binary_num1));
        print_binary(num2, binary_num2, sizeof(binary_num2));
        print_binary(num3, binary_num3, sizeof(binary_num3));
		print_binary(num4, binary_num4, sizeof(binary_num4));
		print_binary_32bit(t1, binary_t1, sizeof(binary_t1));
		print_binary_32bit(t2, binary_t2, sizeof(binary_t2));

        /////////////////////////////////////////////////////////////////////////////////////

        fprintf(outputFile, "num1: %d \n", num1);
        fprintf(outputFile, "num2: %d \n", num2);
        fprintf(outputFile, "t1: %d \n\n", t1);

        fprintf(outputFile, "output_C_1 (exact): %d\n", output_C_1);
        fprintf(outputFile, "output_8x8_instance_out1: %d\n\n", result.out1);
        fprintf(outputFile, "difference C_1 - 8x8_instance_out1 = %d, error percentage = %.2f%%\n\n", diff_A, error_percentage_A);

        /////////////////////////////////////////////////////////////////////////////////////

		fprintf(outputFile, "num3: %d \n", num3);
		fprintf(outputFile, "num4: %d \n", num4);
		fprintf(outputFile, "t2: %d \n\n", t2);

        fprintf(outputFile, "output_C_2 (exact): %d\n", output_C_2);
        fprintf(outputFile, "output_8x8_instance_out2: %d\n\n", result.out2);
		fprintf(outputFile, "difference C_2 - 8x8_instance_out2 = %d, error percentage = %.2f%%\n\n", diff_B, error_percentage_B);

		fprintf(outputFile, "******************************************************************************************************\n\n");

    }
    // Calculate average time taken per call
    double avg_time_C_multiplication = (total_time_C_multiplication / iterations) * 1e6; // Average time in microseconds (µs)
    double avg_time_TIE_multiplication = (total_time_TIE_multiplication / iterations) * 1e6; // Average time in microseconds (µs)
    double percentage_improvement = ((avg_time_C_multiplication - avg_time_TIE_multiplication) / avg_time_C_multiplication) * 100.0;

    // Print results
    printf("Total time taken per call for C_multiplication: %f seconds\n", total_time_C_multiplication);
    printf("Total time taken per call for TIE_multiplication: %f seconds\n\n", total_time_TIE_multiplication);

    printf("Average time taken per call for C_multiplication: %.2f µs\n", avg_time_C_multiplication); // Print in microseconds
    printf("Average time taken per call for TIE_multiplication: %.2f µs\n\n", avg_time_TIE_multiplication); // Print in microseconds
    printf("Percentage improvement of avg_time_TIE_multiplication over avg_time_C_multiplication: %.2f%%\n\n", percentage_improvement);



    fclose(outputFile);
    printf("Results written to multiplication_results.txt\n");
    return 0;
}
