#pragma once
#include "component.h"
#include<vector>
#include "cgui/utils/string.h"
#include "cgui/utils/pos.h"

enum colors { black = 40, red, green, yellow, blue, lime, cyan };
enum LayoutType{stack,tile};//��ʾ���ڲ��ַ�ʽ�����/ƽ�̣�δʵ�֣�

class window :public component //һ������
{
	window(size_t weight, size_t height);
	window(size_t weight, size_t height, std::initializer_list<component> initList);
	

	std::vector<cgui::string> getData();
	size_t getWidth() const;
	size_t getHeight() const;
};

class windowScreen : public component //���ڵġ���ʾ������Ҳ��һ��������
{
public:
	windowScreen(size_t weight, size_t height);

	void addWindow(window& window, cgui::logicPos pos);
	void clearWindow();
	void changeWindowShadow(bool type);
	void changeLayout(LayoutType type);//TODO��layout��ع���
	void deleteWindow(window& what);
	void setBgColor(colors bgcolor);//֧����ɫ���ں��̻�������

	std::vector<cgui::string> getData();
	size_t getWidth() const;
	size_t getHeight() const;
};