./close.sh

ulimit -n 100000
ulimit -c unlimited

echo "start vr gate"
./vr/gate/gatesys start

echo "start vr"
./vr/wr/vrsys start

echo "start tr"
./tr/wr/trsys start

echo "start battle gate"
./battle/gate/gatesys start

echo "start battle"
./battle/wr/battlesys start

