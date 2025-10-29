#include "utils.hpp"

namespace duckdb {

std::string ActivityTypeToString(FIT_ACTIVITY_TYPE activity_type) {
	switch (activity_type) {
	case FIT_ACTIVITY_TYPE_GENERIC:
		return "Generic";
	case FIT_ACTIVITY_TYPE_RUNNING:
		return "Running";
	case FIT_ACTIVITY_TYPE_CYCLING:
		return "Cycling";
	case FIT_ACTIVITY_TYPE_TRANSITION:
		return "Transition";
	case FIT_ACTIVITY_TYPE_FITNESS_EQUIPMENT:
		return "Fitness Equipment";
	case FIT_ACTIVITY_TYPE_SWIMMING:
		return "Swimming";
	case FIT_ACTIVITY_TYPE_WALKING:
		return "Walking";
	case FIT_ACTIVITY_TYPE_SEDENTARY:
		return "Sedentary";
	case FIT_ACTIVITY_TYPE_ALL:
		return "All";
	case FIT_ACTIVITY_TYPE_INVALID:
	default:
		return "";
	}
}

std::string StrokeTypeToString(FIT_STROKE_TYPE stroke_type) {
	switch (stroke_type) {
	case FIT_STROKE_TYPE_NO_EVENT:
		return "No Event";
	case FIT_STROKE_TYPE_OTHER:
		return "Other";
	case FIT_STROKE_TYPE_SERVE:
		return "Serve";
	case FIT_STROKE_TYPE_FOREHAND:
		return "Forehand";
	case FIT_STROKE_TYPE_BACKHAND:
		return "Backhand";
	case FIT_STROKE_TYPE_SMASH:
		return "Smash";
	case FIT_STROKE_TYPE_INVALID:
	default:
		return "";
	}
}

std::string ConvertSportToString(uint8_t sport_code) {
	switch (sport_code) {
	case 0:
		return "Generic";
	case 1:
		return "Running";
	case 2:
		return "Cycling";
	case 3:
		return "Transition";
	case 4:
		return "Fitness Equipment";
	case 5:
		return "Swimming";
	case 6:
		return "Basketball";
	case 7:
		return "Soccer";
	case 8:
		return "Tennis";
	case 9:
		return "American Football";
	case 10:
		return "Training";
	case 11:
		return "Walking";
	case 12:
		return "Cross Country Skiing";
	case 13:
		return "Alpine Skiing";
	case 14:
		return "Snowboarding";
	case 15:
		return "Rowing";
	case 16:
		return "Mountaineering";
	case 17:
		return "Hiking";
	case 18:
		return "Multisport";
	case 19:
		return "Paddling";
	case 20:
		return "Flying";
	case 21:
		return "E-Biking";
	case 22:
		return "Motorcycling";
	case 23:
		return "Boating";
	case 24:
		return "Driving";
	case 25:
		return "Golf";
	case 26:
		return "Hang Gliding";
	case 27:
		return "Horseback Riding";
	case 28:
		return "Hunting";
	case 29:
		return "Fishing";
	case 30:
		return "Inline Skating";
	case 31:
		return "Rock Climbing";
	case 32:
		return "Sailing";
	case 33:
		return "Ice Skating";
	case 34:
		return "Sky Diving";
	case 35:
		return "Snowshoeing";
	case 36:
		return "Snowmobiling";
	case 37:
		return "Stand Up Paddleboarding";
	case 38:
		return "Surfing";
	case 39:
		return "Wakeboarding";
	case 40:
		return "Water Skiing";
	case 41:
		return "Kayaking";
	case 42:
		return "Rafting";
	case 43:
		return "Windsurfing";
	case 44:
		return "Kitesurfing";
	case 45:
		return "Tactical";
	case 46:
		return "Jumpmaster";
	case 47:
		return "Boxing";
	case 48:
		return "Floor Climbing";
	case 53:
		return "Diving";
	case 62:
		return "Hiit";
	case 67:
		return "Yoga";
	case 71:
		return "Disc Golf";
	case 254:
		return "All";
	default:
		return "Unknown (" + std::to_string(sport_code) + ")";
	}
}

std::string ConvertSubSportToString(uint8_t sub_sport_code) {
	switch (sub_sport_code) {
	case 0:
		return "Generic";
	case 1:
		return "Treadmill";
	case 2:
		return "Street";
	case 3:
		return "Trail";
	case 4:
		return "Track";
	case 5:
		return "Spin";
	case 6:
		return "Indoor Cycling";
	case 7:
		return "Road";
	case 8:
		return "Mountain";
	case 9:
		return "Downhill";
	case 10:
		return "Recumbent";
	case 11:
		return "Cyclocross";
	case 12:
		return "Hand Cycling";
	case 13:
		return "Track Cycling";
	case 14:
		return "Indoor Rowing";
	case 15:
		return "Elliptical";
	case 16:
		return "Stair Climbing";
	case 17:
		return "Lap Swimming";
	case 18:
		return "Open Water";
	case 19:
		return "Flexibility Training";
	case 20:
		return "Strength Training";
	case 21:
		return "Warm Up";
	case 22:
		return "Match";
	case 23:
		return "Exercise";
	case 24:
		return "Challenge";
	case 25:
		return "Indoor Skiing";
	case 26:
		return "Cardio Training";
	case 27:
		return "Indoor Walking";
	case 28:
		return "E-Bike Fitness";
	case 29:
		return "BMX";
	case 30:
		return "Casual Walking";
	case 31:
		return "Speed Walking";
	case 32:
		return "Bike To Run Transition";
	case 33:
		return "Run To Bike Transition";
	case 34:
		return "Swim To Bike Transition";
	case 35:
		return "ATV";
	case 36:
		return "Motocross";
	case 37:
		return "Backcountry";
	case 38:
		return "Resort";
	case 39:
		return "RC Drone";
	case 40:
		return "Wingsuit";
	case 41:
		return "Whitewater";
	case 42:
		return "Skate Skiing";
	case 43:
		return "Yoga";
	case 44:
		return "Pilates";
	case 45:
		return "Indoor Running";
	case 46:
		return "Gravel Cycling";
	case 47:
		return "E-Bike Mountain";
	case 48:
		return "Commuting";
	case 49:
		return "Mixed Surface";
	case 50:
		return "Navigate";
	case 51:
		return "Track Me";
	case 52:
		return "Map";
	case 53:
		return "Single Gas Diving";
	case 54:
		return "Multi Gas Diving";
	case 55:
		return "Gauge Diving";
	case 56:
		return "Apnea Diving";
	case 57:
		return "Apnea Hunting";
	case 58:
		return "Virtual Activity";
	case 59:
		return "Obstacle";
	case 62:
		return "Breathing";
	case 65:
		return "Sail Race";
	case 67:
		return "Ultra";
	case 68:
		return "Indoor Climbing";
	case 69:
		return "Bouldering";
	case 254:
		return "All";
	default:
		return "Unknown (" + std::to_string(sub_sport_code) + ")";
	}
}

std::string ConvertManufacturerToString(uint16_t manufacturer_code) {
	switch (manufacturer_code) {
	case 1:
		return "Garmin";
	case 2:
		return "Garmin FR405 Antfs";
	case 3:
		return "Zephyr";
	case 4:
		return "Dayton";
	case 5:
		return "IDT";
	case 6:
		return "SRM";
	case 7:
		return "Quarq";
	case 8:
		return "Ibike";
	case 9:
		return "SRM";
	case 10:
		return "Quarq";
	case 11:
		return "4iiiis";
	case 12:
		return "SRM";
	case 13:
		return "Lemond Fitness";
	case 14:
		return "Dexcom";
	case 15:
		return "Wahoo Fitness";
	case 16:
		return "Octane Fitness";
	case 17:
		return "Archinoetics";
	case 18:
		return "The Sufferfest";
	case 19:
		return "Fullspeedahead";
	case 20:
		return "Virtualtraining";
	case 21:
		return "Feedbacksports";
	case 22:
		return "Stages Cycling";
	case 23:
		return "Stages Cycling";
	case 24:
		return "Stages Cycling";
	case 25:
		return "Stages Cycling";
	case 26:
		return "Speedplay";
	case 27:
		return "Lemond Fitness";
	case 30:
		return "Cycliq";
	case 32:
		return "Powerpod";
	case 255:
		return "Development";
	default:
		return "Unknown (" + std::to_string(manufacturer_code) + ")";
	}
}

} // namespace duckdb
