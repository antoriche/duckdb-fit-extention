#include "include/fit_collector.hpp"
#include "utils.hpp"

#include <string>

namespace duckdb {

// FIT epoch (1989-12-31 00:00:00 UTC) expressed as seconds since the Unix epoch.
static constexpr int64_t FIT_EPOCH_TO_UNIX_SECONDS = 631065600;

// Convert a FIT timestamp (seconds since the FIT epoch) to a DuckDB timestamp with timezone.
static timestamp_tz_t FitTimestampToTz(uint32_t fit_ts) {
	int64_t unix_seconds = static_cast<int64_t>(fit_ts) + FIT_EPOCH_TO_UNIX_SECONDS;
	return timestamp_tz_t(Timestamp::FromEpochSeconds(unix_seconds));
}

// Convert a FIT position (semicircles) to decimal degrees.
static double SemicirclesToDegrees(int32_t semicircles) {
	return semicircles * (180.0 / 2147483648.0);
}

FitDataCollector::FitDataCollector() : current_activity_type(""), current_file_source("") {
}

void FitDataCollector::SetCurrentFile(const string &file_path) {
	current_file_source = file_path;
}

void FitDataCollector::OnMesg(fit::RecordMesg &record) {
	FitRecord fitRecord = {};

	// Timestamp
	if (record.IsTimestampValid()) {
		fitRecord.timestamp = FitTimestampToTz(record.GetTimestamp());
	}

	// Position - Convert semicircles to degrees properly
	if (record.IsPositionLatValid()) {
		FIT_SINT32 lat_semicircles = record.GetPositionLat();
		if (lat_semicircles != FIT_SINT32_INVALID) { // Check for invalid value
			fitRecord.latitude = SemicirclesToDegrees(lat_semicircles);
		}
	}
	if (record.IsPositionLongValid()) {
		FIT_SINT32 long_semicircles = record.GetPositionLong();
		if (long_semicircles != FIT_SINT32_INVALID) { // Check for invalid value
			fitRecord.longitude = SemicirclesToDegrees(long_semicircles);
		}
	}

	// Altitude
	if (record.IsAltitudeValid()) {
		FIT_FLOAT32 alt = record.GetAltitude();
		if (alt != FIT_FLOAT32_INVALID) {
			fitRecord.altitude = alt;
		}
	}
	if (record.IsEnhancedAltitudeValid()) {
		FIT_FLOAT32 enh_alt = record.GetEnhancedAltitude();
		if (enh_alt != FIT_FLOAT32_INVALID) {
			fitRecord.enhanced_altitude = enh_alt;
		}
	}

	// Speed and distance
	if (record.IsDistanceValid()) {
		FIT_FLOAT32 dist = record.GetDistance();
		if (dist != FIT_FLOAT32_INVALID) {      // Check for invalid value
			fitRecord.distance = dist / 1000.0; // Convert from scaled value (cm to m)
		}
	}
	if (record.IsSpeedValid()) {
		FIT_FLOAT32 spd = record.GetSpeed();
		if (spd != FIT_FLOAT32_INVALID) { // Check for invalid value
			fitRecord.speed = spd;
		}
	}
	if (record.IsEnhancedSpeedValid()) {
		FIT_FLOAT32 enh_spd = record.GetEnhancedSpeed();
		if (enh_spd != FIT_FLOAT32_INVALID) { // Check for invalid value
			fitRecord.enhanced_speed = enh_spd;
		}
	}
	if (record.IsVerticalSpeedValid()) {
		FIT_FLOAT32 vert_spd = record.GetVerticalSpeed();
		if (vert_spd != FIT_FLOAT32_INVALID) { // Check for invalid value
			fitRecord.vertical_speed = vert_spd;
		}
	}

	// Power metrics
	if (record.IsPowerValid()) {
		FIT_UINT16 pwr = record.GetPower();
		if (pwr != FIT_UINT16_INVALID) { // Check for invalid value
			fitRecord.power = pwr;
		}
	}
	if (record.IsMotorPowerValid()) {
		FIT_UINT16 motor_pwr = record.GetMotorPower();
		if (motor_pwr != FIT_UINT16_INVALID) { // Check for invalid value
			fitRecord.motor_power = motor_pwr;
		}
	}
	if (record.IsAccumulatedPowerValid()) {
		FIT_UINT32 acc_pwr = record.GetAccumulatedPower();
		if (acc_pwr != FIT_UINT32_INVALID) { // Check for invalid value
			fitRecord.accumulated_power = acc_pwr;
		}
	}
	if (record.IsCompressedAccumulatedPowerValid()) {
		FIT_UINT16 comp_acc_pwr = record.GetCompressedAccumulatedPower();
		if (comp_acc_pwr != FIT_UINT16_INVALID) { // Check for invalid value
			fitRecord.compressed_accumulated_power = comp_acc_pwr;
		}
	}

	// Heart rate and physiological
	if (record.IsHeartRateValid()) {
		FIT_UINT8 hr = record.GetHeartRate();
		if (hr != FIT_UINT8_INVALID) { // Check for invalid value
			fitRecord.heart_rate = hr;
		}
	}
	if (record.IsTotalHemoglobinConcValid()) {
		FIT_FLOAT32 thc = record.GetTotalHemoglobinConc();
		if (thc != FIT_FLOAT32_INVALID) {          // Check for invalid value
			fitRecord.total_hemoglobin_conc = thc; // Already scaled correctly
		}
	}
	if (record.IsTotalHemoglobinConcMinValid()) {
		FIT_FLOAT32 thc_min = record.GetTotalHemoglobinConcMin();
		if (thc_min != FIT_FLOAT32_INVALID) {              // Check for invalid value
			fitRecord.total_hemoglobin_conc_min = thc_min; // Already scaled correctly
		}
	}
	if (record.IsTotalHemoglobinConcMaxValid()) {
		FIT_FLOAT32 thc_max = record.GetTotalHemoglobinConcMax();
		if (thc_max != FIT_FLOAT32_INVALID) {              // Check for invalid value
			fitRecord.total_hemoglobin_conc_max = thc_max; // Already scaled correctly
		}
	}
	if (record.IsSaturatedHemoglobinPercentValid()) {
		FIT_FLOAT32 shp = record.GetSaturatedHemoglobinPercent();
		if (shp != FIT_FLOAT32_INVALID) {                 // Check for invalid value
			fitRecord.saturated_hemoglobin_percent = shp; // Already scaled correctly
		}
	}
	if (record.IsSaturatedHemoglobinPercentMinValid()) {
		FIT_FLOAT32 shp_min = record.GetSaturatedHemoglobinPercentMin();
		if (shp_min != FIT_FLOAT32_INVALID) {                     // Check for invalid value
			fitRecord.saturated_hemoglobin_percent_min = shp_min; // Already scaled correctly
		}
	}
	if (record.IsSaturatedHemoglobinPercentMaxValid()) {
		FIT_FLOAT32 shp_max = record.GetSaturatedHemoglobinPercentMax();
		if (shp_max != FIT_FLOAT32_INVALID) {                     // Check for invalid value
			fitRecord.saturated_hemoglobin_percent_max = shp_max; // Already scaled correctly
		}
	}

	// Cadence
	if (record.IsCadenceValid()) {
		FIT_UINT8 cad = record.GetCadence();
		if (cad != FIT_UINT8_INVALID) { // Check for invalid value
			fitRecord.cadence = cad;
		}
	}
	if (record.IsCadence256Valid()) {
		FIT_FLOAT32 cad256 = record.GetCadence256();
		if (cad256 != FIT_FLOAT32_INVALID) { // Check for invalid value
			fitRecord.cadence256 = cad256;   // Already scaled correctly
		}
	}
	if (record.IsFractionalCadenceValid()) {
		FIT_FLOAT32 frac_cad = record.GetFractionalCadence();
		if (frac_cad != FIT_FLOAT32_INVALID) {       // Check for invalid value
			fitRecord.fractional_cadence = frac_cad; // Already scaled correctly
		}
	}

	// Temperature
	if (record.IsTemperatureValid()) {
		FIT_SINT8 temp = record.GetTemperature();
		if (temp != FIT_SINT8_INVALID) {  // Check for invalid value
			fitRecord.temperature = temp; // Already in Celsius
		}
	}
	if (record.IsCoreTemperatureValid()) {
		FIT_UINT16 core_temp = record.GetCoreTemperature();
		if (core_temp != FIT_UINT16_INVALID) {              // Check for invalid value
			fitRecord.core_temperature = core_temp / 100.0; // Convert from scaled value
		}
	}

	// Cycling metrics
	if (record.IsGradeValid()) {
		FIT_FLOAT32 grd = record.GetGrade();
		if (grd != FIT_FLOAT32_INVALID) { // Check for invalid value
			fitRecord.grade = grd;        // Already scaled correctly
		}
	}
	if (record.IsResistanceValid()) {
		FIT_UINT8 res = record.GetResistance();
		if (res != FIT_UINT8_INVALID) { // Check for invalid value
			fitRecord.resistance = res;
		}
	}
	if (record.IsLeftRightBalanceValid()) {
		FIT_UINT8 lr_bal = record.GetLeftRightBalance();
		if (lr_bal != FIT_UINT8_INVALID) {         // Check for invalid value
			fitRecord.left_right_balance = lr_bal; // Already in percentage
		}
	}
	if (record.IsLeftTorqueEffectivenessValid()) {
		FIT_FLOAT32 lte = record.GetLeftTorqueEffectiveness();
		if (lte != FIT_FLOAT32_INVALID) {              // Check for invalid value
			fitRecord.left_torque_effectiveness = lte; // Already scaled correctly
		}
	}
	if (record.IsRightTorqueEffectivenessValid()) {
		FIT_FLOAT32 rte = record.GetRightTorqueEffectiveness();
		if (rte != FIT_FLOAT32_INVALID) {               // Check for invalid value
			fitRecord.right_torque_effectiveness = rte; // Already scaled correctly
		}
	}
	if (record.IsLeftPedalSmoothnessValid()) {
		FIT_FLOAT32 lps = record.GetLeftPedalSmoothness();
		if (lps != FIT_FLOAT32_INVALID) {          // Check for invalid value
			fitRecord.left_pedal_smoothness = lps; // Already scaled correctly
		}
	}
	if (record.IsRightPedalSmoothnessValid()) {
		FIT_FLOAT32 rps = record.GetRightPedalSmoothness();
		if (rps != FIT_FLOAT32_INVALID) {           // Check for invalid value
			fitRecord.right_pedal_smoothness = rps; // Already scaled correctly
		}
	}
	if (record.IsCombinedPedalSmoothnessValid()) {
		FIT_FLOAT32 cps = record.GetCombinedPedalSmoothness();
		if (cps != FIT_FLOAT32_INVALID) {              // Check for invalid value
			fitRecord.combined_pedal_smoothness = cps; // Already scaled correctly
		}
	}
	if (record.IsLeftPcoValid()) {
		FIT_SINT8 lpco = record.GetLeftPco();
		if (lpco != FIT_SINT8_INVALID) { // Check for invalid value
			fitRecord.left_pco = lpco;   // Already in mm
		}
	}
	if (record.IsRightPcoValid()) {
		FIT_SINT8 rpco = record.GetRightPco();
		if (rpco != FIT_SINT8_INVALID) { // Check for invalid value
			fitRecord.right_pco = rpco;  // Already in mm
		}
	}

	// Running metrics
	if (record.IsVerticalOscillationValid()) {
		FIT_FLOAT32 vo = record.GetVerticalOscillation();
		if (vo != FIT_FLOAT32_INVALID) {         // Check for invalid value
			fitRecord.vertical_oscillation = vo; // Already scaled correctly
		}
	}
	if (record.IsStanceTimePercentValid()) {
		FIT_FLOAT32 stp = record.GetStanceTimePercent();
		if (stp != FIT_FLOAT32_INVALID) {        // Check for invalid value
			fitRecord.stance_time_percent = stp; // Already scaled correctly
		}
	}
	if (record.IsStanceTimeValid()) {
		FIT_FLOAT32 st = record.GetStanceTime();
		if (st != FIT_FLOAT32_INVALID) { // Check for invalid value
			fitRecord.stance_time = st;  // Already scaled correctly
		}
	}
	if (record.IsStanceTimeBalanceValid()) {
		FIT_FLOAT32 stb = record.GetStanceTimeBalance();
		if (stb != FIT_FLOAT32_INVALID) {        // Check for invalid value
			fitRecord.stance_time_balance = stb; // Already scaled correctly
		}
	}
	if (record.IsStepLengthValid()) {
		FIT_FLOAT32 sl = record.GetStepLength();
		if (sl != FIT_FLOAT32_INVALID) { // Check for invalid value
			fitRecord.step_length = sl;  // Already scaled correctly
		}
	}
	if (record.IsVerticalRatioValid()) {
		FIT_FLOAT32 vr = record.GetVerticalRatio();
		if (vr != FIT_FLOAT32_INVALID) {   // Check for invalid value
			fitRecord.vertical_ratio = vr; // Already scaled correctly
		}
	}

	// Continue with other fields with proper invalid value checking...
	// (Rest of the fields follow the same pattern)

	// Set activity type from current session
	fitRecord.activity_type = current_activity_type;
	fitRecord.file_source = current_file_source;

	records.push_back(fitRecord);
}

void FitDataCollector::OnMesg(fit::FileIdMesg &file_id) {
	// Create activity from file ID information
	FitActivity activity = {};
	activity.activity_id = static_cast<uint64_t>(file_id.GetSerialNumber());

	if (file_id.IsTimeCreatedValid()) {
		activity.timestamp = FitTimestampToTz(file_id.GetTimeCreated());
	}

	// Set file ID number if available
	if (file_id.IsNumberValid()) {
		activity.file_id = std::to_string(file_id.GetNumber());
	}

	// Set manufacturer and product info
	if (file_id.IsManufacturerValid()) {
		activity.manufacturer = ConvertManufacturerToString(file_id.GetManufacturer());
	}

	// Try to get product name first, then fall back to product ID
	if (file_id.IsProductNameValid()) {
		std::wstring product_name = file_id.GetProductName();
		// Convert wstring to string (simplified conversion)
		activity.product = std::string(product_name.begin(), product_name.end());
	} else if (file_id.IsProductValid()) {
		activity.product = std::to_string(file_id.GetProduct());
	}

	if (file_id.IsSerialNumberValid()) {
		activity.device_serial_number = file_id.GetSerialNumber();
	}

	activity.file_source = current_file_source;
	activities.push_back(activity);
}

void FitDataCollector::OnMesg(fit::ActivityMesg &activity) {
	// Update activity information if we have one
	if (!activities.empty()) {
		auto &act = activities.back();
		activity_name = "Activity";

		if (activity.IsTimestampValid()) {
			act.timestamp = FitTimestampToTz(activity.GetTimestamp());
		}

		if (activity.IsTotalTimerTimeValid()) {
			act.total_timer_time = activity.GetTotalTimerTime() / 1000.0; // Convert to seconds
		}

		if (activity.IsLocalTimestampValid()) {
			act.local_timestamp = FitTimestampToTz(activity.GetLocalTimestamp());
		}
	}
}

void FitDataCollector::OnMesg(fit::SessionMesg &session) {
	FitSession fit_session = {};

	// Set activity ID if we have activities
	if (!activities.empty()) {
		fit_session.activity_id = activities.back().activity_id;
	}

	fit_session.session_id = sessions.size(); // Simple incremental ID

	if (session.IsTimestampValid()) {
		fit_session.timestamp = FitTimestampToTz(session.GetTimestamp());
	}

	if (session.IsStartTimeValid()) {
		fit_session.start_time = FitTimestampToTz(session.GetStartTime());
	}

	if (session.IsTotalElapsedTimeValid()) {
		fit_session.total_elapsed_time = session.GetTotalElapsedTime() / 1000.0;
	}

	if (session.IsTotalTimerTimeValid()) {
		fit_session.total_timer_time = session.GetTotalTimerTime() / 1000.0;
	}

	if (session.IsTotalDistanceValid()) {
		fit_session.total_distance = session.GetTotalDistance() / 1000.0;
	}

	if (session.IsSportValid()) {
		uint8_t sport_code = session.GetSport();
		fit_session.sport = ConvertSportToString(sport_code);
		// Update current activity type for subsequent records
		current_activity_type = ConvertSportToString(sport_code);
	}

	if (session.IsSubSportValid()) {
		fit_session.sub_sport = ConvertSubSportToString(session.GetSubSport());
	}

	if (session.IsTotalCaloriesValid()) {
		fit_session.total_calories = session.GetTotalCalories();
	}

	if (session.IsAvgSpeedValid()) {
		fit_session.avg_speed = session.GetAvgSpeed() / 1000.0;
	}

	if (session.IsMaxSpeedValid()) {
		fit_session.max_speed = session.GetMaxSpeed() / 1000.0;
	}

	if (session.IsAvgHeartRateValid()) {
		fit_session.avg_heart_rate = session.GetAvgHeartRate();
	}

	if (session.IsMaxHeartRateValid()) {
		fit_session.max_heart_rate = session.GetMaxHeartRate();
	}

	if (session.IsAvgCadenceValid()) {
		fit_session.avg_cadence = session.GetAvgCadence();
	}

	if (session.IsMaxCadenceValid()) {
		fit_session.max_cadence = session.GetMaxCadence();
	}

	if (session.IsAvgPowerValid()) {
		fit_session.avg_power = session.GetAvgPower();
	}

	if (session.IsMaxPowerValid()) {
		fit_session.max_power = session.GetMaxPower();
	}

	if (session.IsNormalizedPowerValid()) {
		fit_session.normalized_power = session.GetNormalizedPower();
	}

	if (session.IsTotalAscentValid()) {
		fit_session.total_ascent = session.GetTotalAscent();
	}

	if (session.IsTotalDescentValid()) {
		fit_session.total_descent = session.GetTotalDescent();
	}

	if (session.IsNumLapsValid()) {
		fit_session.num_laps = session.GetNumLaps();
	}

	fit_session.file_source = current_file_source;
	sessions.push_back(fit_session);
}

void FitDataCollector::OnMesg(fit::LapMesg &lap) {
	FitLap fit_lap = {};

	// Set activity ID and session ID
	if (!activities.empty()) {
		fit_lap.activity_id = activities.back().activity_id;
	}
	if (!sessions.empty()) {
		fit_lap.session_id = sessions.back().session_id;
	}

	fit_lap.lap_id = laps.size(); // Simple incremental ID

	if (lap.IsTimestampValid()) {
		fit_lap.timestamp = FitTimestampToTz(lap.GetTimestamp());
	}

	if (lap.IsStartTimeValid()) {
		fit_lap.start_time = FitTimestampToTz(lap.GetStartTime());
	}

	if (lap.IsTotalElapsedTimeValid()) {
		fit_lap.total_elapsed_time = lap.GetTotalElapsedTime() / 1000.0;
	}

	if (lap.IsTotalTimerTimeValid()) {
		fit_lap.total_timer_time = lap.GetTotalTimerTime() / 1000.0;
	}

	if (lap.IsTotalDistanceValid()) {
		fit_lap.total_distance = lap.GetTotalDistance() / 1000.0;
	}

	if (lap.IsTotalCaloriesValid()) {
		fit_lap.total_calories = lap.GetTotalCalories();
	}

	if (lap.IsAvgSpeedValid()) {
		fit_lap.avg_speed = lap.GetAvgSpeed() / 1000.0;
	}

	if (lap.IsMaxSpeedValid()) {
		fit_lap.max_speed = lap.GetMaxSpeed() / 1000.0;
	}

	if (lap.IsAvgHeartRateValid()) {
		fit_lap.avg_heart_rate = lap.GetAvgHeartRate();
	}

	if (lap.IsMaxHeartRateValid()) {
		fit_lap.max_heart_rate = lap.GetMaxHeartRate();
	}

	if (lap.IsAvgCadenceValid()) {
		fit_lap.avg_cadence = lap.GetAvgCadence();
	}

	if (lap.IsMaxCadenceValid()) {
		fit_lap.max_cadence = lap.GetMaxCadence();
	}

	if (lap.IsAvgPowerValid()) {
		fit_lap.avg_power = lap.GetAvgPower();
	}

	if (lap.IsMaxPowerValid()) {
		fit_lap.max_power = lap.GetMaxPower();
	}

	if (lap.IsTotalAscentValid()) {
		fit_lap.total_ascent = lap.GetTotalAscent();
	}

	if (lap.IsTotalDescentValid()) {
		fit_lap.total_descent = lap.GetTotalDescent();
	}

	if (lap.IsStartPositionLatValid()) {
		int32_t lat_semicircles = lap.GetStartPositionLat();
		if (lat_semicircles != 0x7FFFFFFF) {
			fit_lap.start_position_lat = SemicirclesToDegrees(lat_semicircles);
		}
	}

	if (lap.IsStartPositionLongValid()) {
		int32_t long_semicircles = lap.GetStartPositionLong();
		if (long_semicircles != 0x7FFFFFFF) {
			fit_lap.start_position_long = SemicirclesToDegrees(long_semicircles);
		}
	}

	if (lap.IsEndPositionLatValid()) {
		int32_t lat_semicircles = lap.GetEndPositionLat();
		if (lat_semicircles != 0x7FFFFFFF) {
			fit_lap.end_position_lat = SemicirclesToDegrees(lat_semicircles);
		}
	}

	if (lap.IsEndPositionLongValid()) {
		int32_t long_semicircles = lap.GetEndPositionLong();
		if (long_semicircles != 0x7FFFFFFF) {
			fit_lap.end_position_long = SemicirclesToDegrees(long_semicircles);
		}
	}

	fit_lap.file_source = current_file_source;
	laps.push_back(fit_lap);
}

void FitDataCollector::OnMesg(fit::DeviceInfoMesg &device_info) {
	FitDevice fit_device = {};

	// Set activity ID
	if (!activities.empty()) {
		fit_device.activity_id = activities.back().activity_id;
	}

	fit_device.device_id = devices.size(); // Simple incremental ID

	if (device_info.IsDeviceIndexValid()) {
		fit_device.device_index = device_info.GetDeviceIndex();
	}

	if (device_info.IsDeviceTypeValid()) {
		fit_device.device_type = std::to_string(device_info.GetDeviceType());
	}

	if (device_info.IsManufacturerValid()) {
		fit_device.manufacturer = ConvertManufacturerToString(device_info.GetManufacturer());
	}

	if (device_info.IsProductValid()) {
		fit_device.product = std::to_string(device_info.GetProduct());
	}

	if (device_info.IsSerialNumberValid()) {
		fit_device.serial_number = device_info.GetSerialNumber();
	}

	if (device_info.IsSoftwareVersionValid()) {
		fit_device.software_version = std::to_string(device_info.GetSoftwareVersion() / 100.0);
	}

	if (device_info.IsHardwareVersionValid()) {
		fit_device.hardware_version = std::to_string(device_info.GetHardwareVersion());
	}

	if (device_info.IsCumOperatingTimeValid()) {
		fit_device.cum_operating_time = device_info.GetCumOperatingTime();
	}

	if (device_info.IsBatteryStatusValid()) {
		fit_device.battery_status = std::to_string(device_info.GetBatteryStatus());
	}

	if (device_info.IsSensorPositionValid()) {
		fit_device.sensor_position = std::to_string(device_info.GetSensorPosition());
	}

	if (device_info.IsDescriptorValid()) {
		// Convert wide string to regular string - simplified approach
		fit_device.descriptor = ""; // Skip for now due to wide string conversion
	}

	if (device_info.IsAntTransmissionTypeValid()) {
		fit_device.ant_transmission_type = device_info.GetAntTransmissionType();
	}

	if (device_info.IsAntDeviceNumberValid()) {
		fit_device.ant_device_number = device_info.GetAntDeviceNumber();
	}

	if (device_info.IsAntNetworkValid()) {
		fit_device.ant_network = std::to_string(device_info.GetAntNetwork());
	}

	if (device_info.IsSourceTypeValid()) {
		fit_device.source_type = std::to_string(device_info.GetSourceType());
	}

	if (device_info.IsProductNameValid()) {
		// Convert wide string to regular string - simplified approach
		fit_device.product_name = ""; // Skip for now due to wide string conversion
	}

	if (device_info.IsBatteryVoltageValid()) {
		fit_device.battery_voltage = device_info.GetBatteryVoltage() / 256.0;
	}

	fit_device.file_source = current_file_source;
	devices.push_back(fit_device);
}

void FitDataCollector::OnMesg(fit::EventMesg &event) {
	FitEvent fit_event = {};

	// Set activity ID
	if (!activities.empty()) {
		fit_event.activity_id = activities.back().activity_id;
	}

	fit_event.event_id = events.size(); // Simple incremental ID

	if (event.IsTimestampValid()) {
		fit_event.timestamp = FitTimestampToTz(event.GetTimestamp());
	}

	if (event.IsEventValid()) {
		fit_event.event = std::to_string(event.GetEvent());
	}

	if (event.IsEventTypeValid()) {
		fit_event.event_type = std::to_string(event.GetEventType());
	}

	if (event.IsDataValid()) {
		fit_event.data = event.GetData();
	}

	if (event.IsData16Valid()) {
		fit_event.data16 = event.GetData16();
	}

	if (event.IsScoreValid()) {
		fit_event.score = event.GetScore();
	}

	if (event.IsOpponentScoreValid()) {
		fit_event.opponent_score = event.GetOpponentScore();
	}

	if (event.IsFrontGearNumValid()) {
		fit_event.front_gear_num = event.GetFrontGearNum();
	}

	if (event.IsFrontGearValid()) {
		fit_event.front_gear = event.GetFrontGear();
	}

	if (event.IsRearGearNumValid()) {
		fit_event.rear_gear_num = event.GetRearGearNum();
	}

	if (event.IsRearGearValid()) {
		fit_event.rear_gear = event.GetRearGear();
	}

	if (event.IsDeviceIndexValid()) {
		fit_event.device_index = event.GetDeviceIndex();
	}

	fit_event.file_source = current_file_source;
	events.push_back(fit_event);
}

void FitDataCollector::OnMesg(fit::UserProfileMesg &user_profile) {
	FitUser fit_user = {};

	fit_user.user_id = users.size(); // Simple incremental ID

	if (user_profile.IsGenderValid()) {
		fit_user.gender = std::to_string(user_profile.GetGender());
	}

	if (user_profile.IsAgeValid()) {
		fit_user.age = user_profile.GetAge();
	}

	if (user_profile.IsHeightValid()) {
		fit_user.height = user_profile.GetHeight() / 100.0; // Convert to meters
	}

	if (user_profile.IsWeightValid()) {
		fit_user.weight = user_profile.GetWeight() / 10.0; // Convert to kg
	}

	if (user_profile.IsLanguageValid()) {
		fit_user.language = std::to_string(user_profile.GetLanguage());
	}

	// Note: TimeZone field may not be available in this SDK version
	// Skip timezone for now

	if (user_profile.IsActivityClassValid()) {
		fit_user.activity_class = user_profile.GetActivityClass() / 10.0;
	}

	// Note: Lactate threshold fields may not be available in this SDK version
	// Skip lactate thresholds for now

	if (user_profile.IsDefaultMaxRunningHeartRateValid()) {
		fit_user.default_max_running_hr = user_profile.GetDefaultMaxRunningHeartRate();
	}

	if (user_profile.IsDefaultMaxBikingHeartRateValid()) {
		fit_user.default_max_biking_hr = user_profile.GetDefaultMaxBikingHeartRate();
	}

	// Note: DefaultMaxHr field may not be available in this SDK version
	// Skip for now

	if (user_profile.IsHrSettingValid()) {
		fit_user.hr_setting = std::to_string(user_profile.GetHrSetting());
	}

	if (user_profile.IsSpeedSettingValid()) {
		fit_user.speed_setting = std::to_string(user_profile.GetSpeedSetting());
	}

	if (user_profile.IsDistSettingValid()) {
		fit_user.dist_setting = std::to_string(user_profile.GetDistSetting());
	}

	if (user_profile.IsPowerSettingValid()) {
		fit_user.power_setting = std::to_string(user_profile.GetPowerSetting());
	}

	if (user_profile.IsPositionSettingValid()) {
		fit_user.position_setting = std::to_string(user_profile.GetPositionSetting());
	}

	if (user_profile.IsTemperatureSettingValid()) {
		fit_user.temperature_setting = std::to_string(user_profile.GetTemperatureSetting());
	}

	if (user_profile.IsLocalIdValid()) {
		fit_user.local_id = user_profile.GetLocalId();
	}

	// Note: GlobalId requires an index parameter - use index 0 as default
	if (user_profile.IsGlobalIdValid(0)) {
		// fit_user.global_id = user_profile.GetGlobalId(0); // Skip for now - returns FIT_BYTE
	}

	if (user_profile.IsWakeTimeValid()) {
		fit_user.wake_time = user_profile.GetWakeTime();
	}

	if (user_profile.IsSleepTimeValid()) {
		fit_user.sleep_time = user_profile.GetSleepTime();
	}

	if (user_profile.IsHeightSettingValid()) {
		fit_user.height_setting = std::to_string(user_profile.GetHeightSetting());
	}

	if (user_profile.IsWeightSettingValid()) {
		fit_user.weight_setting = std::to_string(user_profile.GetWeightSetting());
	}

	if (user_profile.IsRestingHeartRateValid()) {
		fit_user.resting_heart_rate = user_profile.GetRestingHeartRate();
	}

	// Note: DefaultMaxSwimmingHr field may not be available in this SDK version
	// Skip for now

	fit_user.file_source = current_file_source;
	users.push_back(fit_user);
}

} // namespace duckdb
