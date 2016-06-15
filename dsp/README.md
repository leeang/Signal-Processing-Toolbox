##Ang's DSP code

### Files
- `header.h` defines all constants and global variables.
- `function.c` includes all functions.
- `model.h` contains speech models exported from MATLAB.
- `mel_filter.h` incorporates precomputed Mel-filter bank gain.
- other files can be found in the `/VisualDSP 5.1.2/Blackfin/ADSP-BF548 EZ-KIT Lite/Drivers/AudioCodec` folder under VisualDSP++ installation directory.
- `Audio_Loopback.c` is modified to implement word recognition.

### Functionality
- commenting `lms_pass(chunk_index);` and uncommenting `// lms(chunk_index);` enable *Adaptive Noise Cancellation*.

### Test
- Files under `test` folder are used to test the efficiency and accuracy of the DSP program for given inputs.
- `cmd_data/cmd01.h` to `cmd_data/cmd27.h` are some test inputs.
