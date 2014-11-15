malloc
------

This is just a personal experiment to understand the vagaries of malloc
implementation a bit better than I ever did before.  Never implemented one
before :-)

Run `make` to get going.  Try out `make test` to see a debug run.

To run with a custom `malloc`, with a shell like `bash` try setting `LD_PRELOAD`
as an environment variable like:

```bash
> LD_PRELOAD=./malloc.so ./test-malloc
``` 
