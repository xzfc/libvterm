#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <vterm.h>

static VTerm *vt;
static VTermState *vt_state;
static VTermScreen *vt_screen;

static void vt_init(bool bold_highbright)
{
	vt = vterm_new(80, 25);
	vterm_set_utf8(vt, true);

	vt_screen = vterm_obtain_screen(vt);
	vterm_screen_reset(vt_screen, 1);

	vt_state = vterm_obtain_state(vt);
	vterm_state_set_bold_highbright(vt_state, bold_highbright);
}

static void vt_pushstr(const char *str)
{
	vterm_input_write(vt, str, strlen(str));
}

static void vt_close()
{
	vterm_free(vt);
}

static void vt_dump_cell()
{
	VTermScreenCell cell;
	vterm_screen_get_cell(vt_screen, (VTermPos){0, 0}, &cell);

	printf("CELL: %c%c fg=%02x%02x%02x bg=%02x%02x%02x\n",
			cell.attrs.bold       [" b"],
			cell.attrs.reverse    [" r"],
			cell.fg.red, cell.fg.green, cell.fg.blue,
			cell.bg.red, cell.bg.green, cell.bg.blue);
}

static void vt_dump_pen()
{
	VTermValue val;

	vterm_state_get_penattr(vt_state, VTERM_ATTR_BOLD, &val);
	bool bold = val.boolean;

	vterm_state_get_penattr(vt_state, VTERM_ATTR_REVERSE, &val);
	bool reverse = val.boolean;

	vterm_state_get_penattr(vt_state, VTERM_ATTR_FOREGROUND, &val);
	VTermColor fg = val.color;

	vterm_state_get_penattr(vt_state, VTERM_ATTR_BACKGROUND, &val);
	VTermColor bg = val.color;

	printf("PEN : %c%c fg=%02x%02x%02x bg=%02x%02x%02x\n",
			bold[" b"], reverse[" r"],

			fg.red, fg.green, fg.blue,
			bg.red, bg.green, bg.blue);
}

int main(int argc, char **argv) {
	char *line = NULL;
	size_t line_size = 0;
	ssize_t read;
	int n = 0;

	bool bold_highbright = argc >= 2 && !strcmp(argv[1], "-b");
	printf("bold_highbright = %d\n\n", bold_highbright);
	vt_init(bold_highbright);

	while ((read = getline(&line, &line_size, stdin)) > 0) {
		size_t line_len = strlen(line);
		if (line[line_len-1] == '\n')
			line[line_len-1] = 0;

		vt_pushstr("\r\e[");
		vt_pushstr(line);
		vt_pushstr("m.");

		printf("LINE: %d\n", ++n);
		printf("IN  : %s\n", line);
		vt_dump_pen();
		vt_dump_cell();
		printf("\n");
	}

	vt_close();
}
