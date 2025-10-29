# FIT File Data Structures Documentation

## Overview

FIT (Flexible and Interoperable Data Transfer) files are a compact binary format developed by Garmin for storing fitness and sports data. This document describes the data structures and message types available in FIT files based on the Garmin FIT SDK version 21.171.0.

## Core Architecture

### File Structure

FIT files consist of:

- **File Header**: Contains basic file information
- **Data Records**: Sequence of messages containing the actual data
- **CRC**: Checksum for data integrity

### Basic Data Types

FIT files support the following base data types:

| Type          | Size (bytes) | Description                  | Invalid Value      |
| ------------- | ------------ | ---------------------------- | ------------------ |
| `FIT_ENUM`    | 1            | Enumerated value             | 0xFF               |
| `FIT_SINT8`   | 1            | Signed 8-bit integer         | 0x7F               |
| `FIT_UINT8`   | 1            | Unsigned 8-bit integer       | 0xFF               |
| `FIT_SINT16`  | 2            | Signed 16-bit integer        | 0x7FFF             |
| `FIT_UINT16`  | 2            | Unsigned 16-bit integer      | 0xFFFF             |
| `FIT_SINT32`  | 4            | Signed 32-bit integer        | 0x7FFFFFFF         |
| `FIT_UINT32`  | 4            | Unsigned 32-bit integer      | 0xFFFFFFFF         |
| `FIT_SINT64`  | 8            | Signed 64-bit integer        | 0x7FFFFFFFFFFFFFFF |
| `FIT_UINT64`  | 8            | Unsigned 64-bit integer      | 0xFFFFFFFFFFFFFFFF |
| `FIT_FLOAT32` | 4            | 32-bit floating point        | NaN                |
| `FIT_FLOAT64` | 8            | 64-bit floating point        | NaN                |
| `FIT_STRING`  | Variable     | UTF-8 null-terminated string | 0x00               |
| `FIT_BYTE`    | 1            | Raw byte data                | 0xFF               |

### Special Data Types

- **`FIT_DATE_TIME`**: Seconds since UTC 00:00 Dec 31, 1989
- **`FIT_LOCAL_DATE_TIME`**: Seconds since 00:00 Dec 31, 1989 in local time zone
- **Position coordinates**: Stored as semicircles (degrees × 2^31 / 180)

## Message Types

FIT files contain various message types, each with a specific structure and purpose.

### Core Activity Messages

#### 1. File ID Message (`FIT_MESG_NUM_FILE_ID = 0`)

Identifies the file type and creator.

**Key Fields:**

- `type`: File type (activity, course, workout, etc.)
- `manufacturer`: Device manufacturer ID
- `product`: Product ID
- `serial_number`: Device serial number
- `time_created`: File creation timestamp

#### 2. Record Message (`FIT_MESG_NUM_RECORD = 20`)

Contains the main time-series data points during an activity.

**Core Fields:**

- `timestamp`: Time of record (seconds since 1989-12-31)
- `position_lat`: Latitude in semicircles
- `position_long`: Longitude in semicircles
- `altitude`: Altitude in meters
- `heart_rate`: Heart rate in beats per minute
- `cadence`: Cadence in revolutions per minute
- `distance`: Distance in meters
- `speed`: Speed in meters per second
- `power`: Power in watts
- `temperature`: Temperature in Celsius

**Enhanced Fields:**

- `enhanced_speed`: High-resolution speed
- `enhanced_altitude`: High-resolution altitude
- `vertical_speed`: Vertical speed in m/s
- `grade`: Grade percentage
- `gps_accuracy`: GPS accuracy in meters

**Cycling-Specific Fields:**

- `left_right_balance`: Power balance between left/right legs
- `left_torque_effectiveness`: Left leg torque effectiveness
- `right_torque_effectiveness`: Right leg torque effectiveness
- `left_pedal_smoothness`: Left pedal smoothness
- `right_pedal_smoothness`: Right pedal smoothness
- `left_pco`: Left platform center offset
- `right_pco`: Right platform center offset

**Running-Specific Fields:**

- `vertical_oscillation`: Vertical oscillation in mm
- `stance_time_percent`: Ground contact time percentage
- `stance_time`: Ground contact time in ms
- `step_length`: Step length in mm
- `vertical_ratio`: Vertical ratio percentage

**E-bike Fields:**

- `motor_power`: Motor power in watts
- `battery_soc`: Battery state of charge percentage
- `ebike_travel_range`: Travel range
- `ebike_battery_level`: Battery level
- `ebike_assist_mode`: Assist mode
- `ebike_assist_level_percent`: Assist level percentage

**Diving Fields:**

- `depth`: Depth in meters
- `absolute_pressure`: Absolute pressure in Pa
- `next_stop_depth`: Next decompression stop depth
- `next_stop_time`: Time to next stop
- `time_to_surface`: Time to surface
- `ndl_time`: No decompression limit time
- `cns_load`: CNS oxygen toxicity load
- `n2_load`: Nitrogen tissue loading

#### 3. Session Message (`FIT_MESG_NUM_SESSION = 18`)

Contains summary data for an entire session or activity segment.

**Core Fields:**

- `timestamp`: Session end time
- `start_time`: Session start time
- `total_elapsed_time`: Total elapsed time including pauses
- `total_timer_time`: Total timer time excluding pauses
- `total_distance`: Total distance
- `sport`: Sport type
- `sub_sport`: Sub-sport type
- `trigger`: What triggered the session end

