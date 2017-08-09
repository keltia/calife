
# Introduction

Calife is a small utility which enable some selected users (present in a configuration file) to become another user, traditionally but not limited to "root" by entering their own password.

The primary goal is to avoid sharing a common password (à la su(1)) while enabling these users to use the power of the superuser.

Entering a password is mandatory as to avoid the security risk associated with leaving an unattended terminal (like sudo(1) has because it uses a timeout-based time window).

It pushes the security boundary down to the users' passwords but we assumes they have been trained for the various administrative tasks they are allowed to achieve and the security risks associated with this mechanism.

Going through the "root" superuser priviledges can be avoided by using the "list" feature as selected users will be able to become another one directly.

Finally, the "group" feature allows users to be put in a specific group in order to avoid listing all of them in the configuration file.

# Distribution

For historical reasons, Calife is under the GNU General Public License which lists right to distribute, copy, modify and use. Modifications to Calife should be send back to me for integration. This way, every modification can be traced back to needs and consistency is maintained.

See the `COPYING` file in the distribution.

The file `compiler.h` is under a modified 3-clause BSD license & (c) Bertrand Petit.

There is no other conditions but if you use it, please tell me about it and I would never refuse a Guinness if we meet one day :-)

Primary web/distribution site for Calife is [here](https://www.keltia.net/programs/calife/).

FTP access is also provided by the following sites:

[main FTP](ftp://mutt.frmug.org/pub/calife/)
[mirror](ftp://postfix.EU.org/pub/calife/)

Calife is also available both as [Mercurial](http://selenic.com/mercurial/) and [Git](http://git-scm.org/) repositories to make following the development an easy task.

[Bitbucket repository](https://bitbucket.org/keltia/calife/)
[Github repository](https://github.com/keltia/calife/)

# Installation

## Autoconf

Calife uses the well-known GNU `autoconf` system for configuration. GNU cc is not mandatory for compiling but Calife has only been tested with the following compilers:

- GNU cc 4.x and later
- Clang 3.x and later
- SUN cc on Sparc systems 

Use the following command to analyse your system and tailor the build process.

    ./configure

Several files will be created

- `Makefile`
- `config.h`
- `calife.1`
- `calife.auth.5`

The main calife binary and the manpages will be installed by default by using `make install`. The default prefix is the usual `/usr/local`.  Use `--prefix` to change the base location.

You can also specify some special options to configure in order to get specific behaviours.

    --with-etcdir=PATH        Directory containing calife.auth (default /etc)
    --disable-shadow=yes|no   Disable getspnam(3) usage for some Linuxes
    --enable-debug=yes|no     Set the debugging mode (default is no)
    --enable-shellhack=yes|no (default is no)
    --enable-dynamic          Build the program dynamically (default static)
    --enable-global-rc        Add /etc/calife.out support.

`--with-etcdir=DIR`

Specify the directory where calife will look for `calife.auth` and `calife.out`. Default is `/etc`. 

`--enable-debug`

Enable more debug messages.

`--enable-shellhack`

Some shells do not like being called by a setuid program.  If you get a "Permission denied" message, try this option.

`--enable-dynamic`

On most systems, calife is statically built to avoid dependencies.  If you plan to use PAM, you should use this option.  On several  systems, this option is the default (Solaris 2.x, HP-UX, Linux and MacOS X/Darwin).

`--enable-global-rc`

Add `/etc/calife.out` support.  This script will be run upon exit.  It could be used to perform certain tasks like sending a mail, checking changes and so forth.

## Linux special case:

Over the years, Linux has been using several different password management schemes. It is possible that configure will not be able to find out which variant is used on your system. In that case, patches are very welcome.

The following option forces calife not to use the `getspnam(3)` function even though the function itself is present in a library.

`--disable-shadow`

Disable `getspnam(3)` usage.

If you need to make any modification to the code or configure in order to make calife working, please contact me by email or file a issue on my website with any information needed. Patches are always welcomed.

The installed binary will be named `calife` by default although historically it was called `...`. A sample configuration file named `calife.auth-dist` will be copied by default in `/etc`.

Please consult the supplied manpages for more information.

## Compatibility

Calife has been developped and tested on the following UNIX systems (actual versions may differ but I can not guarantee full support all time as I may not have access to all of them).

- FreeBSD 1 - 12.x   (main development platform)
- MacOS X 10.2-10.12. (main development platform)
- Linux 0.99+  1.* (1.2.* et 1.3.*), 2.*
- Solaris 2.x (incl. most SVR4 systems)
- HP-UX 8.*, 9.*, 10.* et 11.*

Although I never tested calife on NetBSD and OpenBSD, I do not expect them to be different enough to matter except maybe for PAM matters. Patches are more than welcome. I have not checked whether calife has been put in their respective ports system. Again I would welcome any patches or news about it.

NOTE that version 3 is very much PAM oriented although it should work without it.

The following systems are not officially supported any more but may work (they used to)

- SunOS 4.1.*
- SunOS 3.2
- NeXTStep 3
- SCO 3.2
- HP-UX 8.* et 9.*
- SVR4 NeWSOS 5.02
- Encore Gould NP1
- AIX 3.x & 4.x
- IRIX 5.x (probably 6.x too)
- GNU Hurd

In the pam/ subdirectory, you can find some sample files for PAM configuration.  Choose the one suited to your system and pace it either inside `pam.conf` (Solaris up to 9) or within the `pam.d` directory under the `calife` name.

- `macosx`    For MacOS X 10.3/Panther and up to 10.5/Leopard
- `macosx10`  For MacOS X 10.6/Snow Leopard
- `freebsd5`  For FreeBSD 5.x up to 8.x.
- `freebsd9`  For FreeBSD 9.x and up.
- `solaris`   For Solaris 8, 9 et 10 (to be cut/pasted in pam.conf)
- `linux`

If you want to use LDAP/NIS or any other authentication system based on PAM, you will need to add to or modify the host PAM configuration. Any input on these matter would be appreciated.

The `syslog(3)` functions family is mandatory.

FreeBSD and Debian packages are available thanks to Christian Perrier and Thomas Quinot. Alain Thivillon also did some RPMs for previous versions.  Fedora/Red-Hat/SUSE help is welcome.

## Manpages

Manpages use the mandoc BSD macro package which is now more or less the default on many systems.

# History

Calife was created in 1991 from a common idea between François Berjon and I when we were system administrators. At that time, we wrote a simple wrapper that was checking uid and running a root shell, without asking for a password.

It evolved later when I was working at Antenne 2 (French TV channel) and during my term as an intern in the University of Marne-la-Vallée, where it gained most of its features. NIS support was added later for Laurent Chemla and Brainstorm.

If you want to send any comment, critics or patches, please contact me with the following addres or connect to the following website:

[adresse privée maison](mailto:roberto@keltia.net)<br/>
[Main site](http://www.keltia.net/programs/calife/)

# THANKS

René Cougnenc<br/>
François Berjon         <fcb@gwynedd.frmug.org><br/>
Laurent Chemla          <laurent@brainstorm.fr><br/>
Pierre Beyssac          <pb@fasterix.frmug.org><br/>
Nathanaël Makarévitch   <nat@nataa.frmug.org><br/>
Thomas Quinot           <thomas@cuivre.fr.eu.org><br/>
Christian Perrier       <bubulle@kheops.frmug.org><br/>
Philippe Regnault       <regnauld@starbsd.org><br/>
Thierry Besancon        <Thierry.Besancon@csi.uvsq.fr><br/>
Laurent Wacrenier       <lwa@teaser.fr><br/>
Erwan David             <erwan@rail.eu.org><br/>
Bertrand Petit          <elrond@phoe.frmug.org><br/>
Denis Ducamp            <Denis.Ducamp@hsc.fr><br/>

and all the unnamed ones that have participated to testing.

# $Id: README.md,v f91e6350b459 2014/11/10 10:49:34 roberto $
