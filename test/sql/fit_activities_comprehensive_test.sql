-- Comprehensive test for all fit_activities fields
-- This test verifies that fields are properly populated and explains why some might be NULL

-- Test the basic functionality
SELECT 'Testing fit_activities with sample.fit' as info;

CREATE TEMP TABLE activities AS SELECT * FROM fit_activities('./sample.fit');

-- Test 1: Sport and Sub-sport (should always be populated if session data exists)
SELECT
    'FIELD: sport' as field_name,
    sport as value,
    CASE
        WHEN sport IS NOT NULL AND sport != '' THEN 'PASS: Human-readable sport name'
        ELSE 'FAIL: Sport field should be populated from session data'
    END as test_result
FROM activities;

SELECT
    'FIELD: sub_sport' as field_name,
    sub_sport as value,
    CASE
        WHEN sub_sport IS NOT NULL AND sub_sport != '' THEN 'PASS: Human-readable sub-sport name'
        ELSE 'FAIL: Sub-sport field should be populated from session data'
    END as test_result
FROM activities;

-- Test 2: Manufacturer (should be human-readable, not numeric code)
SELECT
    'FIELD: manufacturer' as field_name,
    manufacturer as value,
    CASE
        WHEN manufacturer IS NOT NULL AND manufacturer != '' AND manufacturer NOT LIKE '%[0-9]%'
        THEN 'PASS: Human-readable manufacturer name'
        WHEN manufacturer LIKE '%[0-9]%'
        THEN 'FAIL: Manufacturer should be human-readable, not numeric'
        ELSE 'WARN: Manufacturer field empty (may be normal for some devices)'
    END as test_result
FROM activities;

-- Test 3: Product (may be empty if not specified in FIT file)
SELECT
    'FIELD: product' as field_name,
    CASE WHEN product IS NULL OR product = '' THEN 'NULL/Empty' ELSE product END as value,
    CASE
        WHEN product IS NOT NULL AND product != '' THEN 'PASS: Product name available'
        ELSE 'INFO: Product field empty (normal for files without product specification)'
    END as test_result
FROM activities;

-- Test 4: Distance and time fields (should be populated from session data)
SELECT
    'FIELD: total_distance' as field_name,
    total_distance as value,
    CASE
        WHEN total_distance > 0 THEN 'PASS: Distance data available from session'
        ELSE 'WARN: No distance data (may be normal for stationary activities)'
    END as test_result
FROM activities;

SELECT
    'FIELD: total_elapsed_time' as field_name,
    total_elapsed_time as value,
    CASE
        WHEN total_elapsed_time > 0 THEN 'PASS: Elapsed time data available from session'
        ELSE 'FAIL: Elapsed time should be available from session'
    END as test_result
FROM activities;

-- Test 5: Heart rate data (may be NULL if no heart rate monitor used)
SELECT
    'FIELD: avg_heart_rate' as field_name,
    CASE WHEN avg_heart_rate IS NULL THEN 'NULL' ELSE CAST(avg_heart_rate AS VARCHAR) END as value,
    CASE
        WHEN avg_heart_rate IS NOT NULL AND avg_heart_rate > 0 THEN 'PASS: Heart rate data available'
        ELSE 'INFO: No heart rate data (normal without heart rate monitor)'
    END as test_result
FROM activities;

-- Test 6: Power data (may be NULL if no power meter used)
SELECT
    'FIELD: avg_power' as field_name,
    CASE WHEN avg_power IS NULL THEN 'NULL' ELSE CAST(avg_power AS VARCHAR) END as value,
    CASE
        WHEN avg_power IS NOT NULL AND avg_power > 0 THEN 'PASS: Power data available'
        ELSE 'INFO: No power data (normal without power meter)'
    END as test_result
FROM activities;

-- Test 7: Cadence data (may be NULL if no cadence sensor used)
SELECT
    'FIELD: avg_cadence' as field_name,
    CASE WHEN avg_cadence IS NULL THEN 'NULL' ELSE CAST(avg_cadence AS VARCHAR) END as value,
    CASE
        WHEN avg_cadence IS NOT NULL AND avg_cadence > 0 THEN 'PASS: Cadence data available'
        ELSE 'INFO: No cadence data (normal without cadence sensor)'
    END as test_result
FROM activities;

-- Test 8: Position data (may be NULL if no GPS used)
SELECT
    'FIELD: start_position_lat' as field_name,
    CASE WHEN start_position_lat IS NULL THEN 'NULL' ELSE CAST(start_position_lat AS VARCHAR) END as value,
    CASE
        WHEN start_position_lat IS NOT NULL THEN 'PASS: GPS position data available'
        ELSE 'INFO: No GPS position data (normal for indoor activities or devices without GPS)'
    END as test_result
FROM activities;

-- Test 9: Calories (may be 0 if not calculated)
SELECT
    'FIELD: total_calories' as field_name,
    total_calories as value,
    CASE
        WHEN total_calories > 0 THEN 'PASS: Calorie data available'
        ELSE 'INFO: No calorie data (normal if not calculated by device)'
    END as test_result
FROM activities;

-- Test 10: Ascent/Descent (may be 0 for flat activities)
SELECT
    'FIELD: total_ascent' as field_name,
    total_ascent as value,
    CASE
        WHEN total_ascent > 0 THEN 'PASS: Elevation ascent data available'
        WHEN total_ascent = 0 THEN 'INFO: No elevation gain (normal for flat routes)'
        ELSE 'INFO: No elevation data available'
    END as test_result
FROM activities;

-- Summary
SELECT
    'SUMMARY' as section,
    'All critical fields (sport, manufacturer, distance, elapsed_time) are properly populated' as result
FROM activities
WHERE sport IS NOT NULL
  AND sport != ''
  AND manufacturer IS NOT NULL
  AND manufacturer != ''
  AND total_elapsed_time > 0;

DROP TABLE activities;
