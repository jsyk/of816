

cpu_clk   = 8000000

; *****************
; Memory regions
; X65 has 2MB of SRAM, which is divided in 32 of 64kB banks.
; *****************
; X65_ROM  = $080000     ; start of forth "ROM", put in bank $8, this is just the start address where to jump from romboot
X65_ROM  = $010000     ; start of forth "ROM", put in bank $01, this is just the start address where to jump from romboot
; Neon_VRAM = $400000
; X65_RAM1 = $010000      ; 64kB unused, then 64kB for dictionary.... hm...
X65_RAM1 = $020000      ; 64kB unused, then 64kB for dictionary.... hm...
; X65_RAM2 = $0A0000      ; what?

; *****************
; MMU
; *****************
; not yet in X65

; NeonMMU   = $080000

; *****************
; I/O Ports
; All I/O registers are in the bank 0.
; *****************

; CX16 legacy registers:
; ----------------------
; $0000           RAMBLOCK            [7:0]       This 8-bit register specifies which 8kB-BLOCK of the 2MB SRAM
;                                                 is mapped into the 8kB RAM-BLOCK FRAME visible at the CPU address $A000 to $BFFF. The contents 
;                                                 of this 8b register is bit-wise ANDed with the register RAMBMASK (at $9F50) and the result 
;                                                 is the SRAM RAM-block number.
;                                                 (As described in memory-map, the 2MB SRAM has 256 of 8kB BLOCKs).
;                                                 These RAM-Blocks are numbered in the 2MB SRAM starting in the MIDDLE and wrapping around: 
;                                                     RAM-Block #0   is from SRAM 0x10_0000 to 0x10_1FFF,
;                                                     RAM-Block #1   is from SRAM 0x10_2000 to 0x10_3FFF, etc.,
;                                                     RAM-Block #127 is from SRAM 0x1F_E000 to 0x1F_FFFF,
;                                                     RAM-Block #128 is from SRAM 0x00_0000 to 0x00_1FFF,
;                                                     RAM-Block #129 is from SRAM 0x00_2000 to 0x00_2FFF, etc.,
;                                                     RAM-Block #255 is from SRAM 0x0F_E000 to 0x0F_FFFF.
;                                                 Note 1: in CX16 parlance this register is called "RAMBANK", but the function is (basically) the same.
;                                                 Note 2: RAM-Blocks 128 to 132 are always mapped to the CPU "low-memory" addresses from $0000 to $9EFF. 
;                                                 (65816: from $00_0000 to $00_9EFF => in Bank 0.)
;                                                 RAM-Blocks 192 to 255 are also available as ROM-Blocks, see below.
;
; $0001           ROMBLOCK            
;                                     [7]         When set to 1, NORA's BootRom is displayed in the ROM-Block Frame at $C000, and bits [4:0] are ignored.
;                                                 When cleared to 0, bits [4:0] define the contents of ROM-Block Frame.
;                                                 BootRom is 512B and mirrored over the 16kB frame.
;
;                                     [6]         When set to 1 together with bit [7], then the next RTI instruction (Return From Interrupt)
;                                                 will automatically clear bits [7] and [6], thus switching the ROM-Block Frame according to bits [4:0].
;                                                 NMI is automatically blocked while bit [6] is set, and all other interrupts should be blocked in Sw.
;                                    
;                                     [5]         reserved, write 0
;                                    
;                                     [4:0]       This 5-bit register specifies which 16kB-BLOCK from the SRAM's 
;                                                 512kB area 0x08_0000 to 0x0F_FFFF is mapped into the 16kB ROM-BLOCK FRAME
;                                                 visible at the CPU address $C000 to $FFFF.
;                                                 There are 32 x 16kB ROM-Blocks in SRAM:
;                                                     ROM-Block #0  is from SRAM 0x08_0000 to 0x08_3FFF (= also known as the 8kB RAM-Blocks #192 and #193),
;                                                     ROM-Block #1  is from SRAM 0x08_4000 to 0x08_7FFF (= also known as the 8kB RAM-Blocks #194 and #195), etc.,
;                                                     ROM-Block #31 is from SRAM 0x0F_C000 to 0x0F_FFFF (= also known as the 8kB RAM-Blocks #254 and #255).
;                                                 The ROMBLOCK register allows addressing up to 64 ROM-Blocks, but only the first 32 are available in the SRAM.

