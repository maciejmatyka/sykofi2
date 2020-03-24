
# loop over .stl files
# convert to the POV-Ray format
# and render the scene 

for i in tmp/binterface0*.stl; 
do 
  stl2pov  $i >$i.inc
  rm object.inc
  mv $i.inc object.inc
  povray scena.pov +W640 +H360 +A +Q11
  mv scena.png $i.inc.png
done;
     
# rename the produced frames
# into frame###.png format 

let n=1
for i in tmp/binterface*.png; do
mv $i tmp/frame$n.png
n=$(( n + 1 ))
done;

# produce the animation in the .mp4 format

ffmpeg -qscale 5 -r 20 -b 9600 -i tmp/frame%d.png tmp/movie.mp4
