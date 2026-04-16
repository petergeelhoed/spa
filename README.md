# Solar Position Algorithm Library

## Project Description
This library provides a set of tools to calculate the solar position, including azimuth and elevation angles, based on various input parameters such as location and time. It is designed for use in applications requiring precise solar position calculations for optimizing solar energy applications.

## Features
- Fast and accurate calculations of solar positions
- Support for multiple coordinate systems
- Easy-to-use API
- Comprehensive unit tests

## Building
To build the library, use the following commands:

1. Clone the repository:
   ```bash
   git clone https://github.com/petergeelhoed/spa.git
   cd spa
   ```
2. Install dependencies:
   ```bash
   pip install -r requirements.txt
   ```
3. Build the project:
   ```bash
   python setup.py build
   ```

## API Documentation
### `calculate_solar_position(latitude, longitude, datetime)`
Calculates the solar position for the given latitude, longitude, and datetime.

**Parameters:**
- `latitude`: Latitude in degrees.
- `longitude`: Longitude in degrees.
- `datetime`: A datetime object representing the date and time.

**Returns:**
- A dictionary containing `azimuth` and `elevation` angle values.

### `set_location(latitude, longitude)`
Sets the default location for solar position calculations.

**Parameters:**
- `latitude`: Latitude in degrees.
- `longitude`: Longitude in degrees.

## Usage Examples
```python
from spa import calculate_solar_position, set_location

# Set location
set_location(34.0522, -118.2437)  # Los Angeles

# Calculate solar position for a specific datetime
position = calculate_solar_position(34.0522, -118.2437, datetime(2026, 4, 16, 18, 0, 0))
print(f"Azimuth: {position['azimuth']}, Elevation: {position['elevation']}")
```

