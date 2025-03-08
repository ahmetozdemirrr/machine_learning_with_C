/* include/mlc/activations.h */

#ifndef MLC_ACTIVATIONS_H
#define MLC_ACTIVATIONS_H

#include <stddef.h>
#include <stdlib.h>
#include <math.h>
#include <mlc/data.h>
#include <mlc/config.h>

/* Activation functions: */

/*************************************************************
 * The functions receive the address of an MlcArray to be 
 * activated from the user and modify it with activation operations 
 * in-place.
 * 
 * The functions perform ReLU, sigmoid, tanh, leaky ReLU, softmax, 
 * or swish activation on the array. The array is modified 
 * directly, and no additional memory is allocated.
 * 
 * Note:
 *  - Functions operate on float (32-bit) data, the internal type 
 *    of the MLC library.
 *  - expf() and tanhf() are float-specific functions defined in math.h.
 *  - Users must preprocess data into an MlcArray using prepare_data() 
 *    from data.h if their input is in a different format or type.
 *  - Functions work on any dimension (1D vectors, 2D matrices, etc.), 
 *    treating the data as a flat array of size elements.
 * 
 * Activation functions return -1 if the input array is NULL or 
 * its size is 0. Otherwise, if the process is successful, they 
 * return 0.
 *************************************************************/

/**********************************
 * Mathematical synopsis of ReLU:
 * 
 *          x if x > 0,
 * relu = {
 *          0 if x <= 0
 * 
 **********************************/
static inline int 
relu(MlcArray * array)
{
    if (check_inputs(array) != 0) return -1;

    for (size_t i = 0; i < array->size; ++i) {
        array->data[i] = (array->data[i] > 0.0f) ? array->data[i] : 0.0f;
    }
    return 0;
}

/**********************************
 * Mathematical synopsis of Sigmoid:
 * 
 * sigmoid(x) = 1 / (1 + e^(-x))
 * 
 **********************************/
static inline int 
sigmoid(MlcArray * array)
{
    if (check_inputs(array) != 0) return -1;

    for (size_t i = 0; i < array->size; ++i) {
        array->data[i] = 1.0f / (1.0f + expf(-array->data[i]));
    }
    return 0;
}

/**********************************
 * Mathematical synopsis of tanh:
 * 
 * tanh(x) = (e^(x) - e^(-x)) / (e^(x) + e^(-x))
 * 
 * Note: 'tanh' is defined in math.h, so we use 'tanh_' to avoid conflicts.
 **********************************/
static inline int 
tanh_(MlcArray * array)
{
    if (check_inputs(array) != 0) return -1;

    for (size_t i = 0; i < array->size; ++i) {
        array->data[i] = tanhf(array->data[i]);
    }
    return 0;
}

/**********************************
 * Mathematical synopsis of Leaky ReLU:
 * 
 *                x if x > 0,
 * leaky relu = {
 *                alpha * x if x <= 0
 * 
 **********************************/
static inline int 
leaky_relu(MlcArray * array, float alpha)
{
    if (check_inputs(array) != 0) return -1;

    for (size_t i = 0; i < array->size; ++i) {
        array->data[i] = (array->data[i] > 0.0f) ? array->data[i] : alpha * array->data[i];
    }
    return 0;
}

/**********************************
 * Mathematical synopsis of Softmax:
 * 
 * softmax(x_i) = e^(x_i) / Σ e^(x_j)
 * 
 * Note: Computes probabilities across the last dimension of the array, 
 * using max subtraction for numerical stability. For 1D arrays, applies 
 * across all elements. For multi-dimensional arrays (2D+), applies 
 * independently along the last dimension, treating preceding dimensions 
 * as separate instances.
 **********************************/
static inline int 
softmax(MlcArray *array)
{
    if (check_inputs(array) != 0) return -1;

    if (array->ndims == 1) {
        /* Step 1: Find the maximum element for numerical stability (1D case) */
        float max = array->data[0];
        for (size_t i = 1; i < array->size; ++i) {
            max = (array->data[i] > max) ? array->data[i] : max;
        }

        /* Step 2: Compute exponentials in-place and their sum */
        float sum = 0.0f;
        for (size_t i = 0; i < array->size; ++i) {
            array->data[i] = expf(array->data[i] - max);
            sum += array->data[i];
        }

        /* Step 3: Normalize to get probabilities */
        for (size_t i = 0; i < array->size; ++i) {
            array->data[i] /= sum;
        }
    } 

    else {
        /* Multi-dimensional case: Apply softmax along the last dimension */
        size_t last_dim = array->shape[array->ndims - 1];  /* len of last dim */
        size_t prefix_size = array->size / last_dim;       /* product of prev. dims */

        for (size_t p = 0; p < prefix_size; ++p) {
            /* Step 1: Find the maximum in the current last-dimension slice */
            float max = array->data[p * last_dim];
            for (size_t i = 1; i < last_dim; ++i) {
                max = (array->data[p * last_dim + i] > max) ? array->data[p * last_dim + i] : max;
            }

            /* Step 2: Compute exponentials and their sum for the slice */
            float sum = 0.0f;
            for (size_t i = 0; i < last_dim; ++i) {
                array->data[p * last_dim + i] = expf(array->data[p * last_dim + i] - max);
                sum += array->data[p * last_dim + i];
            }

            /* Step 3: Normalize the slice */
            for (size_t i = 0; i < last_dim; ++i) {
                array->data[p * last_dim + i] /= sum;
            }
        }
    }
    return 0;
}

/**********************************
 * Mathematical synopsis of Swish:
 * 
 * swish(x) = x * sigmoid(x) = x / (1 + e^(-x))
 * 
 **********************************/
static inline int 
swish(MlcArray * array)
{
    if (check_inputs(array) != 0) return -1;

    for (size_t i = 0; i < array->size; ++i) {
        float x = array->data[i];
        array->data[i] = x * (1.0f / (1.0f + expf(-x)));
    }
    return 0;
}

#endif /* MLC_ACTIVATIONS_H */
