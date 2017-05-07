# squashfuse for macOS with Build Script and Usage instructions

This project is identical to the mainline project except for the fact that it contains this readme with notes and a `build_mac.sh` script.

The build script can be run (`build_mac.sh`) from the checked out version of this code, however you will need the following before continuing:

- [Xcode Command Line Tools](https://developer.apple.com/library/content/technotes/tn2339/_index.html) from Apple
- An installation of [OSXFUSE](http://osxfuse.github.io/)
- [Homebrew](https://brew.sh) software/package manager as well as the following packages:
	- `squashfs`
	- `lz4`
	- `lzo`
	- `xz`
	- `libtool`
	- `autoconf`
	- `automake`
	
Once everything is installed and you have built the program it will be installed under `~/unix/bin`.

## Additions to your shell rc/`.profile` file

Since most shells take their basic cues from BASH, the code will be provided in that format.

### Creating SquashFS File System Images/Archives

**This assumes `mksquashfs` is in your `$PATH`**

Both of these functions can be invoked as if they were programs, the only difference is memory/cpu usage. 

An example invocation: `$ squash_it ./dir1 ./dir2 outputfile.squashfs`

```bash
squash_it() {
	mksquashfs "$@" -comp xz -processors 6 -mem 8G -xattrs
}

squash_it_lite() {
	mksquashfs "$@" -comp xz -processors 2 -mem 2G -xattrs
}
```

### Mounting SquashFS Files

**This assumes `squashfuse` and `squashfuse_ll` are in your `$PATH`**

Both aliases provided take the same parameters:
`<volume name> <file path> [<optional additional options in -oKEY=VALUE format>]`

A list of options can be found in the [OSXFUSE documentation](https://github.com/osxfuse/osxfuse/wiki/Mount-options).

```bash
# Private function
_mount_squashfs() {
  if [ $# -lt 3 ]; then
    echo "Usage: <volname> <path> [<option/flags... n>]" # Mount command is also required but since this is an alias, lets not include it in the description
  else
    local A="$1" && shift
    local B="$1" && shift
    local C="$1" && shift
    "$A" -ordonly -olocal -ovolname="$B" "$@" "$C" "/Volumes/$B"
  fi
}

# Public Aliases
alias mount_squashfs="_mount_squashfs squashfuse "
alias mount_squashfs_unsafe="_mount_squashfs squashfuse_ll "
```


--


# Original Readme

			 squashfuse - Mount SquashFS archives using FUSE

				   https://github.com/vasi/squashfuse

	Squashfuse lets you mount SquashFS archives in user-space. It supports almost
	all features of the SquashFS format, yet is still fast and memory-efficient. So
	that everyone can use it, squashfuse supports many different operating systems
	and is available under a permissing license.

	SquashFS is an efficiently compressed, read-only storage format. Support for it
	has been built into the Linux kernel since 2009. It is very common on Live CDs
	and embedded Linux distributions.

	Quick start:
	  $ ./configure && make
	  $ ./squashfuse foo.squashfs mountpoint


	1. Table of contents
	====================
	0. Introduction
	1. Table of contents
	2. Getting started
	   - System requirements
	   - Getting the source
	   - Building
	   - Usage
	3. About squashfuse
	   - Is squashfuse for you?
	   - What's included?
	   - Features
	   - Known bugs
	4. References
	   - Licensing
	   - Acknowledgements
	   - Links


	2. Getting started
	==================

	2a. System requirements
	-----------------------
	To build and use squashfuse, you must be able to use the terminal of your
	operating system.

	Runtime requirements:
	  - FUSE 2.5 or later
	  - At least one of the following compression libraries
		  - zlib
		  - lzo2
		  - xz (aka. liblzma)
		  - lz4
	  - (optional) libattr, for better extended attribute support on Linux

	Build requirements:
	  - A C compiler
	  - make (any variant)
	  - sed
	  - (optional) pkg-config, for detection of dependencies

	To build from the development repository, rather than a tarball, you'll need:
	  - autoconf 2.60 or later
	  - automake 1.11 or later
	  - libtool 2

	Known fully-supported platforms:
	  - Linux
	  - Mac OS X
	  - FreeBSD
	  - NetBSD
	  - OpenIndiana
	  - Android
	  - other platforms may work too!

	For a precise list of packages you will need on your OS, and other platform
	notes, please see the file `PLATFORMS'.


	2b. Getting the source
	----------------------
	The squashfuse distribution can be downloaded from SourceForge:
	https://sourceforge.net/projects/squashfuse/files/

	The development repository uses git, at GitHub:
	http://github.com/vasi/squashfuse


	2c. Installation
	----------------
	Squashfuse is built with the usual `configure && make'. If you need more
	detailed instructions:

	0. Ensure you're at your terminal, in the directory containing this README.

	1. (if needed) If the file `configure' is already present, skip this step.
	   Otherwise, run `./autogen.sh' to generate one.

	2. Run `./configure' to set up the build. You can find useful configuration
	   options in the file `CONFIGURATION', or by running `./configure --help'.

	   If configure fails, check that you really have all the requirements
	   installed. You may also want to check the `PLATFORMS' file to see if there
	   are any special notes for your operating system.

	3. Run `make' to build `squashfuse'.

	4. (optional) If you want to use squashfuse in this directory, that's ok.
	   But if you'd rather install it, run `make install'. If you need root
	   privileges, `sudo make install' may work.


	2d. Example: Ubuntu
	-------------------

	For example on Ubuntu 16.04:

	sudo apt-get -y install git autoconf libtool make gcc libtool libfuse-dev liblzma-dev
	libtoolize --force
	aclocal
	autoheader
	automake --force-missing --add-missing
	autoconf
	./configure --with-xz=/usr/lib/
	make


	2e. Usage
	---------
	You'll need a SquashFS archive to use squashfuse. If you don't already have
	one, you can create one using the `mksquashfs' utility from the squashfs-tools
	project.

	To create a SquashFS archive:
	  $ mksquashfs DIRECTORY ARCHIVE

	To mount a SquashFS archive with squashfuse:
	  $ squashfuse ARCHIVE MOUNTPOINT

	To unmount when you're done:
	  $ umount MOUNTPOINT         # On Mac/BSD
	  $ fusermount -u MOUNTPOINT  # On Linux

	For more options, see the man page squashfuse(1).


	3. About squashfuse
	===================

	3a. Is squashfuse for you?
	--------------------------
	Squashfuse is a great option if you have a SquashFS archive, and:
	  - You're not running Linux, or
	  - You don't have root access, or
	  - You're too concerned about security to use root, or
	  - You find it inconvenient to elevate privileges, or
	  - SquashFS is not built into your kernel, but FUSE is, or
	  - You want to hack on the SquashFS format without risking kernel panics.

	Squashfuse is probably not the right tool for the job, if:
	  - You don't have FUSE.
		More and more systems have FUSE, but some don't. Squashfuse requires it.
	
	  - You have a very old SquashFS archive.
		Neither squashfuse nor the Linux kernel support SquashFS versions less
		than 4.0. Use `unsquashfs' from the squashfs-tools project.

	  - You want to create or modify a SquashFS archive.
		Neither squashfuse nor the Linux kernel support write access, use
		`mksquashfs' from squashfs-tools.

	  - You want to extract an entire SquashFS archive.
		If you don't want to mount anything, it's more efficient and convenient
		to just use unsquashfs.

	  - You want your root filesystem `/' to be SquashFS.
		This isn't well-tested, though it may be possible.
  
	  - You're highly concerned about bugs.
		The SquashFS kernel module has seen much more testing than squashfuse.


	If you don't yet use SquashFS, consider starting, now that squashfuse exists.
	For many uses, the chief drawbacks of SquashFS were requiring Linux and root
	access, but squashfuse has that covered.

	  - Use SquashFS for archival and backup, instead of tar.
		It offers faster creation (multi-core), and browsing without unpacking.
  
	  - Use SquashFS instead of zip. 
		It has better compression, and faster directory lookup.
	
	  - Use SquashFS instead of compressed disk images like DMG, uzip or Partimage.
		It has better compression and portability.
  

	3b. What's included?
	--------------------
	Squashfuse currently comprises three programs:

	  * squashfuse      Allows you to mount a squashfs filesystem.
  
	  * squashfuse_ll   Like `squashfuse', but implemented using the low-level
						FUSE API. It's a tiny bit faster, but less portable.
  
	  * squashfuse_ls   Lists all the files in a squashfs archive. A demonstration
						of using the squashfuse core in the absence of FUSE.


	3c. Features
	------------
	Squashfuse supports the following SquashFS features: 
	  - zlib, LZO, LZMA2 and LZ4 decompression
	  - Fast, indexed directory lookup
	  - Fast, indexed seeking within files
	  - Caching of decompressed blocks
	  - De-duplicated files
	  - Sparse files
	  - Extended attributes
	  - Files larger than 4GB

	Squashfuse is missing the following features:
	  - LZMA1 compression (deprecated)
	  - Support for SquashFS versions less than 4.0
	  - Multi-core decompression


	3c. Known bugs
	--------------
	- On 32-bit systems with a large inode cache, when mounting a large SquashFS
	  archive created with the "-no-exports" option, squashfuse_ll may use a large
	  amount of memory. This is due to a bug in the FUSE API, where ino_t is shrunk
	  to 32-bits.


	4. References
	=============

	4a. Licensing
	-------------
	Squashfuse is copyright (c) 2012-2014 Dave Vasilevsky <dave@vasilevsky.ca>
	Squashfuse is distributed under the 2-clause BSD license. See the file LICENSE
	for details.


	4b. Acknowledgements
	--------------------
	Thanks to:

	* Phillip Lougher, for designing the SquashFS format, and implementing support
	  in the kernel. Also for providing permission to use and distribute
	  squashfs_fs.h under a BSD-style license.

	* Maël Kerbiriou, for implementing LZ4 support.


	4c. Links
	---------
	* SquashFS
	  - SquashFS home page, includeing squashfs-tools:
		http://squashfs.sourceforge.net/
	  - squashfs-tools for non-Linux: https://github.com/vasi/squashfs-tools

	* FUSE
	  - FUSE home page: http://fuse.sourceforge.net/
	  - OSXFUSE (FUSE for Macs): http://osxfuse.github.io/

	* Other implementations of the SquashFS format
	  - Linux kernel: https://github.com/torvalds/linux/tree/master/fs/squashfs
	  - 7-zip / p7zip: http://www.7-zip.org/
	  - GRUB 2 bootloader: http://www.gnu.org/software/grub/
