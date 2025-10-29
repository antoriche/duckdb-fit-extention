#pragma once

#include "duckdb.hpp"
#include "duckdb/common/types/timestamp.hpp"

namespace duckdb {

// Structure to hold FIT record data - aligned with documentation
struct FitRecord {
	// Basic timestamp and location
	timestamp_tz_t timestamp;
	double latitude;          // Converted from semicircles to degrees
	double longitude;         // Converted from semicircles to degrees
	double altitude;          // Meters
	double enhanced_altitude; // Meters (higher precision)

	// Speed and distance
	double distance;       // Meters (cumulative)
	double speed;          // Meters per second
	double enhanced_speed; // Meters per second (higher precision)
	double vertical_speed; // Meters per second

	// Power metrics (watts)
	uint16_t power;
	uint16_t motor_power; // E-bike motor power
	uint32_t accumulated_power;
	uint16_t compressed_accumulated_power;

	// Heart rate and physiological data
	uint8_t heart_rate; // Beats per minute
	double total_hemoglobin_conc;
	double total_hemoglobin_conc_min;
	double total_hemoglobin_conc_max;
	double saturated_hemoglobin_percent;
	double saturated_hemoglobin_percent_min;
	double saturated_hemoglobin_percent_max;

	// Cadence metrics
	uint8_t cadence; // RPM/steps per minute
	double cadence256;
	double fractional_cadence;

	// Temperature (Celsius)
	int8_t temperature;
	double core_temperature;

	// Cycling metrics
	double grade; // Percentage
	uint16_t resistance;
	uint8_t left_right_balance;        // Percentage
	double left_torque_effectiveness;  // Percentage
	double right_torque_effectiveness; // Percentage
	double left_pedal_smoothness;      // Percentage
	double right_pedal_smoothness;     // Percentage
	double combined_pedal_smoothness;  // Percentage
	int8_t left_pco;                   // Platform center offset (mm)
	int8_t right_pco;                  // Platform center offset (mm)

	// Running metrics
	double vertical_oscillation; // Millimeters
	double stance_time_percent;  // Percentage
	double stance_time;          // Milliseconds
	double stance_time_balance;
	double step_length;    // Millimeters
	double vertical_ratio; // Percentage

	// Cycling/Swimming specific
	double cycle_length; // Meters
	double cycle_length16;
	uint8_t cycles;
	uint32_t total_cycles;

	// Navigation and course
	double time_from_course; // Seconds
	uint8_t gps_accuracy;    // Meters

	// Energy and calories
	uint16_t calories; // Kilocalories

	// Zones and training
	uint8_t zone;
	string activity_type;
	string stroke_type;

	// Advanced metrics
	double time128;
	double grit;
	double flow;
	double current_stress;

	// E-bike specific
	uint16_t ebike_travel_range;
	uint8_t ebike_battery_level;
	uint8_t ebike_assist_mode;
	uint8_t ebike_assist_level_percent;
	double battery_soc; // Percentage

	// Sports specific
	double ball_speed; // Meters per second

	// Diving/Swimming specific
	uint32_t absolute_pressure;  // Pascals
	double depth;                // Meters
	double next_stop_depth;      // Meters
	uint32_t next_stop_time;     // Seconds
	uint32_t time_to_surface;    // Seconds
	uint32_t ndl_time;           // Seconds (No Decompression Limit)
	uint8_t cns_load;            // Percentage
	uint16_t n2_load;            // Percentage
	uint32_t air_time_remaining; // Seconds
	double pressure_sac;         // Pressure Surface Air Consumption
	double volume_sac;           // Volume Surface Air Consumption
	double rmv;                  // Respiratory Minute Volume
	double ascent_rate;          // Meters per second
	double po2;                  // Partial pressure O2

	// Respiratory
	uint8_t respiration_rate; // Breaths per minute
	double enhanced_respiration_rate;

	// Device info
	uint8_t device_index;

	// File source information
	string file_source;

