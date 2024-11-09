#include"cgui/utils/string.h"
#include"cgui/utils/pos.h"
#include"cgui/utils/config.h"
#include"cgui/components/component.h"
#include<vector>
#include<memory>
#include<map>
using namespace std;

class freeCanvas:public component
{
public:
	freeCanvas(cgui::Pos canvasSize);
	freeCanvas(cgui::Pos canvasSize, initializer_list<map<cgui::Pos, shared_ptr<component>>> initList);
	size_t getWidth() const override;
	size_t getHeight() const override;
	vector<cgui::string> getData() const override;
	void addComponent(cgui::Pos pos, shared_ptr<component>);
	void clearAll();
	void deleteComponent(cgui::Pos pos);
	void deleteComponent(shared_ptr<component>);

private:
	size_t width;
	size_t height;
	vector <map<cgui::Pos, shared_ptr<component>>> componentList;
};