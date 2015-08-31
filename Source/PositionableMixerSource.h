/*
  ==============================================================================

    PositionableMixerSource.h
    Created: 23 Jul 2015 1:27:00pm
    Author:  R00070693

	Takes a MixerAudioSource and allows it to be positionable

  ==============================================================================
*/

#ifndef POSITIONABLEMIXERSOURCE_H_INCLUDED
#define POSITIONABLEMIXERSOURCE_H_INCLUDED
#include "..\JuceLibraryCode\JuceHeader.h"
using namespace juce;

class PositionableMixerSource : public PositionableAudioSource,
	public MixerAudioSource,
	virtual public AudioSource,
	public ChangeBroadcaster
{
public:

	PositionableMixerSource();
	~PositionableMixerSource();

	void addInputSource(PositionableAudioSource* newInput,
		const bool deleteWhenRemoved);
	void removeInputSource(PositionableAudioSource* input,
		const bool deleteSource);
	void removeAllInputs();

	void prepareToPlay(int samples, double sampleRate) override;
	void releaseResources() override;
	void getNextAudioBlock(const AudioSourceChannelInfo& bufferToFill) override;

	void setNextReadPosition(int64 newPosition) override;
	int64 getNextReadPosition() const override;
	int64 getTotalLength() const override;
	bool isLooping() const override;
	void setLooping(bool shouldLoop) override;

	//==============================================================================
	void setPosition(double newPosition);
	double getCurrentPosition() const;
	double getLengthInSeconds() const;
	bool hasStreamFinished() const noexcept { return inputStreamEOF; }
	void start();
	void stop();
	bool isPlaying() const noexcept { return playing; }
	void setGain(float newGain) noexcept;
	float getGain() const noexcept { return gain; }

private:
	int64 currentPlayingPosition;
	juce::Array<AudioSource*> inputs;
	BigInteger inputsToDelete;
	CriticalSection lock;
	AudioSampleBuffer tempBuffer;
	double currentSampleRate;
	int bufferSizeExpected;

	PositionableAudioSource* source;
	ResamplingAudioSource* resamplerSource;
	BufferingAudioSource* bufferingSource;
	PositionableAudioSource* positionableSource;
	AudioSource* masterSource;

	CriticalSection callbackLock;
	float volatile gain, lastGain;
	bool volatile playing, stopped;
	double sampleRate, sourceSampleRate;
	int blockSize, readAheadBufferSize;
	bool volatile isPrepared, inputStreamEOF;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PositionableMixerSource)
};



#endif  // POSITIONABLEMIXERSOURCE_H_INCLUDED
