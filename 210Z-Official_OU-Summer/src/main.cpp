/**
 * @file main.cpp
 * @author Zechariah Wang
 * @brief Main logic file. Runs pre-init, init, auton, and op-control logic
 * @version 0.1
 * @date 2023-07-04
 */

#include "main.h"
#include "display/lv_objx/lv_label.h"
#include "pros/motors.h"
#include "vector"
#include "variant"
#include "array"
#include "map"
#include "string"

constexpr     u_int64_t time_dt           = 100000; // Time until initialize phase ends. Effectively infinite.
constexpr     u_int16_t delayAmount    = 10; // Dont overload the CPU during OP control
char          buffer[100];

lv_obj_t *sensor_button_home; lv_obj_t *auton_button_home; lv_obj_t *misc_button_home; lv_obj_t *game_button_home; lv_obj_t *welcomeDisplay; lv_obj_t *home_welcome_text; lv_obj_t *home_page = lv_page_create(lv_scr_act(), NULL);
lv_obj_t *odom_readings_sensor; lv_obj_t *dt_readings_sensor; lv_obj_t *sensor1_readings_sensor; lv_obj_t *sensor2_readings_sensor; lv_obj_t *sensor3_readings_sensor; lv_obj_t *sensor4_readings_sensor; lv_obj_t *return_button_sensor;
lv_obj_t *current_auton_display_selector; lv_obj_t *prev_auton_button_selector; lv_obj_t *next_auton_button_selector; lv_obj_t *select_auton_button_selector; lv_obj_t *return_auton_button_selector;
lv_obj_t *controller_status_game; lv_obj_t *battery_percent_game; lv_obj_t *battery_temp_game; lv_obj_t *time_since_startup_game; lv_obj_t *competition_stat_game; lv_obj_t *return_button_game;
lv_obj_t *debug_line1_misc; lv_obj_t *debug_line2_misc; lv_obj_t *debug_line3_misc; lv_obj_t *debug_line4_misc; lv_obj_t *debug_line5_misc;
lv_obj_t *debug_line6_misc; lv_obj_t *debug_line7_misc; lv_obj_t *debug_line8_misc; lv_obj_t *debug_line9_misc; lv_obj_t *debug_line10_misc; lv_obj_t *return_button_misc;
lv_obj_t *displayDataL1; lv_obj_t *displayDataL2; lv_obj_t *displayDataL3; lv_obj_t *displayDataL4; lv_obj_t *displayDataL5; lv_obj_t *debugLine1; lv_obj_t *debugLine2;

std::map<int, std::string> auton_Legend = {
    { 1, "Solo Win Point" },
    { 2, "LS Priority: Six Disks" },
    { 3, "LS Priority: Single Roller" },
	{ 4, "RS Priority: Six Disks " },
    { 5, "RS Priority: Single Roller" },
    { 6, "Empty Slot" },
    { 7, "Empty Slot" },
    { 8, "Empty Slot" },
    { 9, "Empty Slot" },
    { 10,"Empty Slot" }
};

void display_homePage()
{
	lv_obj_set_hidden(sensor_button_home, false);  
	lv_obj_set_hidden(auton_button_home, false);
	lv_obj_set_hidden(misc_button_home, false);
	lv_obj_set_hidden(game_button_home, false);
	lv_obj_set_hidden(home_welcome_text, false);

	lv_obj_set_hidden(odom_readings_sensor, true);  
	lv_obj_set_hidden(dt_readings_sensor, true);
	lv_obj_set_hidden(sensor1_readings_sensor, true);
	lv_obj_set_hidden(sensor2_readings_sensor, true);
	lv_obj_set_hidden(sensor3_readings_sensor, true);
	lv_obj_set_hidden(sensor4_readings_sensor, true);
	lv_obj_set_hidden(return_button_sensor, true);

	lv_obj_set_hidden(current_auton_display_selector, true);  
	lv_obj_set_hidden(next_auton_button_selector, true);
	lv_obj_set_hidden(select_auton_button_selector, true);
	lv_obj_set_hidden(prev_auton_button_selector, true);
	lv_obj_set_hidden(return_auton_button_selector, true);
}

void display_sensorPage()
{
	lv_obj_set_hidden(odom_readings_sensor, false);  
	lv_obj_set_hidden(dt_readings_sensor, false);
	lv_obj_set_hidden(sensor1_readings_sensor, false);
	lv_obj_set_hidden(sensor2_readings_sensor, false);
	lv_obj_set_hidden(sensor3_readings_sensor, false);
	lv_obj_set_hidden(sensor4_readings_sensor, false);
	lv_obj_set_hidden(return_button_sensor, false);
}

