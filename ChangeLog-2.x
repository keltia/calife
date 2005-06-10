2005-06-10  Ollivier Robert  <roberto@keltia.freenix.fr>

	* 2.8.6-p3
  
	* db.c: Force l'initialisation de plusieurs variables pointant sur de la
	mémoire allouée par malloc(3). Ca évite un segfault lors du free(3) sur
	Solaris et Gentoo (Denis Ducamp <Denis.Ducamp@groar.org>).
  
2004-10-31  Ollivier Robert  <roberto@keltia.freenix.fr>

	* 2.8.6-p2

	* calife.c,db.c,conf.h: Ajout de la syntaxe @group: dans
	calife.auth pour authentifier via l'appartenance au dit groupe.

	* calife.auth.5.in,README: mise à jour.

2004-03-01  Ollivier Robert  <roberto@keltia.freenix.fr>

	* 2.8.6-p1.

	* acconfig.h,config.h.in,configure.in: /etc/calife.out est
	optionnel et dépend de --enable-global-rc.
	
	* calife.c(main): changement de la manière avec laquelle calife
	détermine qui l'a lancé quand getlogin(3) plante. /etc/calife.out
	est maintenant optionnel.

	* configure.in: l'édition de liens est maintenant dynamique par
	défaut sous Linux pour éviter un segfault (interaction avec nss je
	suppose).

2004-02-27  Ollivier Robert  <roberto@keltia.freenix.fr>

	* 2.8.6.

	** SECURITY RELEASE **

	* db.c(verify_password): vérification de la valeur de retour de
	getpass(3) et utilisation de strncpy(3) pour recopier le mot de
	passe saisi, sur Linux le buffer géré par la glibc est corruptible
	et génère un segfault si le mot de passe précédemment entré est
	trop long.

	cf. <URL:http://www.securityfocus.com/archive/1/355510>

2004-01-23  Ollivier Robert  <roberto@keltia.freenix.fr>

	* 2.8.5-p3.

	* calife.c: utilisation du code de su.c pour identifier le login
	de l'utilisateur pour corriger les problèmes de getlogin sous
	Linux.

