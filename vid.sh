rm ./images/*
make
./main
UUID=$(cat /proc/sys/kernel/random/uuid)
ffmpeg -f image2 -i ./images/image%1d.png -r 90 -q 1 -crf 18 -vf "setpts=2.5*PTS" test${UUID}.avi
mv test${UUID}.avi /mnt/c/users/sol/Desktop/14_150_40/test${UUID}.avi

