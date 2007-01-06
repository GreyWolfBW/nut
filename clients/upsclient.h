/* upsclient.h - definitions for upsclient functions

   Copyright (C) 2002  Russell Kroll <rkroll@exploits.org>

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
*/

#ifndef UPSCLIENT_H_SEEN
#define UPSCLIENT_H_SEEN

#ifdef HAVE_SSL
#include <openssl/err.h>
#include <openssl/ssl.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

#define UPSCLI_ERRBUF_LEN	256
#define UPSCLI_NETBUF_LEN	512	/* network i/o buffer */

#include "parseconf.h"

typedef struct {
	char	*host;
	int	port;
	int	fd;
	int	flags;
	int	upserror;
	int	syserrno;
	int	upsclient_magic;

	PCONF_CTX	*pc_ctx;

	char	errbuf[UPSCLI_ERRBUF_LEN];

#ifdef HAVE_SSL
	SSL_CTX	*ssl_ctx;
	SSL	*ssl;
#else
	void	*ssl_ctx;
	void	*ssl;
#endif

}	UPSCONN;

const char *upscli_strerror(UPSCONN *ups);

int upscli_connect(UPSCONN *ups, const char *host, int port, int flags);

/* --- functions that only use the new names --- */

int upscli_get(UPSCONN *ups, unsigned int numq, const char **query, 
		unsigned int *numa, char ***answer);

int upscli_list_start(UPSCONN *ups, unsigned int numq, const char **query);

int upscli_list_next(UPSCONN *ups, unsigned int numq, const char **query,
		unsigned int *numa, char ***answer);

int upscli_sendline(UPSCONN *ups, const char *buf, size_t buflen);

int upscli_readline(UPSCONN *ups, char *buf, size_t buflen);

int upscli_splitname(const char *buf, char **upsname, char **hostname,
			int *port);

int upscli_sslcert(UPSCONN *ups, const char *file, const char *path, int verify);

int upscli_disconnect(UPSCONN *ups);

/* these functions return elements from UPSCONN to avoid direct references */

int upscli_fd(UPSCONN *ups);
int upscli_upserror(UPSCONN *ups);

/* returns 1 if SSL mode is active for this connection */
int upscli_ssl(UPSCONN *ups);	

/* upsclient error list */

#define UPSCLI_ERR_UNKNOWN	0	/* Unknown error */
#define UPSCLI_ERR_VARNOTSUPP	1	/* Variable not supported by UPS */
#define UPSCLI_ERR_NOSUCHHOST	2	/* No such host */
#define UPSCLI_ERR_INVRESP	3	/* Invalid response from server */
#define UPSCLI_ERR_UNKNOWNUPS	4	/* Unknown UPS */
#define UPSCLI_ERR_INVLISTTYPE	5	/* Invalid list type */
#define UPSCLI_ERR_ACCESSDENIED	6	/* Access denied */
#define UPSCLI_ERR_PWDREQUIRED	7	/* Password required */
#define UPSCLI_ERR_PWDINCORRECT	8	/* Password incorrect */
#define UPSCLI_ERR_MISSINGARG	9	/* Missing argument */
#define UPSCLI_ERR_DATASTALE	10	/* Data stale */
#define UPSCLI_ERR_VARUNKNOWN	11	/* Variable unknown */
#define UPSCLI_ERR_LOGINTWICE	12	/* Already logged in */
#define UPSCLI_ERR_PWDSETTWICE	13	/* Already set password */
#define UPSCLI_ERR_UNKNOWNTYPE	14	/* Unknown variable type */
#define UPSCLI_ERR_UNKNOWNVAR	15	/* Unknown variable */
#define UPSCLI_ERR_VARREADONLY	16	/* Read-only variable */
#define UPSCLI_ERR_TOOLONG	17	/* New value is too long */
#define UPSCLI_ERR_INVALIDVALUE	18	/* Invalid value for variable */
#define UPSCLI_ERR_SETFAILED	19	/* Set command failed */
#define UPSCLI_ERR_UNKINSTCMD	20	/* Unknown instant command */
#define UPSCLI_ERR_CMDFAILED	21	/* Instant command failed */
#define UPSCLI_ERR_CMDNOTSUPP	22	/* Instant command not supported */
#define UPSCLI_ERR_INVUSERNAME	23	/* Invalid username */
#define UPSCLI_ERR_USERSETTWICE	24	/* Already set username */
#define UPSCLI_ERR_UNKCOMMAND	25	/* Unknown command */
#define UPSCLI_ERR_INVALIDARG	26	/* Invalid argument */
#define UPSCLI_ERR_SENDFAILURE	27	/* Send failure: %s */
#define UPSCLI_ERR_RECVFAILURE	28	/* Receive failure: %s */
#define UPSCLI_ERR_SOCKFAILURE	29	/* socket failure: %s */
#define UPSCLI_ERR_BINDFAILURE	30	/* bind failure: %s */
#define UPSCLI_ERR_CONNFAILURE	31	/* Connection failure: %s */
#define UPSCLI_ERR_WRITE	32	/* Write error: %s */
#define UPSCLI_ERR_READ		33	/* Read error: %s */
#define UPSCLI_ERR_INVPASSWORD	34	/* Invalid password */
#define UPSCLI_ERR_USERREQUIRED	35	/* Username required */
#define UPSCLI_ERR_SSLFAIL	36	/* SSL is not available */
#define UPSCLI_ERR_SSLERR	37	/* SSL error: %s */
#define UPSCLI_ERR_SRVDISC	38	/* Server disconnected */
#define UPSCLI_ERR_DRVNOTCONN	39	/* Driver not connected */
#define UPSCLI_ERR_NOMEM	40	/* Memory allocation failure */
#define UPSCLI_ERR_PARSE	41	/* Parse error: %s */
#define UPSCLI_ERR_PROTOCOL	42	/* Protocol error */

#define UPSCLI_ERR_MAX		42	/* stop here */

/* list types for use with upscli_getlist */

#define UPSCLI_LIST_VARS	1	/* all variables */
#define UPSCLI_LIST_RW		2	/* just read/write variables */
#define UPSCLI_LIST_CMDS	3	/* instant commands */

/* flags for use with upscli_connect */

#define UPSCLI_CONN_TRYSSL	0x0001	/* try SSL, OK if not supported       */
#define UPSCLI_CONN_REQSSL	0x0002	/* try SSL, fail if not supported     */

#ifdef	HAVE_IPV6
#define UPSCLI_CONN_INET	0x0004  /* IPv4 only */
#define UPSCLI_CONN_INET6	0x0008  /* IPv6 only */
#endif

#ifdef __cplusplus
}
#endif

#endif	/* UPSCLIENT_H_SEEN */
