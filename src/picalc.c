#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <getopt.h>

#if (defined (__unix__) || defined (__unix) || defined (unix) || defined (__linux__) || defined (__FreeBSD__) || defined (__NetBSD__) || defined (__OpenBSD__) || defined (__DragonFly__) || defined (__sun) || (defined (__APPLE__) && defined (__MACH__)))
	#define UNIX_LIKE 1
	#include <unistd.h>
#elif (defined (_WIN32) || defined (_WIN64))
	#define NT_LIKE 1
	#include <windows.h>
#endif

#define RESET         0x0
#define RED           0x1
#define BLUE          0x2
#define GREEN         0x4

#ifdef UNIX_LIKE
	#define F_BOLD    "\033[1m"
	#define C_RED     "\x1b[31m"
	#define C_GREEN   "\x1b[32m"
	#define C_YELLOW  "\x1b[33m"
	#define C_BLUE    "\x1b[34m"
	#define C_MAGENTA "\x1b[35m"
	#define C_CYAN    "\x1b[36m"
	#define C_RESET   "\x1b[0m"
#endif

#define BAR_WIDTH 50

#define VERSION   "1.5a"

typedef long int lint;
void div_xlong (lint*, lint, lint*, lint);
void sum_xlong (lint*, lint*, lint*, lint);
void sub_xlong (lint*, lint*, lint*, lint);
void print_percentage (lint*, lint*, lint);
void print_infos ();
void welcome ();
void print_version ();
void print_param_err ();
void write_to_console (const char*, int, int, int);
int main (int argc, char **argv) {
	lint ncifre = 0, loop_5, loop_239, ncifre_err, vlen;
	lint *k5, *k239, *pi, *temp, k;
	lint sign, divs, prog = 0, lastprog = -1, proghop = 10;
	char bff[10], path[256];
	char copt = 0, v = 0, oarg = 0;
	FILE *f = NULL;
	#if defined (NT_LIKE)
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO consoleInfo;
    WORD saved_attributes;

    GetConsoleScreenBufferInfo(hConsole, &consoleInfo);
    saved_attributes = consoleInfo.wAttributes;
	#endif
	memset (bff, 0, sizeof (bff));
	while ((copt = getopt (argc, argv, "c:o:tp:vhz")) != -1) {
		switch (copt) {
			case 'c':
				oarg++;
				ncifre = (lint)atoi (optarg);
				break;
			case 'o':
				oarg++;
				strncpy (path, optarg, sizeof (path));
				break;
			case 't':
				v = 1;
				break;
			case 'p':
				proghop = atoi (optarg);
				if (proghop < 1 || proghop > 100)
					proghop = 10;
				break;
			case 'v':
			    print_version ();
				exit (0);
			case 'h':
				print_infos ();
				exit (0);
            case 'z':
                welcome ();
                exit (0);
			default:
				print_param_err ();
				exit (1);
		}
	}
	if (oarg != 2) {
		print_param_err ();
		exit (1);
	}
	write_to_console ("Inizio del calcolo in corso...\n", RESET, 0, 1);
	ncifre++;
	loop_5 = (lint)((log(16) / log(10) + ncifre) / (log(5) / log(10)));
	loop_239 = (lint)((log(4) / log(10) + ncifre) / (log(239) / log(10)));
	ncifre_err = (int)(log((loop_5 + loop_239) / 2) / log(10) + 1);
	vlen = ncifre + ncifre_err;
	k5 = (lint*)malloc (sizeof (lint) * vlen);
	k239 = (lint*)malloc (sizeof (lint) * vlen);
	pi = (lint*)malloc (sizeof (lint) * vlen);
	temp = (lint*)malloc (sizeof (lint) * vlen);
	memset (k5, 0, vlen * sizeof (lint));
	memset (k239, 0, vlen * sizeof (lint));
	memset (pi, 0, vlen * sizeof (lint));
	memset (temp, 0, vlen * sizeof (lint));
	write_to_console ("Calcolo iniziato!\n", RESET, 0, 1);
	k5[0] = 3;
	k5[1] = 2;
	for (k = 2; k < vlen; k++)
		k5[k] = 0;
	for (k = 0; k < vlen; k++)
		pi[k] = k5[k];
	sign = 1;
	prog = 0;
	for (k = 0; k < loop_5; k++) {
		div_xlong (k5, (lint)25, k5, vlen);
		sign = -sign;
		divs = (2 * (k + 1) + 1);
		div_xlong (k5, divs, temp, vlen);
		if (sign > 0)
			sum_xlong (pi, temp, pi, vlen);
		else if (sign < 0)
			sub_xlong (pi, temp, pi, vlen);
		prog = (lint)round (50 * k / loop_5);
		print_percentage (&prog, &lastprog, proghop);
	}
	k239[0] = 4;
	for (k = 1; k < vlen; k++)
		k239[k] = 0;
	div_xlong (k239, (lint)239, k239, vlen);
	sign = -1;
	sub_xlong (pi, k239, pi, vlen);
	for (k = 0; k < loop_239; k++) {
		div_xlong (k239, (lint)(239 * 239), k239, vlen);
		sign = -sign;
		divs = (2 * (k + 1) + 1);
		div_xlong (k239, divs, temp, vlen);
		if (sign > 0)
			sum_xlong (pi, temp, pi, vlen);
		else if (sign < 0)
			sub_xlong (pi, temp, pi, vlen);
		prog = (lint)round (50 + (50 * k / loop_239));
		print_percentage (&prog, &lastprog, proghop);
	}
	prog = 100;
	print_percentage (&prog, &lastprog, proghop);
	write_to_console ("\n", RESET, 0, 1);
	if ((f = fopen (path, "w+t")) == NULL) {
		fprintf (stderr, "Error opening file\n");
		exit (2);
	}
	for (k = 0; k < ncifre; k++) {
		if (v) {
            snprintf (bff, sizeof (bff) - 1, "%ld", pi[k]);
			write_to_console (bff, RESET, 0, 0);
		}
		fprintf (f, "%ld", pi[k]);
		if (!k) {
			fprintf (f, ".");
			if (v)
				write_to_console (".", RESET, 0, 0);
		}
	}
	if (v)
		write_to_console ("\n", RESET, 0, 1);
	fprintf (f, "\n");
	fclose (f);
	free (k5);
	free (k239);
	free (temp);
	free (pi);
	write_to_console ("\nCALCOLO FINITO!\n", RED, 1, 1);
	return 0;
}

