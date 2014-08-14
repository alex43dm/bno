
#include <sys/sys_msg.h>
#include <errno.h>

#include <string.h>

int sys_msg(void *buf) {
	union _sysmsg {
		int status;
		struct _ver {
			struct _sysmsg_hdr hdr;
			struct _sysmsg_version version;
			} ver;
		struct _ver_reply {
			struct _sysmsg_hdr_reply hdr;
			struct _sysmsg_version_reply version;
			} ver_reply;
		} *m;

	m = buf;

	switch(m->ver.hdr.subtype) {

	case _SYSMSG_SUBTYPE_VERSION:
		memset(&m->ver_reply, 0, sizeof(m->ver_reply));
		m->ver_reply.hdr.status = 0;
		strcpy(&m->ver_reply.version.name[0], NAME);
		strncpy(&m->ver_reply.version.date[0], __DATE__, 11);
		m->ver_reply.version.version = VERSION;
		m->ver_reply.version.letter = RELEASE[0];
		return(sizeof(m->ver_reply));

	case _SYSMSG_SUBTYPE_DEATH:
	case _SYSMSG_SUBTYPE_TRACE:
	default:
		m->status = EINVAL;
		return(sizeof(m->status));
		}
	}
