for ((x=1000; x<=25000; x+=500)); do
    echo "Running for $x eV"
    ./Rx_Interaction $x
done