**Performance Metrics:**

- `total_calories`: Total calories burned
- `avg_speed` / `max_speed`: Average and maximum speed
- `avg_heart_rate` / `max_heart_rate` / `min_heart_rate`: Heart rate statistics
- `avg_cadence` / `max_cadence`: Cadence statistics
- `avg_power` / `max_power`: Power statistics
- `normalized_power`: Normalized power
- `intensity_factor`: Intensity factor
- `training_stress_score`: Training Stress Score

**Location Data:**

- `start_position_lat` / `start_position_long`: Starting position
- `end_position_lat` / `end_position_long`: Ending position
- `nec_lat` / `nec_long`: Northeast corner of bounding box
- `swc_lat` / `swc_long`: Southwest corner of bounding box

**Elevation Data:**

- `total_ascent` / `total_descent`: Total elevation gain/loss
- `avg_altitude` / `max_altitude` / `min_altitude`: Altitude statistics

#### 4. Activity Message (`FIT_MESG_NUM_ACTIVITY = 34`)

Top-level message describing the entire activity.

**Fields:**

- `timestamp`: Activity timestamp
- `total_timer_time`: Total timer time
- `num_sessions`: Number of sessions
- `type`: Activity type (manual, auto multi-sport)
- `event`: Event type
- `event_type`: Event type classification
- `local_timestamp`: Local timestamp

#### 5. Lap Message (`FIT_MESG_NUM_LAP = 19`)

Contains data for individual laps within a session.

Similar structure to Session message but for individual laps, including:

- Lap timing and distance
- Performance metrics for the lap
- Trigger type (manual, distance, time, etc.)

### Device and Setup Messages

#### 6. Device Info Message (`FIT_MESG_NUM_DEVICE_INFO = 23`)

Information about connected devices.

**Fields:**

- `device_index`: Device identifier
- `device_type`: Type of device
- `manufacturer`: Manufacturer ID
- `product`: Product ID
- `software_version`: Software version
- `hardware_version`: Hardware version
- `serial_number`: Serial number
- `battery_status`: Battery status

#### 7. User Profile Message (`FIT_MESG_NUM_USER_PROFILE = 3`)

User configuration and personal data.

**Fields:**

- `gender`: User gender
- `age`: User age
- `height`: Height in meters
- `weight`: Weight in kg
- `language`: Language setting
- `time_zone`: Time zone setting

### Specialized Data Messages

#### 8. Event Message (`FIT_MESG_NUM_EVENT = 21`)

Records specific events during an activity.

**Fields:**

- `timestamp`: Event timestamp
- `event`: Event type (timer, lap, course_point, etc.)
- `event_type`: Event classification (start, stop, marker)
- `data`: Event-specific data

#### 9. Heart Rate Message (`FIT_MESG_NUM_HR = 132`)

Detailed heart rate data.

**Fields:**

- `timestamp`: Timestamp
- `fractional_timestamp`: Sub-second timestamp
- `time256`: High-resolution time
- `filtered_bpm`: Filtered heart rate values
- `event_timestamp`: Event timestamps

#### 10. Monitoring Message (`FIT_MESG_NUM_MONITORING = 55`)

Daily activity monitoring data.

**Fields:**

- `timestamp`: Timestamp
- `device_index`: Device index
- `calories`: Calories burned
- `distance`: Distance traveled
- `cycles`: Step/pedal cycles
- `activity_type`: Type of activity
- `intensity`: Activity intensity level

### Sensor Data Messages

#### 11. Accelerometer Data Message (`FIT_MESG_NUM_ACCELEROMETER_DATA = 165`)

Raw accelerometer sensor data.

#### 12. Gyroscope Data Message (`FIT_MESG_NUM_GYROSCOPE_DATA = 164`)

Raw gyroscope sensor data.

#### 13. Magnetometer Data Message (`FIT_MESG_NUM_MAGNETOMETER_DATA = 208`)

Raw magnetometer sensor data.

#### 14. Barometer Data Message (`FIT_MESG_NUM_BAROMETER_DATA = 209`)

Barometric pressure sensor data.

### Sleep and Health Messages

#### 15. Sleep Level Message (`FIT_MESG_NUM_SLEEP_LEVEL = 275`)

Sleep stage information.

#### 16. Stress Level Message (`FIT_MESG_NUM_STRESS_LEVEL = 227`)

Stress level measurements.

#### 17. SPO2 Data Message (`FIT_MESG_NUM_SPO2_DATA = 269`)

Blood oxygen saturation data.

#### 18. Respiration Rate Message (`FIT_MESG_NUM_RESPIRATION_RATE = 297`)

Breathing rate measurements.

## Enumerations

### Sport Types