void display_autonPage()
{
	lv_obj_set_hidden(current_auton_display_selector, false);  
	lv_obj_set_hidden(next_auton_button_selector, false);
	lv_obj_set_hidden(prev_auton_button_selector, false);
	lv_obj_set_hidden(return_auton_button_selector, false);
	lv_obj_set_hidden(select_auton_button_selector, false);
}

void display_gamePage()
{
	lv_obj_set_hidden(controller_status_game, false);  
	lv_obj_set_hidden(battery_percent_game, false);
	lv_obj_set_hidden(battery_temp_game, false);
	lv_obj_set_hidden(time_since_startup_game, false);
	lv_obj_set_hidden(competition_stat_game, false);
	lv_obj_set_hidden(return_button_game, false);
}

void display_miscPage()
{
	lv_obj_set_hidden(debug_line1_misc, false);  
	lv_obj_set_hidden(debug_line2_misc, false);
	lv_obj_set_hidden(debug_line3_misc, false);
	lv_obj_set_hidden(debug_line4_misc, false);
	lv_obj_set_hidden(debug_line5_misc, false);
	lv_obj_set_hidden(debug_line6_misc, false);
	lv_obj_set_hidden(return_button_misc, false);
}

void hide_homePage()
{
	lv_obj_set_hidden(sensor_button_home, true);  
	lv_obj_set_hidden(auton_button_home, true);
	lv_obj_set_hidden(misc_button_home, true);
	lv_obj_set_hidden(game_button_home, true);
	lv_obj_set_hidden(home_welcome_text, true);
}

void hide_sensorPage()
{
	lv_obj_set_hidden(odom_readings_sensor, true);  
	lv_obj_set_hidden(dt_readings_sensor, true);
	lv_obj_set_hidden(sensor1_readings_sensor, true);
	lv_obj_set_hidden(sensor2_readings_sensor, true);
	lv_obj_set_hidden(sensor3_readings_sensor, true);
	lv_obj_set_hidden(sensor4_readings_sensor, true);
	lv_obj_set_hidden(return_button_sensor, true);
}

void hide_autonPage()
{
	lv_obj_set_hidden(current_auton_display_selector, true);  
	lv_obj_set_hidden(next_auton_button_selector, true);
	lv_obj_set_hidden(select_auton_button_selector, true);
	lv_obj_set_hidden(prev_auton_button_selector, true);
	lv_obj_set_hidden(return_auton_button_selector, true);
}

void hide_gamePage()
{
	lv_obj_set_hidden(controller_status_game, true);  
	lv_obj_set_hidden(battery_percent_game, true);
	lv_obj_set_hidden(battery_temp_game, true);
	lv_obj_set_hidden(time_since_startup_game, true);
	lv_obj_set_hidden(competition_stat_game, true);
	lv_obj_set_hidden(return_button_game, true);
}

void hide_miscPage()
{
	lv_obj_set_hidden(debug_line1_misc, true);  
	lv_obj_set_hidden(debug_line2_misc, true);
	lv_obj_set_hidden(debug_line3_misc, true);
	lv_obj_set_hidden(debug_line4_misc, true);
	lv_obj_set_hidden(debug_line5_misc, true);
	lv_obj_set_hidden(debug_line6_misc, true);
	lv_obj_set_hidden(return_button_misc, true);
}

static lv_res_t return_to_home(lv_obj_t *btn){
	lv_obj_set_hidden(sensor_button_home, false);  
	lv_obj_set_hidden(auton_button_home, false);
	lv_obj_set_hidden(misc_button_home, false);
	lv_obj_set_hidden(game_button_home, false);
	lv_obj_set_hidden(home_welcome_text, false);
	lv_obj_set_hidden(odom_readings_sensor, true); // sensor
	lv_obj_set_hidden(dt_readings_sensor, true);
	lv_obj_set_hidden(sensor1_readings_sensor, true);
	lv_obj_set_hidden(sensor2_readings_sensor, true);
	lv_obj_set_hidden(sensor3_readings_sensor, true);
	lv_obj_set_hidden(sensor4_readings_sensor, true);
	lv_obj_set_hidden(return_button_sensor, true);
	lv_obj_set_hidden(current_auton_display_selector, true); // auton selector
	lv_obj_set_hidden(next_auton_button_selector, true);
	lv_obj_set_hidden(current_auton_display_selector, true);
	lv_obj_set_hidden(select_auton_button_selector, true);
	lv_obj_set_hidden(prev_auton_button_selector, true);
	lv_obj_set_hidden(return_auton_button_selector, true);
	lv_obj_set_hidden(controller_status_game, true); // game 
	lv_obj_set_hidden(battery_percent_game, true);
	lv_obj_set_hidden(battery_temp_game, true);
	lv_obj_set_hidden(time_since_startup_game, true);
	lv_obj_set_hidden(competition_stat_game, true);
	lv_obj_set_hidden(return_button_game, true);
	lv_obj_set_hidden(debug_line1_misc, true); // misc
	lv_obj_set_hidden(debug_line2_misc, true);
	lv_obj_set_hidden(debug_line3_misc, true);
	lv_obj_set_hidden(debug_line4_misc, true);
	lv_obj_set_hidden(debug_line5_misc, true);
	lv_obj_set_hidden(debug_line6_misc, true);
	lv_obj_set_hidden(return_button_misc, true);
	return 0;
}

