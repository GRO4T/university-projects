rm gifs/* logs/* graphs/*
for f in ./experiments/*/*.sh; do
  time bash "$f" $1 
done
grep -A 5 -r SUMMARY ./experiments/stress_test*/*.log > ./experiments/stress_tests.log