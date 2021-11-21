rm ./images/*
make
./main
UUID=$(cat /proc/sys/kernel/random/uuid)
ffmpeg -f image2 -i ./images/image%1d.png -r 90 -vf "setpts=1.5*PTS" test${UUID}.avi
mv test${UUID}.avi /mnt/c/users/sol/Desktop/3dca_vids/test1${UUID}.avi