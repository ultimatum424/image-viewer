#include "config.h"
using namespace sf;
void RunProgram(RenderWindow& window)
{

}
int main()
{
	setlocale(LC_CTYPE, "rus");
	sf::RenderWindow window(sf::VideoMode(WINDOW_X, WINDOW_Y), "Mini Game");
	RunProgram(window);
	return 0;
}