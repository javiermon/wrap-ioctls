/* 
taken from:
http://stackoverflow.com/questions/15153209/compilation-fails-with-error-next-ioctl-undeclared-first-use-in-this-functi
http://scaryreasoner.wordpress.com/2007/11/17/using-ld_preload-libraries-and-glibc-backtrace-function-for-debugging/
*/

#define _GNU_SOURCE
#define _USE_GNU

#include <signal.h>
#include <execinfo.h>
#include <stdlib.h>
#include <stdio.h>
#include <dlfcn.h>

static void show_stackframe() {
  void *trace[16];
  char **messages = (char **)NULL;
  int i, trace_size = 0;

  trace_size = backtrace(trace, 16);
  messages = backtrace_symbols(trace, trace_size);
  printf("[bt] Execution path:\n");
  for (i=0; i < trace_size; ++i)
        printf("[bt] %s\n", messages[i]);
}

int ioctl(int fd, int request, void *data)
{
        char *msg;
        static int (*next_ioctl)(int fd, int request, void *data) = NULL;

        if (next_ioctl == NULL) {
                fprintf(stderr, "ioctl : wrapping ioctl\n");
                fflush(stderr);
                // next_ioctl = dlsym((void *) -11, /* RTLD_NEXT, */ "ioctl");
                next_ioctl = dlsym(RTLD_NEXT, "ioctl");
                fprintf(stderr, "next_ioctl = %p\n", next_ioctl);
                fflush(stderr);
                if ((msg = dlerror()) != NULL) {
                        fprintf(stderr, "ioctl: dlopen failed : %s\n", msg);
                        fflush(stderr);
                        exit(1);
                } else
                        fprintf(stderr, "ioctl: wrapping done\n");
                fflush(stderr);
        }
        if (request == 1) { /* SCSI_IOCTL_SEND_COMMAND ? */
                /* call back trace */
                fprintf(stderr, "SCSI_IOCTL_SEND_COMMAND ioctl\n");
                fflush(stderr);
                show_stackframe();
        }
        return next_ioctl(fd, request, data);
}