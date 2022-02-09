juce_add_binary_data(BinaryData SOURCES
    footswitch_down.png
    footswitch_up.png
    knob2.png
    led_red_off.png
    led_red_on.png
    logo.png
    ts_background_black.jpg
    SmartAmp.ico
    SmartAmp.png
    ../models/model_ts9_48k_cond2.json
    ../models/model_ts9_cond2.json
)

# Need to build BinaryData with -fPIC flag on Linux
set_target_properties(BinaryData PROPERTIES
    POSITION_INDEPENDENT_CODE TRUE)
