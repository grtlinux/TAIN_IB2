Str1=${1}

Str2=${Str1%.enc}

# 암호해제
echo "[41m < 암호 해제 : [${Str1}] >[0m\n"
log_sec DEC ${Str1}

# 압축해제
echo "[41m < 압축 해제 : [${Str1}] >[0m\n"
gzip -d ${Str2}