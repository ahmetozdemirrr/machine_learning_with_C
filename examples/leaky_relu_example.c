/* examples/leaky_relu_example.c */

#define MLC_DEBUGGER

#include <stdio.h>
#include <mlc/activations.h>

int main() 
{
    double alpha = 0.01;
    double inputs[] = {1.0, -2.0, 3.0, -4.0, 5.0};
    size_t size = sizeof(inputs) / sizeof(inputs[0]);

    if (leaky_relu(NULL, 0, alpha) == 0)
    {
        printf("Leaky relu output: \n");

        for (size_t i = 0; i < size; ++i) 
        {
            printf("%f ", inputs[i]);
        }
        printf("\n");
    }

    else
    {
        printf("some error...\n");
    }
    return 0;
}