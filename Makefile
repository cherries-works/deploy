CC=gcc
INCLUDES=-Iinclude
FLAGS=-Wall -Werror -Wconversion -Wno-unused-parameter
CFLAGS=$(INCLUDES) $(FLAGS)

progname = deploy
build = build
bin = dist
target = $(bin)/$(progname)

main = main.c
config = src/config/parse.c
git = src/git/build.c src/git/clone.c
process = src/process/run.c src/process/restart.c src/process/start.c src/process/rollback.c
utils = src/utils/constants.c src/utils/strings.c src/utils/dir.c src/utils/terminal.c
setup = src/setup/setup.c
render = src/render/render.c src/render/utils.c

src = $(config) $(git) $(process) $(utils) $(setup) $(render) $(main)

obj = $(src:%.c=$(build)/%.o)
dep = $(obj:.o=.d)

all: $(target)

$(target): $(obj) | $(bin)
	$(CC) $(obj) -o $(target)


$(build)/%.o: %.c | $(build)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -MMD -MP -c $< -o $@

-include $(dep)

$(build):
	mkdir -p $(build)

$(bin):
	mkdir -p $(bin)

clean:
	rm -rf $(build) $(bin)

.PHONY: all clean
