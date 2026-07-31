# zephyr-st25r3916

> ## ⚠️ IN DEVELOPMENT — NOT YET WORKING
>
> **This module compiles. It has never driven real silicon.** No register
> transaction, no interrupt, no RF field, no card read has ever been
> observed from this code. The first hardware milestone (the `identity`
> sample answering over SPI) has not been reached yet. Version:
> **0.0.1-wip**. Do not build a product on this today.

A [Zephyr RTOS](https://zephyrproject.org) module providing a driver for the
STMicroelectronics **ST25R3916 / ST25R3916B** high-performance NFC universal
device (reader/writer IC), built by porting ST's **RFAL** (RF Abstraction
Layer, v3.0.1) onto Zephyr's SPI, GPIO, kernel and logging APIs.

Developed for the [OffGridGate](https://offgridgate.com) satellite-connected
gate lock project (nRF54L15 host, ElecHouse ST25R3916B Mini reader module),
but structured as an independent, reusable Zephyr module.

## Honest status

| Piece | State |
|---|---|
| ST RFAL v3.0.1 core + ST25R3916/B chip layer, vendored verbatim | compiles clean (no warnings) under Zephyr 4.4.99 / SDK 1.0.1, `nrf54l15dk/nrf54l15/cpuapp` |
| Zephyr platform shim (SPI, manual CS, IRQ bottom-half work queue, locking, timers, logging) | compiles; **logic reviewed but never executed against hardware** |
| `samples/identity` (read + verify the IC identity register, then full RFAL init) | builds for the nRF54L15 DK in all three configurations (3916, 3916B, 3916B + ElecHouse analog table) |
| Devicetree binding `st,st25r3916` | in place |
| Any RF operation (field on, polling, ISO14443, ISO-DEP) | **not attempted anywhere** |
| Card emulation / listen mode, P2P, wake-up mode, DPO, low-power mode | compiled out or untested; see `src/rfal_platform.h` feature table |
| Power management (PM_DEVICE), shutdown/resume | not implemented |
| Automated tests | none yet (first hardware contact comes first) |

## Layout

```
CMakeLists.txt, Kconfig, zephyr/module.yml   Zephyr module plumbing
dts/bindings/nfc/st,st25r3916.yaml           devicetree binding
include/st25r3916_zephyr.h                   public init entry point
src/                                         the platform shim (Apache-2.0)
  rfal_platform.h                            RFAL's platform contract → Zephyr
  st25r3916_platform.c                       SPI/CS/IRQ/locks/timers/log
third_party/st/                              ST's RFAL v3.0.1 (SLA0052, unmodified)
  rfal/include, rfal/source                  RFAL core
  rfal/source/st25r3916                      ST25R3916/B chip layer
  PROVENANCE.md, SLA0052.txt                 where these files came from, and their licence
third_party/elechouse/                       ElecHouse ST25R3916B Mini antenna-tuning
                                             table (ST-derived), Kconfig-selectable
samples/identity/                            first-light test: who is on the SPI bus?
```

## Licensing — read this before reusing

This repository deliberately contains **two licence regimes**:

- **`src/`, `include/`, `samples/`, the build files** — original work,
  licensed **Apache-2.0** (see `LICENSE`).
- **`third_party/st/` and `third_party/elechouse/`** — STMicroelectronics'
  RFAL and ST-derived configuration data. These are **NOT Apache-2.0** and
  never will be. They are redistributed under ST's **SLA0052** licence
  (full text in `third_party/st/SLA0052.txt`), which permits source
  redistribution with notices retained, and which imposes two conditions
  that matter to you:
  1. **The software may only be used and executed in combination with an
     ST-manufactured NFC IC** (SLA0052 §4). That condition attaches to the
     NFC chip, not the host MCU — running this on a Nordic, NXP or any
     other MCU is fine, because the IC this driver exists to drive (the
     ST25R3916/B) is an ST-manufactured NFC reader IC. Using ST's RFAL
     sources to drive a non-ST NFC chip would not be.
  2. **The ST sources must not be placed under open-source terms**
     (SLA0052 §5). Do not "relicense" this repository wholesale, do not
     copy ST files into Apache/MIT/GPL projects as if they were yours.
     The Apache-2.0 licence above applies **only** to the directories
     listed against it.

Every ST file retains its original ST copyright header. Provenance of the
vendored sources — including how they were cross-verified against a second
independent copy — is documented in `third_party/st/PROVENANCE.md`. This
project is not affiliated with or endorsed by STMicroelectronics.

## Using the module

### Build the identity sample (nRF54L15 DK)

```sh
source ~/zephyrproject/.venv/bin/activate
cd samples/identity
ZEPHYR_BASE=~/zephyrproject/zephyr west build -b nrf54l15dk/nrf54l15/cpuapp .
west flash            # or: probe-rs run / flash per your setup
```

Console is RTT (`probe-rs attach --chip nRF54L15 build/zephyr/zephyr.elf`).
The sample loops until a chip answers: stage 1 reads the IC identity
register raw over SPI (accepts **both** ST25R3916 and ST25R3916B and prints
which one, with silicon revision); stage 2 runs full `rfalNfcInitialize()`,
whose oscillator-stable interrupt proves the IRQ line end-to-end. With
nothing wired it fails cleanly every 3 s — deliberately usable as a live
wiring debugger.

### In your own application

```cmake
list(APPEND ZEPHYR_EXTRA_MODULES /path/to/zephyr-st25r3916)
```

Devicetree (see `dts/bindings/nfc/st,st25r3916.yaml` for the full story):

```dts
&spi00 {
    status = "okay";
    cs-gpios = <&gpio2 10 GPIO_ACTIVE_LOW>;

    st25r3916@0 {
        compatible = "st,st25r3916";
        reg = <0>;
        spi-max-frequency = <4000000>;
        irq-gpios = <&gpio1 11 GPIO_ACTIVE_HIGH>;
        /* en-gpios = <...>;  optional supply gate */
    };
};
```

Then `CONFIG_ST25R3916=y`, pick the variant
(`CONFIG_ST25R3916_VARIANT_B=y` for a ST25R3916B — check the IC marking,
not the module silkscreen), call `st25r3916_zephyr_init()` once, and ST's
own APIs (`rfal_nfc.h` etc.) are available.

## Hardware notes (nRF54L15 + ElecHouse ST25R3916B Mini)

- **The IRQ pin must be on port P0 or P1.** The nRF54L15 has GPIOTE
  instances for P0 (`gpiote30`) and P1 (`gpiote20`) only; **port P2 cannot
  generate GPIO interrupts** — `gpio_pin_interrupt_configure()` on any P2
  pin fails with `-ENOTSUP` at run time (proven on real silicon; the
  failure is invisible at devicetree/compile time). SPI signals and a CS
  *output* on P2 are fine. The driver names this exact trap in its error
  log if you hit it.
- The sample's DK overlay (`samples/identity/boards/`) is **provisional,
  not a blessed pinout**: SPI on `spi00` (P2.01 SCK / P2.02 MOSI / P2.04
  MISO — the DK's default `spi00` pinctrl), CS on P2.10, IRQ on P1.11.
- ElecHouse ST25R3916B Mini connector order:
  **IRQ, CS, SCLK/SCL, MOSI, MISO/SDA, 5V, GND**. It wants a **5 V
  supply**; its logic lines are 3.3 V-compatible.
- The module's antenna tuning table from the ElecHouse Arduino library is
  carried here and selectable with
  `CONFIG_ST25R3916_ANALOG_CONFIG_ELECHOUSE=y` (analog config tables are
  board/antenna-specific; the default table is ST's generic one).
- SPI is mode 1 (CPOL=0, CPHA=1), max 10 MHz; the sample uses 4 MHz for
  flying-lead tolerance.

## Design notes

- **RFAL is a singleton** (its state is file-scope static inside ST's
  sources), so this module binds to exactly one `st,st25r3916` devicetree
  node — a second enabled node is a compile error, not a runtime surprise.
- **The IRQ is serviced in a dedicated work queue thread**, not ISR
  context: `st25r3916Isr()` reads interrupt registers over SPI. The GPIO
  ISR only submits work.
- **Chip select is driven manually** (as a GPIO, not by the SPI
  controller) because ST's comm layer holds CS across logically grouped
  transfers. `ST25R_COM_SINGLETXRX` is enabled, so each CS window carries
  exactly one full-duplex Zephyr `spi_transceive()`.
- ST sources are compiled **verbatim** — no patches. All platform
  adaptation lives in `src/rfal_platform.h`.

## Roadmap (deliberately short)

1. **First light**: run `samples/identity` against the wired ElecHouse
   module — stage 1 (SPI) then stage 2 (IRQ). Nothing else matters until
   this passes.
2. NFC-A polling + ISO-DEP APDU exchange sample (DESFire `GetVersion`).
3. Wake-up (low-power card detection) mode for battery operation.
4. Trim the compiled-in feature set (Kconfig per technology).
