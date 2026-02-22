#!/bin/bash

for ((x=10; x<=100; x+=10)); do
    echo "Running for $x MeV"

    cat > macros/temp.mac <<EOF
/gps/particle mu-
/gps/energy ${x} MeV
/run/beamOn 10000
EOF

    ./exampletask3b macros/temp.mac
done