2004-01-08  Ollivier Robert  <roberto@keltia.freenix.fr>

	* 2.8.5-p2.

	* README: mise à jour des adresses de courrier électronique et des
	versions de Mac OS X (remarque d'Olivier Tharan).

2003-11-04  Ollivier Robert  <roberto@keltia.freenix.fr>
 
	* 2.8.5-p1

	* calife.c: synchro de la liste des signaux à masquer dans les
 	deux cas POSIX et !POSIX. Ajout de SIGTTOU à cette liste pour
 	éviter une « race condition » lors de l'exécution de
 	calife.out. Inspiré par la révision 1.70 de su.c/FreeBSD.
 
2003-10-04  Ollivier Robert  <roberto@keltia.freenix.fr>

	* 2.8.5.

	* configure.in: reconnaissance de MacOS X/Darwin.

	* README: mise à jour.

2003-07-28  Ollivier Robert  <roberto@caerdonn.eurocontrol.fr>

	* 2.8.4-p8.

	* calife.c,db.c,util.c: renommage de la variable "log" en
	"logfile" pour 	éviter des warnings de gcc 3.3 pour lequel "log"
	est maintenant une fonction "nuiltin". (Thomas Quinot via un patch
	incomplet venant de Debian #194874).

2003-05-01  Ollivier Robert  <roberto@keltia.freenix.fr>

	* 2.8.4-p7.

	* config.guess,config.sub: versions mises à jour.

2003-01-12  Ollivier Robert  <roberto@keltia.freenix.fr>

	* 2.8.4-p6.

	* config.sub: intégration support FreeBSD/IA-64 dans configure.

2002-10-14  Ollivier Robert  <roberto@caerdonn.eurocontrol.fr>

	* 2.8.4-p5.

	* util.c(die): passage du mauvais paramètre à va_end().
	(Laurent Wacrenier).

2002-08-08  Ollivier Robert  <roberto@keltia.freenix.fr>

	* 2.8.4-p4.
	
	* calife.c(main): modification de la liste de caractères autorisés
	dans un nom de login (Erwan David).

	* conf.h: passage de MAXLOGNAME à 16 (Erwan David).

2002-08-02  Ollivier Robert  <roberto@keltia.freenix.fr>

	* db.c: Fix pour les éditeurs ne mettant pas un \n à la dernière
	ligne (Erwan David)

2002-01-21  Ollivier Robert  <roberto@caerdonn.eurocontrol.fr>

	* config.h.in,configure.in: sous Debian/IA64, crypt(3) n'est
	défini que si _XOPEN_SOURCE l'est (Thomas Quinot).

2001-12-10  Ollivier Robert  <roberto@caerdonn.eurocontrol.fr>

	* calife.c,db.c,conf.h: modification du passage root/non-root en
	utilisant deux nouvelles macros pour simplifier le code.

2001-10-09  Ollivier Robert  <roberto@keltia.freenix.fr>

	* 2.8.4-p3.

	* calife.c: Fix pour la compilation sur Hurd. Simplification du
	traitement du tty (Thomas Quinot). Fix pour le traitement de utmp
	sur SunOS et Linux.

	* configure.in,configure,acconfig.h,conf.h: ajout de la recherche
	de libgen.h et HAVE_ETC_SHADOW. Regénération de configure.

	* Fix pour Linux/SunOS dans conf.h Trouvé par T. Quinot.
	
2000-11-14  Ollivier Robert  <roberto@keltia.freenix.fr>

	* 2.8.4-p2.

	* calife.c: sort si stdin n'est pas un tty (Laurent Wacrenier - lwa).

	* README: mise à jour des adresses mail et ajout de lwa.

1999-08-20  Ollivier Robert  <roberto@keltia.freenix.fr>

	* configure.in: fixe l'erreur de syntaxe pour test(1).

1999-08-19  Ollivier Robert  <roberto@keltia.freenix.fr>

	* configure.in: corrige la manière de reconnaitre l'édition de
	liens dynamique de l'exécutable. Solaris & HP-UX ne supporte pas
	la compilation statique (Denis Ducamp).
	

1999-08-17  Ollivier Robert  <roberto@keltia.freenix.fr>

	* 2.8.4-RELEASE

	* configure.in: Pas de compilation statique sur Solaris. Les
 	drapeaux ne sont plus forcés sur HP-UX, l'administrateur décide de
 	quels drapeaux a besoin le compilateur (Francois Berjon).

	* db.c: donne à crypt(3) le mot de passe complet plutôt que juste
	la "salt". Ca ne gène pas les crypt(3) basés sur le DES et aide
	beaucoup pour les autres formes comme celles d'OpenBSD et de Linux 
	(Denis Ducamp).

	* Makefile.bsd: n'a pas sa place puisque faire passer configure
	n'est pas une option.

1999-06-01  Ollivier Robert  <roberto@keltia.freenix.fr>

	* 2.8.3-RELEASE
	
	* README: mise à jour.

	* configure.in, configure: reconnaissance enfin correcte des
	shadow passwords des diverses versions de Linux.

1999-05-27  Ollivier Robert  <roberto@keltia.freenix.fr>

	* 2.8.2-RELEASE
	
	* calife.c: changement du copyright pour 1999. Correction bug
	d'allocation mémoire trouvé par Thomas Quinot
	<thomas@cuivre.fr.eu.org>.

	* config.guess,config.sub: mise à jour avec une version
 	récente. Propagation du patch pour reconnaitre les Sparcbook S2
 	(config.guess).

1999-05-04  Ollivier Robert  <roberto@keltia.freenix.fr>

	* configure.in, Makefile.in: tente de trouver le bon groupe qui
	sera utilisé lors de l'installation en fonction de l'OS. Calife
	est uniquement setuid et non pas set{uid,gid}.

1999-05-03  Ollivier Robert  <roberto@keltia.freenix.fr>

	* calife.c: Corrige la vérification des droits du script exécuté à 
	la fin d'une session.

	* db.c, conf.h: Support amélioré de Linux et de ses shadow
	passwords. Par Thomas Quinot <thomas@cuivre.fr.eu.org>.

Fri Feb 14 19:06:08 1997  Ollivier Robert  <roberto@keltia.freenix.fr>

	* 2.8.1-RELEASE
	
	* configure.in: Ajout d'une nouvelle option (--disable-shadow)
 	pour permettre de ne pas utiliser getspnam(3) même s'il existe
 	sur des systèmes Linux.

	* acconfig.h: ajout d'une entrée pour UNUSED_SHADOW.
	* db.c: prise en compte de UNUSED_SHADOW.

Sat Oct 26 15:16:12 1996  Ollivier Robert  <roberto@keltia.freenix.fr>

	* 2.8-RELEASE
	
	* calife.c: Support de "-" comme premier paramètre pour faire
	relire les fichiers d'init (.login,.profile)
	* Meilleur contrôle sur le fichier out_rc.
	* Correction bug si argv[1] == '-'.

Sun Oct 29 23:01:28 1995  Ollivier Robert  <roberto@keltia.freenix.fr>

	* 2.8.beta4
	
        * configure.in: Ajout d'une nouvelle option (--with-program=NAME)
        pour changer le nom du binaire à l'installation (le défaut revient
        à "calife").
        
        * Makefile.in: changement de $(PROGRAM) en $(IPROGRAM) la où il
        faut pour la modif sus-nommée.
        
        * calife.c(main), db.c(open_databases): déplacement de l'appel à
        openlog(3) avant le test de l'existence de l'utilisateur afin
        d'avoir le nom correct dans le log.

Sat Oct 28 15:00:18 1995  Ollivier Robert  <roberto@keltia.freenix.fr>

        * 2.8.beta2
        
        * configure.in : Ajout de deux nouvelles options pour GCC
        (--enable-warnings et --enable-pedantic) et changement de syntaxe
        pour une autre (--disable-password). Recherche de quelques entêtes
        en plus.

        * calife.c(exec_shell): Correction d'un bug dans l'insertion de
        '-' pour le nom du shell.

        * db.c(verify_auth_info): correction de la reconnaissance du shell
        dans calife.auth.

Sun Oct 22 21:48:19 1995  Ollivier Robert  <roberto@keltia.freenix.fr>

        * 2.8.beta
        
        * calife.c: correction d'un bug stupide de "==" à la place de
        "=". Trouvé par Nat.

        * db.c: réécriture de la lecture de calife.auth pour la
        reconnaissance d'un shell possible.  On évite dorénavant de
        modifier un pointeur retourné par malloc(3) -- trouvé grâce à
        Philippe et au malloc de Poul-Henning Kamp dans FreeBSD 2.2. 

        * README: corrections mineures par Nat. Ajout de Philippe comme
        alpha-testeur.

Sat Oct 21 11:30:53 1995 Ollivier Robert <roberto@keltia.freenix.fr>

        * alpha5-2.8
        
        * conf.h: définition de ADMIN_LOG si syslog(3) n'existe pas. 

        * db.c: utilisation de getspnam  uniquement si HAVE_SHADOW_H et si
        HAVE_GETSPNAM. Je pensais  que HAVE_SHADOW_H =>  HAVE_GETSPNAM, ce
        qui n'est pas vrai  de certains Linux  (ils se  reconnaîtront) qui
        ont un shadow.h pour faire joli.

        * Makefile.in: correction de l'utilisation de calife à la place de
        $PROGRAM.

Fri Oct 20 23:31:33 1995 Ollivier Robert <roberto@keltia.freenix.fr>

        * alpha4-2.8
        
        * configure.in,conf.h,acconfig.h:  test de NeXTStep qui à l'entête
        <sys/wait.h> mais qui  utilise "union wait". Potentiellement aussi
        sur d'autres vieilles architectures. 

        * Compilation sur un IRIX.

Wed Oct 18 01:13:06 1995 Ollivier Robert <roberto@keltia.freenix.fr>

        * alpha3-2.8
        
        * README: modifications mineures de Nat. 

        * Makefile.in: corrections  des petites bêtises, changement de nom
        pour calife dont le binaire continuera à s'appeller "...". 

        * accconfig.h: nouveau  fichier   qui regroupe les   variables  et
        commentaires qui ne sont pas gérés par des macros internes GNU. Ca
        permet de générer config.h.in par autoheader. 

        * conf.h: WANT_SYS_WAIT ne sert à rien pour SunOS.

        * configure.in: test de la validité  de set/getrlimit en cherchant
        le symbole qui nous intéresse (HP-UX...). 

        * Compilation sans problème sur un AIX 3.2.5 ! 
        * Compilation sans problème sur un Linux avec -lshadow.

Sun Oct 15 19:46:04 1995 Ollivier Robert <roberto@keltia.freenix.fr>

        * alpha-2.8
        
        * calife.c,db.c,util.c: changement de quelques symboles à cause de
        l'utilisation de GNU autoconf.  On peut faire relire les  fichiers
        .login/.profile en précisant "-" avant le nom d'utilisateur. 

        * conf.h: presque toutes les définitions disparaissent au profit
        du nouveau fichier config.h.in.
        
        * configure,configure.in,config.sub,config.guess,Makefile.in:
        nouveaux fichiers venant de GNU autoconf. Insertion des règles
        correspondantes dans Makefile.in (à compléter).
        
        * calife.1, calife.auth.5: sont remplacés par calife.1.in et
        calife.auth.5.in.
        
        * ADMIN_DB devient AUTH_CONFIG_FILE et CALIFE_OUT devient
        CALIFE_OUT_FILE.

        * Makefile.FreeBSD,Makefile.SunOS,Makefile.Solaris,Makefile.linux,
        Makefile.linux-spwd, Makefile.HPUX: disparaissent. Makefile.bsd
        reste pour FreeBSD si l'utilisateur le veut.
        
        * README: mise à jour.
        
Sat Oct 14 01:24:03 1995 Ollivier Robert <roberto@keltia.freenix.fr>

        * release 2.7.2
        
        * calife.c,db.c,util.c: rajout de contrôles supplémentaires,
        déplacement de code.

        * util.c: rajout de die() par Nat Makarévitch, éditée pour
        s'adapter à mon style et à stdarg à la place de varargs.

        * conf.h: globalisation de ADMIN_DB, ajout du prototype de die(). 

        * global: préparation à la 2.8 qui utilisera GNU configure.

Sat Sep 30 19:40:19 1995  Ollivier Robert  <roberto@keltia.freenix.fr>

        * calife.c: correction d'un bug introduit par les contrôles
        supplémentaires sur le nom d'utilisateur; ajout d'un '\0'. Remise
        en place des anciennes valeurs de rlimit avant l'exec final.

Sun Jul 23 23:55:24 1995  Ollivier Robert  <roberto@keltia.frmug.fr.net>

        * release 2.7.1
        
        *  db.c: support  de la  fonction  crypt(3) utilisant  MD5 sur les
        versions internationales de FreeBSD 2.x.

Mon Apr 24 03:17:00 1995  Ollivier Robert  <roberto@keltia.frmug.fr.net>

        * release 2.7
        
        * calife.c,db.c: réintroduction des listes de logins vers lesquels
        un utilisateur de calife a le droit d'utiliser calife.

        * README: mise à jour.

        * calife.1,calife.auth.5: mise à jour pour les listes.

Sun Mar 12 22:09:36 1995  Ollivier Robert  <roberto@keltia.frmug.fr.net>

        * release 2.6.6

        * conf.h,util.c: linux a déjà une fonction basename.

        * README: mise à jour par Nathanaël Makarevitch.

        * calife.1.txt,calife.auth.5.txt: versions texte des pages de
        manuel.

Sat Jan 28 00:36:51 1995  Ollivier Robert  <roberto@keltia.frmug.fr.net>

        * Makefile.Solaris: nouveau fichier pour Solaris 2.x (testé avec
        la 2.4).

        * conf.h: standardisation de ADMIN_DB à /etc/calife.auth.

        * README: mise à jour.

Wed Jan 11 01:32:48 1995  Ollivier Robert  <roberto@keltia.frmug.fr.net>

        * release 2.6.5

        * calife.c,db.c,util.c,conf.h: elimination du support YP qui n'est
        pas utilise et complique le code. Changement pour le script qui
        ext execute a la sortie du programme, il devient global et
        modifiable a la coompilation en changeant le symbole CALIFE_OUT.

        * calife.1: mention de /etc/calife.out.

        * README: mise à jour.

        * Makefile.*: nettoyage et ajout du symbole CALIFE_OUT.

Fri Jan  6 01:52:45 1995  Ollivier Robert  <roberto@keltia.frmug.fr.net>

        * release 2.6.4

        * conf.h: modifications pour HPUX et ajout d'une entrée pour
        FreeBSD (2.x).

        * calife.c,db.c: utilisation de setresuid(2) sur HPUX à la place
        de seteuid(2) qui n'existe pas.

        * README: mise à jour.

        * Makefile.HPUX: nouveau fichier.

Wed Sep 28 20:27:57 1994  Ollivier Robert  <roberto@keltia.frmug.fr.net>

        * release 2.6.3

        * db.c: incorporation des changements de Laurent Chemla
        (laurent@brasil.frmug.fr.net) pour les systèmes Linux utilisant
        des "shadow passwords" comme SVR4 (struct spwd and al.).

        * Makefile.linux,Makefile.linux-spwd: nouveaux Makefile pour Linux
        avec et sans mots de passe explicites.

        * README: nouveau fichier.

        * Makefile.SunOS: ajout d'un message à propos des pages de manuel.

Mon Sep 26 00:20:07 1994  Ollivier Robert  <roberto@keltia.frmug.fr.net>

        * release 2.6.2

        * conf.h,calife.c: Modification du switch NO_UGLY_QUIRK en
        NOSETUID_SHELL (logique inverse) et enlèvement de ce switch pour
        Linux qui n'en a pas besoin.

        * release 2.6.1

        * calife.c: modification pour wait(2). Changement du #ifdef AIX3
        en !defined(AIX3) && !defined(HAVE_INT_WAIT) pour les système sans
        union. Changement dans les tests pour .calife.out (le bug était
        qu'il protestait s'il ne trouvait pas de .calife.out).

        * conf.h: changement pour SunOS 4.*, plus de YP, demande
        <sys/time.h> si pas de YP d'où le symbole WANT_SYS_TIME.

        * Makefile.SunOS: changement dans les options DEBUG.

Sun Sep 25 20:49:20 1994  Ollivier Robert  <roberto@keltia.frmug.fr.net>

        * release 2.6

        * Makefile.bsd: modification de DESTDIR en BINDIR et ajout d'une
        règle beforeinstall.

        * calife.c: ajout du support pour un fichier exécuté à la fin de
        la session appelé .calife.out dans le catalogue de
        l'utilisateur. Le fichier pourra éventuellement être général.

        * conf.h: ajout du nom du fichier exécuté à la fin : .calife.out.

        * Makefile, Makefile.SunOS, Makefile.bsd: modification pour
        inclure les pages de manuel pour calife(1) et calife.auth(5).

        * calife.1, calife.auth.5: nouveaux fichiers.

        * conf.h: changement pour SunOS qui n'a pas besoin de
        NO_UGLY_QUIRK et ajout du paramètre HAVE_INT_WAIT pour les
        systèmes n'ayant pas ``union wait''.

        * calife.c: ajout des #ifdef pour HAVE_INT_WAIT.

        * version 2.5.2: la 2.5.1 incluait les changements pour SunOS mais
        pas les manuels.

        * Passage à CVS au lieu de RCS ce qui fait que les numéros de
        version n'ont plus rien à voir avec la "release" de calife.

Mon Feb  7 23:00:11 1994  Ollivier Robert  (roberto@keltia.frmug.fr.net)

        * conf.h: modifications pour Linux par Rene pour NO_UGLY_QUIRK qui
        n'est pas necessaire et initgroups(2) qui existe. Les YP ne sont
        pas mises par defaut pour FreeBSD vu que la 1.1 n'est pas
        officielle. 

        * Makefile: rajout de l'option -lshadow pour Linux.

        * conf.h: 1.7.1.1

Sun Feb  6 14:19:51 1994  Ollivier Robert  (roberto@keltia.frmug.fr.net)

        * calife.c: FreeBSD a la fonction initgroups(2), FreeBSD 1.1 n'a
        pas besoin du hack violent pour tcsh, correction bug dans lequel
        wanted_user est ecrase par l'appel a getpwname(calife), Changement
        de _POSIX_SOURCE en POSIX_SIGNAL.

        * conf.h: FreeBSD 1.1 dispose des YP !!

        * calife.c: 2.4
        * conf.h: 1.7
        * db.c: 1.3
        * util.c: 1.2

        * Ajout de Makefile.bsd a utiliser avec le make BSD.

Wed Nov 10 14:22:59 1993  Ollivier Robert  (roberto@keltia.frmug.fr.net)

        * calife.c,db.c: modification pour les logs ; si syslog(3) est
        utilisable alors on l'utilise sans ouvrir de fichier log
        supplementaire.

Fri Sep  3 16:33:20 1993  Ollivier Robert  (roberto@keltia.frmug.fr.net)

        * Separation en trois fichiers sources: calife.c, db.c et util.c
        pour simplifier calife.c.

        * conf.h: Definition des variables globales utilisee par les
        fonctions. Ajout de #define MAIN_MODULE dans calife.c et test dans
        conf.h. 

        * calife.c: 2.3
        * conf.h: 1.6
        * db.c, util.c: 1.1

        * Makefile: reecriture pour mise a jour des fichiers cibles.
        Disparition de calife.debug, devient calife mais compile et
        configure sur place avec les symboles.

Thu Sep  2 19:45:21 1993  Ollivier Robert  (roberto@keltia.frmug.fr.net)

        * calife.c: Refonte interne du code, simplification, changement
        des noms des fonctions pour etre consistent, renvoie de valeurs au
        lieu d'utiliser la variable globale 'allowed' qui disparait.
        (OpenDatabases, VerifyAuthorisations, VerifyPassword): deviennent
        open_databases, verify_auth_info et verify_password.
        (exec_shell): creation.

        * conf.h: mise a jour des fonctions et de leur prototype.

        * calife.c: 2.2
        * conf.h: 1.5

Sun Aug 15 22:27:22 1993  Ollivier Robert  (roberto@keltia.frmug.fr.net)

        * calife.c (main): when we call the shell in order to run the
        shell (NO_UGLY_QUIRK), then we do an exec(2) instead of just
        running the shell.

Wed Jun 23 23:54:27 1993  Ollivier Robert  (roberto@keltia)

		* conf.h (linux): ajout des modifs de René pour le portage Linux.

Mon Jun 21 23:26:44 1993  Ollivier Robert  (roberto@keltia)

		* portage linux par René...

Fri Jun 11 23:13:25 1993  Ollivier Robert  (roberto@keltia)

		* Makefile (386BSD): change CFLAGS in order to include -O2 and
		-m486 on my machine.

		* calife.c (main), if _POSIX_SOURCE is defined then use Posix
		1003.1 signal handling functions i.e. setprocmask(2), sigaddset(2)
		to handle ignored signals.

		* calife.c (main, OpenDatabases, VerifyPassword): syslog(3)
		message format changed in order to stay within 80 cols limit.

		* calife.c (main): change test of user_to_be in order to verify if
		he/she exists before even use fork(2).

$Id: //depot/security/calife/main/ChangeLog#44 $
