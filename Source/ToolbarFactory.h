/*
  ==============================================================================

    Toolbar.h
    Created: 1 Jul 2015 10:51:13am
    Author:  R00070693

  ==============================================================================
*/

#ifndef TOOLBAR_H_INCLUDED
#define TOOLBAR_H_INCLUDED
#include "../JuceLibraryCode/JuceHeader.h"

using namespace juce;

class ToolbarFactory : public ToolbarItemFactory
{
public:

	enum itemIds
	{
		rewind = 1,
		play = 2,
		pause = 3,
		stop = 4,
		fForward = 5
	};

	ToolbarFactory() 
	{
	}

	~ToolbarFactory()
	{}

	void getAllToolbarItemIds(juce::Array<int>& ids) override
	{
		// This returns the complete list of all item IDs that are allowed to
		// go in our toolbar. Any items you might want to add must be listed here. The
		// order in which they are listed will be used by the toolbar customisation panel.
		ids.add(rewind);
		ids.add(separatorBarId);
		ids.add(play);
		ids.add(separatorBarId);
		ids.add(pause);
		ids.add(separatorBarId);
		ids.add(stop);
		ids.add(separatorBarId);
		ids.add(fForward);
	}

	void getDefaultItemSet(juce::Array<int>& ids) override
	{
		// This returns an ordered list of the set of items that make up a
		// toolbar's default set. Not all items need to be on this list, and
		// items can appear multiple times (e.g. the separators used here).

		ids.add(rewind);
		ids.add(separatorBarId);
		ids.add(play);
		ids.add(separatorBarId);
		ids.add(pause);
		ids.add(separatorBarId);
		ids.add(stop);
		ids.add(separatorBarId);
		ids.add(fForward);
	}

	ToolbarItemComponent* createItem(int itemId) override
	{
		switch (itemId)
		{
		case rewind:
			return new ToolbarButton(1, "Rewind", Drawable::createFromImageData(BinaryData::rewind_png, BinaryData::rewind_pngSize), 0);
			break;
		case play:
			return new ToolbarButton(2, "Play", Drawable::createFromImageData(BinaryData::play_png, BinaryData::play_pngSize), 0);
			break;
		case pause:
			return new ToolbarButton(3, "Pause", Drawable::createFromImageData(BinaryData::pause_png, BinaryData::pause_pngSize), 0);
			break;
		case stop:
			return new ToolbarButton(4, "Stop", Drawable::createFromImageData(BinaryData::stop_png, BinaryData::stop_pngSize), 0);
			break;
		case fForward:
			return new ToolbarButton(5, "Fast Forward", Drawable::createFromImageData(BinaryData::ff_png, BinaryData::ff_pngSize), 0);
			break;
		default:
			break;
		}
	}

private:
};

#endif  // TOOLBAR_H_INCLUDED
