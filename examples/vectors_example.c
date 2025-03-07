/* examples/vectors_example.c */

#define MLC_DEBUGGER

#include <mlc/vector.h>
#include <stdio.h>

int main() 
{
    double a[] = {1.0, 2.0, 3.0};
    double b[] = {4.0, 5.0, 6.0};
    double result[3];

    size_t size = 3;

    printf("Testing vector_add:\n");
    printf("a = [%f, %f, %f]\n", a[0], a[1], a[2]);
    printf("b = [%f, %f, %f]\n", b[0], b[1], b[2]);

    if (vector_add(a, b, result, size) == 0) 
    {
        printf("Result = [%f, %f, %f]\n", result[0], result[1], result[2]);
    } 

    else 
    {
        printf("Error in vector_add\n");
    }
    printf("\nTesting vector_sub:\n");
    
    if (vector_sub(a, b, result, size) == 0) 
    {
        printf("Result = [%f, %f, %f]\n", result[0], result[1], result[2]);
    } 

    else 
    {
        printf("Error in vector_sub\n");
    }
    printf("\nTesting vector_dot:\n");
    double dot_result = vector_dot(a, b, size);

    if (dot_result != -1.0) 
    {
        printf("Dot product = %f\n", dot_result);
    } 

    else 
    {
        printf("Error in vector_dot\n");
    }
    printf("\nTesting vector_scale:\n");
    double k = 2.0;
    printf("Scaling a by %f\n", k);

    if (vector_scale(a, k, result, size) == 0) 
    {
        printf("Result = [%f, %f, %f]\n", result[0], result[1], result[2]);
    } 

    else 
    {
        printf("Error in vector_scale\n");
    }

    printf("\nTesting error handling with NULL vector:\n");

    if (vector_add(NULL, b, result, size) == -1) 
    {
        printf("Successfully caught NULL vector error\n");
    } 

    else 
    {
        printf("Failed to catch NULL vector error\n");
    }
    return 0;
}