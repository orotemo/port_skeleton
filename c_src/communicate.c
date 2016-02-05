#include <string.h>

#include <erl_interface.h>
#include <ei.h>

#include "communicate.h"
#include "erl_comm.h"

#define WRITE_BUF_LEN 5 * 1024 * 1000 //5MB max

byte __write_buf[WRITE_BUF_LEN];

void send_str_tuple(char* atom, char *str) {
  ETERM *term = erl_mk_string(str);
  send_response_tuple(atom, term);
  erl_free_term(term);
}

void send_response_tuple(char* atom_str, ETERM *term) {
  ETERM *to_send, *tup_list[2];
  tup_list[0] = erl_mk_atom(atom_str);
  tup_list[1] = term;
  to_send = erl_mk_tuple(tup_list, 2);

  send_response(to_send);

  erl_free_term(tup_list[0]);
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
  memset(__write_buf, 0, WRITE_BUF_LEN);
  erl_encode(response, __write_buf);
  write_cmd(__write_buf, erl_term_len(response));
}
