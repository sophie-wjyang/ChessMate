#!/bin/bash
cd "$(dirname "$0")" || exit


# Check if "chess" or "chess.exe" exists in the current directory
if [ -e "chess" ]; then
    EXECUTABLE="chess"
elif [ -e "chess.exe" ]; then
    EXECUTABLE="chess.exe"
else
    echo "No executable file 'chess' or 'chess.exe' found in the current directory."
    exit 1
fi


# build executable file
echo "Building executable..."

make || exit

echo "...successful"


cd ./tests || exit

# producing outputs
echo "Producing outputs...";

for i in *.in; do
  test="${i%.*}";

  "../$EXECUTABLE" < "$test".in > "$test".out;

  # Perform the replacement and store the result in a temporary file
  exec 3< "$test.in"
  while IFS= read -r line; do
      if [[ $line == ">> "* ]]; then
          IFS= read -r line_to_insert <&3
          echo "${line/>> />> $line_to_insert$'\n'}" >> "$test.tmp"
      else
          echo "$line" >> "$test.tmp"
      fi
  done < "$test.out"

  # Close file descriptor 3
  exec 3<&-

  mv "$test.tmp" "$test.out"

  done;

echo "...successful";

# testing for valgrind errors
if hash valgrind 2>/dev/null;
then

  echo "Checking for valgrind errors...";

  for i in *.in; do
    test="${i%.*}";

    valgrind --leak-check=full --error-exitcode=69 "../$EXECUTABLE" < "$test".in >/dev/null 2>&1;

    if [ $? -ne 0 ]
    then
      echo "Valgrind ERROR: $test";
    else
      echo "Valgrind OKAY: $test";
    fi;

    done;

  echo "...successful";
fi
