CC=gcc
CFLAGS=-I include

config = src/config/parse.c
git = src/git/build.c src/git/clone.c
process = src/process/run.c src/process/restart.c src/process/start.c src/process/rollback.c
utils = src/utils/constants.c src/utils/format.c src/utils/dir.c src/utils/terminal.c
setup = src/setup/setup.c
render = src/render/render.c src/render/utils.c

deploy: main.c $(config) $(git) $(process) $(utils) $(setup) $(render)
	mkdir -p dist
	$(CC) $(CFLAGS) -o dist/deploy main.c $(config) $(git) $(process) $(utils) $(setup) $(render)
