## CPU Emulator & ARM Assembly Extension
### In order to run the ARM Assmbly extention on the Raspberry Pi do the following:
- Clone this repository
- cd into the folder
- run **./assemble gpio.s kernel.img** in order to generate a binary file called kernel.img using our assembler
- Replace the original Raspberry Pi kernel.img file with the one generated
- Power on the Raspberry Pi and you should see the green LED flashing and the sprite being rendered

Enjoy!

Here is what the Flappy Bird sprite looks like:

![alt tag](https://cloud.githubusercontent.com/assets/10284629/10392903/2a4ff760-6e83-11e5-935a-ee80bed3f12d.png)

## Authors
Henryk Hadass
- Email: henryk.hadass14@ic.ac.uk

Michael Radigan 
- Email: michael.radigan14@imperial.ac.uk

Mickey Li
- Email: mickey.li14@imperial.co.uk

Oliver Wheeler
- Email: oliver.wheeler@imperial.co.uk