```cpp
FIT_SPORT_GENERIC = 0
FIT_SPORT_RUNNING = 1
FIT_SPORT_CYCLING = 2
FIT_SPORT_TRANSITION = 3
FIT_SPORT_FITNESS_EQUIPMENT = 4
FIT_SPORT_SWIMMING = 5
FIT_SPORT_BASKETBALL = 6
FIT_SPORT_SOCCER = 7
FIT_SPORT_TENNIS = 8
FIT_SPORT_AMERICAN_FOOTBALL = 9
FIT_SPORT_TRAINING = 10
FIT_SPORT_WALKING = 11
FIT_SPORT_CROSS_COUNTRY_SKIING = 12
FIT_SPORT_ALPINE_SKIING = 13
FIT_SPORT_SNOWBOARDING = 14
FIT_SPORT_ROWING = 15
FIT_SPORT_MOUNTAINEERING = 16
FIT_SPORT_HIKING = 17
FIT_SPORT_MULTISPORT = 18
FIT_SPORT_PADDLING = 19
FIT_SPORT_FLYING = 20
FIT_SPORT_E_BIKING = 21
FIT_SPORT_MOTORCYCLING = 22
FIT_SPORT_BOATING = 23
FIT_SPORT_DRIVING = 24
FIT_SPORT_GOLF = 25
// ... and many more
```

### File Types

```cpp
FIT_FILE_DEVICE = 1
FIT_FILE_SETTINGS = 2
FIT_FILE_SPORT = 3
FIT_FILE_ACTIVITY = 4
FIT_FILE_WORKOUT = 5
FIT_FILE_COURSE = 6
FIT_FILE_SCHEDULES = 7
FIT_FILE_WEIGHT = 9
FIT_FILE_TOTALS = 10
FIT_FILE_GOALS = 11
FIT_FILE_BLOOD_PRESSURE = 14
FIT_FILE_MONITORING_A = 15
FIT_FILE_ACTIVITY_SUMMARY = 20
FIT_FILE_MONITORING_DAILY = 28
// ... and more
```

### Event Types

```cpp
FIT_EVENT_TIMER = 0
FIT_EVENT_WORKOUT = 3
FIT_EVENT_WORKOUT_STEP = 4
FIT_EVENT_POWER_DOWN = 5
FIT_EVENT_POWER_UP = 6
FIT_EVENT_OFF_COURSE = 7
FIT_EVENT_SESSION = 8
FIT_EVENT_LAP = 9
FIT_EVENT_COURSE_POINT = 10
FIT_EVENT_BATTERY = 11
FIT_EVENT_VIRTUAL_PARTNER_PACE = 12
FIT_EVENT_HR_HIGH_ALERT = 13
FIT_EVENT_HR_LOW_ALERT = 14
// ... and many more
```

## Units and Scaling

### Common Units

- **Distance**: Meters
- **Speed**: Meters per second
- **Time**: Seconds (timestamps since 1989-12-31 00:00:00 UTC)
- **Position**: Semicircles (degrees × 2^31 / 180)
- **Heart Rate**: Beats per minute
- **Power**: Watts
- **Cadence**: Revolutions per minute
- **Temperature**: Degrees Celsius
- **Pressure**: Pascals
- **Calories**: Kilocalories

### Scaling Factors

Some fields use scaling factors for improved precision:

- Positions are stored as signed 32-bit integers in semicircles
- Some speeds and distances may be scaled for higher precision
- Temperature might be offset (e.g., stored as degrees + 273.15 for Kelvin)

## Developer Fields

FIT files support developer-defined fields through the Developer Data ID message mechanism, allowing custom data fields beyond the standard profile.

## Best Practices

1. **Timestamp Handling**: Always check if timestamps are system time (< 0x10000000) or UTC time
2. **Invalid Values**: Check for invalid values using the defined constants
3. **Field Validation**: Use the `IsFieldValid()` methods before accessing field values
4. **Units**: Pay attention to units and scaling factors when processing data
5. **Message Ordering**: Process messages in the order they appear in the file
6. **Optional Fields**: Not all fields are present in every message instance

## Simplified Database Tables

For database storage and analysis, FIT data can be represented using two simple denormalized tables:

### 1. Activities Metadata Table

| activity_id         | file_id            | timestamp           | local_timestamp     | start_time          | total_timer_time | total_elapsed_time | total_distance | sport   | sub_sport | manufacturer | product        | device_serial_number | software_version | total_calories | total_ascent | total_descent | avg_heart_rate | max_heart_rate | avg_speed | max_speed | avg_power | max_power | avg_cadence | max_cadence | start_position_lat | start_position_long | end_position_lat | end_position_long |
| ------------------- | ------------------ | ------------------- | ------------------- | ------------------- | ---------------- | ------------------ | -------------- | ------- | --------- | ------------ | -------------- | -------------------- | ---------------- | -------------- | ------------ | ------------- | -------------- | -------------- | --------- | --------- | --------- | --------- | ----------- | ----------- | ------------------ | ------------------- | ---------------- | ----------------- |
| 1234567890123456789 | activity_12345.fit | 2024-09-27 20:45:12 | 2024-09-27 14:45:12 | 2024-09-27 19:15:30 | 5423.5           | 5580.2             | 45230.8        | cycling | road      | Garmin       | Edge 1040      | 4012345678           | 20.50            | 1245           | 892.4        | 756.2         | 152            | 178            | 8.35      | 15.72     | 248       | 425       | 87          | 105         | 51.18185           | -115.57559          | 51.18142         | -115.57201        |
| 1234567890123456720 | activity_12346.fit | 2024-09-28 09:22:45 | 2024-09-28 03:22:45 | 2024-09-28 08:30:15 | 3125.8           | 3245.1             | 12850.3        | running | trail     | Garmin       | Forerunner 965 | 4087654321           | 10.10            | 789            | 425.6        | 398.7         | 165            | 182            | 4.11      | 6.25      | NULL      | NULL      | 180         | 192         | 51.18985           | -115.56423          | 51.19125         | -115.55987        |

