# The Linux code for RC3

This code has not yet been well documented and cleaned up.

Please grep for "Radhika" to locate all RC3 related code changes (which aren't many). 

The files changed are:
include/asm-generic/socket.h;
include/net/sock.h;
include/linux/tcp.h;
include/net/tcp.h;
net/core/sock.c;
net/ipv4/ip_input.c;
net/ipv4/ip_output.c;
net/ipv4/tcp.c;
net/ipv4/tcp_input.c;
net/ipv4/tcp_output.c;
net/ipv4/tcp_timer.c;
net/sched/sch_prio.c;

To replay RC3 results, set your qdisc to prio with 8 bands and your send and receive buffer sizes to around 2GB.

The configuration I tested it on was:
net.ipv4.tcp_dsack = 0;
net.ipv4.tcp_fack = 0;
net.ipv4.tcp_timestamps = 0;
net.core.wmem_max = 2048000000;
net.ipv4.tcp_wmem = 10240 2048000000 2048000000;
net.core.rmem_max = 2048000000;
net.ipv4.tcp_rmem = 10240 2048000000 2048000000;


Not being a Linux kernel expert, there might be some corner cases that have not been dealt with.
If you find any such corner cases, or any bugs, please bring it to my notice.

I apologize for the bad documentation.

Please email me at radhika @ eecs.berkeley.edu for any questions.

Thanks!


