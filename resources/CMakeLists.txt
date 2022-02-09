juce_add_binary_data(BinaryData SOURCES
    amp_clean.jpg
    amp_lead.jpg
    amp_off.jpg
    amp_pic.png
    knob_silver.png
    led_blue_off.png
    led_blue_on.png
    led_blue_on_off.png
    logo.png
    power_switch_down.png
    power_switch_up.png
    SmartAmp.ico
    SmartAmp.png
    ../models/BluesJr.json
    ../models/HT40_Overdrive.json
)

# Need to build BinaryData with -fPIC flag on Linux
set_target_properties(BinaryData PROPERTIES
    POSITION_INDEPENDENT_CODE TRUE)
