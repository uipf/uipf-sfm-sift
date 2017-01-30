SIFT module for UIPF SfM
========================


Install
-------


TBD


This module uses the binary file provided by [David Loewe](http://www.cs.ubc.ca/~lowe/keypoints/). This binary is compiled on a 32bit system (i.e. i386 architecture). If you want to run this binary on a 64bit system you may get the following error:

    bash: ./uipf-sfm-sift: No such file or directory

Even though the file exists, this error occurs because you are [missing multiarch support on your system](http://askubuntu.com/a/133460).

See the following ressources for enabling multiarch support:

- Debian: https://wiki.debian.org/Multiarch/HOWTO
- Ubuntu: https://help.ubuntu.com/community/MultiArch
    


TODO
----

Reimplement this based on <https://robwhess.github.io/opensift/>.
