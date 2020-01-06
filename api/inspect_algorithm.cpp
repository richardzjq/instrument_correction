#include "inspect_algorithm.h"
#include <QtGlobal>
#include <cmath>
#include <QDebug>

/**
 * 求平均值
 */
double average(double *x, int len)
{
    double sum = 0;
	
	if(!x)
	{
         qDebug() << "传入空指针";
         return 0;
	}
	
	if(len <= 0)
	{
         qDebug() << "len应该是正整数";
         return 0;
	}
	
    for (int i = 0; i < len; i++) // 求和
        sum += x[i];
    
	return sum/len; // 得到平均值
}

/**
 * 求方差
 */
double variance(double *x, int len)
{
	if(!x)
	{
         qDebug() << "传入空指针";
         return 0;
	}
	
    if(len - 1 <= 0)
	{
         qDebug() << "len应该大于1";
         return 0;
	}
	
    double average_value = average(x, len);
    double sum = 0;

    for (int i = 0; i < len; i++) // 求和
        sum += pow(x[i] - average_value, 2);
    
    return sum/(len - 1); // 得到平均值
}

/**
 * 求标准差: sqrt(variance/n-1)?
 */
double standard_deviation(double *x, int len)
{
	if(!x)
	{
         qDebug() << "传入空指针";
         return 0;
	}
	
	if(len <= 0)
	{
         qDebug() << "len应该是正整数";
         return 0;
	}
	
    double variance_value = variance(x, len);
	
    return sqrt(variance_value); // 得到标准差
}

/**
 * 求多功能源标准不确定度，包含因子k=sqrt(3)
 */
double standard_uncertainty(double max_allowable_error)
{
    return max_allowable_error/sqrt(3);
}

/**
 * 求合成不确定度，sqrt(pow(标准差,2) + pow(标准不确定度,2))
 */
double synthesis_uncertainty(double standard_deviation_val, double standard_uncertainty_val)
{
    return sqrt(pow(standard_deviation_val,2) + pow(standard_uncertainty_val, 2));
}

/**
 * 求扩展不确定度，k=2
 */
double extend_uncertainty(double synthesis_uncertainty_val)
{
    return synthesis_uncertainty_val * 2;
}

/* 检定算法 */
bool inpsect_check(double check_value, double standard_value, double max_allowable_error)
{
    if (qAbs(check_value - standard_value) <= max_allowable_error){
        return true;
    }

    return false;
}

