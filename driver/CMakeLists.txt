add_executable(main
        main.c
        )

# include libraries & dependencies
target_link_libraries(main    pico_stdlib 
                                # pico_lwip_http
                                # pico_lwip_apps_http
                                hardware_pwm 
                                hardware_adc 
                                hardware_i2c
                                # pico_cyw43_arch_lwip_threadsafe_background
                                )
# for wifi ?
# target_include_directories(${PROGRAM_NAME} PRIVATE
#     ${CMAKE_CURRENT_LIST_DIR}/driver/wifi
# )

# enable usb
pico_enable_stdio_usb(main 1)

# create map/bin/hex file etc.
pico_add_extra_outputs(main)

# turning ------------------------------------------------------------------------------

add_executable(turning
turning.c
        )

# include libraries & dependencies
target_link_libraries(turning    pico_stdlib 
                                # pico_lwip_http
                                # pico_lwip_apps_http
                                hardware_pwm 
                                hardware_adc 
                                hardware_i2c
                                # pico_cyw43_arch_lwip_threadsafe_background
                                )

# enable usb
pico_enable_stdio_usb(turning 1)

# create map/bin/hex file etc.
pico_add_extra_outputs(turning)


# reverse ------------------------------------------------------------------------------

add_executable(reverse
reverse.c
        )

# include libraries & dependencies
target_link_libraries(reverse    pico_stdlib 
                                # pico_lwip_http
                                # pico_lwip_apps_http
                                hardware_pwm 
                                hardware_adc 
                                hardware_i2c
                                # pico_cyw43_arch_lwip_threadsafe_background
                                )

# enable usb
pico_enable_stdio_usb(reverse 1)

# create map/bin/hex file etc.
pico_add_extra_outputs(reverse)




if (NOT PICO_NO_HARDWARE)
    # add_subdirectory(barcode_scanner)
    add_subdirectory(encoder)
    add_subdirectory(irline)
    add_subdirectory(magnetometer)
    add_subdirectory(motor)
    add_subdirectory(ultrasonic)
    # add_subdirectory(wifi)
endif ()
