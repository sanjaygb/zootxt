/*
 * zootxt.c -- hack to print U2 ZOO TV phrases
 *
 * unix version: Gideon Glass <gglass@calvin.edu>   11/93
 * 940114: some (major) bug fixes.
 * 940115: curses stuff instead of stupid ^M screen mangling
 *
 * original idea and code (by now it's pretty much totally rewritten)
 * ZOOSCREEN TEXT 1.0(release)
 * copyright(c) 1993 by John Kinsella.
 */

#include <stdio.h>
#include <string.h>
#include <curses.h>
#include <signal.h>

/*
 * change the string below to be the full path of the phrases file (one phrase
 * per line) that you want the program to read from.   Unless you're on a unix
 * machine at Calvin College, Grand Rapids, Michigan, USA, you'll have to 
 * change this definition.
 *
 * Optionally, you can specify the filename as a command line argument.
 */

#define PHRASES "phrases.txt"

/*
 * linked list to read in data
 */

struct zoolist {
  char phrase[80];
  int len;
  struct zoolist *next;
};

main(argc, argv) 
     int argc;
     char **argv;
{
  char temp[80];
  FILE *zoofile;
  int i, total = 0, ct, x, ct2;
  struct zoolist *head, *zoodata, **zw, *p;
  WINDOW *w;
  extern int LINES, COLS;
  char *filename = PHRASES;
  void h();
  
  w = initscr();
  signal(SIGINT, h);
  signal(SIGQUIT, h);
  setbuf(stdout, (char *)0);
  srandom(getpid() ^ time(0));

  if (argc > 1) 
    filename = argv[1];
  if ((zoofile = (FILE *)fopen(filename, "r")) == NULL) {
    perror(filename);
    exit(1);
  }

  if ((head =zoodata=(struct zoolist *) malloc(sizeof(struct zoolist)))==NULL) {
    perror("malloc:head");
    exit(1);
  }

  while (fgets(temp, 80, zoofile)!=NULL) {
    x = strlen(temp);
    temp[--x] = '\0';
    if ((zoodata->next=(struct zoolist *)malloc(sizeof(struct zoolist)))==NULL) {
      perror("malloc:zoodata");
      exit(1);
    }

    strcpy(zoodata->phrase, temp);
    zoodata->len = x;
    zoodata=zoodata->next;
    zoodata->next=NULL;
    total++;
  }

  if ((zw = (struct zoolist **) malloc(total * sizeof(struct zoolist*))) ==0) {
    perror("malloc:zw");
    exit(1);
  }
  
  for (p = head, i = 0; p; p = p->next, i++)
    zw[i] = p;
    
  fclose(zoofile);
  refresh();
  ct = ct2 = 0;
  strcpy(temp, "EVERYTHING YOU KNOW IS WRONG");
  mvaddstr(LINES/2, COLS/2 - strlen(temp)/2, temp);
  move(LINES-1,0);  
  refresh();
  sleep(2);
  for(;;) {
    i = random() % total;
    mvaddstr(LINES/2, COLS/2 - zw[i]->len/2, zw[i]->phrase);
    move(LINES-1,0);
    refresh();
    
/* pseudo-random 4 second sleep, just for the fun of it */

    if (index(zw[i]->phrase, ' ') != NULL) {
/*      if (ct2 < 10) continue;
      ct2 = 0; */
      if (ct > 50) {
	ct = 0;
	sleep(4);
      } else {
	usleep(zw[i]->len * 19000);
	ct++;
      }
    } else {
/*      ct2++; */
      usleep(zw[i]->len * 15000);
    }
    move(LINES/2, 0);
    clrtoeol();
  }
}

/*
 * h: clean up on signals
 */

void h()
{
  move(LINES/2, 0);
  clrtoeol();
  move(0,0);
  refresh();
  endwin();
  exit(0);
}
