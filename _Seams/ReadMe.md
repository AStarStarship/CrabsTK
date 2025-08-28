This folder contains the software seams for the [Sloth](https://github.com/KabukiStarship/kabuki.press.cookbook), a source code managment tool built from Kabuki Toolkit. Kabuki Toolkit is based on [Simple and Fast Media Layer (SFML)](sfml-dev.org), but we only use ASCII C++ Software and we're going to optmize the crap out of SFML. The goal is to be able to download SFML from GitHub using the system command, then parse the directory structure, strip out all of the comments, remove the extra whitespace from the license and put it back, and change the file names from UpperSakeCase to lower_snake_case.

## Algorism

1. Specify the new packed header to be used and the git repo address.
1. Download the git repo.
   1. Check if the repo was downloaded.
1. For each folder in the repository:
   1. Convert the foldername to lower_snake_case and create a new project folder for the new files, and lets call this folder Foo.
   1. For each file in the directory structure:
      1. Convert the filename to lower_snake_case.
	    1. Open a new file for writing in the new directory structure.
      1. Write the header to the new file.
			1. Scan each byte of the file, stripping out the comments, and converting filenames to lower_snake_case.
1. Copy the Script2 template files to the new folder.

## License

Copyright [Kabuki Starship](https://kabukistarship.com); all rights reserved.

This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not distributed with this file, You can obtain one at <https://mozilla.org/MPL/2.0/>.
