# Rpi kernel module

***A kernel module for toggling an io on a Raspberry Pi.***

---

DONE

- toggle 2 gpio pins(20, 21)

---

## Usage

Wire up leds and button.

Commands for RPi.

```bash
# Install RPi kernel headers & clone repo.
sudo apt-get install raspberrypi-kernel-headers
git clone https://github.com/TParm/Rpi_Kernel_Mod.git

# Navigate to Rpi_Kernel_Mod folder.
cd Rpi_Kernel_Mod/

# Make & remove existing module if needed.
make
sudo rmmod gpiokmod.ko

# Insmod with args (choose toggleSpeed, ioPins... (for x)).
sudo insmod gpiokmod.ko toggleSpeed=x ioPins=x,x 

# Check kernel messages.
dmesg
```
