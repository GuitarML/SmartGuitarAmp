juce_add_binary_data(BinaryData SOURCES
    amp_clean.jpg
    amp_lead.jpg
    amp_off.jpg
    knob_silver.png
    led_blue_off.png
    led_blue_on.png
    led_blue_on_off.png
    led_red_off.png
    led_red_on.png
    power_switch_down.png
    power_switch_up.png
    ../models/bluej_clean_p0088.json
    ../models/bluej_fullD_p0153.json
)

# Need to build BinaryData with -fPIC flag on Linux
set_target_properties(BinaryData PROPERTIES
    POSITION_INDEPENDENT_CODE TRUE)
