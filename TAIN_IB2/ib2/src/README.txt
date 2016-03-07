# IB2

1. Biz.c      : /hw01/ibridge/ib2/cfg/biz.org, /hw01/ibridge/ib2/cfg/biz2.org 파일을 읽는다.
2. 
24. Sock.c     : Socket
2. CSock.c    : Client Socket
20. SSock.c    : Server Socket
15. MSock.c    : Multi Socket
29. X25.c      : X25
30. 
3. Cfg.c      :
4. Err.c      :
5. Exec.c     :
6. FQ2.c      :
7. FQ3.c      :
8. Flck.c     :
9. HWSeed.c   :
10. IBnet.c    :
11. KISeed.c   :
12. Line.c     :
13. Log.c      :
14. MCfg.c     :
16. Mmap.c     :
17. Passwd.c   :
18. Poll.c     :
19. Rand.c     :
21. Seq.c      :
22. Sig.c      :
23. Sinf.c     :
25. Time.c     :
26. Ums.c      :
27. Util.c     :
28. Var.c      :

50. fepstat2.c :
51. log_del.c  :
52. log_sec.c  :
53. log_zip.c  :
54. maachk2.c  :
77. ps_man.c   :

30. bfrcom2.c  :
31. bfscom2.c  :
32. bircom2.c  :
33. biscom2.c  :
34. 
34. bxrctc.c   :
35. bxrhdc.c   :
36. bxrkft1.c  :
37. bxrkft2.c  :
38. bxrkft3.c  :
39. bxrksn.c   :
40. bxrltc.c   :
41. bxrshc.c   :
42. bxrssn.c   :

43. bxshdc.c   :
44. bxskft1.c  :
45. bxskft2.c  :
46. bxskft3.c  :
47. bxsksn.c   :
48. bxsshc.c   :
49. bxsssn.c   :

55. ofrcho2.c  :
56. ofrcom2.c  :
57. ofrkft2.c  :
58. ofscho2.c  :
59. ofscom2.c  :
60. oircho2.c  :
61. oircom2.c  :
62. oischo2.c  :
63. oiscom2.c  :
64. oiscom3.c  :
65. oxakft2.c  :
66. oxameg2.c  :
67. oxascb2.c  :
68. oxatat2.c  :
69. oxrcho2.c  :
70. oxrcom2.c  :
71. oxrdhi2.c  :
72. oxrmeg2.c  :
73. oxscho2.c  :
74. oxscom2.c  :
75. oxsdhi2.c  :
76. oxsmeg2.c  :

##################################################################################################

AP:ofs ---------> FEP:oir -> FEP:oxs ----------> EXT
AP:ofr <--------- FEP:ois <- FEP:oxr <---------- EXT

ofscom2
ofrcom2

oiscom2
oircom2

oxscom2
oxrcom2
osacom

##################################################################################################
[OFSKFT0201]
	DESCRIPTION      = 금융결제원  CD/ATM
	OBSERVATION_FLAG = Y
	#RUN_SCRIPT      = /hw01/ibridge/shl/xrun OFSKFT0201
	RUN_SCRIPT       = /hw01/ibridge/ib2/shl/ibrun2 OFSKFT0201
	RUNNING_SERVER   = AP
	RUNNING_PROCESS  = /hw01/ibridge/ib2/bin/ofscom2
	RUNNING_TIME     = 0~00:25~23:58 | 1~00:25~23:58 | 2~00:25~23:58 | 3~00:25~23:58 | 4~00:25~23:58 | 5~00:25~23:58 | 6~00:25~23:58

[OFSKFT0202]
	DESCRIPTION      = 금융결제원  CD/ATM
	OBSERVATION_FLAG = N
	#RUN_SCRIPT      = /hw01/ibridge/shl/xrun OFSKFT0202
	RUN_SCRIPT       = /hw01/ibridge/ib2/shl/ibrun2 OFSKFT0202
	RUNNING_SERVER   = AP
	RUNNING_PROCESS  = /hw01/ibridge/ib2/bin/ofscom2
	RUNNING_TIME     = 0~00:25~23:58 | 1~00:25~23:58 | 2~00:25~23:58 | 3~00:25~23:58 | 4~00:25~23:58 | 5~00:25~23:58 | 6~00:25~23:58

