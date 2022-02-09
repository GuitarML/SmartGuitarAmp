#add_subdirectory(headless)

target_sources(TS-M1N3 PRIVATE
	myLookAndFeel.cpp
	myLookAndFeel.h
	PluginEditor.cpp
	PluginEditor.h
	PluginProcessor.cpp
	PluginProcessor.h
	RTNeuralLSTM.cpp
	RTNeuralLSTM.h
)

#target_precompile_headers(TS-M1N3 PRIVATE pch.h)
