/* include/mlc/vector.h */

#ifndef MLC_VECTOR_H
#define MLC_VECTOR_H

#include <stddef.h>
#include <mlc/data.h>
#include <mlc/config.h>

/*************************************************************
 * Vector operations:
 * 
 * The functions perform basic vector operations such as 
 * addition, subtraction, dot product, and scaling on MlcArray 
 * input arrays. Operations are performed element-wise or return 
 * a scalar result (for dot product). Results are stored in a 
 * provided output array, which is modified in-place.
 * 
 * The input arrays are expected to have matching sizes and non-NULL 
 * data. No additional memory is allocated beyond what the user 
 * provides via the MlcArray structure.
 * 
 * Functions return -1 (or -1.0f for dot product) if an input 
 * array is NULL or its size is 0. Otherwise, they return 0 upon 
 * successful completion.
 *************************************************************/

/**********************************
 * Mathematical synopsis of vector addition:
 * 
 * result[i] = a[i] + b[i]
 * 
 **********************************/
static inline int
vector_add(MlcArray * a, MlcArray * b, MlcArray * result)
{
    if (check_inputs(a) != 0 || 
        check_inputs(b) != 0 || 
        check_inputs(result) != 0 ||
        a->size != b->size || 
        a->size != result->size
        ) {
        LOG_ERROR("Invalid or mismatched array sizes");
        return -1;
    }

    for (size_t i = 0; i < a->size; ++i) {
        result->data[i] = a->data[i] + b->data[i];
    }
    return 0;
}

/**********************************
 * Mathematical synopsis of vector subtraction:
 * 
 * result[i] = a[i] - b[i]
 * 
 **********************************/
static inline int
vector_sub(MlcArray * a, MlcArray * b, MlcArray * result)
{
    if (check_inputs(a) != 0 || 
        check_inputs(b) != 0 || 
        check_inputs(result) != 0 ||
        a->size != b->size || 
        a->size != result->size
        ) {
        LOG_ERROR("Invalid or mismatched array sizes");
        return -1;
    }

    for (size_t i = 0; i < a->size; ++i) {
        result->data[i] = a->data[i] - b->data[i];
    }
    return 0;
}

/**********************************
 * Mathematical synopsis of dot product:
 * 
 * dot(a, b) = Σ (a[i] * b[i])
 * 
 * Note: Returns -1.0f if an error occurs.
 **********************************/
static inline float
vector_dot(MlcArray * a, MlcArray * b)
{
    if (check_inputs(a) != 0 || 
        check_inputs(b) != 0 || 
        a->size != b->size
        ) {
        LOG_ERROR("Invalid or mismatched array sizes");
        return -1.0f;
    }

    float sum = 0.0f;
    for (size_t i = 0; i < a->size; ++i) {
        sum += (a->data[i] * b->data[i]);
    }
    return sum;
}

/**********************************
 * Mathematical synopsis of vector scaling:
 * 
 * result[i] = k * a[i]
 * 
 **********************************/
static inline int
vector_scale(MlcArray * a, float k, MlcArray * result)
{
    if (check_inputs(a) != 0 || 
        check_inputs(result) != 0 || 
        a->size != result->size
        ) {
        LOG_ERROR("Invalid or mismatched array sizes");
        return -1;
    }

    for (size_t i = 0; i < a->size; ++i) {
        result->data[i] = k * a->data[i];
    }
    return 0;
}

#endif /* MLC_VECTOR_H */
