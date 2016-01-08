#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// From erlang include lib
#include <erl_interface.h>
#include <ei.h>
#include "erl_comm.h"
#include "communicate.h"

#define is_command(fnp, command) \
  strncmp((const char *)ERL_ATOM_PTR(fnp), command, strlen(command)) == 0

void hex_print(byte *buf, int n) {
  int i;
  fprintf(stderr, "<<");
  for (i = 0; i < n; i++)
  {
    fprintf(stderr, "16#%02X", buf[i]);
    if (i < n-1) fprintf(stderr, ",");
  }
  fprintf(stderr,">>\r\n");
  fflush(stderr);

}

int main() {
  erl_init(NULL, 0);

  ETERM *tuplep;
  ETERM *fnp, *argp;
  byte buf[2048];

  while (read_cmd(buf) > 0) {
    tuplep = erl_decode(buf);
    fnp = erl_element(1, tuplep);
    argp = erl_element(2, tuplep);

    if (is_command(fnp, "stop")) {
      break;
    }
    else if (is_command(fnp, "echo")) {
      send_response(argp);
    } else send_ok();
  }
}
