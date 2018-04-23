declare -a arr=("cit-Patents_10000.txt" "cit-Patents_25000.txt" "cit-Patents_50000.txt" "cit-Patents_100000.txt" "cit-Patents_250000.txt" "cit-Patents_500000.txt" "cit-Patents_750000.txt" "cit-Patents.txt")

make clean
make cite_graph
make cite_graph_onlyeq
make columbia_top_sort
make max_flow_test

for i in "${arr[@]}"
do
   ./cite_graph $i
done

for i in "${arr[@]}"
do
   ./cite_graph_onlyeq $i
done

./columbia_top_sort

./max_flow_test