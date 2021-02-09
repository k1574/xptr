/* (C)opyright MMIV-MMV Anselm R. Garbe <garbeam at gmail dot com>
	See "license" file for details. */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <X11/Xlib.h>

#include "../../arg.h"

char *argv0;
static Display *dpy;
static Window root;

void
usage(void)
{
	fprintf(stderr, "usage: %s [-w window] {[-x] [-y]}|{<x> <y>}\n", argv0);
	exit(1);
}

int
getwinptr(Window w, int *x, int *y)
{
	int di;
	unsigned int dui;
	Window dw;
	
	return XQueryPointer(dpy, w, &dw, &dw, &di, &di, x, y, &dui) ;
}

void
main(int argc, char **argv) {
	int x, y;
	int xflag, yflag;

	argv0 = argv[0] ;

	if(!(dpy = XOpenDisplay(0))) {
		fprintf(stderr, "%s: cannot open display\n", argv0);
		exit(1);
	}

	root = RootWindow(dpy, DefaultScreen(dpy)) ;
	xflag = yflag = 0 ;
	ARGBEGIN {
	case 'x' :
		++xflag ;
	break;
	case 'y' :
		++yflag;
	break;
	case 'w' :
		root = strtol(EARGF(usage()), 0, 0) ;
	break;
	default:
		usage();
	} ARGEND
	
	if(argc==2){
		if( (sscanf(argv[0], "%d", &x) != 1)
				||(sscanf(argv[1], "%d", &y) != 1) )
			usage();
		XWarpPointer(dpy, None, root, 0, 0, 0, 0, x, y);
		XSetInputFocus(dpy, PointerRoot, RevertToPointerRoot, CurrentTime);
	}else if(!argc && xflag || yflag){
		getwinptr(root, &x, &y);
		if(xflag) printf("%d\n", x);
		if(yflag) printf("%d\n", y);
	}else
		usage();

	XCloseDisplay(dpy);
	exit(0);
}
