Str1=${1}

Str2=${Str1%.enc}

# ��ȣ����
echo "[41m < ��ȣ ���� : [${Str1}] >[0m\n"
log_sec DEC ${Str1}

# ��������
echo "[41m < ���� ���� : [${Str1}] >[0m\n"
gzip -d ${Str2}