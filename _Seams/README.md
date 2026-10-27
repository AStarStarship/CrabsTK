
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
1. Copy the Crabs template files to the new folder.

## License

Copyright [AStarship™](https://astarship.net).
