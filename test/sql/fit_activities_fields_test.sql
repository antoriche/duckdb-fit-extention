-- Test file to verify all fields in fit_activities are populated correctly

-- Create test table
CREATE TABLE test_activities AS SELECT * FROM fit_activities('./sample.fit');

-- Test 1: Check sport field content
SELECT 'Test 1: Sport field' as test_name, sport FROM test_activities;

-- Test 2: Check sub_sport field content
SELECT 'Test 2: Sub-sport field' as test_name, sub_sport FROM test_activities;

-- Test 3: Check manufacturer field content
SELECT 'Test 3: Manufacturer field' as test_name, manufacturer FROM test_activities;

-- Test 4: Check product field content
SELECT 'Test 4: Product field' as test_name, product FROM test_activities;

-- Test 5: Check all timestamp fields
SELECT 'Test 5: Timestamp fields' as test_name,
       timestamp IS NOT NULL as has_timestamp,
       local_timestamp IS NOT NULL as has_local_timestamp,
       start_time IS NOT NULL as has_start_time
FROM test_activities;

-- Test 6: Check distance and timer fields
SELECT 'Test 6: Distance/Timer fields' as test_name,
       total_distance, total_timer_time, total_elapsed_time
FROM test_activities;

-- Test 7: Check heart rate data
SELECT 'Test 7: Heart rate fields' as test_name,
       avg_heart_rate, max_heart_rate
FROM test_activities;

-- Test 8: Check speed data
SELECT 'Test 8: Speed fields' as test_name,
       avg_speed, max_speed
FROM test_activities;

-- Test 9: Check power data
SELECT 'Test 9: Power fields' as test_name,
       avg_power, max_power
FROM test_activities;

-- Test 10: Check cadence data
SELECT 'Test 10: Cadence fields' as test_name,
       avg_cadence, max_cadence
FROM test_activities;

-- Test 11: Check position data
SELECT 'Test 11: Position fields' as test_name,
       start_position_lat, start_position_long,
       end_position_lat, end_position_long
FROM test_activities;

-- Test 12: Check calorie data
SELECT 'Test 12: Calorie field' as test_name,
       total_calories
FROM test_activities;

-- Test 13: Check ascent/descent data
SELECT 'Test 13: Ascent/Descent fields' as test_name,
       total_ascent, total_descent
FROM test_activities;

-- Clean up
DROP TABLE test_activities;
