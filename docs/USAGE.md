The usage of Deply has been kept to simple by default. It is not a replacement for
bigger tools, instead a tool that helps you to deploy your system in less than a
minute.

# Download

Clone the github repo, and run

```
    $ git clone https://github.com/cherries-works/deploy.git
    $ cd deploy/
    $ make
```

# Get started

Next setup a config file for the Git repository you want to host (see [/docs/CONFIG.md](/docs/CONFIG.md)),
and then run

```
    $ ./dist/deploy --config [path to config file]
```

# Commands
`
    $ ./dist/deploy --help      
`

```
    --config [path]      Specify the location of config file.
    --help               Prints this. 
```
