#include <JuceHeader.h>
#include "MainComponent.h"

class MyWindow : public juce::DocumentWindow
{
public:
    MyWindow()
        : juce::DocumentWindow(
            "MySynth",
            juce::Colours::lightgrey,
            juce::DocumentWindow::allButtons)
    {
        setUsingNativeTitleBar(true);
        setContentOwned(new MainComponent(), true);
        centreWithSize(getWidth(), getHeight());
        setVisible(true);
    }

    void closeButtonPressed() override
    {
        juce::JUCEApplicationBase::quit();
    }
};

class MyApplication : public juce::JUCEApplication
{
public:
    const juce::String getApplicationName() override       { return "MySynth"; }
    const juce::String getApplicationVersion() override    { return "1.0"; }

    void initialise(const juce::String&) override
    {
        mainWindow.reset(new MyWindow());
    }

    void shutdown() override
    {
        mainWindow = nullptr;
    }

private:
    std::unique_ptr<MyWindow> mainWindow;
};

START_JUCE_APPLICATION(MyApplication)
