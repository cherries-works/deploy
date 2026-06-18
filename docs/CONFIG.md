# Configuration - cherries.works Deploy

## Settings (examples)
```
repo = https://github.com/cherries-works/website
branch = main
build = make && ./build/main
run = cd dist && python3 -m http.server 8080
wait = 10s
head = 335962f81cd0e2553f14a75f08ebf5f4bcd3ec92
upgrade = true
pulse = true
```

### build & run
These commands will be run after cd-ing into the given repo.
The name of the repo is not needed to be given, Deploy parses
it himself (in the given example above, it'd be "website").

### wait
The wait setting sets how long the program waits until it
re-checks if the current repo has been updated.

30s / 1m 40s / 1 hour 2 minutes 5 seconds / 20 min 30 sec / 1 day 5 min

Deploy will try to parse any given wait from days, hours, minutes, seconds.

### head
HEAD is an optional parameter, which specifies the hash of the commit from
which you want to start.

Keep in mind that deploy will try to upgrade your current to the newest version
anyway, so you will have to disable that with `upgrade = false` [NOT RECOMMENDED].

### upgrade
Whether or not to upgrade from the current given HEAD.

### prune
Commits are stored per hash, which can cause storage issues. By enabling prune
cherries.works Deploy only keeps 2 commits (keeping the current, and previous HEAD).
