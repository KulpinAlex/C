#!/bin/bash

COUNTER_SUCCESS=0
COUNTER_FAIL=0
COUNTER=0
DIFF_RES=""
echo "" > log.txt

for var in -v -c -l -n -h -o -e -i -s
do
          TEST1="$var for s21_grep.c s21_grep.h pattern.txt"
          ./s21_grep $TEST1 > s21_grep.txt
          grep $TEST1 > grep.txt
          DIFF_RES="$(diff -s s21_grep.txt grep.txt)"
          (( COUNTER++ ))
          if [ "$DIFF_RES" == "Files s21_grep.txt and grep.txt are identical" ]
            then
              (( COUNTER_SUCCESS++ ))
              echo "$COUNTER_FAIL/$COUNTER_SUCCESS/$COUNTER sucess: grep $TEST1"
            else
              (( COUNTER_FAIL++ ))
              echo "$COUNTER_FAIL/$COUNTER_SUCCESS/$COUNTER failed: grep $TEST1"
          fi
          rm s21_grep.txt grep.txt

          TEST2="$var for s21_grep.c"
          ./s21_grep $TEST2 > s21_grep.txt
          grep $TEST2 > grep.txt
          DIFF_RES="$(diff -s s21_grep.txt grep.txt)"
          (( COUNTER++ ))
          if [ "$DIFF_RES" == "Files s21_grep.txt and grep.txt are identical" ]
            then
              (( COUNTER_SUCCESS++ ))
              echo "$COUNTER_FAIL/$COUNTER_SUCCESS/$COUNTER sucess: grep $TEST2"
            else
              (( COUNTER_FAIL++ ))
              echo "$COUNTER_FAIL/$COUNTER_SUCCESS/$COUNTER failed: grep $TEST2"
          fi
          rm s21_grep.txt grep.txt

          TEST3="$var ^int s21_grep.c s21_grep.h pattern.txt"
          ./s21_grep $TEST3 > s21_grep.txt
          grep $TEST3 > grep.txt
          DIFF_RES="$(diff -s s21_grep.txt grep.txt)"
          (( COUNTER++ ))
          if [ "$DIFF_RES" == "Files s21_grep.txt and grep.txt are identical" ]
            then
              (( COUNTER_SUCCESS++ ))
              echo "$COUNTER_FAIL/$COUNTER_SUCCESS/$COUNTER sucess: grep $TEST3"
            else
              (( COUNTER_FAIL++ ))
              echo "$COUNTER_FAIL/$COUNTER_SUCCESS/$COUNTER failed: grep $TEST3"
          fi
          rm s21_grep.txt grep.txt

          TEST4="$var int s21_grep.c"
          ./s21_grep $TEST4 > s21_grep.txt
          grep $TEST4 > grep.txt
          DIFF_RES="$(diff -s s21_grep.txt grep.txt)"
          (( COUNTER++ ))
          if [ "$DIFF_RES" == "Files s21_grep.txt and grep.txt are identical" ]
            then
              (( COUNTER_SUCCESS++ ))
              echo "$COUNTER_FAIL/$COUNTER_SUCCESS/$COUNTER sucess: grep $TEST4"
            else
              (( COUNTER_FAIL++ ))
              echo "$COUNTER_FAIL/$COUNTER_SUCCESS/$COUNTER failed: grep $TEST4"
          fi
          rm s21_grep.txt grep.txt

          TEST5="$var int s21_grep.c"
          ./s21_grep $TEST5 > s21_grep.txt
          grep $TEST5 > grep.txt
          DIFF_RES="$(diff -s s21_grep.txt grep.txt)"
          (( COUNTER++ ))
          if [ "$DIFF_RES" == "Files s21_grep.txt and grep.txt are identical" ]
            then
              (( COUNTER_SUCCESS++ ))
              echo "$COUNTER_FAIL/$COUNTER_SUCCESS/$COUNTER sucess: grep $TEST5"
            else
              (( COUNTER_FAIL++ ))
              echo "$COUNTER_FAIL/$COUNTER_SUCCESS/$COUNTER failed: grep $TEST5"
          fi
          rm s21_grep.txt grep.txt

          TEST6="$var -f pattern.txt s21_grep.c pattern.txt"
          ./s21_grep $TEST6 > s21_grep.txt
          grep $TEST6 > grep.txt
          DIFF_RES="$(diff -s s21_grep.txt grep.txt)"
          (( COUNTER++ ))
          if [ "$DIFF_RES" == "Files s21_grep.txt and grep.txt are identical" ]
            then
              (( COUNTER_SUCCESS++ ))
              echo "$COUNTER_FAIL/$COUNTER_SUCCESS/$COUNTER sucess: grep $TEST6"
            else
              (( COUNTER_FAIL++ ))
              echo "$COUNTER_FAIL/$COUNTER_SUCCESS/$COUNTER failed: grep $TEST6"
          rm s21_grep.txt grep.txt
