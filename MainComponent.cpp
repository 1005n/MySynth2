#include "MainComponent.h"

MainComponent::MainComponent()
    : keyboardComponent(keyboardState, juce::MidiKeyboardComponent::horizontalKeyboard)
{
    // シンセ初期化（ボイスとサウンド追加）
    synth.clearVoices();
    for (int i = 0; i < 8; ++i)
        synth.addVoice(new SineWaveVoice());

    synth.clearSounds();
    synth.addSound(new SineWaveSound());

    addAndMakeVisible(keyboardComponent);
    setSize(600, 200);

    setAudioChannels(0, 2); // 入力0、出力2（ステレオ）
}

MainComponent::~MainComponent()
{
    shutdownAudio();
}

void MainComponent::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
    synth.setCurrentPlaybackSampleRate(sampleRate);
}

void MainComponent::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill)
{
    bufferToFill.clearActiveBufferRegion();

    juce::MidiBuffer incomingMidi;
    keyboardState.processNextMidiBuffer(incomingMidi, bufferToFill.startSample, bufferToFill.numSamples, true);

    synth.renderNextBlock(*bufferToFill.buffer, incomingMidi, bufferToFill.startSample, bufferToFill.numSamples);
}

void MainComponent::releaseResources()
{
}

void MainComponent::resized()
{
    keyboardComponent.setBounds(getLocalBounds());
}
