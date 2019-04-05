x=1
echo " "
while [ $x -le 100 ]
do
  s ="$(./rw_linked_list 1000 1000 100 0.99 0.005 0.005)"
  echo "${s}"
  x=$(( $x + 1 ))
done
