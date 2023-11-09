add_executable(driver
        main.c
        )

# include libraries & dependencies
target_link_libraries(driver    pico_stdlib 
                                hardware_pwm 
                                hardware_adc 
                                hardware_i2c 
                                pico_cyw43_arch_lwip_threadsafe_background
                                pico_lwip_http  )

# enable usb
pico_enable_stdio_usb(driver 1)

# create map/bin/hex file etc.
pico_add_extra_outputs(driver)

if (NOT PICO_NO_HARDWARE)
    add_subdirectory(barcode_scanner)
    add_subdirectory(encoder)
    # add_subdirectory(irline)
    add_subdirectory(magnetometer)
    add_subdirectory(motor)
    # add_subdirectory(ultrasonic)
    # # add_subdirectory(wifi)
endif ()
