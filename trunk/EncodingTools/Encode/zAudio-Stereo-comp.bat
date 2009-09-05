start /wait mplayer mixed.avi -ao pcm:file=encoded.wav \ -vc dummy -aid 1 -vo null 

start /wait oggenc2 -q1 encoded.wav 