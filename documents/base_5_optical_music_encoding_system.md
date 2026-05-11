# Base 5 Optical Music Encoding System

| Value | Signal | Color |
| --- | --- | --- |
| `0` | All High | White |
| `1` | High R | Red |
| `2` | High G | Green |
| `3` | High B | Blue |
| `4` | All Low | Black |

---

## Paper Strip Layout

### Index 0 - 11: Instrument Instructions

| Index | Field |
| --- | --- |
| 0 | **Instrument 1** Opcode |
| 1 - 2 | **Instrument 1** Immediate |
| 3 | **Instrument 2** Opcode |
| 4 - 5 | **Instrument 2** Immediate |
| 6 | **Instrument 3** Opcode |
| 7 - 8 | **Instrument 3** Immediate |
| 9 | **Instrument 4** Opcode |
| 10 - 11 | **Instrument 4** Immediate |

### Index 12 - 14: System

| Index | Field |
| --- | --- |
| 12 | **System** Opcode |
| 13 - 14 | **System** Immediate |

### Index 15: Checksum

| Index | Field |
| --- | --- |
| 15 | Checksum |

---

## Instrument Instruction Set

Format (Base 5): `[Opcode: 1 digit] [Immediate: 2 digits]`

Special: `0` `0` `0`: No-op

| Name | Opcode | Description |
| --- | --- | --- |
| Play Note | 0 - 2 | `Play((Opcode * 25) + toDecimal(Immediate) + OFFSET)` |
| Set Instrument | 3 - 4 | `Instrument = ((Opcode - 3) * 25) + toDecimal(Immediate)` |

---

## System Instruction Set

Format (Base 5): `[Opcode: 1 digit] [Immediate: 2 digits]`

Behavior: Latched. State persists until new command overwrites it.

Special: `0` `0` `0`: No-op

| Name | Opcode | Description |
| --- | --- | --- |
| Set Tempo* | 0 - 1 | `Tempo = ((Opcode * 25) + toDecimal(Immediate) + 1) * 2` |
| Set Volume** | 2 - 3 |`Target = Immediate Digit 1`, `Volume = (((Opcode - 2) * 5) + Immediate Digit 2 + 1) * 10` |
| Set Articulation | 4 | `Target = Immediate Digit 1`, `State = Immediate Digit 2` |

*\* Sets playback speed to (Tempo)% from 4% to 100% in 2% increments.*

*\*\* Sets target volume to (Volume)% from 10% to 100% in 10% increments.*

### Target

| State | Name |
| :--- | :--- |
| `0` | **All** |
| `1` | **Instrument 1** |
| `2` | **Instrument 2** |
| `3` | **Instrument 3** |
| `4` | **Instrument 4** |

### Articulation States

| State | Name | Release Condition |
| :--- | :--- | :--- |
| `0` | **Staccato** | Release at 1/2 step |
| `1` | **Normal** | Release at 1 step |
| `2` | **Legato** | Release upon non-note instruction (`No-op` or `Set Instrument`) or `Play Note` command with a different pitch |
| `3` | **Sustain** | Release upon `Play Note` command with a different pitch |
| `4` | **Infinite** | Release when the identical note is triggered again |

*\* `Set Articulation` command immediately releases all active notes on the targeted channel(s) before the new (or same) state is applied.*

---

## Checksum Implementation

Format (Base 5): `[Checksum: 1 digit]`

### Calculation 

`Checksum = (5 - (sum(data_0_to_14) mod 5)) mod 5`

### Validation

`sum(data_0_to_15) mod 5 == 0`