# Random regression testing

- feed `"\r\e[" + random_csi_sequence + "m."` to libvterm
- dump output of `vterm_screen_get_cell` and `vterm_state_get_penattr`
- repeat 100000 times

CSI sequences:
- reset (0)
- bold (1 22)
- reverse (7 27)
- fg/bg (3x 4x 9x 10x)
- fg/bg with alternative palette (rgb / 256-color)

Usage:

    mkdir out
    ./generate.py > out/in.txt # 100000 CSI sequences, one per line
    ./go.sh /path/to/original/libvterm out/in.txt > out/1.txt
    ./go.sh /path/to/updated/libvterm  out/in.txt > out/2.txt
    cmp out/1.txt out/2.txt # should be the same
