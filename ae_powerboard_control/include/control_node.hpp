#ifndef CONTROL_NODE_HPP
#define CONTROL_NODE_HPP

#include "ros/ros.h"

#include <linux/reboot.h>
#include <sys/reboot.h>

#include "utils.hpp"
#include "i2c_driver.h" 
#include "pb6s40a_control.h"

#include "std_srvs/SetBool.h"
#include "ae_powerboard_control/GetEscDeviceInfo.h"
#include "ae_powerboard_control/GetBoardDeviceInfo.h"
#include "ae_powerboard_control/GetEscErrorLog.h"
#include "ae_powerboard_control/GetEscDataLog.h"
#include "ae_powerboard_control/SetLedColor.h"
#include "ae_powerboard_control/SetLedCustomColor.h"
#include "ae_powerboard_control/SetLedPredefinedEffect.h"
#include "ae_powerboard_control/SetLedCustomEffect.h"
#include "ae_powerboard_control/GetEscResistance.h"

#define DEVICE_I2C_NANO     "/dev/i2c-1"
#define DEVICE_I2C_NX       "/dev/i2c-8"

#define MAIN_TIME_PERIOD_S  0.05
#define STATE_TIME_PERIOD_S 1
#define LED_COUNT_EFFECT    8

class Control
{
    private:
        //  ******* constants ********
        enum Effect_Type
        {
            NO_EFFECT = 0,
            EFFECT_1 = 1,
        };
        //  ******* properties ********
        // ros node
        ros::NodeHandle nh_;
        // ros servers
        ros::ServiceServer esc_dev_info_srv_;
        ros::ServiceServer esc_error_log_srv_;
        ros::ServiceServer esc_data_log_srv_;
        ros::ServiceServer esc_resistance_srv_;
        ros::ServiceServer board_dev_info_srv_;
        ros::ServiceServer led_set_custom_color_srv_;
        ros::ServiceServer led_set_color_srv_;
        ros::ServiceServer led_set_custom_effect_srv_;
        ros::ServiceServer led_set_predefined_effect_srv_;
        ros::ServiceServer board_shutdown_srv_;
        // ros timers
        ros::Timer main_tim_;
        ros::Timer state_tim_;
        //i2c
        I2CDriver i2c_driver_;
        std::string i2c_port_;
        bool i2c_error_;
        Pb6s40aDroneControl *drone_control_;
        Pb6s40aLedsControl *led_control_;
        // **esc**
        //esc error log
        ERROR_WARN_LOG esc_error_log_[4];
        uint8_t esc_error_log_status_;
        //esc data log
        RUN_DATA_Struct esc_data_log_[4];
        uint8_t esc_data_log_status_;
        //esc device info
        ADB_DEVICE_INFO esc_device_info_[4];
        uint8_t esc_device_info_status_;
        //esc resistance 
        RESISTANCE_STRUCT esc_resistance_[4];
        uint8_t esc_restistance_status_;
        // **board**
        //board device info
        POWER_BOARD_INFO board_device_info_;
        bool board_device_info_status_;
        // **led**
        LEDS_COUNT mounted_leds_count_;
        //led effect
        bool led_effect_run_;
        bool led_effect_update_;
        uint8_t led_effect_type_;
        //board status
        uint8_t power_board_status_;

        //  ******* methods *******
        // init
        void Init();
        void DefaultValues();
        void SetupServices();
        void SetupTimers();
        // i2c
        void OpenI2C();
        void CloseI2C();
        //All
        void GetAll();
        //Esc
        void GetEscErrorLog();
        void GetEscDataLog();
        void GetEscDeviceInfo();
        void GetEscResistance();
        //Board
        void GetBoardDeviceInfo();
        bool CallbackBoardShutdown(std_srvs::SetBool::Request &req, std_srvs::SetBool::Response &res);
        //Callback for service
        bool CallbackEscDeviceInfo(ae_powerboard_control::GetEscDeviceInfo::Request &req, ae_powerboard_control::GetEscDeviceInfo::Response &res);
        bool CallbackEscErrorLog(ae_powerboard_control::GetEscErrorLog::Request &req, ae_powerboard_control::GetEscErrorLog::Response &res);
        bool CallbackEscDataLog(ae_powerboard_control::GetEscDataLog::Request &req, ae_powerboard_control::GetEscDataLog::Response &res);
        bool CallbackEscResistance(ae_powerboard_control::GetEscResistance::Request &req, ae_powerboard_control::GetEscResistance::Response &res);
        bool CallbackBoardDeviceInfo(ae_powerboard_control::GetBoardDeviceInfo::Request &req, ae_powerboard_control::GetBoardDeviceInfo::Response &res);
        bool CallbackLedColor(ae_powerboard_control::SetLedColor::Request &req, ae_powerboard_control::SetLedColor::Response &res);
        bool CallbackLedCustomColor(ae_powerboard_control::SetLedCustomColor::Request &req, ae_powerboard_control::SetLedCustomColor::Response &res);
        bool CallbackLedPredefinedEffect(ae_powerboard_control::SetLedPredefinedEffect::Request &req, ae_powerboard_control::SetLedPredefinedEffect::Response &res);
        bool CallbackLedCustomEffect(ae_powerboard_control::SetLedCustomEffect::Request &req, ae_powerboard_control::SetLedCustomEffect::Response &res);
        //Callback for timer
        void CallbackMainTimer(const ros::TimerEvent &event);
        void CallbackStateTimer(const ros::TimerEvent &event);
        //Led effect
        void HandleNoEffect(uint64_t ticks);
        void HandleEffect_1(uint64_t ticks);
    
    public:
        // constructor
        Control(const ros::NodeHandle &nh, std::string i2c_address);
        ~Control();
};

#endif //CONTROL_NODE_HPP