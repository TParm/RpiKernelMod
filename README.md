# Rpi kernel module

***A kernel module for toggling an io on a Raspberry Pi.***

---

DONE

- toggle 2 ios
- count edges

---

## Usage

Wire up leds and button.

Commands for RPi.

```bash
# Install RPi kernel headers & clone repo.
sudo apt-get install raspberrypi-kernel-headers
git clone 

# Navigate to leds_edges_kmod folder.
cd
cd 

# Make & remove existing module if needed.
make
sudo rmmod .ko

# Insmod with args (choose toggleSpeed, ioPins... (for x)).
sudo insmod .ko toggleSpeed=x ioPins=x,x 

# Check kernel messages.
dmesg
```
