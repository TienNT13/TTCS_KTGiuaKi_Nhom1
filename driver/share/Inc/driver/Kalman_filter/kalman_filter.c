/*******************************************************************************
 *
 * Copyright (c) 2020
 * Lumi, JSC.
 * All Rights Reserved
 *
 *
 * Description:
 *
 * Author: HoangNH
 *
 * Last Changed By:  $Author: HoangNH $
 * Revision:         $Revision: 1.0 $
 * Last Changed:     $Date: 21/08/2020 $
 *
 ******************************************************************************/
/******************************************************************************/
/*                              INCLUDE FILES                                 */
/******************************************************************************/
#include "kalman_filter.h"
#include <math.h>
/******************************************************************************/
/*                     EXPORTED TYPES and DEFINITIONS                         */
/******************************************************************************/
/******************************************************************************/
/*                              PRIVATE DATA                                  */
/******************************************************************************/
/******************************************************************************/
/*                              EXPORTED DATA                                 */
/******************************************************************************/
/******************************************************************************/
/*                            PRIVATE FUNCTIONS                               */
/******************************************************************************/
/******************************************************************************/
/*                            EXPORTED FUNCTIONS                              */
/******************************************************************************/
/*
*e_mea: Độ không chắc chắn của phép đo -
*-Mức độ chúng tôi mong đợi đối với phép đo của mình sẽ khác nhau
*
*e_est: Ước tính Không chắc chắn
*- Có thể được bắt đầu với cùng một giá trị như e_mea vì bộ lọc kalman sẽ điều chỉnh giá trị của nó.
*q: Phương sai Quá trình - thường là một số nhỏ trong khoảng từ 0,001 đến 1. */
void KalmanFilterInit(float mea_e, float est_e, float q)
{
  _err_measure=mea_e;
  _err_estimate=est_e;
  _q = q;
}

float KalmanFilter_updateEstimate(float mea)
{
  _kalman_gain = _err_estimate/(_err_estimate + _err_measure);
  _current_estimate = _last_estimate + _kalman_gain * (mea - _last_estimate);
  _err_estimate =  (1.0 - _kalman_gain)*_err_estimate + fabs(_last_estimate-_current_estimate)*_q;
  _last_estimate=_current_estimate;

  return _current_estimate;
}

void KalmanFilter_setMeasurementError(float mea_e)
{
  _err_measure=mea_e;
}

void KalmanFilter_setEstimateError(float est_e)
{
  _err_estimate=est_e;
}

void KalmanFilter_setProcessNoise(float q)
{
  _q=q;
}

float KalmanFilter_getKalmanGain(void) {
  return _kalman_gain;
}

float KalmanFilter_getEstimateError(void) {
  return _err_estimate;
}

/* END FILE */
