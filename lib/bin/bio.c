#include <sbin.h>
#ifndef	R_IO_READ
#define R_IO_READ	4
#endif

int s_bin_io_open (SBinIo *io, const char *uri)
{
	if (!io) {
		return -1;
	}
	return io->fd_open (io->user, uri, R_IO_READ);
}

bool s_bin_io_close (SBinIo *io, int fd)
{
	if (!io) {
		return false;
	}
	return io->fd_close (io->user, fd);
}

int s_bin_io_read_at (SBinIo *io, int fd, ut64 addr, ut8 *buf, int len)
{
	if (!io) {
		return -1;
	}
	return io->fd_read_at (io->user, fd, addr, buf, len);
}

ut64 s_bin_io_size (SBinIo *io, int fd)
{
	if (!io) {
		return 0LL;
	}
	return io->fd_size (io->user, fd);
}
