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

typedef struct s_bin_hash_t {
	const char *type;
	ut64 addr;
	int len;
	ut64 from;
	ut64 to;
	ut8 buf[32];
	const char *cmd;
} SBinHash;

typedef struct s_bin_symbol_t {
	char *name;
	ut64 vaddr;
	ut64 paddr;
	ut32 size;
} SBinSymbol;

typedef bool (*SBinSymbolCb) (void *user, SBinSymbol symbol);

typedef struct s_bin_plugin_t {
	const char *name;
	const char *desc;
	const char *license;
	bool (*symbols)(void *user, SBinSymbolCb add_symbol, SBinFile *arch);
	//plugin should go for add_symbol(user, symbol) on each symbol it finds
} SBinPlugin;

typedef struct s_bin_info_t {
	char *file;
	char *type;
	char *bclass;
	char *rclass;
	char *arch;
	char *cpu;
	char *machine;
	char *os;
	char *subsystem;
	char *rpath;
	char *guid;
	char *debug_file_name;
	const char *lang;
	int bits;
	int has_va;
	int has_pi; // pic/pie
	int has_canary;
	int has_crypto;
	int has_nx;
	int big_endian;
	bool has_lit;
	char *actual_checksum;
	char *claimed_checksum;
	int pe_overlay;
	bool signature;
	ut64 dbg_info;
	SBinHash sum[3];
	ut64 baddr;
	char *intrp;
} SBinInfo;

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
