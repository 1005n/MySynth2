#pragma once

#include <JuceHeader.h>
#include "WaveType.h"

class WaveVoice : public juce::SynthesiserVoice
{
public:
    WaveVoice() {}

    bool canPlaySound(juce::SynthesiserSound* sound) override
    {
        return dynamic_cast<juce::SynthesiserSound*> (sound) != nullptr;
    }

    void startNote(int midiNoteNumber, float velocity,
                   juce::SynthesiserSound*, int) override
    {
        currentAngle = 0.0;
        level = velocity;
        tailOff = 0.0;

        auto cyclesPerSecond = juce::MidiMessage::getMidiNoteInHertz(midiNoteNumber);
        auto sampleRate = getSampleRate();
        if (sampleRate <= 0.0)
            return;

        auto cyclesPerSample = cyclesPerSecond / sampleRate;
        angleDelta = cyclesPerSample * 2.0 * juce::MathConstants<double>::pi;
    }
    void WaveVoice::stopNote(float velocity, bool allowTailOff)
{
    if (allowTailOff)
    {
        // テイルオフ（音の余韻）を使うならここで開始
        clearCurrentNote(); // 今は余韻なしで即停止
    }
    else
    {
        clearCurrentNote();
    }

    // 音を完全に停止する（振幅をゼロに）
    level = 0.0f;
}


    
    void pitchWheelMoved(int) override {}
    void controllerMoved(int, int) override {}

    void renderNextBlock(juce::AudioBuffer<float>& outputBuffer, int startSample, int numSamples) override
    {
        if (angleDelta != 0.0)
        {
            while (--numSamples >= 0)
            {
                float currentSample = generateSample();

                for (int channel = 0; channel < outputBuffer.getNumChannels(); ++channel)
                    outputBuffer.addSample(channel, startSample, currentSample * level);

                currentAngle += angleDelta;
                if (currentAngle >= twoPi)
                    currentAngle -= twoPi;

                ++startSample;
            }
        }
    }

    void setWaveType(WaveType type)
    {
        waveType = type;
    }

private:
    double currentAngle = 0.0;
    double angleDelta = 0.0;
    double level = 0.0;
    double tailOff = 0.0;
    WaveType waveType = WaveType::Sine;

    const double twoPi = 2.0 * juce::MathConstants<double>::pi;

    float generateSample()
    {
        switch (waveType)
        {
            case WaveType::Sine:
                return std::sin(currentAngle);

            case WaveType::Square:
                return std::sin(currentAngle) >= 0.0 ? 1.0f : -1.0f;

            case WaveType::Triangle:
                return (float)(2.0 / juce::MathConstants<double>::pi * std::asin(std::sin(currentAngle)));

            case WaveType::Saw:
                return (float)(2.0 * (currentAngle / twoPi) - 1.0f);

            default:
                return 0.0f;
        }
    }
};