[OFRKFT0201]
	DESCRIPTION      = 금융결제원  CD/ATM
	OBSERVATION_FLAG = Y
	#RUN_SCRIPT      = /hw01/ibridge/shl/xrun OFRKFT0201
	RUN_SCRIPT       = /hw01/ibridge/ib2/shl/ibrun2 OFRKFT0201
	RUNNING_SERVER   = AP
	RUNNING_PROCESS  = /hw01/ibridge/ib2/bin/ofrcom2
	RUNNING_TIME     = 0~00:25~23:58 | 1~00:25~23:58 | 2~00:25~23:58 | 3~00:25~23:58 | 4~00:25~23:58 | 5~00:25~23:58 | 6~00:25~23:58

[OISKFT0201]
	DESCRIPTION      = 금융결제원  CD/ATM
	OBSERVATION_FLAG = Y
	#RUN_SCRIPT      = /hw01/ibridge/shl/xrun OISKFT0201
	RUN_SCRIPT       = /hw01/ibridge/ib2/shl/ibrun2 OISKFT0201
	RUNNING_SERVER   = FEP
	RUNNING_PROCESS  = /hw01/ibridge/ib2/bin/oiscom2
	RUNNING_TIME     = 0~00:25~23:58 | 1~00:25~23:58 | 2~00:25~23:58 | 3~00:25~23:58 | 4~00:25~23:58 | 5~00:25~23:58 | 6~00:25~23:58

[OIRKFT0201]
	DESCRIPTION      = 금융결제원  CD/ATM
	OBSERVATION_FLAG = Y
	#RUN_SCRIPT      = /hw01/ibridge/shl/xrun OIRKFT0201
	RUN_SCRIPT       = /hw01/ibridge/ib2/shl/ibrun2 OIRKFT0201
	RUNNING_SERVER   = FEP
	RUNNING_PROCESS  = /hw01/ibridge/ib2/bin/oircom2
	RUNNING_TIME     = 0~00:25~23:58 | 1~00:25~23:58 | 2~00:25~23:58 | 3~00:25~23:58 | 4~00:25~23:58 | 5~00:25~23:58 | 6~00:25~23:58

[OXSKFT0201]
	DESCRIPTION      = 금융결제원  CD/ATM
	OBSERVATION_FLAG = Y
	#RUN_SCRIPT      = /hw01/ibridge/shl/xrun OXSKFT0201
	RUN_SCRIPT       = /hw01/ibridge/ib2/shl/ibrun2 OXSKFT0201
	RUNNING_SERVER   = FEP
	RUNNING_PROCESS  = /hw01/ibridge/ib2/bin/oxscom2
	RUNNING_TIME     = 0~00:25~23:58 | 1~00:25~23:58 | 2~00:25~23:58 | 3~00:25~23:58 | 4~00:25~23:58 | 5~00:25~23:58 | 6~00:25~23:58

[OXRKFT0201]
	DESCRIPTION      = 금융결제원  CD/ATM
	OBSERVATION_FLAG = Y
	#RUN_SCRIPT      = /hw01/ibridge/shl/xrun OXRKFT0201
	RUN_SCRIPT       = /hw01/ibridge/ib2/shl/ibrun2 OXRKFT0201
	RUNNING_SERVER   = FEP
	RUNNING_PROCESS  = /hw01/ibridge/ib2/bin/oxrcom2
	RUNNING_TIME     = 0~00:25~23:58 | 1~00:25~23:58 | 2~00:25~23:58 | 3~00:25~23:58 | 4~00:25~23:58 | 5~00:25~23:58 | 6~00:25~23:58

[OSAKFT0201]
	DESCRIPTION      = 금융결제원  CD/ATM
	OBSERVATION_FLAG = N
	#RUN_SCRIPT      = /hw01/ibridge/shl/xrun OSAKFT0201
	RUN_SCRIPT       = /hw01/ibridge/ib2/shl/ibrun2 OSAKFT0201
	RUNNING_SERVER   = FEP
	RUNNING_PROCESS  = /hw01/ibridge/ib2/bin/osacom
	RUNNING_TIME     = 0~00:25~23:58 | 1~00:25~23:58 | 2~00:25~23:58 | 3~00:25~23:58 | 4~00:25~23:58 | 5~00:25~23:58 | 6~00:25~23:58

##################################################################################################

AP:bfs ---------> FEP:bir -> FEP:bxs ----------> EXT
AP:bfr <--------- FEP:bis <- FEP:bxr <---------- EXT

bfscom2
bfrcom2

biscom2
bircom2

bxskft1
bxrkft1
bxacom2

