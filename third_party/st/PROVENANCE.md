# Provenance of the ST-derived sources

Everything under `third_party/st/` is STMicroelectronics' RFAL (RF
Abstraction Layer) **v3.0.1**, from ST's STSW-ST25RFAL002 package
("RFAL library for ST25R3916"), redistributed under ST's SLA0052 licence
terms (full text: [`SLA0052.txt`](SLA0052.txt)) with all ST copyright
notices retained. See the repository README for what those terms mean in
practice.

## How these exact files were obtained and verified

ST gates the STSW-ST25RFAL002 download behind an st.com account, so the
files here were taken from a public vendoring and **cross-verified against a
second, independent vendoring** before being committed:

- **Base copy**: the Trezor firmware tree
  (`trezor-firmware`, `core/embed/io/nfc/rfal/`), which documents its import
  as STSW-ST25RFAL002 v3.0.1 from st.com and documents exactly one local
  change (a `DSI` → `DSI_ID` identifier rename to dodge an STM32 HAL macro
  collision).
- **That one change was reverted here** (`DSI_ID` → `DSI` in
  `rfal_isoDep.[ch]` / `rfal_nfcDep.[ch]`), restoring ST's original
  identifier.
- **Cross-check**: after the revert, every file was diffed (whitespace
  ignored) against the independent vendoring in
  `github.com/Mani201/ST25R3916_Lib` (also RFAL v3.0.1). All files match,
  except files that that repository had locally modified itself
  (`rfal_nfcv.c`, `st25r3916_com.c` — where the Trezor copy, not the
  modified one, is carried here).

Two independent imports agreeing byte-for-byte (modulo line endings) is the
strongest provenance available without the gated ST download. If you have an
st.com account, `diff -r` against a fresh STSW-ST25RFAL002 v3.0.1 download
is the definitive check — and if you find a discrepancy, please open an
issue.

`doc/` carries ST's own release notes and MISRA/CERT compliance reports as
shipped (the `rfal.chm` compiled-help binary was not carried over).

## Version note

RFAL v3.0.1 is the newest version verifiable this way. ST's X-CUBE-NFC6
line and later STSW-ST25R-LIB releases may be newer; the v3.0.0 → v3.0.1
delta observed during sourcing was confined to power-down/deinit behaviour
in `st25r3916.c`.

## Local modifications policy

None, beyond the DSI revert described above (which restores ST's original
text). Platform adaptation happens exclusively in `src/rfal_platform.h` /
`src/st25r3916_platform.c` (Apache-2.0, outside this directory). If a
genuine bug fix to ST code ever becomes necessary, it must be recorded in
this file.
