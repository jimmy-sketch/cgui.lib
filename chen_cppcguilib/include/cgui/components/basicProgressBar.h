#pragma once
#include "component.h"

class basicProgressBar : public component
{
public:
	basicProgressBar(int length, int style);
	size_t getWidth() const override;
	size_t getHeight() const override;
	std::vector<cgui::string> getData() const override;

	void updateProgress(int progress);
	int getProgress() const;
	bool isDone() const;

private:
	int length, style, progress;
	char beginChar = '[', fillChar = '=', seqChar = '>', gapChar = ' ', endChar = ']';
};