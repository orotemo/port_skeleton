#ifndef _COMMUNICATE_H
#define _COMMUNICATE_H

void send_str_tuple(char* atom, char *str);
void send_response_tuple(char* atom_str, ETERM *term);
void send_error(char *err_msg);
void send_ok(void);
void send_response(ETERM *response);

#endif //_COMMUNICATE_H
