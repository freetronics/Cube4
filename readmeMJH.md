steps to be taken to allow this example to work :
this file will be updated as I progress and recover the data from experience.

1. Download and install the Arduino IDE { we are now at version 1.8.5 }

2. when attempting to build errors will occur due to missing file Cube.h . To fix this we need the library

2.1 Download the ZIP from the freetronics github repo site (even on Linux) and unzip into "libraries" in the tree where the IDE was installed. 

3. restart the IDE - fine but the compiler enforces stricter rules now than when the example was created.

4. Note : I already had a fork of the repo in ECA.

5. So I removed the "unzipped" file from the repo as it has no version control and used the repo from the ECA user in github. I changed the files to make the compilation work and will now investigate further to merge them, when acceptable, into the mastaer...

6. There are still several warnings at compile time with the Cube code. I will attempt to fix them.


