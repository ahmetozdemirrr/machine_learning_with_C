/* include/mlc/data.h */

#ifndef MLC_DATA_H
#define MLC_DATA_H

#include <stddef.h>
#include <stdlib.h>
#include <mlc/config.h>

typedef enum 
{
    TYPE_INT,
    TYPE_FLOAT,
    TYPE_DOUBLE
} 
DataType;

/**********************************
 * General-purpose array structure for MLC.
 * Supports 1D vectors, 2D matrices, and higher-dimensional tensors.
 *
 * Fields:
 *  - data: Pointer to the float array (flat storage).
 *  - ndims: Number of dimensions (1 for vector, 2 for matrix, etc.).
 *  - shape: Array of dimension sizes (e.g., {rows, cols} for 2D).
 *  - size: Total number of elements (product of shape).
 **********************************/
typedef struct 
{
    float * data;
    size_t ndims;
    size_t * shape;
    size_t size;
} 
MlcArray;

static inline int
check_inputs(MlcArray * array) 
{
    if (array == NULL || array->data == NULL || array->size == 0) {
        LOG_ERROR("`array` NULL or size of `array` is 0");
        return -1;
    }
    return 0;
}

/**********************************
 * Prepares input data into an MlcArray.
 *
 * Arguments:
 *  - input: Raw input data (any supported type).
 *  - ndims: Number of dimensions.
 *  - shape: Array of dimension sizes (e.g., {3} for 1D, {2, 3} for 2D).
 *  - input_type: Type of input data.
 *
 * Returns:
 *  - An MlcArray with converted float data.
 *  - If error occurs, data = NULL.
 **********************************/
static inline MlcArray
prepare_data(void * input, size_t ndims, size_t * shape, DataType input_type) 
{
    MlcArray result = {NULL, ndims, NULL, 0};

    if (input == NULL || ndims == 0 || shape == NULL) {
        LOG_ERROR("Invalid input or dimensions");
        return result;
    }

    /* Calculate total size */
    result.size = 1;
    for (size_t i = 0; i < ndims; ++i) {
        if (shape[i] == 0) {
            LOG_ERROR("Zero dimension in shape");
            return result;
        }
        result.size *= shape[i];
    }
    /* Allocate data and shape */
    result.data = (float *)malloc(result.size * sizeof(float));
    result.shape = (size_t *)malloc(ndims * sizeof(size_t));

    if (result.data == NULL || result.shape == NULL) {
        LOG_ERROR("Memory allocation failed");
        free(result.data);
        free(result.shape);
        result.data = NULL;
        return result;
    }

    /* Copy shape */
    for (size_t i = 0; i < ndims; ++i) {
        result.shape[i] = shape[i];
    }

    /* Convert input */
    switch (input_type) 
    {
        case TYPE_INT: 
        {
            int * in = (int *)input;
            for (size_t i = 0; i < result.size; ++i) {
                result.data[i] = (float)in[i];
            }
            break;
        }
        case TYPE_FLOAT: 
        {
            float * in = (float *)input;
            for (size_t i = 0; i < result.size; ++i) {
                result.data[i] = in[i];
            }
            break;
        }
        case TYPE_DOUBLE: 
        {
            double * in = (double *)input;
            for (size_t i = 0; i < result.size; ++i) {
                result.data[i] = (float)in[i];
            }
            break;
        }
        default:
            LOG_ERROR("Unsupported input type");
            free(result.data);
            free(result.shape);
            result.data = NULL;
            return result;
    }
    return result;
}

/**********************************
 * Frees an MlcArray's allocated memory.
 **********************************/
static inline void
mlc_finish(MlcArray * array) 
{
    if (array != NULL) {
        free(array->data);
        free(array->shape);
        array->data = NULL;
        array->shape = NULL;
    }
}

#endif /* MLC_DATA_H */
