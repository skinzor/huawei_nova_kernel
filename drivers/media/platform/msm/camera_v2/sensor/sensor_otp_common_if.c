

//#define HW_CMR_LOGSWC 0   //file log switch set 0 off,default is 1 on
#define HW_CMR_LOG_TAG "sensor_otp_common_if"

#include <linux/hw_camera_common.h>
#include "msm_sensor.h"
#include "sensor_otp_common_if.h"
#define HI843S_OFILM_RG_RATIO_TYPICAL 0x0259
#define HI843S_OFILM_BG_RATIO_TYPICAL 0x0276

#define IMX219_LITEON_RG_RATIO_TYPICAL 0x220
#define IMX219_LITEON_BG_RATIO_TYPICAL 0x267
#define IMX219_SUNNY_RG_RATIO_TYPICAL  0x234
#define IMX219_SUNNY_BG_RATIO_TYPICAL  0x246
struct otp_function_t otp_function_lists[]=
{
	#include "sensor_otp_milan.h"
};

/*************************************************
  Function    : is_exist_otp_function
  Description: Detect the otp we support
  Calls:
  Called By  : msm_sensor_config
  Input       : s_ctrl
  Output     : index
  Return      : true describe the otp we support
                false describe the otp we don't support

*************************************************/
bool is_exist_otp_function( struct msm_sensor_ctrl_t *s_ctrl, int32_t *index)
{
	int32_t i = 0;

	for (i=0; i<(sizeof(otp_function_lists)/sizeof(otp_function_lists[0])); ++i)
	{
        if(strlen(s_ctrl->sensordata->sensor_name) != strlen(otp_function_lists[i].sensor_name))
        {
            continue;
        }
		if (0 == strncmp(s_ctrl->sensordata->sensor_name, otp_function_lists[i].sensor_name, strlen(s_ctrl->sensordata->sensor_name)))
		{
			*index = i;
			CMR_LOGI("is_exist_otp_function success i = %d\n", i);
			return true;
		}
	}
	return false;
}

#ifdef CONFIG_HUAWEI_DSM
void camera_report_dsm_err_otp(struct msm_sensor_ctrl_t *s_ctrl, int type, uint32_t addr , int flag)
{
	ssize_t len = 0;
	int rc = 0;

	memset(camera_dsm_log_buff, 0, MSM_CAMERA_DSM_BUFFER_SIZE);

	if ( (NULL != s_ctrl) && (NULL != s_ctrl->sensordata) )
	{
		//get module info
		len = snprintf(camera_dsm_log_buff,
				MSM_CAMERA_DSM_BUFFER_SIZE,
				"Sensor name:%s",s_ctrl->sensordata->sensor_name);
	}

	if ( len >= MSM_CAMERA_DSM_BUFFER_SIZE -1 )
	{
		pr_err("write camera_dsm_log_buff overflow.\n");
		return;
	}

	switch (flag)
	{
		case OTP_WRITE_I2C_ERR:
			len += snprintf(camera_dsm_log_buff+len, MSM_CAMERA_DSM_BUFFER_SIZE-len,
				"[Camera OTP]I2C error. I2C write error.\n");
			if (len >= MSM_CAMERA_DSM_BUFFER_SIZE -1)
			{
				pr_err("write camera_dsm_log_buff overflow.\n");
				return ;
			}

			break;

		case OTP_READ_I2C_ERR:
			len += snprintf(camera_dsm_log_buff+len, MSM_CAMERA_DSM_BUFFER_SIZE-len,
				"[Camera OTP]I2C error. I2C read error.\n");
			if (len >= MSM_CAMERA_DSM_BUFFER_SIZE -1)
			{
				pr_err("write camera_dsm_log_buff overflow.\n");
				return ;
			}
			break;

		default:
			break;
	}

	rc = camera_report_dsm_err( type, addr, camera_dsm_log_buff);
	if (rc < 0)
	{
	     pr_err("%s. report dsm err fail.\n", __func__);
	}

	return ;
}
#endif