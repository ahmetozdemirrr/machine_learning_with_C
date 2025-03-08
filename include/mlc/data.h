/* include/mlc/data.h */

#ifndef MLC_DATA_H
#define MLC_DATA_H

#include <stddef.h>
#include <stdlib.h>
#include <string.h>
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
 * Reads a CSV file and converts it to an MlcArray.
 * 
 * Arguments:
 *  - filename: Path to the CSV file.
 * 
 * Returns:
 *  - An MlcArray with float data, assuming 2D structure (rows x cols).
 *  - If error occurs (file not found, malformed CSV, etc.), data = NULL.
 * 
 * Notes:
 *  - Assumes CSV contains comma-separated float values.
 *  - Dynamically allocates memory for data and shape, no fixed limits.
 *  - Automatically detects number of rows and columns in a single pass.
 *  - Stores data in row-major order (flat float array).
 *  - Ignores empty lines and assumes consistent column counts.
 **********************************/
static inline MlcArray
mlc_read_csv(const char * filename) 
{
    MlcArray result = {NULL, 2, NULL, 0};
    FILE * file = fopen(filename, "r");

    if (!file) {
        LOG_ERROR("Cannot open CSV file");
        return result;
    }
    float * data = NULL;
    size_t capacity = 0;
    size_t size = 0;
    size_t rows = 0;
    size_t cols = 0;

    char * line = NULL;
    size_t line_cap = 0;
    ssize_t line_len;

    while ((line_len = getline(&line, &line_cap, file)) != -1) 
    {
        if (line_len <= 1) continue;

        size_t current_cols = 0;
        char * token = strtok(line, ",");

        while (token) 
        {
            if (size >= capacity) {
                capacity = capacity ? capacity * 2 : 1024;
                float * new_data = (float *)realloc(data, capacity * sizeof(float));
                
                if (!new_data) {
                    LOG_ERROR("Memory reallocation failed");
                    free(data);
                    free(line);
                    fclose(file);
                    return result;
                }
                data = new_data;
            }
            data[size++] = atof(token);
            current_cols++;
            token = strtok(NULL, ",");
        }

        if (rows == 0) {
            cols = current_cols;
        } 

        else if (current_cols != cols) {
            LOG_ERROR("Inconsistent column count in CSV");
            free(data);
            free(line);
            fclose(file);
            return result;
        }
        rows++;
    }
    free(line);
    fclose(file);

    if (rows == 0 || cols == 0) {
        LOG_ERROR("Empty or invalid CSV file");
        free(data);
        return result;
    }
    result.size = rows * cols;
    result.shape = (size_t *)malloc(2 * sizeof(size_t));

    if (!result.shape) {
        LOG_ERROR("Memory allocation failed for shape");
        free(data);
        return result;
    }
    result.shape[0] = rows;
    result.shape[1] = cols;

    result.data = (float *)realloc(data, result.size * sizeof(float));
    
    if (!result.data) {
        LOG_ERROR("Memory reallocation failed for data");
        free(data);
        free(result.shape);
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