	// Initialize all fields to invalid/zero values
	FitRecord();
};

// Structure to hold FIT activity metadata
struct FitActivity {
	uint64_t activity_id;
	string file_id;
	timestamp_tz_t timestamp;
	timestamp_tz_t local_timestamp;
	timestamp_tz_t start_time;
	double total_timer_time;
	double total_elapsed_time;
	double total_distance;
	string sport;
	string sub_sport;
	string manufacturer;
	string product;
	uint64_t device_serial_number;
	string software_version;
	uint32_t total_calories;
	double total_ascent;
	double total_descent;
	uint8_t avg_heart_rate;
	uint8_t max_heart_rate;
	double avg_speed;
	double max_speed;
	uint16_t avg_power;
	uint16_t max_power;
	uint8_t avg_cadence;
	uint8_t max_cadence;
	double start_position_lat;
	double start_position_long;
	double end_position_lat;
	double end_position_long;
	string file_source;

	FitActivity();
};

// Structure to hold FIT session data
struct FitSession {
	uint32_t session_id;
	uint64_t activity_id;
	timestamp_tz_t timestamp;
	timestamp_tz_t start_time;
	double total_elapsed_time;
	double total_timer_time;
	double total_distance;
	string sport;
	string sub_sport;
	uint32_t total_calories;
	double avg_speed;
	double max_speed;
	uint8_t avg_heart_rate;
	uint8_t max_heart_rate;
	uint8_t min_heart_rate;
	uint8_t avg_cadence;
	uint8_t max_cadence;
	uint16_t avg_power;
	uint16_t max_power;
	uint16_t normalized_power;
	double intensity_factor;
	double training_stress_score;
	uint32_t total_work;
	double total_ascent;
	double total_descent;
	uint8_t first_lap_index;
	uint8_t num_laps;
	string event;
	string event_type;
	string trigger;
	string file_source;

	FitSession();
};

// Structure to hold FIT lap data
struct FitLap {
	uint32_t lap_id;
	uint32_t session_id;
	uint64_t activity_id;
	timestamp_tz_t timestamp;
	timestamp_tz_t start_time;
	double total_elapsed_time;
	double total_timer_time;
	double total_distance;
	uint32_t total_calories;
	double avg_speed;
	double max_speed;
	uint8_t avg_heart_rate;
	uint8_t max_heart_rate;
	uint8_t min_heart_rate;
	uint8_t avg_cadence;
	uint8_t max_cadence;
	uint16_t avg_power;
	uint16_t max_power;
	double total_ascent;
	double total_descent;
	string lap_trigger;
	string event;
	string event_type;
	double start_position_lat;
	double start_position_long;
	double end_position_lat;
	double end_position_long;
	string file_source;

	FitLap();
};

// Structure to hold FIT device info
struct FitDevice {
	uint32_t device_id;
	uint64_t activity_id;
	uint8_t device_index;
	string device_type;
	string manufacturer;
	string product;
	uint64_t serial_number;
	string software_version;
	string hardware_version;
	uint32_t cum_operating_time;
	string battery_status;
	string sensor_position;
	string descriptor;
	uint8_t ant_transmission_type;
	uint16_t ant_device_number;
	string ant_network;
	string source_type;
	string product_name;
	double battery_voltage;
	string file_source;

	FitDevice();
};

// Structure to hold FIT event data
struct FitEvent {
	uint32_t event_id;
	uint64_t activity_id;
	timestamp_tz_t timestamp;
	string event;
	string event_type;
	uint32_t data;
	uint16_t data16;
	uint16_t score;
	uint16_t opponent_score;
	uint8_t front_gear_num;
	uint8_t front_gear;
	uint8_t rear_gear_num;
	uint8_t rear_gear;
	uint8_t device_index;
	string activity_type;
	timestamp_tz_t start_timestamp;
	string file_source;

	FitEvent();
};

// Structure to hold FIT user profile data
struct FitUser {
	uint32_t user_id;
	string gender;
	uint8_t age;
	double height;
	double weight;
	string language;
	int8_t time_zone;
	double activity_class;
	uint8_t running_lactate_threshold_hr;
	uint8_t cycling_lactate_threshold_hr;
	uint8_t swimming_lactate_threshold_hr;
	uint8_t default_max_running_hr;
	uint8_t default_max_biking_hr;
	uint8_t default_max_hr;
	string hr_setting;
	string speed_setting;
	string dist_setting;
	string power_setting;
	string position_setting;
	string temperature_setting;
	uint32_t local_id;
	uint64_t global_id;
	uint32_t wake_time;
	uint32_t sleep_time;
	string height_setting;
	string weight_setting;
	uint8_t resting_heart_rate;
	uint8_t default_max_swimming_hr;
	string file_source;

	FitUser();
};

} // namespace duckdb
