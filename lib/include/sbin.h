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

typedef struct s_bin_import_t {
	char *name;
	const char *bind;
	const char *type;
	char *classname;
	char *descriptor;
	ut32 ordinal;
	ut32 visibility;
} SBinImport;

typedef struct s_bin_reloc_t {
	ut8 type;
	ut8 additive;
	SBinSymbol *symbol;
	SBinImport *import;
	st64 addend;
	ut64 vaddr;
	ut64 paddr;
	ut32 visibility;
	/* is_ifunc: indirect function, `addend` points to a resolver function
	 * that returns the actual relocation value, e.g. chooses
	 * an optimized version depending on the CPU.
	 * cf. https://gcc.gnu.org/onlinedocs/gcc/Common-Function-Attributes.html
	 */
	bool is_ifunc;
} SBinReloc;

typedef struct r_bin_string_t {
	// TODO: rename string->name (avoid colisions)
	char *string;
	ut64 vaddr;
	ut64 paddr;
	ut32 ordinal;
	ut32 size; // size of buffer containing the string in bytes
	ut32 length; // length of string in chars
	char type; // Ascii Wide cp850 utf8 base64 ...
} SBinString;

typedef struct s_bin_field_t {
	ut64 vaddr;
	ut64 paddr;
	int size;
	ut32 visibility;
	char *name;
	char *type;
	char *comment;
	char *format;
	ut64 flags;
} SBinField;

typedef struct s_bin_header_t {
	ut64 offset;
	char *name;
	char *value;
	ut32 size;
} SBinHeader;

typedef bool (*SBinEntryCb) (void *user, SBinEntry entry);
typedef bool (*SBinSectionCb) (void *user, SBinSection section);
typedef bool (*SBinLineCb) (void *user, SBinDwarfRow line);
typedef bool (*SBinSymbolCb) (void *user, SBinSymbol symbol);
typedef bool (*SBinImportCb) (void *user, SBinImport import);
typedef bool (*SBinStringCb) (void *user, SBinString string);
typedef bool (*SBinFieldCb) (void *user, SBinField field);
typedef bool (*SBinLibCb) (void *user, char *lib);
typedef bool (*SBinRelocCb) (void *user, SBinReloc reloc);
typedef bool (*SBinClassCb) (void *user, SBinClass class);
typedef bool (*SBinHeaderCb) (void *user, SBinHeader header);

typedef struct s_bin_plugin_t {
	const char *name;
	const char *desc;
	const char *license;
	int (*init)(void *user);
	int (*fini)(void *user);
	SBinInfo *(*info)(SBinFile *bfile);
	bool (*entries)(SBinFile *bfile, void *user, SBinEntryCb add_entry);
	bool (*sections)(SBinFile *bfile, void *user, SBinSectionCb add_section);
	bool (*lines)(SBinFile *bfile, void *user, SBinLineCb add_line);
	bool (*symbols)(SBinFile *bfile, void *user, SBinSymbolCb add_symbol);
	//plugin should go for add_symbol(user, symbol) on each symbol it finds
	bool (*imports)(SBinFile *bfile, void *user, SBinImportCb add_import);
	bool (*strings)(SBinFile *bfile, void *user, SBinStringCb add_string);
	bool (*fields)(SBinFile *bfile, void *user, SBinFieldCb add_field);
	bool (*libs)(SBinFile *bfile, void *user, SBinLibCb add_lib);
	bool (*relocs)(SBinFile *bfile, void *user, SBinRelocCb add_reloc);
	bool (*classes)(SBinFile *bfile, void *user, SBinClassCb add_class);
	SdbList/*<SBinMem>*/* (*mem)(SBinFile *bfile);		//this kind sucks, bc it must stay list for now, since that list describes a tree
	bool (*patch_relocs)(SBinFile *bfile, void *user, SBinRelocCb add_reloc);	//is this correct?
	void *user;	//move this into Binfile
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
