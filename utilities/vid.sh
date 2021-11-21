rm /images/*
make
./main
UUID=$(cat /proc/sys/kernel/random/uuid)
ffmpeg -f image2 -i ./images/image%1d.jpg -r 90 test${UUID}.avi
mv test${UUID}.avi /mnt/c/users/sol/Desktop/test1${UUID}.avi