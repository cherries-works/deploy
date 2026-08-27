CC=gcc
INCLUDES=-Iinclude

EXTRA_FLAGS_DEBUG = -fsanitize=address -fno-omit-frame-pointer

FLAGS=-Wall -Werror -Wconversion -Wno-unused-parameter -O3
LDFLAGS=
CFLAGS=$(INCLUDES) $(FLAGS)

progname = deploy
build = build
bin = dist
target = $(bin)/$(progname)

main = main.c
config = src/config/parse.c
git = src/git/build.c src/git/clone.c
process = src/process/run.c src/process/restart.c src/process/start.c src/process/rollback.c src/process/stop.c src/process/event.c src/process/initializer.c src/process/runner.c
utils = src/utils/constants.c src/utils/strings.c src/utils/dir.c src/utils/terminal.c src/utils/args.c src/utils/format.c src/utils/commands.c
setup = src/setup/setup.c
log = src/log/start.c
render = src/render/render.c src/render/utils.c

src = $(config) $(git) $(process) $(utils) $(setup) $(render) $(log) $(main)

obj = $(src:%.c=$(build)/%.o)
dep = $(obj:.o=.d)

all: $(target)

debug: LDFLAGS += $(EXTRA_FLAGS_DEBUG)
debug: CFLAGS += $(EXTRA_FLAGS_DEBUG)
debug: $(target)

$(target): $(obj) | $(bin)
	$(CC) $(obj) -o $(target) $(LDFLAGS)


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
