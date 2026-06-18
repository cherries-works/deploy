#ifndef RENDER_H
#define RENDER_H


extern char *RED;
extern char *GREEN;
extern char *YELLOW;
extern char *BLUE;
extern char *WHITE;

extern char *BOLD;
extern char *DIM;
extern char *ITALIC;
extern char *UNDERLINE;

extern char *RESET;

extern char *ONE_BARS;
extern char *TWO_BARS;
extern char *THREE_BARS;
extern char *FOUR_BARS;
extern char *FIVE_BARS;
extern char *SIX_BARS;
extern char *SEVEN_BARS;
extern char *EIGHT_BARS;
extern char *NINE_BARS;
extern char *TEN_BARS;

extern char *getBars(float val);
extern char* getColor(float value);

extern void render();
extern void help();

#endif