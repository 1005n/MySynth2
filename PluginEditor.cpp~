#include "PluginEditor.h"


MySynthAudioProcessorEditor::MySynthAudioProcessorEditor(MySynthAudioProcessor& p)
	    : AudioProcessorEditor(&p), processorRef(p)
{
	    setSize(400, 300);
}

MySynthAudioProcessorEditor::~MySynthAudioProcessorEditor() {}

void MySynthAudioProcessorEditor::paint(juce::Graphics& g)
{
	    g.fillAll(juce::Colours::black);
	        g.setColour(juce::Colours::white);
		    g.setFont(20.0f);
		        g.drawFittedText("MySynth", getLocalBounds(), juce::Justification::centred, 1);
}

void MySynthAudioProcessorEditor::resized()
{
}