void div_xlong (lint *x_d, lint n_d, lint *x_q, lint len) {
	//x_d = dividendo (int*)
	//n_d = divisore (int)
	//X_q = quoziente (int*)
	lint a = 0;
	lint i = 0;
	for (i = 0; i < len; i++) {
		a = a * 10 + x_d[i];
		x_q[i] = (lint)(a / n_d);
		a = a % n_d;
	}
}
void sum_xlong (lint *x_1, lint *x_2, lint *x_s, lint len) {
	lint r = 0;
	lint i = 0;
	for (i = len - 1; i >= 0; i--) {
		x_s[i] = x_1[i] + x_2[i] + r;
		r = 0;
		if (x_s[i] > 9) {
			r = 1;
			x_s[i] -= 10;
		}
	}
}
void sub_xlong (lint *x_1, lint *x_2, lint *x_s, lint len) {
	lint p = 0;
	lint i = 0;
	for (i = len - 1; i >= 0; i--) {
		x_s[i] = x_1[i] - x_2[i] - p;
		p = 0;
		if (x_s[i] < 0) {
			p = 1;
			x_s[i] += 10;
		}
	}
}
void print_percentage (lint *prog, lint *lastprog, lint proghop) {
	lint i = 0;
	lint barw = BAR_WIDTH * (*prog) / 100;
	char w_prog[20];
	memset (w_prog, 0, sizeof (w_prog));
	if (((*prog) % proghop) == 0 && (*prog) != (*lastprog)) {
            write_to_console ("\r[", RESET, 1, 0);
			for (i = 0; i < barw; i++)
				write_to_console ("=", RED, 1, 0);
			write_to_console (">", RED, 1, 0);
			for (i = 0; i < BAR_WIDTH - barw; i++)
				write_to_console ("-", BLUE, 0, 0);
            write_to_console ("] ", RESET, 1, 0);
            snprintf (w_prog, sizeof (w_prog) - 1, "%ld%%", (*prog));
            write_to_console (w_prog, (*prog) < (lint)75 ? GREEN | BLUE : RED, 1, 1);
			fflush (stdout);
			(*lastprog) = (*prog);
	}
}
void print_infos () {
    write_to_console ("\npicalc          -          a pi calculator\n", RED, 1, 0);
    write_to_console ("\nDeveloped by Niccolo' Ferrari\n", RED | BLUE, 1, 0);
    write_to_console ("\n\nOPTIONS:\n", RESET, 1, 0);
    write_to_console ("\n\t-c <num_cfr>\n\t\tSpecify the number of pi digit that you want to calculate.\n\n\t-o <file_out>\n\t\tSpecify the output file where save pi digits\n", RESET, 0, 0);
    write_to_console ("\n\t-t\n\t\tPrint digits on file and on console. [OPTIONAL].\n\n\t-p <hop>\n\t\tSpecify when refresh progress percentage. [OPTIONAL].\n", RESET, 0, 0);
    write_to_console ("\n\t-h\n\t\tPrint help.\n", RESET, 0, 0);
    write_to_console ("\n\t-v\n\t\tPrint version and license.\n", RESET, 0, 0);
    write_to_console ("\n\t-z\n\t\tRecognize your OS.\n", RESET, 0, 0);
    write_to_console ("\n\nCONTACT:\n", RESET, 1, 0);
    write_to_console ("\n\tniccolo.ferrari@hotmail.it\n", RED | GREEN, 1, 1);
}
void welcome () {
	#if defined (__linux__)
    write_to_console ("Hi Linux user!\n", RED, 0, 1);
	#elif defined (__FreeBSD__)
    write_to_console ("Hi FreeBSD user!\n", RED, 0, 1);
	#elif defined (__OpenBSD__)
    write_to_console ("Hi OpenBSD user!\n", RED, 0, 1);
	#elif defined (__NetBSD__)
    write_to_console ("Hi NetBSD user!\n", RED, 0, 1);
	#elif defined (__DragonFly__)
    write_to_console ("Hi DragonFlyBSD user!\n", RED, 0, 1);
	#elif defined (__sun)
    write_to_console ("Hi Solaris user!\n", RED, 0, 1);
	#elif (defined (__APPLE__) && defined (__MACH__))
    write_to_console ("Hi Mac user!\n", RED, 0, 1);
	#elif defined (__unix__)
    write_to_console ("Hi Unix user!\n", RED, 0, 1);
	#elif defined (_WIN64)
    write_to_console ("Hi Windows 64bit user!\n", RED, 0, 1);
    #elif defined (_WIN32)
    write_to_console ("Hi Windows 32bit user!\n", RED, 0, 1);
	#else
    write_to_console ("S/O not reconized\n", RED, 0, 1);
	#endif
}
void print_version () {
    write_to_console ("This software is under GNU GPL license. See http://minegrado.info/DWN/LICENSE.txt for details.\n", RED | BLUE, 1, 1);
    write_to_console ("crivello version " VERSION " developed by OmegaSoftware (C) a Niccolo' Ferrari company, on 06 feb 2015, with GNU GPL license.\nFor info:\nniccolo.ferrari@hotmail.it\nomegasoftware.altervista.org\nminegrado.info\nminegrado.info/DWN\n", RESET, 1, 1);
}
void print_param_err () {
    write_to_console ("Param error: try \"picalc -h\"\n", RED, 1, 1);
}
void write_to_console (const char* _string, int color, int bold, int flush) {
	#if defined (UNIX_LIKE)
	if (bold)
		printf (F_BOLD);
    	switch (color) {
        	case RED:
		  	printf (C_RED "%s", _string);
            	break;
        	case BLUE:
            	printf (C_BLUE "%s", _string);
            	break;
        	case GREEN:
            	printf (C_GREEN "%s", _string);
            	break;
        	case RED | BLUE:
            	printf (C_MAGENTA "%s", _string);
            	break;
        	case RED | GREEN:
            	printf (C_YELLOW "%s", _string);
            	break;
        	case GREEN | BLUE:
            	printf (C_CYAN "%s", _string);
            	break;
        	default:
            	printf ("%s", _string);
            	break;
    }
    printf (C_RESET);
    #elif defined (NT_LIKE)
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO consoleInfo;
    WORD saved_attributes;
    GetConsoleScreenBufferInfo(hConsole, &consoleInfo);
    saved_attributes = consoleInfo.wAttributes;

    WORD w_color = saved_attributes;
    switch (color) {
        case RED:
            w_color = FOREGROUND_RED;
            break;
        case BLUE:
            w_color = FOREGROUND_BLUE;
            break;
        case GREEN:
            w_color = FOREGROUND_GREEN;
            break;
        case RED | BLUE:
            w_color = FOREGROUND_RED | FOREGROUND_BLUE;
            break;
        case RED | GREEN:
            w_color = FOREGROUND_RED | FOREGROUND_GREEN;
            break;
        case GREEN | BLUE:
            w_color = FOREGROUND_GREEN | FOREGROUND_BLUE;
            break;
        default:
            w_color = saved_attributes;
            break;
    }
    if (bold)
        w_color |= FOREGROUND_INTENSITY;
    SetConsoleTextAttribute(hConsole, w_color);

    printf("%s", _string);

    SetConsoleTextAttribute(hConsole, saved_attributes);
	#endif
	if (flush)
        fflush (stdout);
}