**Column Descriptions:**

- `activity_id` (BIGINT): Primary key
- `file_id` (VARCHAR): Original filename
- `timestamp` (TIMESTAMP): Activity end time (UTC)
- `local_timestamp` (TIMESTAMP): Activity end time (local timezone)
- `start_time` (TIMESTAMP): Activity start time (UTC)
- `total_timer_time` (FLOAT): Total timer time excluding pauses (seconds)
- `total_elapsed_time` (FLOAT): Total elapsed time including pauses (seconds)
- `total_distance` (FLOAT): Total distance (meters)
- `sport` (VARCHAR): Sport type (cycling, running, swimming, etc.)
- `sub_sport` (VARCHAR): Sub-sport type (road, trail, mountain, etc.)
- `manufacturer` (VARCHAR): Device manufacturer
- `product` (VARCHAR): Device product name
- `device_serial_number` (BIGINT): Device serial number
- `software_version` (VARCHAR): Device software version
- `total_calories` (INTEGER): Total calories burned (kcal)
- `total_ascent` (FLOAT): Total elevation gain (meters)
- `total_descent` (FLOAT): Total elevation loss (meters)
- `avg_heart_rate` (INTEGER): Average heart rate (bpm)
- `max_heart_rate` (INTEGER): Maximum heart rate (bpm)
- `avg_speed` (FLOAT): Average speed (m/s)
- `max_speed` (FLOAT): Maximum speed (m/s)
- `avg_power` (INTEGER): Average power (watts) - NULL for non-power sports
- `max_power` (INTEGER): Maximum power (watts) - NULL for non-power sports
- `avg_cadence` (INTEGER): Average cadence (rpm/spm)
- `max_cadence` (INTEGER): Maximum cadence (rpm/spm)
- `start_position_lat` (FLOAT): Starting latitude (degrees)
- `start_position_long` (FLOAT): Starting longitude (degrees)
- `end_position_lat` (FLOAT): Ending latitude (degrees)
- `end_position_long` (FLOAT): Ending longitude (degrees)

### 2. Time-Series Records Table

| activity_id         | timestamp           | distance | speed | heart_rate | cadence | power | altitude | position_lat | position_long | temperature | gps_accuracy | vertical_speed | calories | left_right_balance | vertical_oscillation | stance_time_percent | stance_time | step_length | motor_power | battery_soc | depth | absolute_pressure | respiration_rate | core_temperature |
| ------------------- | ------------------- | -------- | ----- | ---------- | ------- | ----- | -------- | ------------ | ------------- | ----------- | ------------ | -------------- | -------- | ------------------ | -------------------- | ------------------- | ----------- | ----------- | ----------- | ----------- | ----- | ----------------- | ---------------- | ---------------- |
| 1234567890123456789 | 2024-09-27 19:15:30 | 0.0      | 0.0   | 68         | 0       | 0     | 1245.5   | 51.18185     | -115.57559    | 23          | 3            | 0.0            | 0        | NULL               | NULL                 | NULL                | NULL        | NULL        | NULL        | NULL        | NULL  | NULL              | NULL             | NULL             |
| 1234567890123456789 | 2024-09-27 19:15:31 | 2.8      | 2.8   | 72         | 45      | 125   | 1245.2   | 51.18187     | -115.57562    | 23          | 3            | -0.3           | 1        | 52                 | NULL                 | NULL                | NULL        | NULL        | 15          | 85.2        | NULL  | NULL              | NULL             | NULL             |
| 1234567890123456789 | 2024-09-27 19:15:32 | 8.5      | 5.7   | 78         | 78      | 185   | 1244.8   | 51.18192     | -115.57568    | 24          | 3            | -0.4           | 2        | 49                 | NULL                 | NULL                | NULL        | NULL        | 18          | 85.0        | NULL  | NULL              | NULL             | NULL             |
| 1234567890123456720 | 2024-09-28 08:30:15 | 0.0      | 0.0   | 89         | 0       | NULL  | 1156.2   | 51.18985     | -115.56423    | 18          | 3            | 0.0            | 0        | NULL               | 8.5                  | 32.1                | 245.8       | 1150        | NULL        | NULL        | NULL  | NULL              | NULL             | NULL             |
| 1234567890123456720 | 2024-09-28 08:30:16 | 3.2      | 3.2   | 92         | 165     | NULL  | 1156.8   | 51.18988     | -115.56425    | 18          | 3            | 0.6            | 1        | NULL               | 8.2                  | 31.8                | 238.4       | 1165        | NULL        | NULL        | NULL  | NULL              | NULL             | NULL             |
| 1234567890123456720 | 2024-09-28 08:30:17 | 9.8      | 6.6   | 95         | 172     | NULL  | 1157.5   | 51.18994     | -115.56431    | 18          | 3            | 0.7            | 2        | NULL               | 8.0                  | 31.2                | 235.1       | 1185        | NULL        | NULL        | NULL  | NULL              | NULL             | NULL             |

**Column Descriptions:**