; RAMBLOCK    = $00
; ROMBLOCK    = $01

;
; NORA USB/UART registers:
; -------------------------
;     Address         Reg.name            Bits        Description
;     $9F55           USB_UART_CTRL
;                                         [2:0] = Baud Rate
;                                                 000 = reserved
;                                                 001 = reserved
;                                                 010 = 9600 Bd
;                                                 011 = 57600 Bd
;                                                 100 = 115200 Bd = DEFAULT
;                                                 101 = 230400 Bd
;                                                 110 = 1000000 Bd (non-standard freq)
;                                                 111 = 3000000 Bd (non-standard freq)
;                                         [3] = Enable Parity generation/checking, DEFAULT=0
;                                         [4] = Parity Configuration: 0=even (DEFAULT), 1=odd.
;                                         [5] = Enable HW-FlowCtrl (RTS/CTS) - honoring the CTS signal.
;                                                 When the CTS input is active (low), transmission from FIFO is allowed.
;                                                 When the CTS input is inactive (high), transmissions are blocked.
;                                                 DEFAULT = OFF.
;                                         [6] = Enable IRQ activation on not(RX-FIFO-empty)
;                                         [7] = Enable IRQ activation on not(TX-FIFO-full)
;
;     $9F56           USB_UART_STAT                   USB(FTDI)-UART Status
;                                         [0] = reserved, 0.
;                                         [1] = Is RX FIFO full?
;                                         [2] = Is TX FIFO empty AND all chars were sent?
;                                         [3] = Is TX FIFO full (no more TX chars can be accepted)?
;                                         [4] = Framing Error Flag, latching, clear by writing 0.
;                                         [5] = Parity Error Flag, latching, clear by writing 0.
;                                         [6] = CTS signal Status: 1=CTS inactive (voltage=high), 0=CTS active (voltage=low).
;                                         [7] = Is RX FIFO empty?
;
;     $9F57           USB_UART_DATA       [7:0]       Reading dequeues data from the RX FIFO.
;                                                     Writing enqueues to the TX FIFO.

USB_UART_CTRL = $9F55
USB_UART_STAT = $9F56
USB_UART_DATA = $9F57


; SERio     = $100008
; SERstat   = $100009
; ; Presumably baud and line setup
; SERctrlA  = $10000A               ; NeonForth sets this to $8D
; SERctrlB  = $10000B               ; ditto $06
; SERctrlC  = $10000C               ; ditto $00

; ; MIDI ports
; MIDIio    = $100020
; MIDIstat  = $100021

; ; PS/2 Keyboard Port
; PS2Kio    = $100022
; PS2Kstat  = $100023

; ; PS/2 Mouse Port
; PS2Mio    = $100024
; PS2Mstat  = $100025

; ; *****************
; ; RTC
; ; *****************

; RTCus     = $100100               ; 2 bytes
; RTCms     = $100102               ; 2 bytes
; RTCsec    = $100104               ; 1 byte
; RTCmin    = $100105               ; 1 byte
; RTChour   = $100106               ; 1 byte
; RTCday    = $100108               ; 2 bytes

; ; *****************
; ; I2C
; ; *****************

; I2C2io    = $100014
; I2C2ctrl  = $100015               ; read: b7 = busy; write: $01 = start, $02 = stop, $04 = receive, $08 = send, $44 = receive/ack

; ; *****************
; ; SPI
; ; *****************

; SPI2io    = $10001C
; SPI2ctrl  = $10001D               ; read: b3 = tx busy, b2 = rx full; write: $01 = start, $00 = stop; init = $00
; SPI2ctrl2 = $10001E               ; init = $00
; SPI2ctrl3 = $10001F               ; init = $05


; ; *****************
; ; VDC
; ; *****************

; VDCbase   = $100120               ; base address, there are $20 registers

; ; *****************
; ; FDC
; ; *****************

; FDCio     = $090004
; FDCstat   = $090005               ; b7=0 = tx busy



