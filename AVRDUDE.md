Issues with Avrdude
===================

Avrdude is used to flash/program Arduino AVR processors. The it synchronizes with the AVR
bootloader (e.g. optiboot) is rather primitive and fails when there is a long round-trip latency.

The avrdude `stk500_getsync()` code can be found at
http://svn.savannah.nongnu.org/viewvc/trunk/avrdude/stk500.c?revision=1321&root=avrdude&view=markup
and appromately does the following:

1. send GET_SYNC command (GET_SYNC + CRC_EOP)
2. drain the serial input
1. send GET_SYNC command
1. drain serial input
1. repeat 10 times:
  1. send GET_SYNC command
  2. read 2 chars input
  3. exit loop if chars received are INSYNC + OK

Afterwards it sends the next command and expects a proper response and does not tolerate if there
are straggling INSYNC responses.

One the bootloader end, if a command is not received within a timeout (typ 1 second) the 
bootloader starts the old program and thus drops out of programming mode.

The big problem here is that the initial draining of the serial input assumes very low latency,
which is OK when a directly attached serial port is used but can fail when the serial port is
remote. There are two main failure modes observed:
1. the first iteration of the loop receives an INSYNC response from one of the initial GET_SYNC
   commands and then the first real command receives a subsequent INSYNC response, which it does
   not expect and fails.
2. the round-trip time from receiving a response to getting the next command to the AVR exceeds
   the bootloader's timeout, this often occurs before the bootloader receives the first GET_SYNC
   command.

### Command codes:

code    | hex | ascii
---------------------
GET_SYNC | 0x30 | 0
CRC_EOP  | 0x20 | space
INSYNC   | 0x14 |
OK       | 0x10 |


