char *RED = "\033[31m";
char *GREEN = "\033[32m";
char *YELLOW = "\033[33m";
char *BLUE = "\033[34m";
char *WHITE = "\033[37m";

char *BOLD = "\033[1m";
char *DIM = "\033[2m";
char *ITALIC = "\033[3m";
char *UNDERLINE = "\033[4m";

char *RESET = "\033[0m";

char *ONE_BARS =    "█░░░░░░░░░";
char *TWO_BARS =    "██░░░░░░░░";
char *THREE_BARS =  "███░░░░░░░";
char *FOUR_BARS =   "████░░░░░░";
char *FIVE_BARS =   "█████░░░░░";
char *SIX_BARS =    "██████░░░░";
char *SEVEN_BARS =  "███████░░░";
char *EIGHT_BARS =  "████████░░";
char *NINE_BARS =   "█████████░";
char *TEN_BARS =    "██████████";

char *getBars(float value) {
    if(value < 10.0f) return ONE_BARS;
    if(value < 20.0f) return TWO_BARS;
    if(value < 30.0f) return THREE_BARS;
    if(value < 40.0f) return FOUR_BARS;
    if(value < 50.0f) return FIVE_BARS;
    if(value < 60.0f) return SIX_BARS;
    if(value < 70.0f) return SEVEN_BARS;
    if(value < 80.0f) return EIGHT_BARS;
    if(value < 90.0f) return NINE_BARS;
    return TEN_BARS;
}

char* getColor(float value) {
    if (value < 60.0f) return GREEN;
    if (value < 80.0f) return YELLOW;
    return RED;
}