static lv_res_t sensor_on_click(lv_obj_t *btn){
	hide_homePage();
	hide_autonPage();
	hide_gamePage();
	hide_miscPage();
	display_sensorPage();
	return 0;
}

static lv_res_t auton_on_click(lv_obj_t *btn){
	hide_homePage();
	hide_sensorPage();
	hide_gamePage();
	hide_miscPage();
	display_autonPage();
	return 0;
}

static lv_res_t misc_on_click(lv_obj_t *btn){
	hide_homePage();
	hide_sensorPage();
	hide_gamePage();
	hide_autonPage();
	display_miscPage();
	return 0;
}

static lv_res_t game_on_click(lv_obj_t *btn){
	hide_homePage();
	hide_sensorPage();
	hide_autonPage();
	hide_miscPage();
	display_gamePage();
	return 0;
}

//-- LVGL on input functions //--
static lv_res_t selectAuton(lv_obj_t *btn){
	static bool pressed = true;
	if (pressed) { auton_finalized = 1; } 
	return 0;
}

//-- LVGL prev auton selector functions
static lv_res_t onPrevPress(lv_obj_t *btn){
    selected_auton -= 1;
    if (selected_auton > 10){
        selected_auton = 1;
		sprintf(buffer, SYMBOL_LIST " Selected Path %d: %s", selected_auton, auton_Legend[selected_auton].c_str());
        lv_label_set_text(current_auton_display_selector, buffer);
    }
    else if (selected_auton < 1){
        selected_auton = 10;
		sprintf(buffer, SYMBOL_LIST " Selected Path %d: %s", selected_auton, auton_Legend[selected_auton].c_str());
        lv_label_set_text(current_auton_display_selector, buffer);
	}
	sprintf(buffer, SYMBOL_LIST " Selected Path %d: %s", selected_auton, auton_Legend[selected_auton].c_str());
    lv_label_set_text(current_auton_display_selector, buffer);
	return 1;
}

//-- LVGL next auton selector functions
static lv_res_t onNextPress(lv_obj_t *btn){
	selected_auton += 1;
    if (selected_auton > 10){
        selected_auton = 1;
		sprintf(buffer, SYMBOL_LIST " Selected Path %d: %s", selected_auton, auton_Legend[selected_auton].c_str());
        lv_label_set_text(current_auton_display_selector, buffer);
    }
    else if (selected_auton < 1){
        selected_auton = 10;
		sprintf(buffer, SYMBOL_LIST " Selected Path %d: %s", selected_auton, auton_Legend[selected_auton].c_str());
        lv_label_set_text(debugLine1, buffer);
    }
	sprintf(buffer, SYMBOL_LIST " Selected Path %d: %s", selected_auton, auton_Legend[selected_auton].c_str());
    lv_label_set_text(current_auton_display_selector, buffer);
	return 1;
}

