#include <stdio.h>

#include "render.h"
#include "config.h"

void render(Status s) {
    printf("%s%sCherries Deploy%s ───────────────────────────────────── v0.1.0 ──── \n", BOLD, RED, RESET);
    printf("%s PID %-16d ─────────────────────────────────────────── %s\n", BOLD, s.pid, RESET);
    printf("%s STATUS %-13d ─────────────────────────────────────────── %s\n", BOLD, s.status, RESET);
    printf("%s HASH %-15s ─────────────────────────────────────────── %s\n", BOLD, s.hash, RESET);
    printf("%s───────────────────────────────────────────────────────────────── %s\n", BOLD, RESET);
    printf("%s EVENTS: ──────────────────────────────────────────────────────── %s\n", BOLD, RESET);
}

void renderStatus(Deploy d, Status s) {}
