start /wait mplayer mixed.avi -ao pcm:file=encoded.wav \ -vc dummy -aid 1 -vo null 

start /wait oggenc2 -c1 -q1 encoded.wav 