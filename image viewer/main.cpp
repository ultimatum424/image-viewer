#include "config.h"


using namespace sf;
using namespace std;


void setNextFile(Files &files, Texture &texture, RenderWindow &window, string error_file) 
{
	if (files.can_get_next) 
	{
		string file_name = files.dir_name;
		file_name.append(files.list[files.number]);
		if (!texture.loadFromFile(file_name))
			texture.loadFromFile(error_file);
		files.can_get_next = false;
		window.setTitle(files.list[files.number]);
	}
}
Vector2i getMove(Mouse_struct &mouse, RenderWindow &window)
{
	Vector2i move = { 0, 0 };
	if (mouse.is_clicked) 
	{
		mouse.pos = mouse.new_pos;
		mouse.new_pos = Mouse::getPosition(window);
		move = Vector2i({ mouse.pos.x - mouse.new_pos.x, mouse.pos.y - mouse.new_pos.y });
	}
	if (mouse.is_released) 
	{
		mouse.is_released = false;
		mouse.is_clicked = false;
	}
	return move;
}
void initZoom(Zoom &zoom, int inc, Vector2f &img_center)
{
	float t = zoom.modes[zoom.number + inc] / zoom.modes[zoom.number];
	zoom.number += inc;
	img_center.x *= t;
	img_center.y *= t;
	zoom.scale *= t;
	zoom.is_not_scaled = zoom.number == 0;
}
void initNextFile(bool &next_file, float &scale, int &file_number, int inc) 
{
	file_number += inc;
	scale = 1.f;
	next_file = true;
}
bool isInRect(Vector2i el_coor, Vector2f rect_coor, Vector2i rect_size) 
{
	return (el_coor.x >= rect_coor.x && el_coor.y >= rect_coor.y &&
		el_coor.x <= rect_coor.x + rect_size.x && el_coor.y <= rect_coor.y + rect_size.y);
}
void drawButton(RenderWindow &app, Texture txtr, Vector2i img_start, Vector2i img_size, Vector2f app_pos) 
{
	Sprite sprt;
	sprt.setTexture(txtr);
	sprt.setTextureRect(IntRect(img_start, img_size));
	View view(FloatRect(0.f, 0.f, float(app.getSize().x), float(app.getSize().y)));
	app.setView(view);
	sprt.setPosition(app_pos);
	app.draw(sprt);
}
void setButton(button &but, RenderWindow &window, Vector2f pos_on_display)
{
	but.pos = pos_on_display;
	drawButton(window, but.texture, but.start, but.size, but.pos);
}
void setInterface(Interface &intface, RenderWindow &window) 
{
	Vector2f app_size = Vector2f(window.getSize());
	setButton(intface.left, window, Vector2f({ 0, app_size.y - but_size }));
	setButton(intface.right, window, Vector2f({ float(but_size), app_size.y - but_size }));
	setButton(intface.minus, window, Vector2f({ app_size.x - 2 * but_size, app_size.y - but_size }));
	setButton(intface.plus, window, Vector2f({ app_size.x - but_size, app_size.y - but_size }));
}
void setEvent(RenderWindow &window, Mouse_struct &mouse, Interface &intface, Zoom &zoom, Files &files, Img &img)
{
	Event event;
	while (window.pollEvent(event)) 
	{
		if (event.type == Event::MouseButtonPressed)
			if (event.key.code == Mouse::Left)
			{
				mouse.is_clicked = true;
				mouse.pos = mouse.new_pos = Mouse::getPosition(window);
			}
		if (event.type == Event::MouseButtonReleased)
			if (event.key.code == Mouse::Left)
				mouse.is_released = true;
		if (event.type == Event::KeyPressed || mouse.is_released) 
		{
			if ((event.key.code == Keyboard::Add || isInRect(mouse.new_pos, intface.plus.pos, intface.plus.size)) && zoom.number < scale_count - 1)
				initZoom(zoom, 1, img.center);
			else if ((event.key.code == Keyboard::Subtract || isInRect(mouse.new_pos, intface.minus.pos, intface.minus.size)) && zoom.number > 0)
				initZoom(zoom, -1, img.center);
			else if (zoom.is_not_scaled)
				if ((event.key.code == Keyboard::Left || isInRect(mouse.new_pos, intface.left.pos, intface.left.size)) && files.number > 0)
					initNextFile(files.can_get_next, zoom.scale, files.number, -1);
				else if ((event.key.code == Keyboard::Right || isInRect(mouse.new_pos, intface.right.pos, intface.right.size)) && files.number < files.list.size() - 1)
					initNextFile(files.can_get_next, zoom.scale, files.number, 1);
		}
		if (event.type == Event::Closed)
			window.close();
	}
}
int getFilesList(string mask, vector <string>  &files)
{
	
	HANDLE h_find;
	WIN32_FIND_DATA find_data;
	std::wstring stemp = std::wstring(mask.begin(), mask.end());
	LPCTSTR s = stemp.c_str();
	h_find = FindFirstFile(s, &find_data);

	if (h_find != INVALID_HANDLE_VALUE)
	{
		do 
		{
			files.push_back(find_data.cFileName);
		} while (FindNextFile(h_find, &find_data));
	}

	FindClose(h_find);

	return 0;
}
void getFormFilesList(Files &files) 
{
	for (int i = 0; i < files.formats.size(); ++i)
	{
		string mask = files.dir_name;
		getFilesList(mask.append("*.").append(files.formats[i]), files.list);
	}
}

