add_executable(irline
    irline.c
        )

# include libraries & dependencies
target_link_libraries(irline pico_stdlib hardware_gpio hardware_adc)

pico_enable_stdio_usb(irline 1)


# create map/bin/hex file etc.
pico_add_extra_outputs(irline)

# add url via pico_set_program_url
# example_auto_set_url(encoder)
