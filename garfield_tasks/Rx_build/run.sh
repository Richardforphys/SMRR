for ((x=1000; x<=20000; x+=1000)); do
    echo "Running for $x eV"
    ./Rx_Interaction $x
done