# DNS-Monitor / Packet Sniffer (C++)

## Overview

A small Linux-based packet monitoring tool written in C++ using raw sockets.
This project was created as a learning and fun project to explore low-level networking, packet capturing, Ethernet frames, and IP header analysis.

The application listens directly on a network interface and prints source and destination IPv4 addresses of captured packets in real time.

> **Note:** Despite the name "DNS-Monitoring", the current implementation captures all IPv4 traffic on the selected interface and does not specifically parse DNS packets.

---

## Features

* Raw packet capture using `AF_PACKET`
* Ethernet frame inspection
* IPv4 packet detection
* Source IP extraction
* Destination IP extraction
* Simple console-based monitoring
* Graceful shutdown using `CTRL + C`
* Lightweight and dependency-free

---

## Technologies Used

* C++
* Linux Raw Sockets
* POSIX Networking APIs
* Ethernet Frame Parsing
* IPv4 Header Parsing
* Signal Handling

---

## How It Works

1. Creates a raw socket.
2. Binds the socket to a specific network interface.
3. Receives Ethernet frames directly from the kernel.
4. Checks whether the packet contains IPv4 traffic.
5. Extracts:

   * Source IP Address
   * Destination IP Address
6. Prints the information to the terminal.

Example output:

```text
### DNS-Monitoring ###

[+] socket created
[+] Bound to P:53
[+] Listening..

[+] IP contacted -> ===...===
[8.8.8.8] [DEST.]
[192.168.178.42] [SOURCE]
```

---

## Requirements

### Operating System

* Linux

### Compiler

* GCC / G++

Example:

```bash
g++ --version
```

### Root Privileges

Raw sockets require elevated permissions.

Run the program as root:

```bash
sudo ./dns-monitor
```

---

## Build Instructions

Compile using:

```bash
g++ main.cpp -o dns-monitor -pthread
```

or with additional warnings:

```bash
g++ main.cpp -o dns-monitor -pthread -Wall -Wextra
```

---

## Running

Start the monitor:

```bash
sudo ./dns-monitor
```

Press ENTER when prompted.

To stop monitoring:

```text
CTRL + C
```

Output:

```text
=== Monitoring ended ===
```

---

## Network Interface

The current implementation is hardcoded to:

```cpp
wlx202351d079cd
```

If your interface name is different, replace:

```cpp
if_nametoindex("wlx202351d079cd");
```

with your interface.

To view available interfaces:

```bash
ip link show
```

or

```bash
ifconfig
```

---

## Current Limitations

The project is intentionally simple and currently:

* Does not parse DNS packets
* Does not filter UDP port 53 traffic
* Only prints IPv4 addresses
* Does not display packet payloads
* Does not support IPv6
* Uses a fixed network interface
* Runs in a single monitoring loop

---

## Possible Future Improvements

Ideas for extending the project:

### DNS Parsing

* Parse DNS headers
* Display queried domains
* Show DNS response records

### Filtering

* Capture only DNS traffic
* Filter by protocol
* Filter by IP address

### Logging

* Save packets to log files
* Export to CSV
* Export to JSON

### Statistics

* Top contacted IPs
* Packet counters
* Traffic graphs

### User Interface

* Interactive terminal UI
* ncurses dashboard
* Web dashboard

### Protocol Support

* IPv6
* TCP
* UDP
* ICMP

---

## Educational Purpose

This project was created for educational purposes to learn:

* Linux networking internals
* Raw sockets
* Ethernet frame structures
* IP packet structures
* Low-level packet analysis
* Network monitoring concepts

---

## Security Notice

This tool captures network traffic directly from a network interface.

Only use it:

* On systems you own
* On networks you are authorized to monitor
* For educational and testing purposes

Always comply with local laws and network policies.

---

## License

This project is released under the MIT License.

Feel free to modify, improve, and learn from the code.

---

## Author

Created as a personal networking and systems programming learning project in C++.