void initialize() { // Init function control

    static lv_style_t sensor_button_style;                         
    lv_style_copy(&sensor_button_style, &lv_style_pretty);                    
    sensor_button_style.body.main_color = LV_COLOR_MAKE(47, 144, 212);         
    sensor_button_style.body.grad_color = LV_COLOR_MAKE(47, 144, 212);
	sensor_button_style.body.radius = 8;                       
    sensor_button_style.text.color = LV_COLOR_WHITE;     

    static lv_style_t auton_button_style;                         
    lv_style_copy(&sensor_button_style, &lv_style_pretty);                    
    sensor_button_style.body.main_color = LV_COLOR_MAKE(47, 144, 212);         
    sensor_button_style.body.grad_color = LV_COLOR_MAKE(47, 144, 212);
	sensor_button_style.body.radius = 8;                       
    sensor_button_style.text.color = LV_COLOR_WHITE;  

    static lv_style_t misc_button_style;                         
    lv_style_copy(&sensor_button_style, &lv_style_pretty);                    
    sensor_button_style.body.main_color = LV_COLOR_MAKE(47, 144, 212);         
    sensor_button_style.body.grad_color = LV_COLOR_MAKE(47, 144, 212);
	sensor_button_style.body.radius = 8;                       
    sensor_button_style.text.color = LV_COLOR_WHITE;  

    static lv_style_t game_button_style;                         
    lv_style_copy(&sensor_button_style, &lv_style_pretty);                    
    sensor_button_style.body.main_color = LV_COLOR_MAKE(47, 144, 212);         
    sensor_button_style.body.grad_color = LV_COLOR_MAKE(47, 144, 212);
	sensor_button_style.body.radius = 8;                       
    sensor_button_style.text.color = LV_COLOR_WHITE;           

    static lv_style_t style_home_page;                         
    lv_style_copy(&style_home_page, &lv_style_pretty);              
    style_home_page.body.main_color = lv_color_hsv_to_rgb(0, 0, 7);   
    style_home_page.body.grad_color = lv_color_hsv_to_rgb(0, 0, 7);    
    style_home_page.body.border.width = 1;   
	style_home_page.text.color = LV_COLOR_WHITE; 
	lv_obj_set_size(home_page, 500, 300);
	lv_obj_set_style(home_page, &style_home_page);

	sensor_button_home = lv_btn_create(lv_scr_act(), NULL);
	lv_btn_set_style(sensor_button_home, LV_BTN_STYLE_REL, &sensor_button_style);
	lv_btn_set_style(sensor_button_home, LV_BTN_STYLE_PR, &sensor_button_style);
	lv_btn_set_action(sensor_button_home, LV_BTN_ACTION_CLICK, sensor_on_click); 
    lv_obj_align(sensor_button_home, NULL, LV_ALIGN_CENTER, -100, -20);
	lv_obj_set_height(sensor_button_home, 60);
	lv_obj_set_width(sensor_button_home, 160);
    lv_obj_set_style(sensor_button_home, &sensor_button_style);
	lv_obj_t *sensor_buttonText = lv_label_create(home_page, NULL);
    lv_label_set_text(sensor_buttonText, "");  
    lv_obj_set_x(sensor_buttonText, 50); 
    sensor_buttonText = lv_label_create(sensor_button_home, NULL);
    lv_label_set_text(sensor_buttonText, SYMBOL_GPS " SENSOR");
	
	auton_button_home = lv_btn_create(lv_scr_act(), NULL);
	lv_btn_set_style(auton_button_home, LV_BTN_STYLE_REL, &sensor_button_style);
	lv_btn_set_style(auton_button_home, LV_BTN_STYLE_PR, &sensor_button_style);
	lv_btn_set_action(auton_button_home, LV_BTN_ACTION_CLICK, auton_on_click); 
    lv_obj_align(auton_button_home, NULL, LV_ALIGN_CENTER, 70, -20);
	lv_obj_set_height(auton_button_home, 60);
	lv_obj_set_width(auton_button_home, 160);
    lv_obj_set_style(auton_button_home, &sensor_button_style);
	lv_obj_t *auton_buttonText = lv_label_create(home_page, NULL);
    lv_label_set_text(auton_buttonText, "");  
    lv_obj_set_x(auton_buttonText, 50); 
    auton_buttonText = lv_label_create(auton_button_home, NULL);
    lv_label_set_text(auton_buttonText, SYMBOL_DIRECTORY " AUTON");

	misc_button_home = lv_btn_create(lv_scr_act(), NULL);
	lv_btn_set_style(misc_button_home, LV_BTN_STYLE_REL, &sensor_button_style);
	lv_btn_set_style(misc_button_home, LV_BTN_STYLE_PR, &sensor_button_style);
	lv_btn_set_action(misc_button_home, LV_BTN_ACTION_CLICK, misc_on_click); 
    lv_obj_align(misc_button_home, NULL, LV_ALIGN_CENTER, 70, 50);
	lv_obj_set_height(misc_button_home, 60);
	lv_obj_set_width(misc_button_home, 160);
    lv_obj_set_style(misc_button_home, &sensor_button_style);
	lv_obj_t *misc_buttonText = lv_label_create(home_page, NULL);
    lv_label_set_text(misc_buttonText, "");  
    lv_obj_set_x(misc_buttonText, 50); 
    misc_buttonText = lv_label_create(misc_button_home, NULL);
    lv_label_set_text(misc_buttonText, SYMBOL_COPY " MISC");

	game_button_home = lv_btn_create(lv_scr_act(), NULL);
	lv_btn_set_style(game_button_home, LV_BTN_STYLE_REL, &sensor_button_style);
	lv_btn_set_style(game_button_home, LV_BTN_STYLE_PR, &sensor_button_style);
	lv_btn_set_action(game_button_home, LV_BTN_ACTION_CLICK, game_on_click); 
    lv_obj_align(game_button_home, NULL, LV_ALIGN_CENTER, -100, 50);
	lv_obj_set_height(game_button_home, 60);
	lv_obj_set_width(game_button_home, 160);
    lv_obj_set_style(game_button_home, &sensor_button_style);
	lv_obj_t *game_buttonText = lv_label_create(home_page, NULL);
    lv_label_set_text(game_buttonText, "");  
    lv_obj_set_x(game_buttonText, 50); 
    game_buttonText = lv_label_create(game_button_home, NULL);
    lv_label_set_text(game_buttonText, SYMBOL_CHARGE " GAME");

    home_welcome_text =  lv_label_create(lv_scr_act(), NULL);
    lv_label_set_text(home_welcome_text, "Welcome 210Z, ");
    lv_obj_align(home_welcome_text, NULL, LV_ALIGN_IN_LEFT_MID, 85, -80);

	// Sensor Page
    odom_readings_sensor =  lv_label_create(lv_scr_act(), NULL);
    lv_label_set_text(odom_readings_sensor, SYMBOL_GPS " X: 0.0 Y: 0.0 Theta: 0.0");
    lv_obj_align(odom_readings_sensor, NULL, LV_ALIGN_IN_LEFT_MID, 20, -75);

    dt_readings_sensor =  lv_label_create(lv_scr_act(), NULL);
    lv_label_set_text(dt_readings_sensor, SYMBOL_LIST " FL: 0.0 BL: 0.0 FL: 0.0 FR: 0.0");
    lv_obj_align(dt_readings_sensor, NULL, LV_ALIGN_IN_LEFT_MID, 20, -55);

    sensor1_readings_sensor =  lv_label_create(lv_scr_act(), NULL);
    lv_label_set_text(sensor1_readings_sensor, SYMBOL_REFRESH " Cata Pos: 0.0");
    lv_obj_align(sensor1_readings_sensor, NULL, LV_ALIGN_IN_LEFT_MID, 20, -35);

    sensor2_readings_sensor =  lv_label_create(lv_scr_act(), NULL);
    lv_label_set_text(sensor2_readings_sensor, SYMBOL_LOOP " Intake Status: 0.0");
    lv_obj_align(sensor2_readings_sensor, NULL, LV_ALIGN_IN_LEFT_MID, 20, -15);

    sensor3_readings_sensor =  lv_label_create(lv_scr_act(), NULL);
    lv_label_set_text(sensor3_readings_sensor, SYMBOL_IMAGE " Ultrasonic Status: 0.0");
    lv_obj_align(sensor3_readings_sensor, NULL, LV_ALIGN_IN_LEFT_MID, 20, 5);

    sensor4_readings_sensor =  lv_label_create(lv_scr_act(), NULL);
    lv_label_set_text(sensor4_readings_sensor, SYMBOL_EJECT "  Limit Status: 0.0");
    lv_obj_align(sensor4_readings_sensor, NULL, LV_ALIGN_IN_LEFT_MID, 20, 25);

	return_button_sensor = lv_btn_create(lv_scr_act(), NULL);
	lv_btn_set_style(return_button_sensor, LV_BTN_STYLE_REL, &sensor_button_style);
	lv_btn_set_style(return_button_sensor, LV_BTN_STYLE_PR, &sensor_button_style);
	lv_btn_set_action(return_button_sensor, LV_BTN_ACTION_CLICK, return_to_home); 
    lv_obj_align(return_button_sensor, NULL, LV_ALIGN_CENTER, 100, 110);
	lv_obj_set_height(return_button_sensor, 30);
	lv_obj_set_width(return_button_sensor, 160);
    lv_obj_set_style(return_button_sensor, &sensor_button_style);
	lv_obj_t *return_buttonText = lv_label_create(home_page, NULL);
    lv_label_set_text(return_buttonText, "");  
    lv_obj_set_x(return_buttonText, 50); 
    return_buttonText = lv_label_create(return_button_sensor, NULL);
    lv_label_set_text(return_buttonText, SYMBOL_CLOSE " Return");

	// Auton page
    current_auton_display_selector =  lv_label_create(lv_scr_act(), NULL);
    lv_label_set_text(current_auton_display_selector, " Current Selected Path: Solo Win Point");
    lv_obj_align(current_auton_display_selector, NULL, LV_ALIGN_CENTER, -30, -30);

	prev_auton_button_selector = lv_btn_create(lv_scr_act(), NULL);
	lv_btn_set_style(prev_auton_button_selector, LV_BTN_STYLE_REL, &sensor_button_style);
	lv_btn_set_style(prev_auton_button_selector, LV_BTN_STYLE_PR, &sensor_button_style);
	lv_btn_set_action(prev_auton_button_selector, LV_BTN_ACTION_CLICK, onPrevPress); 
    lv_obj_align(prev_auton_button_selector, NULL, LV_ALIGN_CENTER, -145, 30);
	lv_obj_set_height(prev_auton_button_selector, 40);
	lv_obj_set_width(prev_auton_button_selector, 130);
    lv_obj_set_style(prev_auton_button_selector, &sensor_button_style);
	lv_obj_t *prev_buttonText_auton = lv_label_create(home_page, NULL);
    lv_label_set_text(prev_buttonText_auton, "");  
    lv_obj_set_x(prev_buttonText_auton, 50); 
    prev_buttonText_auton = lv_label_create(prev_auton_button_selector, NULL);
    lv_label_set_text(prev_buttonText_auton, SYMBOL_PREV " PREV");

	select_auton_button_selector = lv_btn_create(lv_scr_act(), NULL);
	lv_btn_set_style(select_auton_button_selector, LV_BTN_STYLE_REL, &sensor_button_style);
	lv_btn_set_style(select_auton_button_selector, LV_BTN_STYLE_PR, &sensor_button_style);
	lv_btn_set_action(select_auton_button_selector, LV_BTN_ACTION_CLICK, selectAuton); 
    lv_obj_align(select_auton_button_selector, NULL, LV_ALIGN_CENTER, -5, 30);
	lv_obj_set_height(select_auton_button_selector, 40);
	lv_obj_set_width(select_auton_button_selector, 130);
    lv_obj_set_style(select_auton_button_selector, &sensor_button_style);
	lv_obj_t *select_buttonText_auton = lv_label_create(home_page, NULL);
    lv_label_set_text(select_buttonText_auton, "");  
    lv_obj_set_x(select_buttonText_auton, 50); 
    select_buttonText_auton = lv_label_create(select_auton_button_selector, NULL);
    lv_label_set_text(select_buttonText_auton, SYMBOL_UPLOAD " SELECT");

	next_auton_button_selector = lv_btn_create(lv_scr_act(), NULL);
	lv_btn_set_style(next_auton_button_selector, LV_BTN_STYLE_REL, &sensor_button_style);
	lv_btn_set_style(next_auton_button_selector, LV_BTN_STYLE_PR, &sensor_button_style);
	lv_btn_set_action(next_auton_button_selector, LV_BTN_ACTION_CLICK, onNextPress); 
    lv_obj_align(next_auton_button_selector, NULL, LV_ALIGN_CENTER, 135, 30);
	lv_obj_set_height(next_auton_button_selector, 40);
	lv_obj_set_width(next_auton_button_selector, 130);
    lv_obj_set_style(next_auton_button_selector, &sensor_button_style);
	lv_obj_t *next_buttonText_auton = lv_label_create(home_page, NULL);
    lv_label_set_text(next_buttonText_auton, "");  
    lv_obj_set_x(next_buttonText_auton, 50); 
    next_buttonText_auton = lv_label_create(next_auton_button_selector, NULL);
    lv_label_set_text(next_buttonText_auton, "NEXT " SYMBOL_NEXT);

	return_auton_button_selector = lv_btn_create(lv_scr_act(), NULL);
	lv_btn_set_style(return_auton_button_selector, LV_BTN_STYLE_REL, &sensor_button_style);
	lv_btn_set_style(return_auton_button_selector, LV_BTN_STYLE_PR, &sensor_button_style);
	lv_btn_set_action(return_auton_button_selector, LV_BTN_ACTION_CLICK, return_to_home); 
    lv_obj_align(return_auton_button_selector, NULL, LV_ALIGN_CENTER, 100, 110);
	lv_obj_set_height(return_auton_button_selector, 30);
	lv_obj_set_width(return_auton_button_selector, 160);
    lv_obj_set_style(return_auton_button_selector, &sensor_button_style);
	lv_obj_t *return_buttonText_auton = lv_label_create(home_page, NULL);
    lv_label_set_text(return_buttonText_auton, "");  
    lv_obj_set_x(return_buttonText_auton, 50); 
    return_buttonText_auton = lv_label_create(return_auton_button_selector, NULL);
    lv_label_set_text(return_buttonText_auton, SYMBOL_CLOSE" Return");

	// game page
    controller_status_game =  lv_label_create(lv_scr_act(), NULL);
    lv_label_set_text(controller_status_game, "Controller Status: Adequate ");
    lv_obj_align(controller_status_game, NULL, LV_ALIGN_IN_LEFT_MID, 20, -75);

    battery_percent_game =  lv_label_create(lv_scr_act(), NULL);
    lv_label_set_text(battery_percent_game, "Battery Percentage: 100%");
    lv_obj_align(battery_percent_game, NULL, LV_ALIGN_IN_LEFT_MID, 20, -55);

    battery_temp_game =  lv_label_create(lv_scr_act(), NULL);
    lv_label_set_text(battery_temp_game, "Battery Temperature: 100%");
    lv_obj_align(battery_temp_game, NULL, LV_ALIGN_IN_LEFT_MID, 20, -35);

    time_since_startup_game =  lv_label_create(lv_scr_act(), NULL);
    lv_label_set_text(time_since_startup_game, "Time Since Startup: 0ms");
    lv_obj_align(time_since_startup_game, NULL, LV_ALIGN_IN_LEFT_MID, 20, -15);

    competition_stat_game =  lv_label_create(lv_scr_act(), NULL);
    lv_label_set_text(competition_stat_game, "Competition Status: ENABLED");
    lv_obj_align(competition_stat_game, NULL, LV_ALIGN_IN_LEFT_MID, 20, 5);

	return_button_game = lv_btn_create(lv_scr_act(), NULL);
	lv_btn_set_style(return_button_game, LV_BTN_STYLE_REL, &sensor_button_style);
	lv_btn_set_style(return_button_game, LV_BTN_STYLE_PR, &sensor_button_style);
	lv_btn_set_action(return_button_game, LV_BTN_ACTION_CLICK, return_to_home); 
    lv_obj_align(return_button_game, NULL, LV_ALIGN_CENTER, 100, 110);
	lv_obj_set_height(return_button_game, 30);
	lv_obj_set_width(return_button_game, 160);
    lv_obj_set_style(return_button_game, &sensor_button_style);
	lv_obj_t *return_buttonText_game = lv_label_create(home_page, NULL);
    lv_label_set_text(return_buttonText_game, "");  
    lv_obj_set_x(return_buttonText_game, 50); 
    return_buttonText_game = lv_label_create(return_button_game, NULL);
    lv_label_set_text(return_buttonText_game, SYMBOL_CLOSE " Return");

	// misc page
    debug_line1_misc =  lv_label_create(lv_scr_act(), NULL);
    lv_label_set_text(debug_line1_misc, "Debug Line  ");
    lv_obj_align(debug_line1_misc, NULL, LV_ALIGN_IN_LEFT_MID, 20, -75);

    debug_line2_misc =  lv_label_create(lv_scr_act(), NULL);
    lv_label_set_text(debug_line2_misc, "Debug Line  ");
    lv_obj_align(debug_line2_misc, NULL, LV_ALIGN_IN_LEFT_MID, 20, -55);

    debug_line3_misc =  lv_label_create(lv_scr_act(), NULL);
    lv_label_set_text(debug_line3_misc, "Debug Line  ");
    lv_obj_align(debug_line3_misc, NULL, LV_ALIGN_IN_LEFT_MID, 20, -35);

    debug_line4_misc =  lv_label_create(lv_scr_act(), NULL);
    lv_label_set_text(debug_line4_misc, "Debug Line  ");
    lv_obj_align(debug_line4_misc, NULL, LV_ALIGN_IN_LEFT_MID, 20, -15);

    debug_line5_misc =  lv_label_create(lv_scr_act(), NULL);
    lv_label_set_text(debug_line5_misc, "Debug Line  ");
    lv_obj_align(debug_line5_misc, NULL, LV_ALIGN_IN_LEFT_MID, 20, 5);

    debug_line6_misc =  lv_label_create(lv_scr_act(), NULL);
    lv_label_set_text(debug_line6_misc, "Debug Line  ");
    lv_obj_align(debug_line6_misc, NULL, LV_ALIGN_IN_LEFT_MID, 20, 25);

	return_button_misc = lv_btn_create(lv_scr_act(), NULL);
	lv_btn_set_style(return_button_misc, LV_BTN_STYLE_REL, &sensor_button_style);
	lv_btn_set_style(return_button_misc, LV_BTN_STYLE_PR, &sensor_button_style);
	lv_btn_set_action(return_button_misc, LV_BTN_ACTION_CLICK, return_to_home); 
    lv_obj_align(return_button_misc, NULL, LV_ALIGN_CENTER, 100, 110);
	lv_obj_set_height(return_button_misc, 30);
	lv_obj_set_width(return_button_misc, 160);
    lv_obj_set_style(return_button_misc, &sensor_button_style);
	lv_obj_t *return_buttonText_misc = lv_label_create(home_page, NULL);
    lv_label_set_text(return_buttonText_misc, "");  
    lv_obj_set_x(return_buttonText_misc, 50); 
    return_buttonText_misc = lv_label_create(return_button_misc, NULL);
    lv_label_set_text(return_buttonText_misc, SYMBOL_CLOSE " Return");

	hide_sensorPage();
	hide_autonPage();
	hide_gamePage();
	hide_miscPage();

	GPS_ENABLED = false; // DETERMINES WHETHER OR NOT USING GPS SENSOR
 
	//-- Reset sensors and auton selector init //--
	pros::delay(3000);
	data.reset_all_primary_sensors();
	// imu_sensor.set_rotation(0);
	odom.set_horizontal_tracker_specs(2.75, 1.7);
	odom.set_vertical_tracker_specs(2.75, 4.6);
	imu_sensor.tare_rotation();
	sprintf(buffer, SYMBOL_LIST " Selected Path %d: %s", selected_auton, auton_Legend[selected_auton].c_str());
    lv_label_set_text(current_auton_display_selector, buffer);
	// select.receive_selector_input(time); // Enabled Auton Selector (STEP 1)
}

