#add_subdirectory(headless)

target_sources(SmartAmp PRIVATE
	Activations.cpp
	Activations.h
	Convolution.cpp
	Convolution.h
	ConvolutionLayer.cpp
	ConvolutionLayer.h
	ConvolutionStack.cpp
	ConvolutionStack.h
      Eq4Band.cpp
      Eq4Band.h
	myLookAndFeel.cpp
	myLookAndFeel.h
	PluginEditor.cpp
	PluginEditor.h
	PluginProcessor.cpp
	PluginProcessor.h
	WaveNet.cpp
	WaveNet.h
	WaveNetLoader.cpp
	WaveNetLoader.h
)

#target_precompile_headers(SmartAmp PRIVATE pch.h)
