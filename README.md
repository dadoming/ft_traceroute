# ft_traceroute

## Table of Contents
- [Description](#description)
- [Features](#features)
- [Installation](#installation)
- [Usage](#usage)

## Description
ft_traceroute is a simple implementation of the traceroute command. It is a network diagnostic tool used to track the route packets take from an IP network on their way to a given host. It utilizes the ICMP protocol to send packets with increasing TTL values to the target host, and prints the IP addresses of the routers that the packets pass through.

For example, if we try to traceroute to google.com, it will work something like:<br>

```
Source (Your Machine)
  |
  v
+---------+
| Router1 | <-- Hop 1 (TTL=1)
+---------+
  |
  v
+---------+
| Router2 | <-- Hop 2 (TTL=2)
+---------+
  |
  v
+---------+
| Router3 | <-- Hop 3 (TTL=3)
+---------+
  |
  v
+---------+
| Router4 | <-- Hop 4 (TTL=4)
+---------+
  |
  v
+-------------+
| Destination | (e.g., Google) | <-- Final Destination
+-------------+
```

## Features

- **Traces Route:** Sends packets with increasing TTL values to the target host, printing the IP addresses of the routers that the packets pass through.
- **Customizable with Flags:**
  - `-f [first_ttl]`: Specifies the first TTL value.
  - `-m [max_ttl]`: Specifies the maximum TTL value.
  - `-q [nqueries]`: Specifies the number of queries per hop.
  - `-n`: Prevents the program from resolving IP addresses to hostnames.
  - `-h`: Displays the help message.

## Installation

Clone this repository and navigate to the project directory:

```bash
git clone https://github.com/dadoming/ft_traceroute.git
cd ft_ping
```

Compile the program using the provided Makefile:

```bash
make
```

## Usage

Run the program with the target host as an argument:

```bash
sudo ./ft_traceroute [hostname (example:google.com)]
```
