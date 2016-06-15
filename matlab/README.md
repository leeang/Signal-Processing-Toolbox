##Ang's MATLAB code

### .m files
- `detection.m` is the main process to recognize a speech word.
- `export_bank_gain.m` exports the Mel filter bank gain into `.c` file
- `export_model.m` exports the speech model into `.c` file

### data files
+ the default file name for model is `Model.mat`.
+ speech data files are in `data` folder.

### folders
+ `function` folder includes all the functions required by the main file (`detection.m`). This folder is automatically added to the current path.
+ `plot` folder archives the programs to plot the figures inserted in the final report.