Texture getTexture(std::string name) 
{
	Texture but_texture;
	but_texture.loadFromFile(name);
	return but_texture;
}
float getScale(Vector2f img_size, Vector2f app_size) 
{
	if (img_size.x > app_size.x || img_size.y > app_size.y) 
	{
		Vector2f ratio = { app_size.x / img_size.x, app_size.y / img_size.y };
		return (ratio.x < ratio.y) ? ratio.x : ratio.y;
	}
	return 1.f;
}
float checkExtremeCenter(float center, float app_size, float img_size, float scale)
{
	float temp1 = app_size / 2;
	center = (center < temp1) ? temp1 : center;
	float temp2 = img_size * scale - temp1;
	center = (center > temp2) ? temp2 : center;
	return center;
}
float getExtremeCenter(float img_center, float app_size, float img_size, float scale) 
{
	if (img_size * scale > app_size)
		return checkExtremeCenter(img_center, app_size, img_size, scale);
	return scale * img_size / 2;
}
int drawImage(Texture &texture, RenderWindow& app, bool is_not_scale, float &scale, Vector2i move, Vector2f &img_center) 
{

	Vector2f app_size = Vector2f(app.getSize());
	Vector2f img_size = Vector2f(texture.getSize());
	if (app_size.x < min_app_size)
		app_size.x = min_app_size;
	if (app_size.y < min_app_size)
		app_size.y = min_app_size;
	app.setSize(Vector2u(app_size));

	Sprite sprite;
	sprite.setTexture(texture);

	if (is_not_scale)
		scale = getScale(img_size, app_size);

	sprite.scale(scale, scale);

	img_center.x += move.x;
	img_center.y += move.y;

	img_center.x = getExtremeCenter(img_center.x, app_size.x, img_size.x, scale);
	img_center.y = getExtremeCenter(img_center.y, app_size.y, img_size.y, scale);

	FloatRect rect = { Vector2f({ 0, 0 }), app_size };
	View view(rect);
	view.setCenter(img_center);
	app.setView(view);

	sprite.setPosition(0, 0);

	app.draw(sprite);

	return 0;
}


void RunProgram(RenderWindow& window)
{
	Init init;
	getFormFilesList(init.files);
	while (window.isOpen())
	{
		setEvent(window, init.mouse, init.intface, init.zoom, init.files, init.img);

		window.clear();

		setNextFile(init.files, init.img.texture, window, error_file);
		drawImage(init.img.texture, window, init.zoom.is_not_scaled, init.zoom.scale, getMove(init.mouse, window), init.img.center);
		setInterface(init.intface, window);
	}
		window.display();

}
int main()
{
	setlocale(LC_CTYPE, "rus");
	sf::RenderWindow window(sf::VideoMode(600, 600), "Просмотр изображений");
	RunProgram(window);
	return 0;
}