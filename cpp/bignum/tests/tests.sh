
EX="clang++ -Wall -Werror -fsanitize=address -I .. -o test "

function Comp()
{
  printf "%s... " "$1"
  $EX "$1" 1>>log.txt 2>>err.txt && echo "success" || echo "FAILED"
}

function Ncomp()
{
  printf "%s... " "$1"
  $EX "$1" 1>>ncomp_log.txt 2>>ncomp_err.txt && echo "FAILED" || echo "success"
}

function Run()
{
  printf "%s... " "$1"
  $EX "$1" 1>>log.txt 2>>err.txt && ./test 1>>run_log.txt 2>>run_err.txt && echo "success" || echo "FAILED"
}

for path in "$(pwd)"/*
do
  file=$(basename "$path")
  [[ $file == comp*.cpp ]] && Comp "$file"
  [[ $file == ncomp*.cpp ]] && Ncomp "$file"
  [[ $file == run*.cpp ]] && Run "$file"
done

rm -f ./test
