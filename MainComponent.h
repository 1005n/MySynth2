#pragma once

#include <JuceHeader.h>
#include "WaveVoice.h"

class MainComponent  : public juce::AudioAppComponent
{
public:
    MainComponent();
    ~MainComponent() override;

    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;

    void resized() override;

private:
    // シンセ
    juce::Synthesiser synth;

    // 仮想MIDIキーボード用
    juce::MidiKeyboardState keyboardState;
    juce::MidiKeyboardComponent keyboardComponent;

    // 音量スライダー
    juce::Slider volumeSlider;
    juce::Label volumeLabel;

    // ピッチスライダー
    juce::Slider pitchSlider;
    juce::Label pitchLabel;

    // サンプルレートとピッチ
    double currentSampleRate = 44100.0;
    double pitchShiftSemitones = 0.0;

    juce::ComboBox waveSelector;
    int waveformId = 1; // 初期はサイン波


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};

