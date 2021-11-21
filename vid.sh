rm ./images/*
make
./main
UUID=$(cat /proc/sys/kernel/random/uuid)
ffmpeg -f image2 -i ./images/image%1d.png -r 30 -q 1 -crf 18 -vf "setpts=2*PTS" test${UUID}.avi
mv test${UUID}.avi /mnt/c/users/sol/Desktop/3dca_vids/test${UUID}.avi