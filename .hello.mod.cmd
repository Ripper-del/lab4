savedcmd_/home/ripper/lab4_kernel/hello.mod := printf '%s\n'   hello.o | awk '!x[$$0]++ { print("/home/ripper/lab4_kernel/"$$0) }' > /home/ripper/lab4_kernel/hello.mod
