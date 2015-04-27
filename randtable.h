#define MY_MAX_RAND    10000
#define RAND_SIZE 1000

short int tab_rand[] = {
9894, 7154, 3138, 2471, 1185, 5068, 5321, 2005, 4426, 9682,
7620, 5755, 2704, 383, 9063, 5393, 8784, 9433, 4383, 4135,
8383, 2647, 8318, 6679, 8449, 7623, 6324, 9097, 5645, 1853,
3987, 1259, 878, 5193, 943, 5667, 785, 6320, 9200, 7395,
198, 7977, 5827, 8678, 1587, 3360, 2525, 7640, 7520, 3171,
1788, 8964, 4569, 9065, 8021, 4652, 75, 5105, 4745, 1846,
2278, 1223, 1815, 8858, 52, 9735, 1193, 2287, 1784, 1434,
5376, 5605, 7076, 4796, 7199, 7103, 9265, 496, 9301, 6076,
6134, 9125, 2440, 2219, 3977, 1136, 8786, 6077, 484, 7595,
9725, 4301, 4278, 9014, 174, 3006, 2842, 4214, 5003, 5904,
9521, 7642, 8656, 1100, 2497, 325, 7109, 1857, 4298, 6845,
8447, 2531, 340, 7466, 9819, 2154, 7531, 5907, 5675, 4170,
1366, 1042, 459, 9470, 3667, 4187, 3710, 2201, 5251, 7644,
3008, 3356, 3657, 3770, 8562, 9803, 7348, 2027, 524, 6626,
4656, 1235, 8220, 5305, 7630, 2191, 7105, 6655, 2360, 4191,
4181, 6946, 6647, 568, 1071, 8528, 3408, 4214, 8124, 1410,
8640, 3723, 4596, 4685, 4641, 3269, 1126, 3552, 7028, 8962,
5581, 3177, 9203, 1717, 6418, 8938, 6999, 9048, 6984, 4645,
9688, 7244, 6256, 4103, 7893, 3341, 6339, 2297, 4031, 4130,
641, 2131, 1656, 2089, 7703, 1374, 3778, 4254, 9567, 1796,
781, 8197, 3019, 4894, 2516, 5974, 7335, 8872, 1445, 2620,
6386, 7367, 1606, 8394, 3230, 6278, 1226, 9457, 1016, 8186,
4211, 9433, 4363, 1720, 8629, 8667, 6643, 3275, 9469, 3693,
503, 5069, 5297, 9441, 3208, 2917, 3563, 6192, 4824, 55,
9365, 1358, 8025, 4965, 4108, 1779, 3060, 4350, 4433, 3145,
1152, 9026, 3056, 8132, 3228, 3599, 5711, 7607, 9493, 5048,
1854, 9189, 8458, 3218, 6574, 2381, 8007, 3211, 2114, 9271,
140, 8103, 3252, 1910, 5266, 7519, 2921, 8081, 3920, 7437,
5309, 2214, 8451, 4656, 646, 3296, 7178, 3755, 842, 9321,
8472, 5514, 4441, 335, 5803, 4078, 5954, 9271, 5253, 1185,
7490, 8200, 9466, 9309, 613, 2360, 9235, 7737, 7977, 5467,
3805, 9109, 6158, 8457, 8753, 1095, 9848, 2578, 7646, 399,
5418, 811, 2932, 9111, 9817, 5122, 7949, 9622, 570, 3193,
7638, 6224, 5717, 5093, 424, 9237, 8187, 9065, 2988, 9807,
3379, 5156, 296, 9222, 1766, 6811, 9941, 3420, 2995, 3015,
3217, 5106, 7557, 945, 4555, 1074, 4445, 5292, 7389, 6901,
6884, 3991, 9344, 7068, 2698, 3355, 6180, 9599, 4028, 6866,
8481, 1666, 7570, 9293, 1701, 9396, 3156, 7840, 818, 7836,
9815, 408, 8778, 8938, 8717, 4316, 3047, 5484, 2337, 4288,
3485, 3278, 4379, 611, 1905, 8208, 8393, 1742, 3807, 3951,
6381, 6947, 6161, 3606, 8011, 2404, 8164, 6328, 3411, 2938,
2077, 85, 2015, 7712, 4626, 1065, 6099, 1896, 1943, 1474,
6892, 2558, 9002, 9514, 6154, 4008, 6592, 7275, 3865, 8937,
4975, 5760, 1467, 813, 6016, 2613, 2293, 6001, 2097, 882,
8820, 6534, 5186, 1325, 2039, 7414, 8962, 6195, 7114, 9784,
5059, 3357, 3581, 7844, 5335, 1948, 1430, 9871, 9815, 2113,
6433, 2915, 7502, 8339, 7194, 8168, 2139, 4182, 6589, 9427,
953, 4988, 2960, 6154, 9316, 293, 7851, 6823, 71, 4803,
9099, 683, 8234, 2380, 2443, 193, 311, 4961, 3392, 6985,
3016, 7318, 9565, 9542, 4972, 1358, 1178, 2818, 8526, 6509,
354, 9010, 3179, 3780, 6907, 4881, 5031, 8841, 6581, 3844,
760, 9210, 7563, 3682, 9858, 279, 986, 2240, 8246, 7095,
5226, 6695, 631, 9657, 645, 4607, 9238, 7431, 6212, 7872,
4886, 5165, 5222, 9175, 4176, 3109, 1802, 6584, 3586, 4472,
9743, 2953, 1938, 5840, 4746, 1419, 8197, 1911, 5341, 2175,
6062, 8876, 8249, 5967, 1789, 4479, 43, 7867, 9228, 7846,
3226, 1673, 5645, 4408, 2604, 4616, 438, 6190, 6699, 8119,
8466, 610, 3461, 2097, 9975, 626, 1050, 1492, 2013, 5837,
2853, 1230, 4447, 8334, 8835, 2767, 4112, 4944, 9857, 6796,
3344, 681, 6074, 666, 1829, 9388, 6757, 9996, 9322, 3918,
5543, 9625, 252, 9890, 9891, 5366, 4229, 9135, 1640, 4668,
9136, 4891, 2620, 3822, 9811, 2500, 9246, 9359, 9064, 5336,
4495, 2607, 8469, 8062, 3714, 7195, 233, 1210, 6758, 353,
9952, 6791, 3957, 4903, 7611, 497, 4337, 3716, 7284, 7614,
2050, 4175, 8526, 6961, 8456, 9632, 6879, 7251, 1919, 8261,
1945, 563, 1512, 2258, 9844, 6586, 7752, 8576, 9967, 53,
4894, 7350, 6909, 777, 5279, 5098, 2267, 1588, 9673, 1478,
6629, 4340, 5165, 2984, 1353, 3957, 6731, 4184, 1440, 2050,
9752, 6581, 8870, 8475, 3684, 2762, 8342, 4675, 8618, 3747,
4009, 4171, 287, 1623, 754, 6329, 2087, 9418, 4864, 235,
3970, 6696, 5898, 5273, 596, 4448, 2941, 8318, 9713, 6791,
7891, 4921, 8714, 3293, 6047, 5318, 741, 4393, 5598, 7608,
8921, 7855, 378, 6046, 48, 801, 3758, 3668, 9991, 6195,
5278, 5949, 1462, 2373, 762, 4356, 5015, 9893, 3982, 2170,
7108, 2287, 9292, 9691, 4342, 4841, 4248, 2281, 891, 7252,
5176, 3546, 343, 4146, 8879, 4528, 6797, 5887, 6357, 1913,
4607, 4491, 1221, 7191, 7734, 1593, 872, 9377, 1938, 4690,
978, 4048, 9174, 6942, 3132, 9166, 2739, 6755, 6382, 9735,
4767, 9581, 7804, 155, 4423, 939, 203, 340, 5358, 9419,
189, 3424, 8720, 3792, 8797, 3441, 576, 8402, 7650, 921,
7239, 7650, 979, 111, 7561, 9773, 5879, 3968, 3516, 302,
4852, 978, 69, 832, 7083, 1579, 1904, 5930, 1960, 6128,
9006, 780, 1526, 5126, 8303, 5126, 2161, 695, 798, 4431,
1639, 1908, 2247, 4757, 3156, 5653, 3571, 3590, 7493, 3626,
9995, 5119, 2132, 3281, 872, 1664, 1686, 1654, 3806, 3938,
4093, 347, 1408, 7992, 5441, 3954, 9177, 8945, 538, 3615,
7747, 7162, 299, 9088, 3305, 8718, 7985, 6452, 7620, 154,
7679, 5593, 9741, 6675, 5400, 6953, 1140, 3956, 1874, 8565,
5052, 7030, 7746, 8962, 8188, 3317, 4782, 3677, 9582, 2537,
9913, 6341, 254, 5508, 2218, 2283, 4171, 823, 5012, 6563,
9450, 3022, 8512, 4858, 9282, 5466, 9466, 6096, 464, 721,
6938, 7189, 7770, 167, 592, 8203, 3888, 8730, 6753, 3209,
6880, 2741, 9052, 5963, 426, 7869, 7724, 3233, 9084, 1754,
9051, 1969, 8799, 5318, 8585, 3625, 6817, 2255, 7202, 443,
347, 5662, 9908, 7292, 7749, 7666, 9837, 8888, 8004, 6064,
8918, 8031, 4098, 5465, 84, 395, 3838, 2030, 2532, 3008,
284, 1110, 2460, 2891, 4660, 5238, 8109, 338, 2173, 4485,
4837, 7421, 517, 4312, 4664, 6800, 3063, 7426, 2135, 6622,
8159, 1133, 7086, 32, 1842, 2714, 2679, 4350, 5783, 299,
911, 9855, 219, 23, 9171, 2017, 6641, 1994, 9827, 6835};

