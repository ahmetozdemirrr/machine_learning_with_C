/* examples/comprehensive_test.c */

#define MLC_DEBUGGER

#include <stdio.h>
#include <string.h>
#include <mlc/data.h>
#include <mlc/activations.h>
#include <mlc/vector.h>

static void print_array(const char * label, MlcArray * arr) 
{
    printf("%s (ndims=%zu, shape=[", label, arr->ndims);
    for (size_t i = 0; i < arr->ndims; ++i) {
        printf("%zu", arr->shape[i]);
        if (i < arr->ndims - 1) printf(", ");
    }
    printf("]):\n");

    if (arr->ndims == 1) {
        for (size_t i = 0; i < arr->size; ++i) {
            printf("%f ", arr->data[i]);
        }
        printf("\n");
    } 

    else if (arr->ndims == 2) {
        for (size_t i = 0; i < arr->shape[0]; ++i) {
            for (size_t j = 0; j < arr->shape[1]; ++j) {
                printf("%f ", arr->data[i * arr->shape[1] + j]);
            }
            printf("\n");
        }
    } 

    else if (arr->ndims == 3) {
        for (size_t i = 0; i < arr->shape[0]; ++i) {
            printf("Slice %zu:\n", i);
            for (size_t j = 0; j < arr->shape[1]; ++j) {
                for (size_t k = 0; k < arr->shape[2]; ++k) {
                    printf("%f ", arr->data[i * arr->shape[1] * arr->shape[2] + j * arr->shape[2] + k]);
                }
                printf("\n");
            }
        }
    }
    printf("\n");
}

int main() 
{
    /************************************************************
     * 1D: vector */
    int vec_data[] = {1, -2, 3, -4, 5};
    size_t vec_shape[] = {5};

    MlcArray vec = prepare_data(vec_data, 1, vec_shape, TYPE_INT);

    if (vec.data == NULL) {
        printf("Error preparing 1D vector\n");
        return 1;
    }
    printf("=== 1D Vector Tests ===\n");
    print_array("Original 1D", &vec);

    relu(&vec);
    print_array("After ReLU", &vec);

    sigmoid(&vec);
    print_array("After Sigmoid", &vec);

    /*************************************************************
     * 2D: Matris */
    float mat_data[] = {1.0f, -2.0f, 3.0f, 4.0f, -5.0f, 6.0f};
    size_t mat_shape[] = {2, 3};

    MlcArray mat = prepare_data(mat_data, 2, mat_shape, TYPE_FLOAT);

    if (mat.data == NULL) {
        printf("Error preparing 2D matrix\n");
        return 1;
    }
    printf("=== 2D Matrix Tests ===\n");
    print_array("Original 2D", &mat);

    tanh_(&mat);
    print_array("After Tanh", &mat);

    leaky_relu(&mat, 0.01f);
    print_array("After Leaky ReLU (alpha=0.01)", &mat);

    /*************************************************************
     * 3D: Tensör */
    double tensor_data[] = {1.0, -1.0, 2.0, 0.0, 3.0, -2.0, 4.0, 1.0, -1.0, 2.0, -3.0, 0.5};
    size_t tensor_shape[] = {3, 2, 2};

    MlcArray tensor = prepare_data(tensor_data, 3, tensor_shape, TYPE_DOUBLE);

    if (tensor.data == NULL) {
        printf("Error preparing 3D tensor\n");
        return 1;
    }
    printf("=== 3D Tensor Tests ===\n");
    print_array("Original 3D", &tensor);

    swish(&tensor);
    print_array("After Swish", &tensor);

    softmax(&tensor);
    print_array("After Softmax", &tensor);

    /*************************************************************/

    printf("=== Vector Operation Tests ===\n");
    float vec_a_data[] = {1.0f, 2.0f, 3.0f};
    float vec_b_data[] = {4.0f, 5.0f, 6.0f};
    size_t vec_op_shape[] = {3};

    MlcArray vec_a = prepare_data(vec_a_data, 1, vec_op_shape, TYPE_FLOAT);
    MlcArray vec_b = prepare_data(vec_b_data, 1, vec_op_shape, TYPE_FLOAT);
    MlcArray vec_result = prepare_data(vec_a_data, 1, vec_op_shape, TYPE_FLOAT);  // Boş sonuç için

    vector_add(&vec_a, &vec_b, &vec_result);
    print_array("vector_add (a + b)", &vec_result);

    vector_sub(&vec_a, &vec_b, &vec_result);
    print_array("vector_sub (a - b)", &vec_result);

    float dot = vector_dot(&vec_a, &vec_b);
    printf("vector_dot (a . b): %f\n", dot);

    float k = 2.0f;
    vector_scale(&vec_a, k, &vec_result);
    print_array("vector_scale (k * a)", &vec_result);

    printf("=== Error Handling ===\n");
    MlcArray null_arr = {NULL, 1, vec_shape, 5};

    if (relu(&null_arr) == -1) {
        printf("Caught NULL error in relu\n");
    }
    if (vector_add(&null_arr, &vec_b, &vec_result) == -1) {
        printf("Caught NULL error in vector_add\n");
    }
    mlc_finish(&vec);
    mlc_finish(&mat);
    mlc_finish(&tensor);
    mlc_finish(&vec_a);
    mlc_finish(&vec_b);
    mlc_finish(&vec_result);

    return 0;
}
