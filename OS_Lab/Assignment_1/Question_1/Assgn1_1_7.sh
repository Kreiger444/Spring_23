gcd(){a=$1;b=$2;while [$b!=0];do(r=$(($a%$b)));a=$b;b=$r;done;return $a;}rev lcm.txt
