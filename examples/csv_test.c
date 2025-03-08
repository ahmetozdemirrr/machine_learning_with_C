/* examples/csv_test.c */

#include <stdio.h>
#include "mlc/activations.h"
#include "mlc/data.h"

int main() 
{
    /* read data from .csv */
    MlcArray array = mlc_read_csv("big_test.csv");
    if (array.data == NULL) {
        printf("Hata: CSV okunamadı!\n");
        return 1;
    }

    /* print original data */
    printf("Orijinal CSV verisi (%zu x %zu):\n", array.shape[0], array.shape[1]);
    for (size_t i = 0; i < array.shape[0]; ++i) {
        for (size_t j = 0; j < array.shape[1]; ++j) {
            size_t idx = i * array.shape[1] + j;
            printf("%6.1f ", array.data[idx]);
        }
        printf("\n");
    }

    /* apply sigmoid activation */
    sigmoid(&array);

    /* sigmoid applied version */
    printf("\nSigmoid uygulanmış veri:\n");
    for (size_t i = 0; i < array.shape[0]; ++i) {
        for (size_t j = 0; j < array.shape[1]; ++j) {
            size_t idx = i * array.shape[1] + j;
            printf("%6.3f ", array.data[idx]);
        }
        printf("\n");
    }

    /* free memory */
    mlc_finish(&array);
    return 0;
}
