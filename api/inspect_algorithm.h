#ifndef INSPECT_ALGORITHM_H
#define INSPECT_ALGORITHM_H


/* 每个点测试次数，最后求平均值 */
#define CHECK_TIMES 10

/**
 * 求平均值
 */
double average(double *x, int len);
/**
 * 求方差
 */
double variance(double *x, int len);
/**
 * 求标准差: sqrt(variance/n-1)?
 */
double standard_deviation(double *x, int len);
/**
 * 求多功能源标准不确定度，包含因子k=sqrt(3)
 */
double standard_uncertainty(double max_allowable_error);
/**
 * 求合成不确定度，sqrt(pow(标准差,2) + pow(标准不确定度,2))
 */
double synthesis_uncertainty(double standard_deviation_val, double standard_uncertainty_val);
/**
 * 求扩展不确定度，k=2
 */
double extend_uncertainty(double synthesis_uncertainty_val);


#endif // INSPECT_ALGORITHM_H
