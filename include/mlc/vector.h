/* include/mlc/vector.h */

#ifndef MLC_VECTOR_H
#define MLC_VECTOR_H

#include <stddef.h>
#include <mlc/config.h>

/*************************************************************
 * Vector operations:
 * 
 * The functions perform basic vector operations such as 
 * addition, subtraction, dot product, and scaling on input 
 * arrays. Operations are performed element-wise or return a 
 * scalar result (for dot product), and results are stored in 
 * a provided output array when applicable.
 * 
 * The input arrays are expected to be non-NULL and of the 
 * specified size. No additional memory is allocated, and 
 * operations are designed to be efficient and in-place where 
 * possible.
 * 
 * Functions return -1 (or -1.0 for dot product) if an input 
 * vector is NULL or the size is 0. Otherwise, they return 0 
 * upon successful completion.
 *************************************************************/

/**********************************
 * Mathematical synopsis of vector addition:
 * 
 * result[i] = a[i] + b[i]
 * 
 **********************************/
static inline int
vector_add(double * a, double * b, double * result, size_t size)
{
	if (a == NULL || b == NULL || result == NULL || size == 0)
    {
        LOG_ERROR("Vectors are NULL or size of vectors is 0");
        return -1;
    }

	for (size_t i = 0; i < size; ++i)
	{
		result[i] = a[i] + b[i];
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
vector_sub(double * a, double * b, double * result, size_t size)
{
	if (a == NULL || b == NULL || result == NULL || size == 0)
    {
        LOG_ERROR("Vectors are NULL or size of vectors is 0");
        return -1;
    }

	for (size_t i = 0; i < size; ++i)
	{
		result[i] = a[i] - b[i];
	}
	return 0;
}

/**********************************
 * Mathematical synopsis of dot product:
 * 
 * dot(a, b) = Σ (a[i] * b[i])
 * 
 * Note: Returns -1.0 if an error occurs.
 **********************************/
static inline double
vector_dot(double * a, double * b, size_t size)
{
	if (a == NULL || b == NULL || size == 0)
    {
        LOG_ERROR("Vectors are NULL or size of vectors is 0");
        return -1.0;
    }
	double sum = 0.0;

	for (size_t i = 0; i < size; ++i)
	{
		sum += (a[i] * b[i]);
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
vector_scale(double * a, double k, double * result, size_t size)
{
	if (a == NULL || result == NULL || size == 0)
    {
        LOG_ERROR("Vectors are NULL or size of vectors is 0");
        return -1;
    }

	for (size_t i = 0; i < size; ++i)
	{
		result[i] = a[i] * k;
	}
	return 0;
}

#endif /* MLC_VECTOR_H */
