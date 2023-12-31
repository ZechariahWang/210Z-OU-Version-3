#include "main.h"


const uint64_t LINE_SENSOR_VAL = 4096; // max value to be considered an object
const uint64_t LINE_SENSOR_THRESHHOLD = 1000; // min value to notdetect anything

void power_intake(){
    if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_L1)){ intake_motor.move_voltage(-12000); }
    else if ((controller.get_digital(pros::E_CONTROLLER_DIGITAL_L2))){ intake_motor.move_voltage(12000); }
    else{ intake_motor.move_voltage(0); }
}

static bool cata_initiated = 1;
static bool cata_need_to_reset_ima_kms = true;
static bool cata_movement_enabled = false;
static bool elec_elastic_stat = false;
uint16_t cataDelay = 700;
uint16_t c_delay_counter = 0;
uint16_t c_piston_counter = 0;
static bool shot_enabled = false;

void prime_catapult(){
    if (cata_sensor.get_value() < LINE_SENSOR_THRESHHOLD){  cata_motor.move_voltage(12000); } // Cata arm is raised, time to start priming

    // Once we reach the bottom stop moving arm
    else if (cata_sensor.get_value() >= LINE_SENSOR_THRESHHOLD && cata_sensor.get_value() <= LINE_SENSOR_VAL && cata_movement_enabled == false) { 
        cata_motor.move_voltage(0); 
        cata_initiated = 0;
        cata_need_to_reset_ima_kms = false;
    }
}

void activate_cata(){
    // Validate cata shot, if good set values to be true
    if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_R1) && cata_sensor.get_value() >= LINE_SENSOR_THRESHHOLD && cata_sensor.get_value() <= LINE_SENSOR_VAL){ 
        shot_enabled = true;
        cata_need_to_reset_ima_kms = true;
        cata_initiated = 1;
        cata_movement_enabled = true;
    }

    // Shot is good, move motor for a bit to allow arm to follow through, then reset values 
    if (shot_enabled) {
        cata_motor.move_voltage(12000); 
        c_delay_counter++;
        if (c_delay_counter > 70){
            cata_need_to_reset_ima_kms = true;
            cata_initiated = 1;
            c_delay_counter = 0;
            shot_enabled = false;
            cata_movement_enabled = false;
        }
    }
}

void raw_cata(){
    if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_R1)) {
        cata_motor.move_voltage(-12000);
    }
    else{
        cata_motor.move_voltage(0);
    }
}
