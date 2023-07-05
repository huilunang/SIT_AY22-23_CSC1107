#!/bin/bash
USER_22=CSC1107_22
DIR_22=~/Folder_22

# 1) Display logged-in user upon ssh successful
echo "
=============================================
@@@@@@@@@@@@@@ WELCOME USER @@@@@@@@@@@@@@@@@
=============================================
    - hostname..........: $(uname -n)
    - user..............: $(whoami)
=============================================
"

# 2) Display current working directory
echo Current Working Directory: $(pwd)

# 3) Create new user in OS called "CSC1107_22"
echo Creating user \"$USER_22\"...
useradd -m $USER_22

# 4) Change default shell from sh to bash for user "CSC1107_22" in the file "/etc/passwd"
echo Changing \"$USER_22\" default shell to bash...
USER_SHELL_PATH=$(grep $USER_22 /etc/passwd)
USER_SHELL=$($USER_SHELL_PATH | grep "\(/bin/[A-Za-z]*\)")
if [ USER_SHELL_PATH != "" && USER_SHELL != "\bin\bash"  ] then;
    NEW_USER_SHELL_PATH=$(echo $USER_SHELL_PATH | sed "s:/bin/[A-Za-z]*:/bin/bash:g")
    sudo sed -i "s|$USER_SHELL_PATH|$NEW_USER_SHELL_PATH|" /etc/passwd
else
    # terminate and indicate error
    echo Cannot find user CSC1107_22...
    echo Terminating...
    exit 1
fi

# 5) Use `grep` cmd to get the user in the "/etc/passwd" file, for visual check if default shell has been changed for the user
echo Displaying \"$USER_22\" default shell...
grep $USER_22 /etc/passwd

# 6) Check if the folder "Folder_22" is in home directory
echo Creating folder \"$DIR_22\"...
if [ -d $DIR_22 ];
then
    echo Detected folder exits...
    echo Recreating folder \"$DIR_22\"...
    rm -rf $DIR_22
else
    echo Creating folder \"$DIR_22\"...
fi
mkdir $DIR_22
echo Folder \"$DIR_22\" has been successfully created...

# 7) `cd`` into "Folder_22"
echo Changing directory into \"$DIR_22\"...
cd $DIR_22

# 8) copy CSC1107_22_user.c, CSC1107_22_kernel.c, Makefile from "/linux/CSC1107_assignment/" folder to the "Folder_22"
ASSIGN_FOLDER_PATH=~/linux/CSC1107_assignment/
echo Copying contents from \"$ASSIGN_FOLDER_PATH\" to folder \"$DIR_22\"...
cp -r $ASSIGN_FOLDER_PATH* .

# 9) Compile loadable kernel module from "CSC1107_22_kernel.c" using the "Makefile"
echo Compiling loadable kernel module...
make

# 10) List all the file in current folder `ls`
echo Listing files in \"$DIR_22\"...
ls -l

# 11) Insert the compiled loadable kernel module into linux kernel
echo Inserting compiled loadable kernel module \"$USER_22\".ko into linux kernel...
sudo insmod $USER_22.ko

# 12) Use `lsmod`, `grep` to display if the loadable kernel mod is successfully inserted
echo Displaying inserted loadable kernel module...
lsmod | grep $USER_22.ko

# 13) Use `modinfo` to display the loadable kernal mod information
echo Displaying inserted loadable kernel module information...
modinfo $USER_22.ko

# 14) Use `dmesg` to display last 5 message in the kernel log buffer
echo Displaying last 5 message in the kernel log buffer...
dmesg | tail -5

# 15) Compile user space application CSC1107_22_user.c into executable "CSC1107_22_user"
echo Compiling user space application CSC1107_22_user.c into executable \"CSC1107_22_user\"...
gcc -o CSC1107_22_user CSC1107_22_user.c

# 16) Invoke & execute the executable "CSC1107_22_user"
echo Executing \"CSC1107_22_user\"...
./CSC1107_22_user

# 17) Observe print-out message on the screen, when waiting for user-input

# 18) Observe print-out message on the screen of the 5 items returned from kernel space

# 19) `tail -12 /var/log/syslog` to check if the message is being printed out by the program
echo Displaying system logs...
tail -12 /var/log/syslog

# 20) remove loadable kernel module from linux kernel
echo Removing loadable kernel module from linux kernel...
sudo rmmod $USER_22.ko

# 21) Use `dmesg` to display the last 3 message in kernel log buffer. To check if loadable kernel module is being successfully removed
echo Displaying kernel log buffer...
echo Checking kernel module is successfully removed...
dmesg | tail -3

# 22) Delete "CSC1107_22" user from raspi OS
echo Deleting user \"$USER_22\"...
userdel -r $USER_22

# 23) Use `grep` to get the line containing "CSC1107_22" previously in the file "/etc/passwd". If nothing means successfully removed
USER_SHELL_PATH=$(grep $USER_22 /etc/passwd)
if [ $USER_SHELL_PATH == '' ];
then
    echo User \"$USER_22\" has been successfully removed...
else
    echo User \"$USER_22\" is not removed...
fi

# 24) Display the current working directory on screen
echo Current working directory...
pwd

# 25) List all the files in the current working directory
echo Listing all the files in working dir of \"${pwd}\"...
ls -l

# 26) Print a message on screen
echo The bash shell script of CSC1107_22 has finished all tasks, and stop here.