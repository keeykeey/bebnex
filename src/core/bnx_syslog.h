
/*
 *
 */


#ifndef _BNX_SYSLOG_H_INCLUDED_
#define _BNX_SYSLOG_H_INCLUDED_


typedef struct {
    bnx_uint_t            facility;
    bnx_uint_t            severity;
    bnx_str_t             tag;

    bnx_addr_t            server;
    bnx_connection_t      conn;
    unsigned              bussy:1;
    unsigned              nohostname:1;
} bnx_syslog_peer_t;


char *bnx_syslog_process_conf(bnx_conf_t *cf, bnx_syslog_peer_t *peer);
u_char *bnx_syslog_add_header(bnx_syslog_peer_t *peer, u_char *buf);
void bnx_syslog(bnx_log_t *log, bnx_uint_t level, u_char *buf,
    size_t len);
ssize_t bnx_syslog_send(bnx_syslog_peer_t *peer, u_char *buf, size_t len);


#endif /* _BNX_SYSLOG_H_INCLUDED_ */