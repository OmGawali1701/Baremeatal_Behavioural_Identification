# See the build system documentation in IDF programming guide
# for more information about component CMakeLists.txt files.

idf_component_register(
    SRCS          # list the source files of this component
    "app_main.c"
    "wifi_manager.c"
    "mqtt_manager.c"
    "sensor_manager.c"
    "static_data.c"
    "dynamic_data.c"
    "json_builder.c"
    "mq_sensor.c"
    "uart_checker.c"
    INCLUDE_DIRS "."        # optional, add here public include directories
    PRIV_INCLUDE_DIRS   # optional, add here private include directories
    REQUIRES            # optional, list the public requirements (component names)
    PRIV_REQUIRES       # optional, list the private requirements
)

