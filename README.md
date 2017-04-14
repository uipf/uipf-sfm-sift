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
    
License
-------

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
See the [GNU General Public License](LICENSE.md) for more details.

> **NOTE THAT THIS PROGRAM USES OTHER CODE THAT IS PROTECTED BY PATENT!**
>
> You may not be allowed to use it without explicit permission!
> Please refer to the following website for license details and contact information:
> <http://www.cs.ubc.ca/~lowe/keypoints/>

TODO
----

Reimplement this based on one of the following:

- <https://robwhess.github.io/opensift/>.
- <http://vision.ucla.edu/~vedaldi/code/siftpp.html>.
