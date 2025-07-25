#include "MainComponent.h"
#include "WaveVoice.h"
#include "SineWaveSound.h"

//==============================================================================
MainComponent::MainComponent()
    : keyboardComponent(keyboardState, juce::MidiKeyboardComponent::horizontalKeyboard)
{
    // サンプルレートとブロックサイズは prepareToPlay で設定される
    setSize (600, 200);

    // 仮想MIDIキーボードの設定
    addAndMakeVisible(keyboardComponent);

    // 音量スライダー設定
    volumeSlider.setRange(0.0, 1.0);
    volumeSlider.setValue(0.8);
    volumeSlider.onValueChange = [this] {
        synth.setCurrentPlaybackSampleRate(currentSampleRate); // 再設定
    };
    addAndMakeVisible(volumeSlider);
    volumeLabel.setText("Volume", juce::dontSendNotification);
    addAndMakeVisible(volumeLabel);

    // ピッチスライダー設定
    pitchSlider.setRange(-24.0, 24.0); // ±24半音
    pitchSlider.setValue(0.0);
    pitchSlider.onValueChange = [this] {
        pitchShiftSemitones = pitchSlider.getValue();
    };
    addAndMakeVisible(pitchSlider);
    pitchLabel.setText("Pitch", juce::dontSendNotification);
    addAndMakeVisible(pitchLabel);

// 波形選択 ComboBox の初期化
waveSelector.addItem("Sine", 1);
waveSelector.addItem("Square", 2);
waveSelector.addItem("Saw", 3);
waveSelector.setSelectedId(1);

// 選択が変わったら波形IDを更新
waveSelector.onChange = [this]()
{
    waveformId = waveSelector.getSelectedId(); // waveformId もメンバ変数として宣言しておく
};

// 表示用
addAndMakeVisible(waveSelector);
synth.clearSounds(); // 念のため前の Sound をクリア
synth.addSound(new SineWaveSound());



    // シンセサイザー初期化
    for (int i = 0; i < 4; ++i)
{
    auto* voice = new WaveVoice();
    voice->setWaveType(WaveType::Sine); // 初期値はサイン波
    synth.addVoice(voice);
}

    // オーディオ初期化
    setAudioChannels(0, 2); // 入力0, 出力2（ステレオ）
}

MainComponent::~MainComponent()
{
    shutdownAudio();
}


//==============================================================================
void MainComponent::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
    currentSampleRate = sampleRate;
    synth.setCurrentPlaybackSampleRate(sampleRate);
}

void MainComponent::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill)
{
    bufferToFill.clearActiveBufferRegion();

    juce::MidiBuffer incomingMidi;
    keyboardState.processNextMidiBuffer(incomingMidi, bufferToFill.startSample, bufferToFill.numSamples, true);

 for (int i = 0; i < synth.getNumVoices(); ++i)
    {
        if (auto* waveVoice = dynamic_cast<WaveVoice*>(synth.getVoice(i)))
        {
            waveVoice->setWaveType(static_cast<WaveType>(waveformId));
        }
    }

    // スライダーの音量を反映
    float gain = (float)volumeSlider.getValue();

    // 一時バッファにシンセの出力を合成
    juce::AudioBuffer<float> tempBuffer;
    tempBuffer.setSize(bufferToFill.buffer->getNumChannels(), bufferToFill.numSamples);
    tempBuffer.clear();

    synth.renderNextBlock(tempBuffer, incomingMidi, 0, bufferToFill.numSamples);

    // 音量調整しながら出力バッファへコピー
    for (int channel = 0; channel < bufferToFill.buffer->getNumChannels(); ++channel)
        bufferToFill.buffer->addFrom(channel, bufferToFill.startSample, tempBuffer, channel, 0, bufferToFill.numSamples, gain);
}

void MainComponent::releaseResources()
{
    // オーディオ解放
}

//==============================================================================
void MainComponent::resized()
{
    keyboardComponent.setBounds(10, 10, getWidth() - 20, 60);
    volumeLabel.setBounds(10, 80, 50, 20);
    volumeSlider.setBounds(60, 80, getWidth() - 70, 20);
    pitchLabel.setBounds(10, 110, 50, 20);
    pitchSlider.setBounds(60, 110, getWidth() - 70, 20);
    waveSelector.setBounds(10, 90, 150, 24);
}
