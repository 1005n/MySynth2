#pragma once


#include <juce_audio_processors/juce_audio_processors.h>
#include<juce_audio_utils/juce_audio_utils.h>
#include<juce_dsp/juce_dsp.h>
#include "PluginProcessor.h"

class MySynthAudioProcessorEditor : public juce::AudioProcessorEditor
{
	public:
		    MySynthAudioProcessorEditor(MySynthAudioProcessor&);
		        ~MySynthAudioProcessorEditor() override;

			    void paint(juce::Graphics&) override;
			        void resized() override;

	private:
				    MySynthAudioProcessor& processorRef;

				        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MySynthAudioProcessorEditor)
};



