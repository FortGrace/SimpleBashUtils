#!/bin/bash

ALL=0
OKS=0
FAILS=0
DIFF_RES=""
TEST_FILE="test.txt"


for var in -n -s "" -E -T --number-nonblank --number --squeeze-blank
          do
          ((ALL++))
          TEST1="$var $TEST_FILE"
          ./s21_cat $TEST1 > s21.txt
          cat $TEST1 > original.txt
          DIFF_RES="$(diff -s s21.txt original.txt)"
          if [ "$DIFF_RES" == "Files s21.txt and original.txt are identical" ]
            then
              (( OKS++ ))
            else
              (( FAILS++ ))
          fi
          if [ "$var" == "" ]
          then
            echo "TEST #$ALL. NO FLAG"
          else
            echo "TEST #$ALL. FLAG $var"
          fi
          rm s21.txt original.txt
done

echo "There were: $ALL tests in total"
echo "$OKS OKs"
echo "And $FAILS fails"
echo "We tested all flags and their absence"


if [ $ALL == $OKS ]
    then
        echo "So, 100% of the tests are successfully passed =)"
else
        echo "So, I need to сorrect mistakes in s21_cat =)"
fi
