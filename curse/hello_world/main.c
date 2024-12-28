#include <ncurses.h>

int main()
{
	initscr();
	printw("Hello, world!");
	refresh();
	nocbreak();
	noecho();
	endwin();
	return 0;
}
