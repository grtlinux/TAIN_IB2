Str1=${1}

Str2=${Str1%.enc}

# æœ»£«ÿ¡¶
echo "[41m < æœ»£ «ÿ¡¶ : [${Str1}] >[0m\n"
log_sec DEC ${Str1}

# æ–√‡«ÿ¡¶
echo "[41m < æ–√‡ «ÿ¡¶ : [${Str1}] >[0m\n"
gzip -d ${Str2}