//--DONT TOUCH THESE FUNCTIONS--\*
void disabled() {}
void competition_initialize() {}
//------------------------------\*

/**
 * @brief Main autonomous function. PID prereqs:
 * @brief 90 DEGREES CONSTANTS: 6, 0, 45
 * @brief 45 DEGREE CONSTANTS: 6, 0.003, 35
 * @brief PID units are in inches
 *  
 */

void PurePursuitTestPath(){
    std::vector<CurvePoint> Path;

	double finalX = 15;
	double finalY = 100;
    CurvePoint StartPos(utility::get_x(), utility::get_y(), 0, 0, 10, 5, 1);
    CurvePoint newPoint1(-20, 30, 0, 0, 10, 5, 1);
    CurvePoint newPoint2(-10, 80, 0, 0, 10, 5, 1);
    CurvePoint newPoint3(15, 95, 0, 0, 10, 5, 1);
    CurvePoint end(finalX, finalY, 0, 0, 10, 5, 1);
    Path.push_back(StartPos);
    Path.push_back(newPoint1); 
    Path.push_back(newPoint2);
    Path.push_back(newPoint3);
	Path.push_back(end);

    while (true){
      odom.odometry_position_update();
      if (fabs(sqrt(pow(finalX - utility::get_x(), 2) + pow(finalY - utility::get_y(), 2))) <= 5){
		move_to_point(finalX, finalY, 900000, 900000, 3, 2.5);
        utility::stop();
        break;
      }
      FollowCurve(Path, 0);
      pros::delay(10);
    }
}

