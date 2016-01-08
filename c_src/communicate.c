#include <string.h>

#include <erl_interface.h>
#include <ei.h>

#include "communicate.h"
#include "erl_comm.h"

void send_str_tuple(char* atom, char *str) {
  ETERM *tup_list[2];
  ETERM *to_send;
  tup_list[0] = erl_mk_atom(atom);
  tup_list[1] = erl_mk_string(str);
  to_send = erl_mk_tuple(tup_list, 2);

  send_response(to_send);

  erl_free_term(tup_list[0]);
  erl_free_term(tup_list[1]);
  erl_free_compound(to_send);
}

void send_error(char *err_msg) {
  ETERM *tup_list[2];
  ETERM *to_send;
  tup_list[0] = erl_mk_atom("sql_error");
  tup_list[1] = erl_mk_string(err_msg);
  to_send = erl_mk_tuple(tup_list, 2);

  send_response(to_send);

  erl_free_term(tup_list[0]);
  erl_free_term(tup_list[1]);
  erl_free_compound(to_send);
}

void send_ok(void) {
  ETERM *to_send;

  to_send = erl_mk_atom("ok");
  send_response(to_send);
  erl_free_term(to_send);
}

void send_response(ETERM *response) {
  byte buf[2048];
  bzero(buf, 2048);
  erl_encode(response, buf);
  write_cmd(buf, erl_term_len(response));
}