- `activity_id` (BIGINT): Foreign key to Activities Metadata table
- `timestamp` (TIMESTAMP): Record timestamp (UTC)
- `distance` (FLOAT): Cumulative distance from start (meters)
- `speed` (FLOAT): Instantaneous speed (m/s)
- `heart_rate` (INTEGER): Heart rate (bpm)
- `cadence` (INTEGER): Cadence (rpm for cycling, steps/min for running)
- `power` (INTEGER): Power output (watts) - NULL for non-power sports
- `altitude` (FLOAT): Elevation (meters)
- `position_lat` (FLOAT): Latitude (degrees)
- `position_long` (FLOAT): Longitude (degrees)
- `temperature` (INTEGER): Temperature (°C)
- `gps_accuracy` (INTEGER): GPS accuracy (meters)
- `vertical_speed` (FLOAT): Vertical speed (m/s)
- `calories` (INTEGER): Cumulative calories burned (kcal)
- `left_right_balance` (INTEGER): Power balance L/R (percentage)
- `vertical_oscillation` (FLOAT): Vertical oscillation (mm)
- `stance_time_percent` (FLOAT): Ground contact time (percentage)
- `stance_time` (FLOAT): Ground contact time (ms)
- `step_length` (FLOAT): Step length (mm)
- `motor_power` (INTEGER): E-bike motor power (watts)
- `battery_soc` (FLOAT): E-bike battery level (percentage)
- `depth` (FLOAT): Diving depth (meters)
- `absolute_pressure` (INTEGER): Absolute pressure (Pa)
- `respiration_rate` (FLOAT): Breathing rate (breaths/min)
- `core_temperature` (FLOAT): Core body temperature (celsius)

### 3. User Profile Table

| user_id | gender | age | height | weight | language | time_zone | activity_class | running_lactate_threshold_hr | cycling_lactate_threshold_hr | swimming_lactate_threshold_hr | default_max_running_hr | default_max_biking_hr | default_max_hr | hr_setting | speed_setting | dist_setting | power_setting | position_setting | temperature_setting | local_id | global_id  | wake_time | sleep_time | height_setting | weight_setting | resting_heart_rate | default_max_swimming_hr |
| ------- | ------ | --- | ------ | ------ | -------- | --------- | -------------- | ---------------------------- | ---------------------------- | ----------------------------- | ---------------------- | --------------------- | -------------- | ---------- | ------------- | ------------ | ------------- | ---------------- | ------------------- | -------- | ---------- | --------- | ---------- | -------------- | -------------- | ------------------ | ----------------------- |
| 1001    | male   | 35  | 1.78   | 75.5   | english  | -7        | 7.0            | 165                          | 168                          | NULL                          | 185                    | 185                   | 185            | metric     | metric        | metric       | metric        | degree           | celsius             | 12345    | 9876543210 | 25200     | 79200      | metric         | metric         | 58                 | NULL                    |
| 1002    | female | 28  | 1.65   | 62.0   | english  | -5        | 6.5            | 158                          | 162                          | 155                           | 192                    | 192                   | 192            | metric     | metric        | metric       | metric        | degree           | celsius             | 12346    | 9876543211 | 24000     | 81000      | metric         | metric         | 62                 | 187                     |

**Column Descriptions:**

- `user_id` (INTEGER): Primary key
- `gender` (VARCHAR): User gender (male, female)
- `age` (INTEGER): User age (years)
- `height` (FLOAT): Height (meters)
- `weight` (FLOAT): Weight (kg)
- `language` (VARCHAR): Language setting
- `time_zone` (INTEGER): Time zone offset (hours from UTC)
- `activity_class` (FLOAT): Fitness activity class (0.0-10.0)
- `running_lactate_threshold_hr` (INTEGER): Running lactate threshold heart rate (bpm)
- `cycling_lactate_threshold_hr` (INTEGER): Cycling lactate threshold heart rate (bpm)
- `swimming_lactate_threshold_hr` (INTEGER): Swimming lactate threshold heart rate (bpm)
- `default_max_running_hr` (INTEGER): Default max running heart rate (bpm)
- `default_max_biking_hr` (INTEGER): Default max cycling heart rate (bpm)
- `default_max_hr` (INTEGER): Default max heart rate (bpm)
- `hr_setting` (VARCHAR): Heart rate units setting
- `speed_setting` (VARCHAR): Speed units setting
- `dist_setting` (VARCHAR): Distance units setting
- `power_setting` (VARCHAR): Power units setting
- `position_setting` (VARCHAR): Position format setting
- `temperature_setting` (VARCHAR): Temperature units setting
- `local_id` (INTEGER): Local user ID
- `global_id` (BIGINT): Global user ID
- `wake_time` (INTEGER): Wake time (seconds from midnight)
- `sleep_time` (INTEGER): Sleep time (seconds from midnight)
- `height_setting` (VARCHAR): Height units setting
- `weight_setting` (VARCHAR): Weight units setting
- `resting_heart_rate` (INTEGER): Resting heart rate (bpm)
- `default_max_swimming_hr` (INTEGER): Default max swimming heart rate (bpm)

### 4. Sessions Table

