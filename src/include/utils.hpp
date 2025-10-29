#pragma once

#include <string>
#include "fit_profile.hpp"

namespace duckdb {

/**
 * Converts FIT_ACTIVITY_TYPE enum value to human-readable string
 * @param activity_type The FIT activity type enum value
 * @return String representation of the activity type
 */
std::string ActivityTypeToString(FIT_ACTIVITY_TYPE activity_type);

/**
 * Converts FIT_STROKE_TYPE enum value to human-readable string
 * @param stroke_type The FIT stroke type enum value
 * @return String representation of the stroke type
 */
std::string StrokeTypeToString(FIT_STROKE_TYPE stroke_type);

/**
 * Converts FIT sport code to human-readable string
 * @param sport_code The FIT sport code (numeric value)
 * @return String representation of the sport type
 */
std::string ConvertSportToString(uint8_t sport_code);

/**
 * Converts FIT sub-sport code to human-readable string
 * @param sub_sport_code The FIT sub-sport code (numeric value)
 * @return String representation of the sub-sport type
 */
std::string ConvertSubSportToString(uint8_t sub_sport_code);

/**
 * Converts FIT manufacturer code to human-readable string
 * @param manufacturer_code The FIT manufacturer code (numeric value)
 * @return String representation of the manufacturer name
 */
std::string ConvertManufacturerToString(uint16_t manufacturer_code);

} // namespace duckdb