void autonomous(){  // Autonomous function control
	slew.set_slew_distance({7, 7});
	slew.set_slew_min_power({70, 70});
	mov_t.set_dt_constants(3.125, 1.6, 600); // Parameters are : Wheel diameter, gear ratio, motor cartridge type

	// selector.recieve_selector_input(time); // Enabled Auton Selector (STEP 1) ONLY FOR PROTOTYPE USE
	// select.select_current_auton(); // Enable Auton Selector (STEP 2) 

	// rot.set_r_constants(6, 0, 45);
	// rot.set_rotation_pid(270, 90);

	// boomerang(20, 20, 90, 90000000, 90000000, 0.8, 2, 2);
	// move_to_point(20, 20, 20000, 20000, 1.5, 3);

	//PurePursuitTestPath();
	move_to_point(10, 15, 20000, 20000, 1.5, 3);

	rot_r.set_r_constants(6, 0, 45);
	rot_r.set_rotation_pid(315, 90);

    mov_t.set_t_constants(0.45, 0, 5, 50);
	mov_t.set_translation_pid(-5, 70, false);

	// move_to_point(-20, 40, 20000, 20000, 1.5, 3);
	// move_to_point(-10, 80, 20000, 20000, 1.5, 3);
	// move_to_point(10, 100, 20000, 20000, 1.5, 3);
	// move_to_point(20, 110, 20000, 20000, 1.5, 3);

	PurePursuitTestPath();

	rot_r.set_r_constants(6, 0, 45);
	rot_r.set_rotation_pid(220, 90);

    mov_t.set_t_constants(0.45, 0, 5, 50);
	mov_t.set_translation_pid(-3, 70, false);

    // mov_t.set_t_constants(0.45, 0, 5, 50);
	// mov_t.set_translation_pid(40, 70, false);

    // mov_t.set_t_constants(0.45, 0, 5, 50);
	// mov_t.set_translation_pid(-40, 70, false);

}

/**
 * @brief Main driver control function. All code used during game phase outside of auton
 * 
 */

void opcontrol(){ // Driver control function
	char buffer[300]; // Display Buffer
	while (true){
		op_mov.x_drive_dt_Control();
		data_displayer.output_sensor_data(); // Display robot stats and info
		data_displayer.output_game_data(); // Display robot stats and info
		data_displayer.display_data();
		data_displayer.output_misc_data();
		odom.odometry_position_update();
		pros::delay(delayAmount); // Dont hog CPU ;)
	}
}
