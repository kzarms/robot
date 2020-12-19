# Code examples for raspberry camera setup
--rotation 180 \
--width 	max. 2592 	2592 	Breite des Bildes in Pixel
--height    

raspivid -o video.h264 -t 10000 -hv --rotation 180 --width 640 --height 480


raspivid -l -o tcp://0.0.0.0:5000 -w 640 -h 480 --rotation 180

raspivid -w 320 -h 240 --rotation 180

######
# Install module for H.264
sudo apt install -y gpac

raspivid -t 10000 -w 640 -h 480 --rotation 180 -fps 25 -b 1200000 -p 0,0,640,480 -o pivideo.h264