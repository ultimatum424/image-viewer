#pragma once
#include "SFML\Graphics.hpp"
#include <iostream>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>
#include <windows.h> 
#include <direct.h>
#include <sys/types.h>

using namespace std;
using namespace sf;


const string error_file = "../image/error.png";
const int but_size = 50;
const int but_count = 4;
const float min_app_size = 500;
const int scale_count = 8;
const string but_file = "../image/buttons2.png";


struct Mouse_struct 
{
	Vector2i pos = { 0, 0 };
	Vector2i new_pos = { 0, 0 };
	bool is_clicked = false;
	bool is_released = false;
};
struct Files 
{
	vector <string> list;
	vector <string> formats = { "jpg", "bmp", "png" };
	string dir_name = "D:/project/";
	int number = 0;
	bool can_get_next = true;
};

struct Img 
{
	Vector2f center;
	Texture texture;
};
struct button 
{
	Texture texture;
	Vector2i start;
	Vector2i size;
	Vector2f pos;
};
Texture getTexture(std::string name);
struct Interface 
{
	button left = { getTexture(but_file), Vector2i({ 0, 0 }), Vector2i({ but_size, but_size }), Vector2f({ 0, 0 }) };
	button right = { getTexture(but_file), Vector2i({ but_size, 0 }), Vector2i({ but_size, but_size }), Vector2f({ 0, 0 }) };
	button minus = { getTexture(but_file), Vector2i({ but_size * 2, 0 }), Vector2i({ but_size, but_size }), Vector2f({ 0, 0 }) };
	button plus = { getTexture(but_file), Vector2i({ but_size * 3, 0 }), Vector2i({ but_size, but_size }), Vector2f({ 0, 0 }) };
};
struct Zoom 
{
	float modes[scale_count] = { 1, 1.5, 2, 2.5, 3, 4, 6, 8 };
	int number = 0;
	bool is_not_scaled = true;
	float scale = 1.f;
};
struct Init
{
	Img img;
	Mouse_struct mouse;
	Zoom zoom;
	Files files;
	Interface intface;
};




