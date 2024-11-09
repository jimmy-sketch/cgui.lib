#include"cgui/components/freeCanvas.h"

freeCanvas::freeCanvas(cgui::Pos size)
{
	this->height = size.row;
	this->width = size.col;
}

freeCanvas::freeCanvas(cgui::Pos canvasSize, initializer_list<map<cgui::Pos, shared_ptr<component>>> initList)
{
	this->height = canvasSize.row;
	this->width = canvasSize.col;
	for (auto kv = initList.begin(); kv <= initList.end(); kv++)
		this->componentList.push_back((*kv));
}

size_t freeCanvas::getHeight() const
{
	return this->height;
}
size_t freeCanvas::getWidth() const
{
	return this->width;
}

vector<cgui::string> freeCanvas::getData() const
{
	
}