| session_id | activity_id         | timestamp           | start_time          | total_elapsed_time | total_timer_time | total_distance | sport   | sub_sport | total_calories | avg_speed | max_speed | avg_heart_rate | max_heart_rate | min_heart_rate | avg_cadence | max_cadence | avg_power | max_power | normalized_power | intensity_factor | training_stress_score | total_work | total_ascent | total_descent | first_lap_index | num_laps | event   | event_type | avg_altitude | max_altitude | min_altitude | avg_grade | max_grade | min_grade | avg_pos_grade | max_pos_grade | avg_neg_grade | max_neg_grade | avg_temperature | max_temperature | min_temperature | trigger |
| ---------- | ------------------- | ------------------- | ------------------- | ------------------ | ---------------- | -------------- | ------- | --------- | -------------- | --------- | --------- | -------------- | -------------- | -------------- | ----------- | ----------- | --------- | --------- | ---------------- | ---------------- | --------------------- | ---------- | ------------ | ------------- | --------------- | -------- | ------- | ---------- | ------------ | ------------ | ------------ | --------- | --------- | --------- | ------------- | ------------- | ------------- | ------------- | --------------- | --------------- | --------------- | ------- |
| 1          | 1234567890123456789 | 2024-09-27 20:45:12 | 2024-09-27 19:15:30 | 5580.2             | 5423.5           | 45230.8        | cycling | road      | 1245           | 8.35      | 15.72     | 152            | 178            | 125            | 87          | 105         | 248       | 425       | 265              | 0.85             | 89.5                  | 1345234    | 892.4        | 756.2         | 0               | 3        | session | stop       | 1124.5       | 1245.5       | 1025.8       | 2.1       | 12.5      | -8.9      | 3.8           | 12.5          | -2.3          | -8.9          | 23.5            | 28.2            | 19.8            | manual  |
| 2          | 1234567890123456720 | 2024-09-28 09:22:45 | 2024-09-28 08:30:15 | 3245.1             | 3125.8           | 12850.3        | running | trail     | 789            | 4.11      | 6.25      | 165            | 182            | 142            | 180         | 192         | NULL      | NULL      | NULL             | NULL             | NULL                  | NULL       | 425.6        | 398.7         | 0               | 2        | session | stop       | 1278.9       | 1456.2       | 1156.2       | 3.3       | 15.8      | -12.1     | 5.2           | 15.8          | -3.1          | -12.1         | 18.1            | 21.5            | 15.2            | manual  |

**Column Descriptions:**

- `session_id` (INTEGER): Primary key
- `activity_id` (BIGINT): Foreign key to Activities table
- `timestamp` (TIMESTAMP): Session end time (UTC)
- `start_time` (TIMESTAMP): Session start time (UTC)
- `total_elapsed_time` (FLOAT): Total elapsed time including pauses (seconds)
- `total_timer_time` (FLOAT): Total timer time excluding pauses (seconds)
- `total_distance` (FLOAT): Total distance (meters)
- `sport` (VARCHAR): Sport type
- `sub_sport` (VARCHAR): Sub-sport type
- `total_calories` (INTEGER): Total calories burned (kcal)
- `avg_speed` (FLOAT): Average speed (m/s)
- `max_speed` (FLOAT): Maximum speed (m/s)
- `avg_heart_rate` (INTEGER): Average heart rate (bpm)
- `max_heart_rate` (INTEGER): Maximum heart rate (bpm)
- `min_heart_rate` (INTEGER): Minimum heart rate (bpm)
- `avg_cadence` (INTEGER): Average cadence (rpm/spm)
- `max_cadence` (INTEGER): Maximum cadence (rpm/spm)
- `avg_power` (INTEGER): Average power (watts)
- `max_power` (INTEGER): Maximum power (watts)
- `normalized_power` (INTEGER): Normalized power (watts)
- `intensity_factor` (FLOAT): Intensity factor
- `training_stress_score` (FLOAT): Training Stress Score
- `total_work` (INTEGER): Total work (joules)
- `total_ascent` (FLOAT): Total elevation gain (meters)
- `total_descent` (FLOAT): Total elevation loss (meters)
- `first_lap_index` (INTEGER): Index of first lap
- `num_laps` (INTEGER): Number of laps
- `event` (VARCHAR): Event type
- `event_type` (VARCHAR): Event type classification
- `avg_altitude` (FLOAT): Average altitude (meters)
- `max_altitude` (FLOAT): Maximum altitude (meters)
- `min_altitude` (FLOAT): Minimum altitude (meters)
- `avg_grade` (FLOAT): Average grade (%)
- `max_grade` (FLOAT): Maximum grade (%)
- `min_grade` (FLOAT): Minimum grade (%)
- `avg_pos_grade` (FLOAT): Average positive grade (%)
- `max_pos_grade` (FLOAT): Maximum positive grade (%)
- `avg_neg_grade` (FLOAT): Average negative grade (%)
- `max_neg_grade` (FLOAT): Maximum negative grade (%)
- `avg_temperature` (FLOAT): Average temperature (°C)
- `max_temperature` (FLOAT): Maximum temperature (°C)
- `min_temperature` (FLOAT): Minimum temperature (°C)
- `trigger` (VARCHAR): Session trigger type

### 5. Laps Table

