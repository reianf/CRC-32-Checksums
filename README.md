These are assignments from my Systems Programming course.<br><br>

<b>prog1_1.c</b><br>
This program takes a single command line argument FILE, which will be a target elf file. It will print the FILE's CRC-32 checksum as an 8 digit hexidecimal number.
<br><br>
Example output:<br>
FA13E1C4
<br><br>
<b>prog1_2.c</b><br>
This program takes a single command line argument FILE, which will be a target elf file. It will print the FILE's CRC-32 checksum of only the program header table, not any other sections.<br><br>
Example output:<br>
0013AE34
<br><br>
<b>prog1_3.c</b><br>
This program takes two command line arguments, FILE and SECTION_NAME. The FILE will be a target elf file and the SECTION_NAME will be a string identifying which sections to print out CRC-32 checksums. If there are multiple sections that match the provided SECTION_NAME, the program will print out CRC-32 checksums for each of
them.<br><br>
Example output:<br>
5FCE304C<br>
6A84EFAB
