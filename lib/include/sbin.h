#ifndef SBIN_H
#define SBIN_H

#include <sdb.h>

typedef int (*SBinIoFdOpen) (void *user, const char *uri, int flags);
typedef bool (*SBinIoFdClose) (void *user, int fd);
typedef int (*SBinIoFdReadAt) (void *user, int fd, ut64 addr, ut8 *buf, int len);
typedef ut64 (*SBinIoFdSize) (void *user, int fd);

//io-interface

typedef struct s_bin_io_t {
	void *user;
	SBinIoFdOpen fd_open;
	SBinIoFdClose fd_close;
	SBinIoFdReadAt fd_read_at;
	SBinIoFdSize fd_size;
} SBinIo;

typedef struct s_bin_file_t {
	int fd;		//file descriptor
	ut32 bfd;	//bin-file descriptor
	SBinIo *io;	//io-interface used for reading from fd //this should be an id
	//plugin
	/* put info here */
} SBinFile;

typedef struct s_bin_t {
	/* bfds */
} SBin;

SBinFile *s_bin_file_new (SBin *bin, SBinIo *io, const char *uri);
SBinFile *s_bin_file_new_from_fd (SBin *bin, SBinIo *io, ut32 fd);

int s_bin_io_open (SBinIo *io, const char *uri);
bool s_bin_io_close (SBinIo *io, int fd);
int s_bin_io_read_at (SBinIo *io, int fd, ut64 addr, ut8 *buf, int len);
ut64 s_bin_io_size (SBinIo *io, int fd);


#endif