##################################################################################################
[OFSKFT5101]
	DESCRIPTION      = 금융결제원  일괄전송
	OBSERVATION_FLAG = Y
	#RUN_SCRIPT      = /hw01/ibridge/shl/xrun OFSKFT5101
	RUN_SCRIPT       = /hw01/ibridge/ib2/shl/ibrun2 OFSKFT5101
	RUNNING_SERVER   = AP
	RUNNING_PROCESS  = /hw01/ibridge/ib2/bin/bfscom2
	RUNNING_TIME     = 0~00:25~23:55 | 1~00:25~23:55 | 2~00:25~23:55 | 3~00:25~23:55 | 4~00:25~23:55 | 5~00:25~23:55 | 6~00:25~23:55

[OFRKFT5101]
	DESCRIPTION      = 금융결제원  일괄전송
	OBSERVATION_FLAG = Y
	#RUN_SCRIPT      = /hw01/ibridge/shl/xrun OFRKFT5101
	RUN_SCRIPT       = /hw01/ibridge/ib2/shl/ibrun2 OFRKFT5101
	RUNNING_SERVER   = AP
	RUNNING_PROCESS  = /hw01/ibridge/ib2/bin/bfrcom2
	RUNNING_TIME     = 0~00:25~23:55 | 1~00:25~23:55 | 2~00:25~23:55 | 3~00:25~23:55 | 4~00:25~23:55 | 5~00:25~23:55 | 6~00:25~23:55

[OISKFT5101]
	DESCRIPTION      = 금융결제원  일괄전송
	OBSERVATION_FLAG = Y
	#RUN_SCRIPT      = /hw01/ibridge/shl/xrun OISKFT5101
	RUN_SCRIPT       = /hw01/ibridge/ib2/shl/ibrun2 OISKFT5101
	RUNNING_SERVER   = FEP
	RUNNING_PROCESS  = /hw01/ibridge/ib2/bin/biscom2
	RUNNING_TIME     = 0~00:25~23:55 | 1~00:25~23:55 | 2~00:25~23:55 | 3~00:25~23:55 | 4~00:25~23:55 | 5~00:25~23:55 | 6~00:25~23:55

[OIRKFT5101]
	DESCRIPTION      = 금융결제원  일괄전송
	OBSERVATION_FLAG = Y
	#RUN_SCRIPT      = /hw01/ibridge/shl/xrun OIRKFT5101
	RUN_SCRIPT       = /hw01/ibridge/ib2/shl/ibrun2 OIRKFT5101
	RUNNING_SERVER   = FEP
	RUNNING_PROCESS  = /hw01/ibridge/ib2/bin/bircom2
	RUNNING_TIME     = 0~00:25~23:55 | 1~00:25~23:55 | 2~00:25~23:55 | 3~00:25~23:55 | 4~00:25~23:55 | 5~00:25~23:55 | 6~00:25~23:55

[BXSKFT5101]
	DESCRIPTION      = 금융결제원  일괄전송
	OBSERVATION_FLAG = N
	#RUN_SCRIPT      = /hw01/ibridge/shl/xrun BXSKFT5101
	RUN_SCRIPT       = /hw01/ibridge/ib2/shl/ibrun2 BXSKFT5101
	RUNNING_SERVER   = FEP
	RUNNING_PROCESS  = /hw01/ibridge/ib2/bin/bxskft1
	RUNNING_TIME     = 0~00:25~23:55 | 1~00:25~23:55 | 2~00:25~23:55 | 3~00:25~23:55 | 4~00:25~23:55 | 5~00:25~23:55 | 6~00:25~23:55

[BXRKFT5101]
	DESCRIPTION      = 금융결제원  일괄전송
	OBSERVATION_FLAG = N
	#RUN_SCRIPT      = /hw01/ibridge/shl/xrun BXRKFT5101
	RUN_SCRIPT       = /hw01/ibridge/ib2/shl/ibrun2 BXRKFT5101
	RUNNING_SERVER   = FEP
	RUNNING_PROCESS  = /hw01/ibridge/ib2/bin/bxrkft1
	RUNNING_TIME     = 0~00:25~23:55 | 1~00:25~23:55 | 2~00:25~23:55 | 3~00:25~23:55 | 4~00:25~23:55 | 5~00:25~23:55 | 6~00:25~23:55

[BXAKFT5101]
	DESCRIPTION      = 금융결제원  일괄전송
	OBSERVATION_FLAG = Y
	RUN_SCRIPT       = /hw01/ibridge/ib2/shl/bxacom2.sh BXAKFT5101
	RUNNING_SERVER   = FEP
	RUNNING_PROCESS  = /hw01/ibridge/ib2/shl/bxacom2.sh
	RUNNING_TIME     = 0~00:25~23:55 | 1~00:25~23:55 | 2~00:25~23:55 | 3~00:25~23:55 | 4~00:25~23:55 | 5~00:25~23:55 | 6~00:25~23:55

##################################################################################################