| lap_id | session_id | activity_id         | timestamp           | start_time          | total_elapsed_time | total_timer_time | total_distance | total_calories | avg_speed | max_speed | avg_heart_rate | max_heart_rate | min_heart_rate | avg_cadence | max_cadence | avg_power | max_power | total_ascent | total_descent | lap_trigger | event | event_type | avg_altitude | max_altitude | min_altitude | start_position_lat | start_position_long | end_position_lat | end_position_long | max_neg_grade | avg_temperature | wkt_step_index | opponent_score | stroke_count |
| ------ | ---------- | ------------------- | ------------------- | ------------------- | ------------------ | ---------------- | -------------- | -------------- | --------- | --------- | -------------- | -------------- | -------------- | ----------- | ----------- | --------- | --------- | ------------ | ------------- | ----------- | ----- | ---------- | ------------ | ------------ | ------------ | ------------------ | ------------------- | ---------------- | ----------------- | ------------- | --------------- | -------------- | -------------- | ------------ |
| 1      | 1          | 1234567890123456789 | 2024-09-27 19:35:45 | 2024-09-27 19:15:30 | 1215.3             | 1215.3           | 15125.2        | 415            | 12.45     | 15.72     | 148            | 165            | 135            | 92          | 105         | 285       | 425       | 245.8        | 189.5         | distance    | lap   | marker     | 1085.2       | 1245.5       | 1025.8       | 51.18185           | -115.57559          | 51.18856         | -115.57012        | -6.8          | 22.1            | NULL           | NULL           | NULL         |
| 2      | 1          | 1234567890123456789 | 2024-09-27 19:55:22 | 2024-09-27 19:35:45 | 1177.0             | 1177.0           | 15052.8        | 410            | 12.78     | 14.85     | 155            | 172            | 142            | 89          | 98          | 245       | 385       | 325.1        | 278.9         | distance    | lap   | marker     | 1165.8       | 1198.5       | 1089.2       | 51.18856           | -115.57012          | 51.19485         | -115.56425        | -8.2          | 24.8            | NULL           | NULL           | NULL         |
| 3      | 2          | 1234567890123456720 | 2024-09-28 08:56:42 | 2024-09-28 08:30:15 | 1587.5             | 1545.2           | 6425.1         | 394            | 4.15      | 5.85      | 162            | 175            | 148            | 178         | 185         | NULL      | NULL      | 212.8        | 198.5         | distance    | lap   | marker     | 1285.4       | 1356.2       | 1156.2       | 51.18985           | -115.56423          | 51.19225         | -115.55987        | -9.5          | 17.8            | NULL           | NULL           | NULL         |

**Column Descriptions:**

- `lap_id` (INTEGER): Primary key
- `session_id` (INTEGER): Foreign key to Sessions table
- `activity_id` (BIGINT): Foreign key to Activities table
- `timestamp` (TIMESTAMP): Lap end time (UTC)
- `start_time` (TIMESTAMP): Lap start time (UTC)
- `total_elapsed_time` (FLOAT): Lap elapsed time (seconds)
- `total_timer_time` (FLOAT): Lap timer time (seconds)
- `total_distance` (FLOAT): Lap distance (meters)
- `total_calories` (INTEGER): Calories burned in lap (kcal)
- `avg_speed` (FLOAT): Average speed (m/s)
- `max_speed` (FLOAT): Maximum speed (m/s)
- `avg_heart_rate` (INTEGER): Average heart rate (bpm)
- `max_heart_rate` (INTEGER): Maximum heart rate (bpm)
- `min_heart_rate` (INTEGER): Minimum heart rate (bpm)
- `avg_cadence` (INTEGER): Average cadence (rpm/spm)
- `max_cadence` (INTEGER): Maximum cadence (rpm/spm)
- `avg_power` (INTEGER): Average power (watts)
- `max_power` (INTEGER): Maximum power (watts)
- `total_ascent` (FLOAT): Elevation gain in lap (meters)
- `total_descent` (FLOAT): Elevation loss in lap (meters)
- `lap_trigger` (VARCHAR): What triggered the lap (manual, distance, time, etc.)
- `event` (VARCHAR): Event type
- `event_type` (VARCHAR): Event type classification
- `avg_altitude` (FLOAT): Average altitude (meters)
- `max_altitude` (FLOAT): Maximum altitude (meters)
- `min_altitude` (FLOAT): Minimum altitude (meters)
- `start_position_lat` (FLOAT): Lap start latitude (degrees)
- `start_position_long` (FLOAT): Lap start longitude (degrees)
- `end_position_lat` (FLOAT): Lap end latitude (degrees)
- `end_position_long` (FLOAT): Lap end longitude (degrees)
- `max_neg_grade` (FLOAT): Maximum negative grade (%)
- `avg_temperature` (FLOAT): Average temperature (°C)
- `wkt_step_index` (INTEGER): Workout step index
- `opponent_score` (INTEGER): Opponent score (for sports)
- `stroke_count` (INTEGER): Stroke count (swimming)

### 6. Device Information Table

| device_id | activity_id         | device_index | device_type | manufacturer | product        | serial_number | software_version | hardware_version | cum_operating_time | battery_status | sensor_position | descriptor  | ant_transmission_type | ant_device_number | ant_network | source_type | product_name   | battery_voltage |
| --------- | ------------------- | ------------ | ----------- | ------------ | -------------- | ------------- | ---------------- | ---------------- | ------------------ | -------------- | --------------- | ----------- | --------------------- | ----------------- | ----------- | ----------- | -------------- | --------------- |
| 1         | 1234567890123456789 | 0            | gps         | garmin       | edge_1040      | 4012345678    | 20.50            | 1.0              | 15780000           | good           | left_crank_arm  | power_meter | 0                     | 12345             | ant_plus    | ant_plus    | Edge 1040      | 3.2             |
| 2         | 1234567890123456789 | 1            | heart_rate  | garmin       | hrm_pro        | 4098765432    | 3.20             | 2.1              | 8920000            | ok             | chest           | hrm         | 0                     | 54321             | ant_plus    | ant_plus    | HRM-Pro        | 3.1             |
| 3         | 1234567890123456720 | 0            | gps         | garmin       | forerunner_965 | 4087654321    | 10.10            | 1.5              | 12450000           | good           | wrist           | watch       | 0                     | 67890             | ant_plus    | local       | Forerunner 965 | 3.8             |

