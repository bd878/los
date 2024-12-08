#include <stdlib.h>
#include <curses.h>
#include <signal.h>

static void finish(int sig);

int
main(int argc, char *argv[])
{
	int num = 0;

	(void) signal(SIGINT, finish);  /* interrupts */

	(void) initscr();
	(void) scrollok(WINDOW, TRUE);
	keypad(stdscr, TRUE);			/* enable keyboard mapping */
	(void) cbreak();				/* take input chars one at a time */
	(void) echo();					/* echo input */

	if (has_colors()) {
		start_color();

		/* color assignment */
		init_pair(1, COLOR_RED,     COLOR_BLACK);
        init_pair(2, COLOR_GREEN,   COLOR_BLACK);
		init_pair(3, COLOR_YELLOW,  COLOR_BLACK);
		init_pair(4, COLOR_BLUE,    COLOR_BLACK);
		init_pair(5, COLOR_CYAN,    COLOR_BLACK);
		init_pair(6, COLOR_MAGENTA, COLOR_BLACK);
		init_pair(7, COLOR_WHITE,   COLOR_BLACK);
	}

	for (;;) {
		int c = getch(); /* accept single keystroke of input */
		attrset(COLOR_PAIR(num % 8));
		num++;
	}

	finish(0);
}

static void finish(int sig)
{
	endwin();
	printf("exit\n");

	exit(0);
}
