while true
do
    # sudo ./bin/spoofing.out -s 192.168.0.1 -t 192.168.0.10 -T 08:00:27:83:f4:95 -d enp0s3
    sudo ./bin/spoofing.out -s 192.168.0.1 -t 192.168.0.10 -d enp0s3    
    printf "\n"
    sleep 5
done