**Column Descriptions:**

- `device_id` (INTEGER): Primary key
- `activity_id` (BIGINT): Foreign key to Activities table
- `device_index` (INTEGER): Device index in the activity
- `device_type` (VARCHAR): Type of device (gps, heart_rate, bike_power, etc.)
- `manufacturer` (VARCHAR): Device manufacturer
- `product` (VARCHAR): Product identifier
- `serial_number` (BIGINT): Device serial number
- `software_version` (VARCHAR): Software version
- `hardware_version` (VARCHAR): Hardware version
- `cum_operating_time` (INTEGER): Cumulative operating time (seconds)
- `battery_status` (VARCHAR): Battery status (new, good, ok, low, critical)
- `sensor_position` (VARCHAR): Sensor position (left_crank_arm, chest, wrist, etc.)
- `descriptor` (VARCHAR): Device descriptor
- `ant_transmission_type` (INTEGER): ANT+ transmission type
- `ant_device_number` (INTEGER): ANT+ device number
- `ant_network` (VARCHAR): ANT+ network type
- `source_type` (VARCHAR): Source type (ant_plus, antfs, bluetooth, etc.)
- `product_name` (VARCHAR): Human-readable product name
- `battery_voltage` (FLOAT): Battery voltage

### 7. Events Table

| event_id | activity_id         | timestamp           | event | event_type | data | data16 | score | opponent_score | front_gear_num | front_gear | rear_gear_num | rear_gear | device_index | activity_type | start_timestamp     | radar_threat_level_max | radar_threat_count | rider_position |
| -------- | ------------------- | ------------------- | ----- | ---------- | ---- | ------ | ----- | -------------- | -------------- | ---------- | ------------- | --------- | ------------ | ------------- | ------------------- | ---------------------- | ------------------ | -------------- |
| 1        | 1234567890123456789 | 2024-09-27 19:15:30 | timer | start      | 0    | NULL   | NULL  | NULL           | NULL           | NULL       | NULL          | NULL      | 0            | NULL          | 2024-09-27 19:15:30 | NULL                   | NULL               | NULL           |
| 2        | 1234567890123456789 | 2024-09-27 19:35:45 | lap   | marker     | 1    | NULL   | NULL  | NULL           | 2              | 50         | 8             | 15        | 0            | NULL          | NULL                | NULL                   | NULL               | NULL           |
| 3        | 1234567890123456789 | 2024-09-27 20:45:12 | timer | stop_all   | 0    | NULL   | NULL  | NULL           | NULL           | NULL       | NULL          | NULL      | 0            | NULL          | NULL                | NULL                   | NULL               | NULL           |
| 4        | 1234567890123456720 | 2024-09-28 08:30:15 | timer | start      | 0    | NULL   | NULL  | NULL           | NULL           | NULL       | NULL          | NULL      | 0            | NULL          | 2024-09-28 08:30:15 | NULL                   | NULL               | NULL           |

**Column Descriptions:**

- `event_id` (INTEGER): Primary key
- `activity_id` (BIGINT): Foreign key to Activities table
- `timestamp` (TIMESTAMP): Event timestamp (UTC)
- `event` (VARCHAR): Event type (timer, lap, workout, battery, etc.)
- `event_type` (VARCHAR): Event type classification (start, stop, marker, etc.)
- `data` (INTEGER): Event-specific data
- `data16` (INTEGER): Additional 16-bit event data
- `score` (INTEGER): Score (for sports with scoring)
- `opponent_score` (INTEGER): Opponent score
- `front_gear_num` (INTEGER): Front gear number
- `front_gear` (INTEGER): Front gear teeth count
- `rear_gear_num` (INTEGER): Rear gear number
- `rear_gear` (INTEGER): Rear gear teeth count
- `device_index` (INTEGER): Device index
- `activity_type` (VARCHAR): Activity type during event
- `start_timestamp` (TIMESTAMP): Start timestamp for timer events
- `radar_threat_level_max` (INTEGER): Maximum radar threat level
- `radar_threat_count` (INTEGER): Radar threat count
- `rider_position` (VARCHAR): Rider position

### Usage Notes

- **Foreign Keys**:
  - `sessions.activity_id` references `activities.activity_id`
  - `laps.session_id` references `sessions.session_id`
  - `laps.activity_id` references `activities.activity_id`
  - `devices.activity_id` references `activities.activity_id`
  - `events.activity_id` references `activities.activity_id`
- **Timestamps**: Store in UTC, convert to local time as needed
- **Coordinates**: Converted from semicircles to decimal degrees
- **NULL Values**: Many sport-specific fields will be NULL when not applicable
- **Indexing**: Add indexes on foreign keys and timestamp columns for performance

## Conclusion

The FIT format provides a comprehensive and extensible framework for storing fitness and activity data. Understanding these data structures is crucial for developing applications that can read, write, and analyze FIT files effectively.

This documentation is based on the Garmin FIT SDK version 21.171.0 and covers the most commonly used message types and fields. For complete details, refer to the official Garmin FIT SDK documentation.
