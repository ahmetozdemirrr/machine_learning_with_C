/* examples/sigmoid_example.c */

#include <stdio.h>
#include <mlc/activations.h>

int main() 
{
    double inputs[] = {1.0, -2.0, 3.0, -4.0, 5.0};
    size_t size = sizeof(inputs) / sizeof(inputs[0]);

    if (sigmoid(inputs, size) == 0)
    {
        printf("sigmoid output: \n");

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