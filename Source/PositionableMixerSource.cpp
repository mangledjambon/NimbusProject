/*
  ==============================================================================

    PositionableMixerSource.cpp
    Created: 23 Jul 2015 1:27:00pm
    Author:  R00070693

	Takes a MixerAudioSource and allows it to be positionable

  ==============================================================================
*/

#include "PositionableMixerSource.h"

PositionableMixerSource::PositionableMixerSource() :
	source(nullptr),
	resamplerSource(nullptr),
	bufferingSource(nullptr),
	positionableSource(nullptr),
	masterSource(nullptr),
	gain(1.0f),
	lastGain(1.0f),
	playing(false),
	stopped(true),
	sampleRate(44100.0),
	sourceSampleRate(0.0),
	blockSize(4096),
	readAheadBufferSize(0),
	isPrepared(false),
	inputStreamEOF(false)
{}

PositionableMixerSource::~PositionableMixerSource()
{
	removeAllInputs();
}

void PositionableMixerSource::addInputSource(PositionableAudioSource* input,
	const bool deleteWhenRemoved)
{
	if (input != 0 && !inputs.contains(input))
	{
		lock.enter();
		double localRate = currentSampleRate;
		int localBufferSize = bufferSizeExpected;
		lock.exit();

		if (localRate != 0.0)
			input->prepareToPlay(localBufferSize, localRate);

		const ScopedLock sl(lock);

		inputsToDelete.setBit(inputs.size(), deleteWhenRemoved);
		inputs.add(input);
	}
}

void PositionableMixerSource::removeInputSource(PositionableAudioSource* input,
	const bool deleteInput)
{
	if (input != 0)
	{
		lock.enter();
		const int index = inputs.indexOf(input);

		if (index >= 0)
		{
			inputsToDelete.shiftBits(index, 1);
			inputs.remove(index);
		}

		lock.exit();

		if (index >= 0)
		{
			input->releaseResources();

			if (deleteInput)
				delete input;
		}
	}
}

void PositionableMixerSource::removeAllInputs()
{
	lock.enter();
	Array<AudioSource*> inputsCopy(inputs);
	BitArray inputsToDeleteCopy(inputsToDelete);
	inputs.clear();
	lock.exit();

	for (int i = inputsCopy.size(); --i >= 0;)
		if (inputsToDeleteCopy[i])
			inputsCopy.remove(i);
}

// inherited from MixerAudioSource =====================================================================
void PositionableMixerSource::prepareToPlay(int samplesPerBlockExpected, double newSampleRate) 
{
	tempBuffer.setSize(2, samplesPerBlockExpected);

	const ScopedLock sl(lock);

	currentSampleRate = newSampleRate;
	bufferSizeExpected = samplesPerBlockExpected;

	for (int i = inputs.size(); --i >= 0;)
		inputs.getUnchecked(i)->prepareToPlay(samplesPerBlockExpected, newSampleRate);
}

void PositionableMixerSource::releaseResources() 
{
	const ScopedLock sl(lock);

	for (int i = inputs.size(); --i >= 0;)
		inputs.getUnchecked(i)->releaseResources();

	tempBuffer.setSize(2, 0);

	currentSampleRate = 0;
	bufferSizeExpected = 0;
}

void PositionableMixerSource::getNextAudioBlock(const AudioSourceChannelInfo& info) 
{
	const ScopedLock sl(lock);

	if (inputs.size() > 0)
	{
		inputs.getUnchecked(0)->getNextAudioBlock(info);

		if (inputs.size() > 1)
		{
			tempBuffer.setSize(jmax(1, info.buffer->getNumChannels()),
				info.buffer->getNumSamples());

			AudioSourceChannelInfo info2(&tempBuffer, 0, info.numSamples);

			for (int i = 1; i < inputs.size(); ++i)
			{
				inputs.getUnchecked(i)->getNextAudioBlock(info2);

				for (int chan = 0; chan < info.buffer->getNumChannels(); ++chan)
					info.buffer->addFrom(chan, info.startSample, tempBuffer, chan, 0, info.numSamples);
			}
		}
	}
	else
	{
		info.clearActiveBufferRegion();
	}
}
// =====================================================================================================

// inherited from PositionableAudioSource ==============================================================
void PositionableMixerSource::setPosition(double newPosition)
{
	if (sampleRate > 0.0)
		setNextReadPosition((int64)(newPosition * sampleRate));
}

double PositionableMixerSource::getCurrentPosition() const
{
	if (sampleRate > 0.0)
		return getNextReadPosition() / sampleRate;

	return 0.0;
}

double PositionableMixerSource::getLengthInSeconds() const
{
	if (sampleRate > 0.0)
		return getTotalLength() / sampleRate;

	return 0.0;
}

void PositionableMixerSource::setNextReadPosition(int64 newPosition) 
{
	currentPlayingPosition = newPosition;

	for (int i = inputs.size(); --i >= 0;)
		((PositionableAudioSource*)inputs.getUnchecked(i))->setNextReadPosition(newPosition);
}

int64 PositionableMixerSource::getNextReadPosition() const 
{
	return currentPlayingPosition;
}

int64 PositionableMixerSource::getTotalLength() const 
{
	int maxLength = 0;

	for (int i = inputs.size(); --i >= 0;)
		maxLength = jmax((int64)maxLength,
			((PositionableAudioSource*)inputs.getUnchecked(i))->getTotalLength());

	return maxLength;
}

bool PositionableMixerSource::isLooping() const 
{
	const ScopedLock sl(callbackLock);
	return positionableSource != nullptr && positionableSource->isLooping();
}

void PositionableMixerSource::setLooping(bool shouldLoop) {   }
// =====================================================================================================

void PositionableMixerSource::start()
{
	if ((!playing) && masterSource != nullptr)
	{
		{
			const ScopedLock sl(callbackLock);
			playing = true;
			stopped = false;
			inputStreamEOF = false;
		}

		sendChangeMessage();
	}
}

void PositionableMixerSource::stop()
{
	if (playing)
	{
		{
			const ScopedLock sl(callbackLock);
			playing = false;
		}

		int n = 500;
		while (--n >= 0 && !stopped)
			Thread::sleep(2);

		sendChangeMessage();
	}
}

void PositionableMixerSource::setGain(const float newGain) noexcept
{
	gain = newGain;
}