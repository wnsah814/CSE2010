#!/bin/bash

# 변수 설정
STUDENT_ID="2021092379"
LAB_NUM="lab06"
SOURCE_FILE="${LAB_NUM}/${STUDENT_ID}.c"
EXECUTABLE_NAME="${STUDENT_ID}"
INPUT_FOLDER="testcase/${LAB_NUM}_solution/"
OUTPUT_FOLDER="outputs/${LAB_NUM}/"

# outputs/lab03 폴더가 없으면 생성
mkdir -p "$OUTPUT_FOLDER"

# 소스 코드 컴파일
gcc -o "$EXECUTABLE_NAME" "$SOURCE_FILE"

# 컴파일이 성공적으로 되었는지 확인
if [ $? -eq 0 ]; then
    # testcase/${LAB_NUM}_solution/ 폴더에 있는 모든 input 파일에 대해 수행
    for input_file in "$INPUT_FOLDER"/input*.txt; do
        TEST_NUM=$(basename "$input_file" | sed 's/input\([0-9]*\).txt/\1/')
        OUTPUT_FILE="${OUTPUT_FOLDER}output${TEST_NUM}.txt"
        
        # 실행하여 출력 파일 생성
        ./"$EXECUTABLE_NAME" "$input_file" "$OUTPUT_FILE"
        
        # 생성된 출력 파일과 예상 출력 파일을 비교
        if [ -f "$OUTPUT_FILE" ]; then
            EXPECTED_OUTPUT_FILE="${INPUT_FOLDER}output${TEST_NUM}.txt"
            # 비교할 때 공백을 무시하고 출력 비교
            diff_output=$(diff -b "$OUTPUT_FILE" "$EXPECTED_OUTPUT_FILE")

            if [ -z "$diff_output" ]; then
                echo "테스트 $TEST_NUM: 출력이 정확합니다."
            else
                echo "테스트 $TEST_NUM: 출력이 예상과 다릅니다:"
                echo "$diff_output"
            fi
        else
            echo "테스트 $TEST_NUM: 출력 파일을 생성하지 못했습니다."
        fi
    done
else
    echo "컴파일 에러가 발생했습니다."
fi
