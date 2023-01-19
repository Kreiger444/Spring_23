while read -r line; do
    n=${line}
    touch output.txt
    primes=( $(for ((i=2; i<n; i++)); do echo 1; done))
    for((i=2;i*i<=n;i++)); do
        if [ ${primes[i]} -eq 1 ]; then
            for((j=i*i;j<=n;j+=i)); do
                primes[j]=0
            done
        fi
    done
    for((i=2;i<n;i++)); do
        if [ ${primes[i]} -eq 1 ]; then
            echo $i >> output.txt
        fi
    done
done < input.txt
