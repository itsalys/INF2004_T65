add_executable(ultrasonic
        ultrasonic.c
        )

target_link_libraries(ultrasonic
        pico_stdlib              # for core functionality
        )

# create map/bin/hex file etc.
pico_add_extra_outputs(ultrasonic)

# add url via pico_set_program_url
# example_auto_set_url(ultrasonic)

pico_enable_stdio_usb(ultrasonic 1)