fi
done

for var in -vc -cn -ln -nv -hs -ol -e -ih -sn
do
          TEST1="$var for s21_grep.c s21_grep.h pattern.txt"
          ./s21_grep $TEST1 > s21_grep.txt
          grep $TEST1 > grep.txt
          DIFF_RES="$(diff -s s21_grep.txt grep.txt)"
          (( COUNTER++ ))
          if [ "$DIFF_RES" == "Files s21_grep.txt and grep.txt are identical" ]
            then
              (( COUNTER_SUCCESS++ ))
              echo "$COUNTER_FAIL/$COUNTER_SUCCESS/$COUNTER sucess: grep $TEST1"
            else
              (( COUNTER_FAIL++ ))
              echo "$COUNTER_FAIL/$COUNTER_SUCCESS/$COUNTER failed: grep $TEST1"
          fi
          rm s21_grep.txt grep.txt

          TEST2="$var for s21_grep.c"
          ./s21_grep $TEST2 > s21_grep.txt
          grep $TEST2 > grep.txt
          DIFF_RES="$(diff -s s21_grep.txt grep.txt)"
          (( COUNTER++ ))
          if [ "$DIFF_RES" == "Files s21_grep.txt and grep.txt are identical" ]
            then
              (( COUNTER_SUCCESS++ ))
              echo "$COUNTER_FAIL/$COUNTER_SUCCESS/$COUNTER sucess: grep $TEST2"
            else
              (( COUNTER_FAIL++ ))
              echo "$COUNTER_FAIL/$COUNTER_SUCCESS/$COUNTER failed: grep $TEST2"
          fi
          rm s21_grep.txt grep.txt

          TEST3="$var ^int s21_grep.c s21_grep.h pattern.txt"
          ./s21_grep $TEST3 > s21_grep.txt
          grep $TEST3 > grep.txt
          DIFF_RES="$(diff -s s21_grep.txt grep.txt)"
          (( COUNTER++ ))
          if [ "$DIFF_RES" == "Files s21_grep.txt and grep.txt are identical" ]
            then
              (( COUNTER_SUCCESS++ ))
              echo "$COUNTER_FAIL/$COUNTER_SUCCESS/$COUNTER sucess: grep $TEST3"
            else
              (( COUNTER_FAIL++ ))
              echo "$COUNTER_FAIL/$COUNTER_SUCCESS/$COUNTER failed: grep $TEST3"
          fi
          rm s21_grep.txt grep.txt

          TEST4="$var int s21_grep.c"
          ./s21_grep $TEST4 > s21_grep.txt
          grep $TEST4 > grep.txt
          DIFF_RES="$(diff -s s21_grep.txt grep.txt)"
          (( COUNTER++ ))
          if [ "$DIFF_RES" == "Files s21_grep.txt and grep.txt are identical" ]
            then
              (( COUNTER_SUCCESS++ ))
              echo "$COUNTER_FAIL/$COUNTER_SUCCESS/$COUNTER sucess: grep $TEST4"
            else
              (( COUNTER_FAIL++ ))
              echo "$COUNTER_FAIL/$COUNTER_SUCCESS/$COUNTER failed: grep $TEST4"
          fi
          rm s21_grep.txt grep.txt

          TEST5="$var int s21_grep.c"
          ./s21_grep $TEST5 > s21_grep.txt
          grep $TEST5 > grep.txt
          DIFF_RES="$(diff -s s21_grep.txt grep.txt)"
          (( COUNTER++ ))
          if [ "$DIFF_RES" == "Files s21_grep.txt and grep.txt are identical" ]
            then
              (( COUNTER_SUCCESS++ ))
              echo "$COUNTER_FAIL/$COUNTER_SUCCESS/$COUNTER sucess: grep $TEST5"
            else
              (( COUNTER_FAIL++ ))
              echo "$COUNTER_FAIL/$COUNTER_SUCCESS/$COUNTER failed: grep $TEST5"
          fi
          rm s21_grep.txt grep.txt

          TEST6="$var -f pattern.txt s21_grep.c pattern.txt"
          ./s21_grep $TEST6 > s21_grep.txt
          grep $TEST6 > grep.txt
          DIFF_RES="$(diff -s s21_grep.txt grep.txt)"
          (( COUNTER++ ))
          if [ "$DIFF_RES" == "Files s21_grep.txt and grep.txt are identical" ]
            then
              (( COUNTER_SUCCESS++ ))
              echo "$COUNTER_FAIL/$COUNTER_SUCCESS/$COUNTER sucess: grep $TEST6"
            else
              (( COUNTER_FAIL++ ))
              echo "$COUNTER_FAIL/$COUNTER_SUCCESS/$COUNTER failed: grep $TEST6"
          rm s21_grep.txt grep.txt
fi
done

echo "SUCCESS: $COUNTER_SUCCESS"
echo "FAIL: $COUNTER_FAIL"
rm log.txt