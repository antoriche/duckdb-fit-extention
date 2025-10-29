# DuckDB FIT Extension

This repository is based on the [DuckDB extension template](https://github.com/duckdb/extension-template).

This extension allows you to read Garmin `.fit` files using DuckDB, providing access to GPS tracks, heart rate data, power metrics, and other sensor information from fitness devices.

## Features

- **Read FIT Files**: Parse Garmin FIT files directly in SQL queries
- **Multiple Data Views**: Access records, activities, sessions, laps, devices, events, and user profiles
- **Rich Data Types**: Support for GPS coordinates, timestamps, sensor data, and metadata
- **Performance Optimized**: Efficient parsing using the official Garmin FIT SDK

## Quick Start

### Installation

```bash
# Build the extension
GEN=ninja make

# Install and load
./build/release/duckdb
```

### Basic Usage

```sql
SELECT timestamp, latitude, longitude, heart_rate, power, speed
FROM fit('sample.fit')
WHERE latitude IS NOT NULL
LIMIT 10;

-- Get activity summary
SELECT * FROM fit_activities('sample.fit');

-- Analyze lap performance
SELECT lap_id, total_distance, avg_speed, avg_heart_rate, avg_power
FROM fit_laps('sample.fit');
```

## Table Functions

| Function                   | Description                                        |
| -------------------------- | -------------------------------------------------- |
| `fit(filename)`            | Main records table with GPS tracks and sensor data |
| `fit_activities(filename)` | Activity metadata and summaries                    |
| `fit_sessions(filename)`   | Training session information                       |
| `fit_laps(filename)`       | Individual lap data and splits                     |
| `fit_devices(filename)`    | Device information and sensor details              |
| `fit_events(filename)`     | Activity events and markers                        |
| `fit_users(filename)`      | User profile information                           |

## Data Schema

The extension provides comprehensive access to FIT file data:

### Records Table

- **Location**: `latitude`, `longitude`, `altitude`, `enhanced_altitude`
- **Movement**: `speed`, `enhanced_speed`, `distance`, `vertical_speed`
- **Power**: `power`, `motor_power`, `accumulated_power`
- **Physiological**: `heart_rate`, `cadence`, `temperature`, `respiration_rate`
- **Cycling**: `grade`, `left_right_balance`, `torque_effectiveness`, `pedal_smoothness`
- **Running**: `vertical_oscillation`, `stance_time`, `step_length`, `vertical_ratio`
- **And many more fields...**

## GitHub Copilot Integration

This project is optimized for GitHub Copilot development:

### Setup

1. Install recommended VS Code extensions from `.vscode/extensions.json`
2. Review `.copilot-instructions.md` for detailed development context
3. Use the provided build tasks and debugging configurations

### Development Workflow

```bash
# Quick build and test
Ctrl/Cmd + Shift + P → "Tasks: Run Task" → "build"
Ctrl/Cmd + Shift + P → "Tasks: Run Task" → "test"

# Launch DuckDB with extension
Ctrl/Cmd + Shift + P → "Tasks: Run Task" → "run-duckdb"
```

See [`DEVELOPMENT.md`](DEVELOPMENT.md) for comprehensive development guidelines.

## Building

### Prerequisites

- CMake 3.5+
- Ninja (recommended)
- vcpkg (for dependencies)
- DuckDB (included as submodule)

### Build Steps

```bash
# Clone with submodules
git clone --recursive https://github.com/your-repo/duckdb-fit-extension.git

# Build with Ninja (recommended)
GEN=ninja make

# Or build with make
make

# Run tests
make test
```

## Dependencies

- **DuckDB**: Core database engine
- **OpenSSL**: Cryptographic functions (via vcpkg)
- **Garmin FIT SDK**: Official FIT file parsing (included)

## Testing

```bash
# Run all tests
make test

# Interactive testing
./build/release/duckdb
LOAD 'build/release/extension/fit/fit.duckdb_extension';
SELECT * FROM read_fit('sample.fit') LIMIT 5;
```

## Examples

### Analyze Heart Rate Zones

```sql
SELECT
    CASE
        WHEN heart_rate < 120 THEN 'Zone 1'
        WHEN heart_rate < 140 THEN 'Zone 2'
        WHEN heart_rate < 160 THEN 'Zone 3'
        WHEN heart_rate < 180 THEN 'Zone 4'
        ELSE 'Zone 5'
    END as hr_zone,
    COUNT(*) as seconds,
    AVG(power) as avg_power
FROM read_fit('workout.fit')
WHERE heart_rate > 0
GROUP BY hr_zone
ORDER BY hr_zone;
```

### GPS Track Analysis

```sql
SELECT
    timestamp,
    latitude,
    longitude,
    speed * 3.6 as speed_kmh,  -- Convert m/s to km/h
    altitude,
    heart_rate
FROM read_fit('ride.fit')
WHERE latitude IS NOT NULL
ORDER BY timestamp;
```

### Power Analysis

```sql
SELECT
    session_id,
    avg_power,
    max_power,
    normalized_power,
    intensity_factor,
    training_stress_score
FROM read_fit_sessions('power_workout.fit');
```

## Contributing

1. Fork the repository
2. Create a feature branch
3. Make your changes following the development guidelines
4. Add tests for new functionality
5. Submit a pull request

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## Acknowledgments

- [DuckDB](https://duckdb.org/) for the excellent database engine
- [Garmin](https://developer.garmin.com/fit/) for the FIT SDK and protocol documentation
- The DuckDB community for the extension template and support
