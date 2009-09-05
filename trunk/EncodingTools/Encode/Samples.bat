start /wait mencoder.exe -nosound -mc 0 -ovc copy -endpos 00:02:00 -o Video-crop.avi mixed.avi 

     start /wait x264 "Video-crop.avi" --deldup 40 --bitrate 350 --keyint 300 --ref 15 --bframes 6 --b-adapt 2 --b-pyramid --weightb --direct auto --subme 9 --partitions all --me hex --merange 30 --8x8dct --no-dct-decimate --output "video-test350.mp4" 

    start /wait x264 "Video-crop.avi" --deldup 40 --bitrate 320 --keyint 300 --ref 15 --bframes 6 --b-adapt 2 --b-pyramid --weightb --direct auto --subme 9 --partitions all --me hex --merange 30 --8x8dct --no-dct-decimate --output "video-test320.mp4" 

    start /wait x264 "Video-crop.avi" --deldup 40 --bitrate 300 --keyint 300 --ref 15 --bframes 6 --b-adapt 2 --b-pyramid --weightb --direct auto --subme 9 --partitions all --me hex --merange 30 --8x8dct --no-dct-decimate --output "video-test300.mp4" 

    start /wait x264 "Video-crop.avi" --deldup 40 --bitrate 280 --keyint 300 --ref 15 --bframes 6 --b-adapt 2 --b-pyramid --weightb --direct auto --subme 9 --partitions all --me hex --merange 30 --8x8dct --no-dct-decimate --output "video-test280.mp4" 

    start /wait x264 "Video-crop.avi" --deldup 40 --bitrate 250 --keyint 300 --ref 15 --bframes 6 --b-adapt 2 --b-pyramid --weightb --direct auto --subme 9 --partitions all --me hex --merange 30 --8x8dct --no-dct-decimate --output "video-test250.mp4" 